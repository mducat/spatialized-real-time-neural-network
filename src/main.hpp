#ifndef MAIN_HPP_
#define MAIN_HPP_

#include <string>
#include <vector>
#include <deque>
#include <memory>
#include <set>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "UniTree.hpp"

namespace srtnn {
    template<int N, typename T = float>
    class Pos {
    public:
        Pos() {}
        Pos(T val) {
            for (uint i = 0; i < N; ++i)
                _p[i] = val;
        }

        inline T &operator[](int i) {
            return _p[i];
        }

        T _p[N];
    };


    template<uint N>
    class Neuron {
    public:
        Neuron()
        {}

        Neuron(Pos<N> pos) :
            _pos(pos)
        {}

        inline float &operator[](int id){
            return _pos[id];
        }

        Pos<N> _pos;
        std::vector<Neuron *> _connects;
    };

    template<uint N>
    class Brain;

    template<uint N>
    class Event {
    public:
        Event(float tSrc, float tDest) : 
            _tSrc(tSrc), _tDest(tDest), _tD(_tDest-_tSrc)
        {}

        bool operator<(const Event&that) const {
            return this->_tDest < that._tDest;
        }

        void compute(Brain<N> *brain) {
            float tEnd = _tDest+(rand()%1024)/1024.0+0.5;

            Event<N> ev(_tDest, tEnd);
            ev._neuronSrc = _neuronDest;
            ev._neuronDest = _neuronDest->_connects[rand()%_neuronDest->_connects.size()];
            brain->_events.insert(ev);
        }

        float _tSrc;
        float _tDest;
        float _tD;
        Neuron<N> *_neuronSrc;
        Neuron<N> *_neuronDest;
    };

    template<uint N>
    class Synapse {
        Synapse(Neuron<N> &src, Neuron<N> &dst) {
        }

        ~Synapse() {
        }

        void compute() {
            
        }
    };

    template<uint N>
    class Brain {
    public:
        Brain() {
            Pos<N> center(512);
            Pos<N> size(512);
            _tree = std::make_shared<UniTree<Neuron<N>, Pos<N>, N>>(center, size);

            // init neurons && add it to _tree
            uint neuron_quantity = 2048;
            _neurons.resize(neuron_quantity); // make 256 neurons

            for (Neuron<N> *&n : _neurons) {
                n = new Neuron<N>;
                for (uint j = 0; j < N; j++){
                    n->_pos[j] = rand()%(int)(size[j]*2) - size[j] + center[j];
                }
                _tree->addData(n);
            }

            // connect the neurons
            for (uint i = 0; i < neuron_quantity; i++){
                Pos<N> getsize(32);
                std::vector<Neuron<N> *> vec = _tree->getInArea(_neurons[i]->_pos, getsize);
                std::random_shuffle(&(*vec.begin()), &(*vec.end()));
                float prob = 2;
                for (auto neuron : vec){
                    if (neuron != _neurons[i] && rand()/((float)RAND_MAX) < prob){ // connect neurons
                        _neurons[i]->_connects.push_back(neuron);
                        prob *= 0.6;
                    }
                }
            }

            // defibrillate
            for (Neuron<N> *&n : _neurons){
                for (Neuron<N> *&nn : n->_connects){
                    if (0.25 < (rand()%4096)/4096.0)
                        continue;

                    float tStart = (rand()%4096)/256.0;
                    float tEnd = tStart+(rand()%4096)/1024.0;

                    Event<N> ev(tStart, tEnd);
                    ev._neuronDest = nn;
                    ev._neuronSrc = n;
                    _events.insert(ev);
                }
            }
        }

        ~Brain() {
            for (Neuron<N> *&n : _neurons) {
                delete n;
            }
        }

        void computeEvent(Event<N> &ev) {
            ev.compute(this);

            /*Neuron<N> *neuronDest = ev._neuronDest;
            Neuron<N> *neuronSrc = ev._neuronSrc;
            (void)(neuronDest);
            (void)(neuronSrc);*/
            //neuronDest->simulate(ev, neuronSrc);
        }

