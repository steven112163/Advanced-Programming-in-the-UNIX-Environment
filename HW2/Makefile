CXX = g++

LOGGER_DIR = logger_objs/
LOGGER_FLAGS = -O3 -std=c++17 -Wall -I.
LOGGER_FILES = hw2.cpp
LOGGER_OBJS = $(addprefix $(LOGGER_DIR), $(LOGGER_FILES:.cpp=.o))
LOGGER = logger

SO_DIR = so_objs/
SO_FLAGS = -O3 -std=c++17 -shared -fPIC -ldl -Wall -I.
SO_FILES = shared_object.cpp
SO_OBJS = $(addprefix $(SO_DIR), $(SO_FILES:.cpp=.o))
SHARED_OBJECT = logger.so

.PHONY: all create compile_logger compile_so clean

all: create compile_logger compile_so

create:
	mkdir -p $(LOGGER_DIR)
	mkdir -p $(SO_DIR)

compile_logger: $(LOGGER_OBJS)
	$(CXX) $^ -o $(LOGGER) $(LOGGER_FLAGS)

$(LOGGER_DIR)%.o: %.cpp
	$(CXX) -c $< -o $@ $(LOGGER_FLAGS)

compile_so: $(SO_OBJS)
	$(CXX) $^ -o $(SHARED_OBJECT) $(SO_FLAGS)

$(SO_DIR)%.o: %.cpp
	$(CXX) -c $< -o $@ $(SO_FLAGS)

clean:
	rm -rf $(LOGGER) $(SHARED_OBJECT) $(LOGGER_DIR) $(SO_DIR)