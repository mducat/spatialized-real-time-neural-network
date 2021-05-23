#include "main.hpp"

int main(int , char **) {
    const uint N = 2;
    
    srtnn::Brain<N> brain;
    brain.runFor(1);
    Display<N> dis(brain);
    while (dis._window->isOpen()){
    //for (uint i = 0; i < 600; i++){
        brain.runFor(1.0/60);
        dis.draw();
    }
}   