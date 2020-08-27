CC         := cc
CFLAGS     := -Wall -Werror -Wpedantic -ansi -O3 -g
BUILD_DIR  := build
SOURCE_DIR := src
BIN        := nnvc

SOURCE     := $(SOURCE_DIR)/*.c
TARGET     := $(BUILD_DIR)/$(BIN)

$(TARGET): $(BUILD_DIR) $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

.PHONY: clean
.PHONY: debug

clean:
	rm -rf $(BUILD_DIR)

debug: $(TARGET)
	gdb -ex run -- ./$(TARGET)
