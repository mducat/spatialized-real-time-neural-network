#ifndef UNITREE_HPP_
#define UNITREE_HPP_

#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <string.h>
#include <functional>
#include <vector>

template <class T, class G, uint D>
class UniTree {
public:
    UniTree(G center, G size) : // 
        _center(center), _size(size)
    {
    }

    ~UniTree() {
        if (_prev)
            delete _prev;
        else if (_dataMode == false)
            for (uint i = 0; i < (int)std::pow(2, D); i++) {
                if (_nexts[i]){
                    _nexts[i]->_prev = 0;
                    delete _nexts[i];
                }
            }
    }

    void addData(T *thing) {
        G dpos;
        //check if thing is in this boundarys
        
        for (uint i = 0; i < D; i++) {
            dpos[i] = (*thing)[i] - _center[i];
            if (0) // inib out of boundary check
            if (std::abs(dpos[i]) > _size[i]) { // if out of boundary
                std::cerr << "out of boundary" << std::endl;
                if (_prev == 0){ // alloc prev
                    G newCenter;
                    G newSize;
                    for (uint j = 0; j < D; j++) {
                        newSize[j] = _size[j]*2;
                        newCenter[j] = _center[j];
                        if (dpos[j] < -_size[i])
                            newCenter[j] -= _size[j];
                        else if (dpos[j] > _size[i])
                            newCenter[j] += _size[j];
                    }
                    _prev = new UniTree<T, G, D>(newCenter, newSize);
                    _prev->_nexts[0] = this;
                    _prev->_dataMode = false;
                    std::cerr << newSize[0] << std::endl;
                }
                _prev->addData(thing); // add to prev
                return;
            }
        }

        if (_dataMode) {
            // put in data slot
            for (int i = 0; i < pow(2, D); i++) {
                if (_data[i] == 0){
                    _data[i] = thing;
                    _dataNb++;
                    return;
                }
            }
            // create nexts nodes
            _dataMode = false;
            T *buffer[(int)std::pow(2, D)];
            memcpy(buffer, _data, sizeof(T*)*(int)std::pow(2, D));
            memset(_nexts, 0, sizeof(UniTree<T, G, D> *)*(int)std::pow(2, D));
            for (int i = 0; i < pow(2, D); i++) {
                //generate new center and new size
                G newCenter;
                G newSize;
                for (int j = 0; (uint)j < D; j++) {
                    newSize[j] = _size[j]/2;
                    newCenter[j] = _center[j]+newSize[j]*(((i >> j) & 1)*2-1);
                }
                //generate _nexts nodes
                _nexts[i] = new UniTree<T, G, D>(newCenter, newSize);
                _nexts[i]->_prev = this;
            }
            for (uint i = 0; i < pow(2, D); i++)
                addData(buffer[i]);
            addData(thing);
        } else {
            // give to next node
            int val = 0;
            for (uint i = 0; i < D; i++)
                if (dpos[i] > 0) // calc coords for next node
                    val |= 1 << i;
            _nexts[val]->addData(thing);
        }
    }

    bool contain(G &center, G &size, G &p){
        for (uint i = 0; i < D; i++){
            if (abs(center[i] - p[i]) > size[i])
                return false;
        }
        return true;
    }

    bool contain(G &center, G &size, T &p){
        for (uint i = 0; i < D; i++){
            if (abs(center[i] - p[i]) > size[i])
                return false;
        }
        return true;
    }

    bool intersect(G &center1, G &size1, G &center2, G &size2){
        G sizeSome;
        for (uint i = 0; i < D; i++) {
            sizeSome[i] = size1[i] + size2[i];
        }
        return contain(center1, sizeSome, center2);
    }

    std::vector<T *> getInArea(G &center, G &size) {
        std::vector<T *> ret;

        if (!intersect(center, size, _center, _size))
            return ret;
        if (_dataMode){
            for (uint i = 0; i < std::pow(2, D); i++){
                if (_data[i] && contain(center, size, *_data[i])) {
                    ret.push_back(_data[i]);
                }
            }
        } else {
            for (uint i = 0; i < std::pow(2, D); i++){
                std::vector<T *> vec = _nexts[i]->getInArea(center, size);
                ret.insert(ret.end(), vec.begin(), vec.end());
            }
        }
        return ret;
    }

    void removeFromThisNode(){
        if (_prev) {
            int val = 0;
            for (uint i = 0; i < D; i++)
                if (_center[i] - _prev->_center[i] > 0) // calc coords for next node
                    val |= 1 << i;
            //_prev->_nexts[val] = new UniTree<T, G, D>(_center, _size);
            //_prev->_nexts[val]->_prev = _prev;
            //_prev = 0;
            if (!_dataMode)
                for (uint i = 0; i < std::pow(2, D); i++){
                    _nexts[i]->_prev = 0;
                    delete _nexts[i];
                }
            memset(_data, 0, sizeof(_data));
            _dataNb = 0;
            _dataMode = true;
        } else {
            delete this;
        }
    }

    bool isEmpty() {
        if (!_dataMode){
            for (int i = 0; i < std::pow(2, D); i++)
                if (_nexts[i]->_dataNb)
                    return false;
            return true;
        } else
            return !_dataNb;
    }

    bool erase(T *p) {
        if (_dataMode){
            for (int i = 0; i < std::pow(2, D); i++) {
                if (_data[i] == p){
                    _data[i] = 0;
                    _dataNb--;
                    return true;
                }
            }
            return false;
        } else {
            int val = 0;
            for (uint i = 0; i < D; i++)
                if ((*p)[i] - _center[i] > 0) // calc coords for next node
                    val |= 1 << i;
            bool ok = _nexts[val]->erase(p);
            if (_prev && isEmpty())
                removeFromThisNode();
            return ok;
        }
    }

    bool erase(G &center, G &size) {
        bool some = false;
        if (!intersect(center, size, _center, _size))
            return some;
        G upLeft;
        G downRight;
        for (uint i = 0; i < D; i++){
            upLeft[i] = _center[i]-_size[i];
            downRight[i] = _center[i]+_size[i];
        }

        if (_prev && contain(center, size, upLeft) && contain(center, size, downRight)){
            if (!isEmpty())
                some = true;
            removeFromThisNode();
            return some;
        }

        if (_dataMode){
            for (int i = 0; i < std::pow(2, D); i++) {
                if (_data[i] && contain(center, size, *_data[i])){
                    _data[i] = 0;
                    some = true;
                    _dataNb--;
                }
            }
        } else {
            for (int i = 0; i < std::pow(2, D); i++) {
                some = _nexts[i]->erase(center, size) || some;   
            }
            if (_prev && isEmpty())
                removeFromThisNode();
        }
        return some;
    }

    void draw(std::function<void(G &, G &, uint d)> func, uint d = 0) {
        if (d && !_prev)
            std::cerr << "what" << std::endl;
        if (!_dataMode)
            for (uint i = 0; i < (uint)std::pow(2, D); i++)
                _nexts[i]->draw(func, d+1); 
        func(_center, _size, d);
    }

    G _center;
    G _size;
    UniTree<T, G, D> *_prev = 0;
    union {
        UniTree<T, G, D> *_nexts[(int)std::pow(2, D)];
        T *_data[(int)std::pow(2, D)] = {0};
    };
    bool _dataMode = true;
    int _dataNb = 0;
};

#endif /* !UNITREE_HPP_ */
