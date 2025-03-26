BUILD_DIR := build
EXECUTABLE := SolarSystemSimulation

all: build run

build:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. && $(MAKE)

run:
	./$(BUILD_DIR)/$(EXECUTABLE)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all build run clean