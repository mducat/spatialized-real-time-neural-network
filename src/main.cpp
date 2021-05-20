#include "main.hpp"

int main(int , char **) {
    srtnn::Brain<2> brain;
    brain.runFor(1);
    Display dis(brain);
    while (dis._window->isOpen()){
        dis.draw();
        brain.runFor(1);
    }
}   