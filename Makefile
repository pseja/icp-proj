TARGET=icp-proj

SRC_DIR=src
EXAMPLES_DIR=examples
DOC_DIR=doc

BUILD_DIR="build"

.PHONY: all run doxygen pack clean

all:
	mkdir -p $(BUILD_DIR)
	@echo "Building project..."
	cmake -S . -B $(BUILD_DIR)
	@echo "Compiling source files..."
	cmake --build $(BUILD_DIR)
	@echo "Build completed."

run: all
	@echo "Running project..."
# the QT_QPA_PLATFORM_PLUGIN_PATH is needed to run the application on merlin
	QT_QPA_PLATFORM_PLUGIN_PATH=/usr/local/share/Qt-5.9.2/5.9.2/gcc_64/plugins/platforms ./"build"/icp-proj

doxygen:
	@echo "Not implemented yet"

pack:
	zip -r xcsirim00-xpsejal00-xsovakv00.zip $(SRC_DIR) $(EXAMPLES_DIR) $(DOC_DIR) README.txt Makefile

clean:
	rm -rf $(BUILD_DIR)