        void runFor(float t){
            float tObjective = _currentTime + t;
            while (1) {
                if (_events.size() == 0){
                    std::cerr << "Brain is ded" << std::endl;
                    break;
                }
                Event<N> ev = *(_events.begin());
                if (ev._tDest > tObjective) 
                    break;
                _events.erase(_events.begin());
                _currentTime = ev._tDest;    
                computeEvent(ev);
            }
            _currentTime = tObjective;
            std::cout << _currentTime << std::endl;
        }

        void cmpFunc(Event<N>& a, Event<N>& b) {
            return a._tDest < b._tDest;
        }

        std::vector<Neuron<N> *> _neurons;
        std::multiset<Event<N>> _events;
        std::shared_ptr<UniTree<Neuron<N>, Pos<N>, N>> _tree;
        float _currentTime = 0;
    };
};



//Display the Network
template<uint N>
class Display {
public:
    Display(srtnn::Brain<N> &brain, uint width = 1024, uint height = 1024) : 
        _width(width), _height(height), _brain(brain)
    {
        _window = new sf::RenderWindow(sf::VideoMode(1024, 1024), "Spatialized Real Time Neural Network");
        _window->setFramerateLimit(60);
        _window->setPosition(sf::Vector2i(5, 5));
        
        _circle_neuron.setFillColor(sf::Color::White);
        _circle_neuron.setRadius(3);
        _circle_neuron.setOrigin(sf::Vector2f(3, 3));
        
        _circle_event.setFillColor(sf::Color(0, 0, 255, 200));
        _circle_event.setRadius(2);
        _circle_event.setOrigin(sf::Vector2f(2, 2));
        
        _rectangle_tree.setOutlineThickness(2);
        _rectangle_tree.setOutlineColor(sf::Color(255, 0, 0, 64));
        _rectangle_tree.setFillColor(sf::Color(0, 0, 0, 0));
    }

    void draw() {
        
        _brain._tree->draw(
        [this]
        (srtnn::Pos<N> &center, srtnn::Pos<N> &size, uint d){
            d++;
            //float thick = 8.0/d;
            float thick = 1;
            _rectangle_tree.setOutlineThickness(thick);

            _rectangle_tree.setSize(sf::Vector2f(size[0]*2-thick*2, size[1]*2-thick*2));
            _rectangle_tree.setPosition(
                sf::Vector2f((center[0]-size[0])+thick, (center[1]-size[1])+thick));
            _window->draw(_rectangle_tree);
        });

        for (srtnn::Neuron<N> *&n : _brain._neurons) {
            for (srtnn::Neuron<N> *nn : n->_connects){
                sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f((*n)[0], (*n)[1])),
                    sf::Vertex(sf::Vector2f((*nn)[0], (*nn)[1]))
                };
                line[0].color = sf::Color(0, 255, 0, 200);
                line[1].color = sf::Color(255, 0, 0, 200);
                _window->draw(line, 2, sf::Lines);
            }
        }

        for (srtnn::Neuron<N> *&n : _brain._neurons) {
            _circle_neuron.setPosition(sf::Vector2f((*n)[0], (*n)[1]));
            _window->draw(_circle_neuron);
        }

        sf::Event event;
        while (_window->pollEvent(event)){
            if (event.type == sf::Event::Closed)
                _window->close();
        }

        for (const srtnn::Event<N> &ev : _brain._events){
            srtnn::Pos<N> p;
            
            float r = (_brain._currentTime-ev._tSrc)/ev._tD;

            if (r < 0 || r > 1){
                continue;
            }

            p[0] = (*ev._neuronSrc)[0]*(1-r) + (*ev._neuronDest)[0]*r;
            p[1] = (*ev._neuronSrc)[1]*(1-r) + (*ev._neuronDest)[1]*r;
            _circle_event.setPosition(sf::Vector2f(p[0], p[1]));
            _window->draw(_circle_event);
        }

        _window->display();
        _window->clear();
        _frame++;
    }

    sf::RectangleShape _rectangle_tree;
    sf::CircleShape _circle_neuron;
    sf::CircleShape _circle_event;
    uint _frame;
    sf::RenderWindow *_window;
    uint _width;
    uint _height;
    srtnn::Brain<N> &_brain;
};  

#endif 

