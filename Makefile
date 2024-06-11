CC := g++
CFLAGS := -Wall -pedantic -std=c++20 -fsanitize=address -g
# -I indicate an include directory, where compiler search headers
INCLUDE_DIRS := -I./literals -I./parser -I./pos -I./spreadsheet -I./expr
PARSER_DIR := -L./parser/x86_64-linux-gnu 
PARSER := -lexpression_parser

# $(wildcard literals/*.cpp) => get all source files in the selected directory
SRCS := $(wildcard *.cpp) $(wildcard literals/*.cpp) $(wildcard pos/*.cpp) $(wildcard spreadsheet/*.cpp) $(wildcard expr/*.cpp)
OBJS := $(SRCS:.cpp=.o)

all: unit_tests

unit_tests: $(OBJS) unit_tests.o
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(OBJS) $(PARSER_DIR) $(PARSER)

# like on 3 presentation, slide 48. Create object files. $@ - name of target, $< - name of prerequisite
# ./make_one.sh is bash script for creating one file ONLY for progtest.
%.o: %.cpp
	./make_one.sh
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@ 

clean:
	rm -f unit_tests $(OBJS) unit_tests.o a.out all_in_one.cpp 
