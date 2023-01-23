# Resource to learn Make: https://www.gnu.org/software/make/manual/html_node/index.html

CC:=gcc
CFLAGS:=-g
# To turn off warnings change line above to "CFLAGS:=-g -w"
BIN_NAME:=53gitlog
SRC_DIR:=src
INCLUDE_DIR:=include
BIN_DIR:=bin
PRG_SUFFIX:=.out

# These are called rules. Rules have targets (what to make), and prerequisites (what needs to exist).
# Make won't build a target if it's prerequisites haven't changed; it's smart enough not to re-do it's work. The syntax for rules is:
# target(s): prerequisite(s)
# \trecipe (how to make the target)
# ...
# The tab character is important (no spaces), and the recipe is a series of shell command(s)
all: clean setup p0test_main p1test_main p3test_main linkedlisttest_main 53gitlog 
	
53gitlog: src/hw2.c  src/helpers2.c src/linkedlist.c src/53gitlog.c
	$(CC) -I $(INCLUDE_DIR) $(CFLAGS) $(SRC_DIR)/53gitlog.c  $(SRC_DIR)/linkedlist.c $(SRC_DIR)/helpers2.c $(SRC_DIR)/hw2.c -o $(BIN_DIR)/53gitlog

#compile all the main files
p0test_main: setup src/hw2.c src/helpers2.c src/linkedlist.c tests/p0test_main.c
	$(CC) -I $(INCLUDE_DIR) $(CFLAGS) $(SRC_DIR)/helpers2.c $(SRC_DIR)/linkedlist.c $(SRC_DIR)/hw2.c tests/p0test_main.c -o $(BIN_DIR)/p0test_main

p1test_main: setup src/hw2.c src/helpers2.c src/linkedlist.c tests/p1test_main.c
	$(CC) -I $(INCLUDE_DIR) $(CFLAGS) $(SRC_DIR)/helpers2.c $(SRC_DIR)/linkedlist.c $(SRC_DIR)/hw2.c tests/p1test_main.c -o $(BIN_DIR)/p1test_main

p3test_main: setup src/hw2.c src/helpers2.c src/linkedlist.c tests/p3test_main.c
	$(CC) -I $(INCLUDE_DIR) $(CFLAGS) $(SRC_DIR)/linkedlist.c $(SRC_DIR)/helpers2.c $(SRC_DIR)/hw2.c tests/p3test_main.c -o $(BIN_DIR)/p3test_main

linkedlisttest_main: setup src/hw2.c src/helpers2.c src/linkedlist.c tests/linkedlisttest_main.c
	$(CC) -I $(INCLUDE_DIR) $(CFLAGS) $(SRC_DIR)/linkedlist.c $(SRC_DIR)/helpers2.c $(SRC_DIR)/hw2.c tests/linkedlisttest_main.c -o $(BIN_DIR)/linkedlisttest_main

setup:
	@mkdir -p $(BIN_DIR)


# Standard (ish) rule to clean out files we'd like to re-generate
clean:
	rm -rf $(BIN_DIR) 

# By default, make will assume each target is a file. This means that it won't actually run the clean target if a file called ```clean``` exists.
# The following special target overrides this behavior; it's prerequisites are always run.
.PHONY: clean
