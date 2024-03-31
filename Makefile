# Unconstitutional build script

LD_FLAGS=
CC_FLAGS=-Isrc/include

BUILD_DIR=./build

BIN=$(BUILD_DIR)/brightman

OBJS= \
	$(BUILD_DIR)/main.o


all: $(BIN)

$(BIN): $(OBJS)
	gcc -o $@ $^ $(LD_FLAGS)

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(BUILD_DIR)
	gcc -c -o $@ $< $(CC_FLAGS)

