#include <stdio.h>
#include <stdlib.h>

int main(void) {
    const char *filename = "file.hole";
    FILE *f = fopen(filename, "w");
    if (!f) {
        perror("Помилка відкриття файлу");
        return 1;
    }

    fprintf(f, "=== Початок файлу ===\n");

    long offset = 5000;
    if (fseek(f, offset, SEEK_CUR) != 0) {
        perror("Помилка fseek");
        fclose(f);
        return 1;
    }

    fprintf(f, "=== Кінець файлу ===\n");

    fclose(f);
    printf("Файл '%s' створено з діркою у %ld байт.\n", filename, offset);
    return 0;
}
