
BUILD_DIR = cmake_build
INSTALL_PREFIX = /usr/local

.PHONY: all build install doc test clean publish

all: build doc

build: $(BUILD_DIR)/Makefile
	$(MAKE) -C $(BUILD_DIR)

$(BUILD_DIR)/Makefile:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -DCMAKE_INSTALL_PREFIX=$(INSTALL_PREFIX) ..

install: build
	$(MAKE) -C $(BUILD_DIR) install

doc:
	doxygen doc/Doxyfile
	cp LICENSE_1_0.txt documentation/html

test: build
	./$(BUILD_DIR)/test_is_picked_systematic

clean:
	rm -fr documentation
	rm -fr $(BUILD_DIR)/CMakeFiles 
	rm -f  $(BUILD_DIR)/*
	rmdir $(BUILD_DIR)

publish:
	rsync -a --delete --delete-excluded -v documentation/html/ renauddetry@shell.sourceforge.net:/home/groups/t/tr/trsl/htdocs
