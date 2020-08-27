CC          := cc
CFLAGS      := -Wall -Wpedantic -ansi -O3 -g
BIN         := nnvc
BUILD_DIR   := build
SOURCE_DIR  := src
INSTALL_DIR := /usr/bin

SOURCE      := $(SOURCE_DIR)/*.c
TARGET      := $(BUILD_DIR)/$(BIN)

$(TARGET): $(BUILD_DIR) $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

.PHONY: clean
.PHONY: debug
.PHONY: test
.PHONY: install

clean:
	rm -rf $(BUILD_DIR)

debug: $(TARGET)
	gdb -ex run --arg ./$(TARGET) -D examples/test.vs

test: $(TARGET)
	gdb -ex run --arg ./$(TARGET) -DT

install: $(TARGET)
	install $(TARGET) $(INSTALL_DIR)
