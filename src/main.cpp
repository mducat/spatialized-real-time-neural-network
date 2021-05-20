#include "main.hpp"

int main(int , char **) {
    srtnn::Brain<2> brain;
    brain.runFor(1);
    Display dis(brain);
    while (dis._window->isOpen()){
    //for (uint i = 0; i < 600; i++){
        brain.runFor(1.0/60);
        dis.draw();
    }
}   