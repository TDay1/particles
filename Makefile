CC := nvcc
SRC_DIR := ./src
OBJ_DIR := ./obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cu)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cu,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS :=
CPPFLAGS := -O3 --std=c++11

All: particle-sim

particle-sim: $(OBJ_FILES)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cu
	$(CC) -I/headers/Particle.h $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_DIR)/Particle.o $(OBJ_DIR)/main.o $(OBJ_DIR)/output.o $(OBJ_DIR)/simulation.o *~ particle-sim out*

