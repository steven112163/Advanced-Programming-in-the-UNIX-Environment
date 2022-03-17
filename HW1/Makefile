CXX = g++
FLAGS = -O3 -std=c++17 -Wall -I.
FILES = hw1.cpp
DIR = objs/
OBJS = $(addprefix $(DIR), $(FILES:.cpp=.o))
TARGET = hw1



.PHONY: all create compile clean

all: create compile

create:
	mkdir -p $(DIR)

compile: $(OBJS)
	$(CXX) $^ -o $(TARGET) $(FLAGS)

$(DIR)%.o: %.cpp
	$(CXX) -c $< -o $@ $(FLAGS)

clean:
	rm -rf $(TARGET) $(DIR)