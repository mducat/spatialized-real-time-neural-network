
#pragma once

#define DEBUGINFORMATION std::string(" | from ") + std::string(__FILE__) + std::string(" L") + std::to_string(__LINE__)

#include "ByteObject.hpp"


#if _WIN32
//#include <windows.h>
#else
#include <unistd.h>
#endif

#ifdef _WIN32 
typedef unsigned int uint;
#endif 

#if BYTEOBJECT_DEBUG
template <typename T>
void ByteObject::addDebugSymbol() {
    std::string typeName = demangle(typeid(T).name());
    uint32_t lenght = typeName.length();
    writeToByteObjectT(&lenght, sizeof(lenght));
    writeToByteObjectT(typeName.c_str(), typeName.length());
    std::cout << "addDebugSymbol:" << typeName << std::endl;
}

template <typename T>
void ByteObject::matchDebugSymbole() {
    std::string typeName;
    uint32_t lenght;
    readFromByteObjectT(&lenght, sizeof(lenght));
    typeName.resize(lenght);
    readFromByteObjectT(&typeName[0], lenght);
    std::cout << "readDebugSymbol:" << typeName << " = " << demangle(typeid(T).name()) << std::endl;
    if (typeName != demangle(typeid(T).name()))
        throw std::runtime_error("ByteObject: got "+typeName + " expected "+demangle(typeid(T).name()));
}
#endif

template <typename T>
ByteObject &ByteObject::operator<<(std::vector<T> const &thing) {
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif
    uint32_t size = thing.size();
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "vector " << demangle(typeid(T).name()) << std::endl;
    #endif
    *this << size;
    for (T const &v : thing)
        *this << v;
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "vector end" << std::endl;
    #endif
    return *this;
}

template <typename T>
ByteObject &ByteObject::operator<<(std::deque<T> const &thing) {
#if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
#endif
    uint32_t size = thing.size();
#if BYTEOBJECT_DEBUG
    indent();
    std::cout << "vector " << demangle(typeid(T).name()) << std::endl;
#endif
    *this << size;
    for (T const &v : thing)
        *this << v;
#if BYTEOBJECT_DEBUG
    indent();
    std::cout << "vector end" << std::endl;
#endif
    return *this;
}

template <typename... Ts>
ByteObject &ByteObject::operator<<(std::tuple<Ts...> const &thing) {
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif
    std::apply([this](auto &&... args) {((*this << args), ...);}, thing);
    return *this;
}

template <typename T>
ByteObject &ByteObject::operator<<(std::list<T> const &thing) {
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif
    uint32_t size = thing.size();
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "list " << demangle(typeid(T).name()) << std::endl;
    #endif
    *this << size;
    for (T const &v : thing)
        *this << v;
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "list end" << std::endl;
    #endif
    return *this;
}

template <typename T, typename U>
ByteObject &ByteObject::operator<<(std::map<T, U> const &thing) {
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif
    uint32_t size = thing.size();
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "map " << demangle(typeid(T).name()) << " " << demangle(typeid(U).name()) << std::endl;
    #endif
    *this << size;
    for (std::pair<T, U> const &v : thing)
        *this << v.first << v.second;
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "map end" << std::endl;
    #endif
    return *this;
}

template <typename T, typename U>
ByteObject &ByteObject::operator<<(std::multimap<T, U> const &thing) {
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif
    uint32_t size = thing.size();
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "multimap " << demangle(typeid(T).name()) << " " << demangle(typeid(U).name()) << std::endl;
    #endif
    *this << size;

    for (std::pair<T, U> const &v : thing)
        *this << v.first << v.second;
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "multimap end" << std::endl;
    #endif
    return *this;
}

template <typename T, typename U>
ByteObject &ByteObject::operator<<(std::unordered_map<T, U> const &thing) {
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif
    uint32_t size = thing.size();
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "unordered_map " << demangle(typeid(T).name()) << " " << demangle(typeid(U).name()) << std::endl;
    #endif
    *this << size;

    for (std::pair<T, U> const &v : thing) {
        *this << v.first << v.second;
    }
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "unordered_map end" << std::endl;
    #endif
    return *this;
}

