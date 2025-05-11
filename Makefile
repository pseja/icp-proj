TARGET=icp-proj
ZIP_NAME=xcsirim00-xpsejal00-xsovakv00

SRC_DIR=src
EXAMPLES_DIR=examples
DOC_DIR=doc
BUILD_DIR=build
GEN_DIR=generated

.PHONY: all run gen doxygen pack clean

all:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(GEN_DIR)
	@echo "Building project..."
	cmake -S . -B $(BUILD_DIR)
	@echo "Compiling source files..."
	cmake --build $(BUILD_DIR)
	@echo "Build completed."

run: all
	@echo "Running project..."
# the QT_QPA_PLATFORM_PLUGIN_PATH is needed to run the application on merlin
	QT_QPA_PLATFORM_PLUGIN_PATH=/usr/local/share/Qt-5.9.2/5.9.2/gcc_64/plugins/platforms ./$(BUILD_DIR)/$(TARGET)

gen:
	@echo "Compiling all .cpp files in $(GEN_DIR)/ ..."
	@for file in $(GEN_DIR)/*.cpp; do \
		if [ -f "$$file" ]; then \
			echo "Compiling $$file..."; \
			outfile="$${file%.cpp}"; \
			g++ -std=c++17 -fPIC "$$file" -o "$$outfile" $$(pkg-config --cflags --libs Qt5Core Qt5Network Qt5Xml); \
			echo "Created binary: $$outfile"; \
		fi; \
	done
	@echo "All generated .cpp files compiled."

doxygen: all
	doxygen

pack:
	zip -r $(ZIP_NAME).zip $(SRC_DIR) $(EXAMPLES_DIR) $(DOC_DIR) README.txt Makefile Doxyfile CMakeLists.txt

clean:
	rm -rf $(BUILD_DIR) $(DOC_DIR)/html $(DOC_DIR)/xml $
	find $(GEN_DIR) -type f -not -name "*.*" -exec rm -f {} \;
