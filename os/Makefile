export PROJECT_DIR = $(shell pwd)
export INC_DIR = $(PROJECT_DIR)/include

export SRC_DIR = $(PROJECT_DIR)/src
export BUILD_DIR = $(PROJECT_DIR)/build

export TEST_SRC_DIR = $(PROJECT_DIR)/testing
export TEST_BUILD_DIR = $(PROJECT_DIR)/build-test



all:
	make -C $(BUILD_DIR) all

clean:
	make -C $(BUILD_DIR) clean

# $(BUILDDIR)/test-hal-emu: $(BUILDDIR)/test-hal.o $(BUILDDIR)/hal-emu.o
# 	gcc -o $(BUILDDIR)/test-hal-emu $(BUILDDIR)/test-hal.o $(BUILDDIR)/hal-emu.o -lcunit

# $(BUILDDIR)/test-fs-emu: $(BUILDDIR)/test-fs.o $(BUILDDIR)/hal-emu.o $(BUILDDIR)/fs.o $(BUILDDIR)/state.o
# 	gcc -o $(BUILDDIR)/test-fs-emu $(BUILDDIR)/test-fs.o $(BUILDDIR)/hal-emu.o $(BUILDDIR)/fs.o $(BUILDDIR)/state.o -lcunit

# $(BUILDDIR)/test-hal.o: $(TESTDIR)/test_hal.c Makefile
# 	gcc -c -g -o $(BUILDDIR)/test-hal.o $(INCLUDE) $(TESTDIR)/test_hal.c

# $(BUILDDIR)/test-fs.o: $(TESTDIR)/test_fs.c Makefile
# 	gcc -c -g -o $(BUILDDIR)/test-fs.o $(INCLUDE) $(TESTDIR)/test_fs.c

# $(BUILDDIR)/hal-emu.o: $(SRCDIR)/hal-emu.c Makefile
# 	gcc -c -g -o $(BUILDDIR)/hal-emu.o $(INCLUDE) -DDEBUG $(SRCDIR)/hal-emu.c

# $(BUILDDIR)/fs.o: $(SRCDIR)/fs.c Makefile
# 	gcc -c -g -o $(BUILDDIR)/fs.o $(INCLUDE) -DDEBUG $(SRCDIR)/fs.c

# $(BUILDDIR)/state.o: $(SRCDIR)/state.c Makefile
# 	gcc -c -g -o $(BUILDDIR)/state.o $(INCLUDE) -DDEBUG $(SRCDIR)/state.c
