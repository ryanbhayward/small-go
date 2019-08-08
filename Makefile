CC=g++
CFLAGS=-Wall -Wextra -std=c++14
LIBS=
OUT_DIR=bin
SRC_OUT_DIR=$(OUT_DIR)/src

SRC_DIR=src
CXX_SRC=$(filter-out $(SRC_DIR)/main.cc, $(wildcard $(SRC_DIR)/*.cc))
OBJS:=${CXX_SRC:$(SRC_DIR)/%.cc=$(SRC_OUT_DIR)/%.o} 
INCLUDE_DIRS=
INC_PARAMS=$(foreach d, $(INCLUDE_DIRS), -I$d)

TEST_OUT_DIR=$(OUT_DIR)/test
TEST_DIR=test
TEST_SRC=$(wildcard $(TEST_DIR)/*.cc)
TEST_OBJS:=${TEST_SRC:$(TEST_DIR)/%.cc=$(TEST_OUT_DIR)/%.o}
TESTS:=${TEST_SRC:$(TEST_DIR)/%.cc=$(TEST_OUT_DIR)/%}

.PHONY: all release test memtest debug check contribute static style clean

all: directories small_go
release: CFLAGS += -O3 
release: all
test: directories $(TESTS)
	for t in $(TESTS); do echo "$$t"; $$t || exit -1; done
memtest: directories $(TESTS)
	for t in $(TESTS); do valgrind --leak-check=yes --error-exitcode=1 $$t || exit 1; done
debug: CFLAGS += -g -DMM_DEBUG
debug: all test
check: CFLAGS += -Werror
check: all
contribute: check static style test memtest
directories: $(OUT_DIR) $(SRC_OUT_DIR) $(TEST_OUT_DIR) 

small_go: $(OBJS) $(OUT_DIR)/main.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $(OUT_DIR)/small_go

$(TESTS): $(OBJS) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(OBJS) $@.o $(LIBS) -o $@

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(SRC_OUT_DIR):
	mkdir -p $(SRC_OUT_DIR)

$(TEST_OUT_DIR):
	mkdir -p $(TEST_OUT_DIR)

# Compile main
$(OUT_DIR)/main.o: $(SRC_DIR)/main.cc
	$(CC) $(CFLAGS) $(INC_PARAMS) -c -o $@ $^

# Compile Tests
$(TEST_OUT_DIR)/%.o: $(TEST_DIR)/%.cc
	$(CC) $(CFLAGS) $(INC_PARAMS) -I $(SRC_DIR) -c -o $@ $^

# Compile Source
$(SRC_OUT_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CC) $(CFLAGS) $(INC_PARAMS) -c -o $@ $^

# Run static analysis
static:
	cppcheck --language=c++ $(SRC_DIR)/*

# Run Style checking
style:
	cpplint $(SRC_DIR)/* $(TEST_DIR)/*

clean:
	rm -rf $(OUT_DIR)/*.o $(OUT_DIR)/**/*.o $(OUT_DIR)/small_go $(TESTS)
