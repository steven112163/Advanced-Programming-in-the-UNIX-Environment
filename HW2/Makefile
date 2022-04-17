CXX = g++

HW_DIR = hw_objs
HW_FLAGS = -O3 -std=c++17 -Wall -I.
HW_FILES = hw2.cpp
HW_OBJS = $(addprefix $(HW_DIR)/, $(HW_FILES:.cpp=.o))
HW_TARGET = logger

LOGGER_DIR = logger_objs
LOGGER_FLAGS = -O3 -std=c++17 -shared -fPIC -ldl -Wall -I. -I./include
LOGGER_FILES = $(wildcard src/*.cpp)
LOGGER_OBJS = $(patsubst src/%.cpp, $(LOGGER_DIR)/%.o, $(LOGGER_FILES))
LOGGER_TARGET = logger.so

.PHONY: all create compile_hw compile_logger clean

all: create compile_hw compile_logger

create:
	mkdir -p $(HW_DIR)
	mkdir -p $(LOGGER_DIR)

compile_hw: $(HW_OBJS)
	$(CXX) $^ -o $(HW_TARGET) $(HW_FLAGS)

$(HW_DIR)/%.o: %.cpp
	$(CXX) -c $< -o $@ $(HW_FLAGS)

compile_logger: $(LOGGER_OBJS)
	$(CXX) $^ -o $(LOGGER_TARGET) $(LOGGER_FLAGS)

$(LOGGER_DIR)/%.o: src/%.cpp
	$(CXX) -c $< -o $@ $(LOGGER_FLAGS)

clean:
	rm -rf $(HW_TARGET) $(LOGGER_TARGET) $(HW_DIR) $(LOGGER_DIR)