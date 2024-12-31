
#pragma once

#define DEBUGINFORMATION std::string(" | from ") + std::string(__FILE__) + std::string(" L") + std::to_string(__LINE__)

#include "ByteObject.hh"
#include <vector>
#include <string.h>
#include <string>
#include <ostream>
#include <exception>
#include <iostream>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <memory>
#include <cxxabi.h>
#include <iomanip>
#include <tuple>


#if _WIN32
//#include <windows.h>
#else
#include <unistd.h>
#endif

#ifdef _WIN32 
typedef unsigned int uint;
#endif 

class ByteObjectError : public std::exception {
    public:
        ByteObjectError(std::string const &message, std::string const &component = "ByteObjectError")
        : _message(message), _component(component) {};

        std::string const &getComponent() const noexcept { return _component;};
        const char *what() const noexcept {return _message.c_str();};
    private:
        std::string _message;
        std::string _component;
};


#if BYTEOBJECT_DEBUG
class Depth {
public:
    Depth(unsigned int *i) : _i(i) {
        (*_i)++;
    }

    ~Depth() {
        (*_i)--;
    }

    unsigned int *_i;
};
#endif

class ByteObject {
public:
    ByteObject();
    ByteObject(uint size);
    ByteObject(std::vector<uint8_t> const &msg, uint size);
    ByteObject(std::vector<uint8_t> const &msg);
    ByteObject(uint8_t const *msg, uint size);

    ~ByteObject(){}

    #if BYTEOBJECT_DEBUG
    void indent() const;
    std::string demangle(char const *name) const;
    #endif

    void reserve(size_t const &size);

    ByteObject &operator+(ByteObject const &that) const;

    ByteObject &operator<<(ByteObject const &thing);

    #if BYTEOBJECT_DEBUG
    template <typename T>
    void addDebugSymbol();
    template <typename T>
    void matchDebugSymbole();
    #endif

    template <typename T>
    ByteObject &operator<<(std::vector<T> const &thing);
    template <typename... Ts>
    ByteObject &operator<<(std::tuple<Ts...> const &thing);
    template <typename T>
    ByteObject &operator<<(std::list<T> const &thing);
    template <typename T, typename U>
    ByteObject &operator<<(std::map<T, U> const &thing);
    template <typename T, typename U>
    ByteObject &operator<<(std::multimap<T, U> const &thing);
    template <typename T, typename U>
    ByteObject &operator<<(std::unordered_map<T, U> const &thing);
    template <typename T>
    ByteObject &operator<<(std::set<T> const &thing);
    template <typename T, typename U>
    ByteObject &operator<<(std::set<T, U> const &thing);
    template <typename T>
    ByteObject &operator<<(std::unordered_set<T> const &thing);
    template <typename T>
    ByteObject &operator<<(std::shared_ptr<T> const &thing);
    template <typename T>
    ByteObject &operator<<(std::unique_ptr<T> const &thing);
    ByteObject &operator<<(std::string const &thing);
    template <typename T>
    ByteObject &operator<<(T const &thing);
    template <typename T>

    void writeToByteObjectT(T const *thing, size_t size);

    virtual void writeToByteObject(void const *thing, size_t size);


    ByteObject &operator>>(ByteObject &thing);

    template <typename T>
    ByteObject &operator>>(std::vector<T> &thing);
    template <typename... Ts>
    ByteObject &operator>>(std::tuple<Ts...> &thing);
    template <typename T>
    ByteObject &operator>>(std::list<T> &thing);
    template <typename T, typename U>
    ByteObject &operator>>(std::map<T, U> &thing);
    template <typename T, typename U>
    ByteObject &operator>>(std::multimap<T, U> &thing);
    template <typename T, typename U>
    ByteObject &operator>>(std::unordered_map<T, U> &thing);
    template <typename T>
    ByteObject &operator>>(std::set<T> &thing);
    template <typename T, typename U>
    ByteObject &operator>>(std::set<T, U> &thing);
    template <typename T>
    ByteObject &operator>>(std::unordered_set<T> &thing);
    template <typename T>
    ByteObject &operator>>(std::shared_ptr<T> &thing);
    template <typename T>
    ByteObject &operator>>(std::unique_ptr<T> &thing);
    ByteObject &operator>>(std::string &thing);
    template <typename T>
    ByteObject &operator>>(T &thing);
    template <typename T>
    void readFromByteObjectT(T *thing, size_t size);

    virtual void readFromByteObject(void *thing, size_t size);


    std::vector<uint8_t> const &dumpForNetwork() const;

    void setCursor(int val);

    void clear();

    #if BYTEOBJECT_DEBUG
    mutable int status;
    mutable uint _depth = 0;

    #endif

    std::vector<uint8_t> _data;
    std::unordered_map<void *, uint32_t> _ptrMap;
    std::unordered_map<void *, std::shared_ptr<void>> _ptrMapRead;
    uint64_t _size = 0;
    uint64_t _cursor = 0;
    // bool _debugMod = false;

private:
};

class ByteObjectSize : public ByteObject {
public:
    void writeToByteObject(void const *thing, size_t size) override;
    void readFromByteObject(void *thing, size_t size) override;
};

class ByteObjectStream : public ByteObject {
public:
    ByteObjectStream(int fd, size_t iBuffer = 2048, size_t oBuffer = 2048);

    ~ByteObjectStream();

    void flush();

    void writeBuffer(uint8_t const *buf, size_t size);
    void readBuffer(uint8_t *buf, size_t size);

    void writeToByteObject(void const *thing, size_t size) override;
    void readFromByteObject(void *thing, size_t size) override;

    uint8_t *_iBuffer;
    size_t _iBufferSize;
    int _icursorL = 0;
    int _icursorR = 0;
    uint8_t *_oBuffer;
    size_t _oBufferSize;
    int _ocursorL = 0;
    int _ocursorR = 0;

    int _fd;
};


// class ByteObjectStream : public ByteObject {}

std::basic_ostream<char> &operator<<(std::basic_ostream<char> &stream, const ByteObject &obj);
std::basic_istream<char> &operator>>(std::basic_istream<char> &stream, ByteObject &obj);
