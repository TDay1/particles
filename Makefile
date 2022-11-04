CC := nvcc
SRC_DIR := ./src
OBJ_DIR := ./obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cu) $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(OBJ_DIR)/main.o $(OBJ_DIR)/output.o $(OBJ_DIR)/simulation.o $(OBJ_DIR)/particleCollision.o
LDFLAGS := -O3
CPPFLAGS := -O3 --std=c++11

All: particle-sim

particle-sim: $(OBJ_FILES)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cu
	$(CC) $(CPPFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CPPFLAGS) -c -o $@ $< -mavx -mavx2

clean:
	rm -f $(OBJ_FILES) *~ particle-sim out*

