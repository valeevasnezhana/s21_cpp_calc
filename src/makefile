
CMAKE_DEFINES =

BUILD_DIR = build
INSTALL_DIR = ~
DOC_DIR = docs
EXE_NAME = SmartCalc_v2
FILES_TO_COVER = calculator.cc tokenizer.cc translator.cc

.PHONY: all
all: build

.PHONY: configure
configure:
	cmake -B $(BUILD_DIR) . $(CMAKE_DEFINES)

.PHONY: install
install: build uninstall
	cp -R $(BUILD_DIR)/$(EXE_NAME).app $(INSTALL_DIR)

.PHONY: uninstall
uninstall:
	rm -rf $(INSTALL_DIR)/$(EXE_NAME).app

.PHONY: run
run: install
	open $(INSTALL_DIR)/$(EXE_NAME).app

.PHONY: html
html:
	rm -rf $(DOC_DIR)/html/html
	doxygen ./docs/html/Doxyfile
	open ./docs/html/html/files.html

.PHONY: dvi
dvi:
	rm -rf $(DOC_DIR)/dvi/perlmod/
	doxygen ./docs/dvi/Doxyfile
	cd docs/dvi/perlmod/&&(make dvi)
	open docs/dvi/perlmod/doxylatex.dvi

.PHONY: dist
dist: clean
	tar -cvz -f $(EXE_NAME).tar.gz $(shell find . -type f -name "*")

.PHONY: build
build: configure
	cd build && cmake --build . --target SmartCalc_v2

.PHONY: test
test: configure
	cd build && cmake --build . --target tokenizer_test
	cd build && cmake --build . --target model_integration
	./$(BUILD_DIR)/tokenizer_test
	./$(BUILD_DIR)/model_integration

.PHONY: tests
tests: test

.PHONY: gcov_report
gcov_report: add_lcov_definition configure tests
	mkdir -p gcov
	cp $(shell find build -type f -name "*.gcda") gcov
	cp $(shell find build -type f -name "*.gcno") gcov
	geninfo $(addprefix gcov/, $(addsuffix .gcda, $(FILES_TO_COVER))) -o gcov/data.info --gcov-tool /usr/bin/gcov --exclude v1/
	genhtml gcov/data.info -o report
	open report/index.html

.PHONY: style
style:
	cp ../materials/linters/.clang-format ./
	clang-format -n -style=file $(shell find . -not -path "./build/*" -type f -name "*.cc")
	clang-format -n -style=file $(shell find . -not -path "./build/*" -type f -name "*.h")
	rm .clang-format

.PHONY: clean
clean:
	rm -rf *.tar.gz
	rm -rf $(DOC_DIR)/html/html
	rm -rf $(DOC_DIR)/dvi/perlmod
	rm -rf report
	rm -rf $(BUILD_DIR)
	rm -rf gcov

.PHONY: rebuild
rebuild: clean build

.PHONY: add_lcov_definition
add_lcov_definition:
		$(eval CMAKE_DEFINES += -DCMAKE_CXX_FLAGS="-fprofile-arcs -ftest-coverage -fno-elide-constructors")
		$(eval CMAKE_DEFINES += -DCMAKE_EXE_LINKER_FLAGS="-fprofile-arcs")

# --no-external
