CC          := cc
CFLAGS      := -Wall -Wpedantic -ansi -O3 -g
LDFLAGS     := $(shell pkg-config --cflags --libs sdl2) \
               $(shell pkg-config --cflags --libs cairo) \
               -lm -lsvgtiny
BIN         := nnvs
MAN         := nnvs.1
BUILD_DIR   := build
SOURCE_DIR  := src
INSTALL_DIR := /usr/bin
MAN_DIR     := /usr/share/man/man1

SOURCE      := $(SOURCE_DIR)/*.c
TARGET      := $(BUILD_DIR)/$(BIN)

$(TARGET): $(BUILD_DIR) $(SOURCE_DIR)/*
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET) $(LDFLAGS)

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
	install $(MAN)    $(MAN_DIR)/$(MAN)