template <typename T>
ByteObject &ByteObject::operator<<(std::set<T> const &thing) {
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif
    uint32_t size = thing.size();
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "set " << demangle(typeid(T).name()) << std::endl;
    #endif
    *this << size;
    for (T const &v : thing)
        *this << v;
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "set end" << std::endl;
    #endif
    return *this;
}

template <typename T, typename U>
ByteObject &ByteObject::operator<<(std::set<T, U> const &thing) {
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif
    uint32_t size = thing.size();
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "set " << demangle(typeid(T).name()) << std::endl;
    #endif
    *this << size;
    for (T const &v : thing)
        *this << v;
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "set end" << std::endl;
    #endif
    return *this;
}

template <typename T>
ByteObject &ByteObject::operator<<(std::unordered_set<T> const &thing) {
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif
    uint32_t size = thing.size();
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "unordered_set " << demangle(typeid(T).name()) << std::endl;
    #endif
    *this << size;
    for (T const &v : thing)
        *this << v;
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "unordered_set end" << std::endl;
    #endif

    return *this;
}

template <typename T>
ByteObject &ByteObject::operator<<(std::shared_ptr<T> const &thing) {
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "shared ptr " << demangle(typeid(T).name()) << std::endl;
    #endif
    *this << thing.get();
    if (_ptrMap.find((void*)thing.get()) == _ptrMap.end()) {
        #if BYTEOBJECT_DEBUG
        Depth depthL(&_depth);
        indent();
        std::cout << "shared ptr new " << thing.get() << std::endl;
        #endif
        *this << *thing;
        _ptrMap.emplace((void*)thing.get(), _size);
    }
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "shared ptr end" << std::endl;
    #endif
    return *this;
}

template <typename T>
ByteObject &ByteObject::operator<<(std::unique_ptr<T> const &thing) {
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    indent();
    std::cout << "unique_ptr " << demangle(typeid(T).name()) << std::endl;
    #endif

    *this << *thing;

    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "unique_ptr end" << std::endl;
    #endif
    return *this;
}


template <typename T>
ByteObject &ByteObject::operator<<(T const &thing) {
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif

    writeToByteObjectT(&thing, sizeof(T));

    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << sizeof(T) << " " << demangle(typeid(T).name()) << " ";
    for (uint i = 0; i < sizeof(thing); i++) {
        uint8_t a = *(((uint8_t *)&thing)+i);
        std::cout << std::setfill('0') << std::setw(2) << std::hex << (int)a << " ";
    }
    std::cout << std::endl;
    #endif
    return *this;
}

template <typename T>
void ByteObject::writeToByteObjectT(T const *thing, size_t size) {
    memcpy((T*)thing, thing, 0); // DO NOT REMOVE: You forgot to overload "operator<<()" on the class specified bellow
    // if (_debugMod)
    //     addDebugSymbol<T>();
    writeToByteObject(thing, size);
}

template <typename T>
ByteObject &ByteObject::operator>>(std::vector<T> &thing) {
    thing.clear();
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif
    uint32_t size;
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "vector " << demangle(typeid(T).name()) << std::endl;
    #endif
    *this >> size;
    thing.resize(size);
    for (T &v : thing)
        *this >> v;
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "vector end" << std::endl;
    #endif
    return *this;
}


template <typename... Ts>
ByteObject &ByteObject::operator>>(std::tuple<Ts...> &thing) {
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif
    std::apply([this](auto &&... args) {((*this >> args), ...);}, thing);
    return *this;
}

template <typename T>
ByteObject &ByteObject::operator>>(std::list<T> &thing) {
    thing.clear();
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif
    uint32_t size;
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "list " << demangle(typeid(T).name()) << std::endl;
    #endif
    *this >> size;
    thing.resize(size);
    for (T &v : thing)
        *this >> v;
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "list end" << std::endl;
    #endif
    return *this;
}

template <typename T, typename U>
ByteObject &ByteObject::operator>>(std::map<T, U> &thing) {
    thing.clear();
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif
    uint32_t size;
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "map " << demangle(typeid(T).name()) << " " << demangle(typeid(U).name()) << std::endl;
    #endif
    *this >> size;
    for (uint32_t i = 0; i < size; i++) {
        T t;
        U u;
        *this >> t >> u;
        thing.emplace(t, u);
    }
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "map end" << std::endl;
    #endif
    return *this;
}

