PROJECT_NAME="oblivion"
BUILD_DIR="build"
SOURCE_DIR="src"

.PHONY: all clean build run

all: build

build:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. && $(MAKE)

run: build
	./$(BUILD_DIR)/$(SOURCE_DIR)/$(PROJECT_NAME)

clean:
	rm -rf $(BUILD_DIR)

# pack:
# 	zip xcsirim00-xpsejal00-xsovakv00.zip
