#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

void print_help(const char *progname) {
    printf("Використання: %s [опції] <ім'я_файлу>\n", progname);
    printf("Опції:\n");
    printf("  -h, --help           Вивести довідкову інформацію\n");
    printf("  -o, --output <файл>  Записати результат у вказаний файл\n");
    printf("  -p, --position <N>   Початкова позиція у файлі (байти, з 0)\n");
    printf("  -c, --count <N>      Кількість байтів для читання\n");
    printf("  -n, --newline        Додати символ нового рядка наприкінці виводу\n");
    printf("Приклад:\n");
    printf("  %s -p 10 -c 20 -o out.txt input.txt\n", progname);
}

int main(int argc, char *argv[]) {
    const char *output_file = NULL;
    long position = 0;
    long count = -1;
    int newline = 0;

    int opt;
    int option_index = 0;

    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"output", required_argument, 0, 'o'},
        {"position", required_argument, 0, 'p'},
        {"count", required_argument, 0, 'c'},
        {"newline", no_argument, 0, 'n'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "ho:p:c:n", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'h':
                print_help(argv[0]);
                return 0;
            case 'o':
                output_file = optarg;
                break;
            case 'p':
                position = atol(optarg);
                if (position < 0) return 1;
                break;
            case 'c':
                count = atol(optarg);
                if (count < 0) return 1;
                break;
            case 'n':
                newline = 1;
                break;
            default:
                return 1;
        }
    }

    if (optind >= argc) return 1;

    const char *input_file = argv[optind];
    FILE *in = fopen(input_file, "rb");
    if (!in) return 1;

    FILE *out = stdout;
    if (output_file) {
        out = fopen(output_file, "wb");
        if (!out) {
            fclose(in);
            return 1;
        }
    }

    if (fseek(in, position, SEEK_SET) != 0) {
        fclose(in);
        if (output_file) fclose(out);
        return 1;
    }

    char buffer[1024];
    long bytes_left = count;
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, (count == -1 ? sizeof(buffer) : (bytes_left < (long)sizeof(buffer) ? (size_t)bytes_left : sizeof(buffer))), in)) > 0) {
        fwrite(buffer, 1, bytes_read, out);
        if (count != -1) {
            bytes_left -= (long)bytes_read;
            if (bytes_left <= 0) break;
        }
    }

    if (newline) fputc('\n', out);

    fclose(in);
    if (output_file) fclose(out);
    return 0;
}