template <typename T, typename U>
ByteObject &ByteObject::operator>>(std::multimap<T, U> &thing) {
    thing.clear();
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif
    uint32_t size;
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "multimap " << demangle(typeid(T).name()) << " " << demangle(typeid(U).name()) << std::endl;
    #endif
    *this >> size;

    for (uint32_t i = 0; i < size; i++) {
        T t;
        U u;
        *this >> t >> u;
        thing.emplace(t, u);
    }
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "multimap end" << std::endl;
    #endif
    return *this;
}

template <typename T, typename U>
ByteObject &ByteObject::operator>>(std::unordered_map<T, U> &thing) {
    thing.clear();
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif
    uint32_t size;
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "unordered_map " << demangle(typeid(T).name()) << " " << demangle(typeid(U).name()) << std::endl;
    #endif
    *this >> size;
    for (uint32_t i = 0; i < size; i++) {
        T t;
        U u;
        *this >> t >> u;
        thing.emplace(t, u);
    }
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "unordered_map end" << std::endl;
    #endif
    return *this;
}

template <typename T>
ByteObject &ByteObject::operator>>(std::set<T> &thing) {
    thing.clear();
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif
    uint32_t size;
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "set " << demangle(typeid(T).name()) << std::endl;
    #endif
    *this >> size;

    for (uint32_t i = 0; i < size; i++) {
        T t;
        *this >> t;
        thing.emplace(t);
    }
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "set end" << std::endl;
    #endif
    return *this;
}

template <typename T, typename U>
ByteObject &ByteObject::operator>>(std::set<T, U> &thing) {
    thing.clear();
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif
    uint32_t size;
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "set " << demangle(typeid(T).name()) << std::endl;
    #endif
    *this >> size;

    for (uint32_t i = 0; i < size; i++) {
        T t;
        *this >> t;
        thing.emplace(t);
    }
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "set end" << std::endl;
    #endif
    return *this;
}

template <typename T>
ByteObject &ByteObject::operator>>(std::unordered_set<T> &thing) {
    thing.clear();
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif
    uint32_t size;
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "unordered_set " << demangle(typeid(T).name()) << std::endl;
    #endif
    *this >> size;
    for (uint32_t i = 0; i < size; i++) {
        T t;
        *this >> t;
        thing.emplace(t);
    }
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "unordered_set end" << std::endl;
    #endif
    return *this;
}

template <typename T>
ByteObject &ByteObject::operator>>(std::shared_ptr<T> &thing) {
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif
    T* ptr;
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "shared ptr " << demangle(typeid(T).name()) << std::endl;
    #endif
    *this >> ptr;
    if (_ptrMapRead.find((void*)ptr) == _ptrMapRead.end()) {
        std::shared_ptr<T> shared = std::make_shared<T>();
        #if BYTEOBJECT_DEBUG
        Depth depthL(&_depth);
        indent();
        std::cout << "shared ptr new " << ptr << std::endl;
        #endif
        *this >> *shared;
        _ptrMapRead.emplace((void *)ptr, shared);
    }
    thing = std::static_pointer_cast<T>(_ptrMapRead[(void *)ptr]);
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "shared ptr end" << std::endl;
    #endif

    return *this;
}


template <typename T>
ByteObject &ByteObject::operator>>(std::unique_ptr<T> &thing) {
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    indent();
    std::cout << "shared ptr " << demangle(typeid(T).name()) << std::endl;
    #endif

    thing = std::make_unique<T>();
    *this >> *thing;

    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "shared ptr end" << std::endl;
    #endif

    return *this;
}

template <typename T>
ByteObject &ByteObject::operator>>(T &thing) {
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif

    readFromByteObjectT(&thing, sizeof(T));

    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << sizeof(T) << " " << demangle(typeid(T).name()) << " ";
    for (uint i = 0; i < sizeof(thing); i++) {
        uint8_t a = *(((uint8_t *)&thing)+i);
        std::cout << std::setfill('0') << std::setw(2) << std::hex << (int)a << " ";
    }
    std::cout << std::endl;
    #endif

    return *this;
}



template <typename T>
void ByteObject::readFromByteObjectT(T *thing, size_t size) {
    memcpy(thing, thing, 0);  // DO NOT REMOVE: You forgot to overload "operator>>()" on the class specified bellow
    // if (_debugMod)
    //     matchDebugSymbole<T>();
    readFromByteObject(thing, size);
}
