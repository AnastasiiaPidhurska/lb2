#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <unistd.h>
#include <time.h>

void print_help(const char *progname) {
    printf("Використання: %s [опції] [каталог]\n", progname);
    printf("Опції:\n");
    printf("  -h, --help   Вивести довідку про використання\n");
    printf("  -l, --long   Вивести додаткову інформацію (права, розмір, дата)\n");
    printf("  -a, --all    Показати приховані файли (що починаються з .)\n");
    printf("Приклади:\n");
    printf("  %s             # Вивести вміст поточного каталогу\n", progname);
    printf("  %s -l /home    # Вивести детальну інформацію про /home\n", progname);
    printf("  %s -a -l       # Вивести усі файли, включаючи приховані\n", progname);
}

void print_permissions(mode_t mode) {
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

int main(int argc, char *argv[]) {
    int show_all = 0;
    int long_format = 0;
    int opt;
    int option_index = 0;

    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"long", no_argument, 0, 'l'},
        {"all",  no_argument, 0, 'a'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "hla", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'h':
                print_help(argv[0]);
                return 0;
            case 'l':
                long_format = 1;
                break;
            case 'a':
                show_all = 1;
                break;
            default:
                fprintf(stderr, "Невідома опція. Використайте --help для довідки.\n");
                return 1;
        }
    }

    const char *dir_path = (optind < argc) ? argv[optind] : ".";
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("Помилка відкриття каталогу");
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (!show_all && entry->d_name[0] == '.')
            continue;

        if (!long_format) {
            printf("%s  ", entry->d_name);
        } else {
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);
            struct stat st;
            if (stat(full_path, &st) == 0) {
                print_permissions(st.st_mode);
                printf(" %5ld ", (long)st.st_size);
                char timebuf[64];
                struct tm *tm_info = localtime(&st.st_mtime);
                strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M", tm_info);
                printf("%s  %s\n", timebuf, entry->d_name);
            }
        }
    }

    if (!long_format) printf("\n");
    closedir(dir);
    return 0;
}
