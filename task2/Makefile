CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRC = dir.c
BIN_DIR = bin
TARGET = $(BIN_DIR)/dir

all: dirs $(TARGET)

dirs:
	mkdir -p $(BIN_DIR)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BIN_DIR)

help:
	@echo "Команди:"
	@echo "  make all   - зібрати програму"
	@echo "  make run   - запустити програму"
	@echo "  make clean - видалити бінарні файли"
