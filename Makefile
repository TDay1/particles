SRC_DIR := ./src
OBJ_DIR := ./obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS := 
CPPFLAGS := -Wall -pedantic -o3 -Wextra

All: particle-sim

particle-sim: $(OBJ_FILES)
	g++ $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ -I/headers/Particle.h $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_DIR)/* *~ particle-sim out.txt out.mp4

