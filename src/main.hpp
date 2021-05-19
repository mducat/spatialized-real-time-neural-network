#ifndef MAIN_HPP_
#define MAIN_HPP_

#include <string>
#include <vector>
#include <deque>
#include "UniTree.hpp"

namespace srtnn{
    class Event {
    public:
        Event(float t) : 
            _t(t)
        {}

        float _t;
    };
    
    template<uint N>
    class Neuron {
    public:
        Neuron(float val) {
            for (uint i = 1; i < N; i++)
                _p = val;
        }

        inline float operator[](int id){
            return _p[id];
        }

        float _p[N];
    };


    template<uint N>
    class Brain {
    public:
        Brain() {
            float center[N] = {0};
            float size[N] = {100000};

            
            _tree = new UniTree<Neuron<N>, float[N], N>(center, size);
        }

        ~Brain() {}
    
        UniTree<Neuron<N>, float[N], N> *_tree;
    };
};

#endif 

