CXX := g++
CXXFLAGS := -Wall -pedantic -std=c++20 -fsanitize=address -g
PARSER_DIR := -L./parser/x86_64-linux-gnu 
PARSER := -lexpression_parser

# get all source files
SRCS := unit_tests.cpp literals/literals.cpp pos/pos.cpp spreadsheet/spreadsheet.cpp expr/expr.cpp
OBJS := $(SRCS:.cpp=.o) # change .cpp to .o

all: deps unit_tests

unit_tests: $(OBJS) unit_tests.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(PARSER_DIR) $(PARSER)

# Create object files. $@ - name of target, $< - name of first prerequisite
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# clean all created files
clean:
	rm -f unit_tests $(OBJS) Makefile.d

# generate deps for all files
deps:
	$(CXX) -MM $(SRCS) > Makefile.d

# run application
run:
	./unit_tests

-include Makefile.d