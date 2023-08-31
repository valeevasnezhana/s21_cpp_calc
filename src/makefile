CMAKE_DEFINES = 

BUILD_DIR = build
INSTALL_DIR = ~
DOC_DIR = doc
EXE_NAME = SmartCalc_v2
FILES_TO_COVER = Calculator.cc Tokenizer.cc Translator.cc

all: build

configure:
	cmake -B $(BUILD_DIR) . $(CMAKE_DEFINES)

install: build uninstall
	cp -R $(BUILD_DIR)/$(EXE_NAME).app $(INSTALL_DIR)

uninstall:
	rm -rf $(INSTALL_DIR)/$(EXE_NAME).app

run: install
	open $(INSTALL_DIR)/$(EXE_NAME).app

html:
	makeinfo --no-split --html $(DOC_DIR)/doc.texi -o $(DOC_DIR)/doc.html

dvi:
	texi2dvi $(DOC_DIR)/doc.texi
	
dist: clean
	tar -cvz -f $(EXE_NAME).tar.gz $(shell find . -type f -name "*")

build: configure
	cd build && cmake --build . --target SmartCalc_v2

test: configure
	cd build && cmake --build . --target tokenizer_test
	cd build && cmake --build . --target model_integration
	./$(BUILD_DIR)/tokenizer_test
	./$(BUILD_DIR)/model_integration

tests: test

gcov_report: add_lcov_definition configure tests
	mkdir -p gcov
	cp $(shell find build -type f -name "*.gcda") gcov
	cp $(shell find build -type f -name "*.gcno") gcov
	geninfo $(addprefix gcov/, $(addsuffix .gcda, $(FILES_TO_COVER))) -o gcov/data.info --gcov-tool /usr/bin/gcov --exclude v1
	genhtml gcov/data.info -o report
	open report/index.html

style:
	cp ../materials/linters/.clang-format ./
	clang-format -i -style=file $(shell find . -not -path "./build/*" -not -path "./cmake_build_debug/*" -type f -name "*.cc")
	clang-format -i -style=file $(shell find . -not -path "./build/*" -not -path "./cmake_build_debug/*" -type f -name "*.h")
	rm .clang-format

clean:
	rm -rf *.tar.gz
	rm -rf $(DOC_DIR)/*.html
	rm -rf report
	rm -rf $(BUILD_DIR)
	rm -rf gcov

rebuild: clean build

add_lcov_definition:
		$(eval CMAKE_DEFINES += -DCMAKE_CXX_FLAGS="-fprofile-arcs -ftest-coverage -fno-elide-constructors")
		$(eval CMAKE_DEFINES += -DCMAKE_EXE_LINKER_FLAGS="-fprofile-arcs")

# --no-external
