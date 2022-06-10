CXX = g++

DIR = objs
FLAGS = -O3 -std=c++17 -Wall -I. -lcapstone
FILES = hw4.cpp debugger.cpp
OBJS = $(addprefix $(DIR)/, $(FILES:.cpp=.o))
TARGET = hw4



.PHONY: all create compile clean

all: create compile

create:
	mkdir -p $(DIR)

compile: $(OBJS)
	$(CXX) $^ -o $(TARGET) $(FLAGS)

$(DIR)/%.o: %.cpp
	$(CXX) -c $< -o $@ $(FLAGS)

clean:
	rm -rf $(TARGET) $(DIR)