#include "Simulation.h"
#include <iostream>

int main() {
    std::cout << "Solar System Simulation\n"
              << "=======================\n"
              << "Controls:\n"
              << "  WASD - Move camera forward/left/back/right\n"
              << "  Q/E  - Move camera down/up\n" 
              << "  Mouse - Look around (hold right mouse button)\n"
              << "  F    - Toggle free-look mode (capture mouse)\n"
              << "  Shift - Move faster\n"
              << "  ESC  - Exit\n\n";
    
    Simulation simulation;
    
    if (!simulation.initialize(1280, 720)) {
        std::cerr << "failed to initialize simulation" << std::endl;
        return -1;
    }
    
    simulation.run();
    
    return 0;
}