
#include <ostream>
#include <fstream>
#include "ByteObject.tcc"


// ByteObject
ByteObject::ByteObject()
{
    // *this << debugMod;
    // _debugMod = debugMod;

}

ByteObject::ByteObject(uint size)
{
    reserve(size);
    // *this << debugMod;
    // _debugMod = debugMod;
}

ByteObject::ByteObject(std::vector<uint8_t> const &msg, uint size) :
    _data(msg), _size(size)
{
    // *this >> _debugMod;
}

ByteObject::ByteObject(std::vector<uint8_t> const &msg) :
    _data(msg), _size(msg.size())
{
    // *this >> _debugMod;
}

ByteObject::ByteObject(uint8_t const *msg, uint size) :
    _size(size)
{
    _data.resize(_size);
    memcpy(&_data[0], msg, _size);
    // *this >> _debugMod;
}

void ByteObject::writeToByteObject(void const *thing, size_t size) {
    _data.resize(_size + size);
    uint8_t *ptr = &_data[_size];
    memcpy(ptr, thing, size);
    _size += size;
}

void ByteObject::readFromByteObject(void *thing, size_t size) {
    uint8_t *ptr = &_data[_cursor];
    memcpy(thing, ptr, size);
    _cursor += size;
}

std::vector<uint8_t> const &ByteObject::dumpForNetwork() const {
    return _data;
}

void ByteObject::setCursor(int val) {
    _cursor = val;
}

void ByteObject::clear() {
    _data.clear();
    _size = 0;
    _cursor = 0;
    _ptrMap.clear();
    _ptrMapRead.clear();
    // if (clearDebug) {
    //     *this << _debugMod;
    // }
}

void ByteObject::reserve(size_t const &size) {
    _data.reserve(size);
}

#if BYTEOBJECT_DEBUG
void ByteObject::indent() const {
    for (uint i = 0; i < _depth; i++)
        std::cout << " ";
}

std::string ByteObject::demangle(char const *name) const {
    char *str = abi::__cxa_demangle(name, 0, 0, &status);
    std::string string = str;
    free(str);
    return string;
}
#endif

ByteObject &ByteObject::operator+(const ByteObject &that) const 
{
    int newSize = that._size + this->_size;
    std::vector<uint8_t> newData;
    newData.insert(newData.end(), this->_data.begin(), this->_data.end());
    newData.insert(newData.end(), that._data.begin(), that._data.end());
    ByteObject *obj = new ByteObject(newData, newSize);
    return *obj;
}


ByteObject &ByteObject::operator<<(ByteObject const &thing) {
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    indent();
    std::cout << "ByteObject size=" << thing._size << std::endl;
    #endif
    *this << thing._size;
    writeToByteObjectT(thing._data.data(), thing._data.size());

    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "ByteObject end" << std::endl;
    #endif
    return *this;
}

ByteObject &ByteObject::operator<<(std::string const &thing) {
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif
    uint32_t size = thing.size();
    *this << size;
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << size << " std::string " << std::endl;
    #endif
    writeToByteObjectT(&thing[0], size);

    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "string = " << thing << std::endl;
    #endif
    return *this;
};

ByteObject &ByteObject::operator>>(ByteObject &thing) {
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    indent();
    std::cout << "ByteObject size=" << thing._size << std::endl;
    #endif
    thing.clear();
    *this >> thing._size;
    thing._data.resize(thing._size);
    readFromByteObjectT(thing._data.data(), thing._size);
    // thing >> thing._debugMod;

    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "ByteObject end" << std::endl;
    #endif
    return *this;
}

ByteObject &ByteObject::operator>>(std::string &thing) {
    #if BYTEOBJECT_DEBUG
    Depth depthL(&_depth);
    #endif
    uint32_t size;
    *this >> size;
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << size << " std::string " << std::endl;
    #endif

    thing.resize(size);
    readFromByteObjectT(&thing[0], size);
    #if BYTEOBJECT_DEBUG
    indent();
    std::cout << "string = " << thing << std::endl; 
    #endif
    return *this;
};


std::basic_istream<char> &operator>>(std::basic_istream<char> &stream, ByteObject &obj) {
    obj.clear();
    stream.read((char *)&obj._size, sizeof(obj._size));
    obj._data.resize(obj._size);
    stream.read((char *)&(obj._data[0]), obj._size);
    // obj >> obj._debugMod;
    return stream;
}

std::basic_ostream<char> &operator<<(std::basic_ostream<char> &stream, const ByteObject &obj)
{
    stream.write((char *)&obj._size, sizeof(obj._size));
    stream.write((char *)&(obj._data[0]), obj._size);
    return stream;
}   
//



// ByteObjectSize
void ByteObjectSize::writeToByteObject(void const *thing, size_t size) {
    (void)thing;
    _size += size;
};

void ByteObjectSize::readFromByteObject(void *thing, size_t size) {
    (void)thing;
    _cursor += size;
}
//

void writeAll(int fd, uint8_t const *buf, size_t size) {
    while (size > 0) {
        int wirteSize = write(fd, buf, size);
        if (wirteSize < 0) {
            perror("Byteobject write failed");
            throw std::runtime_error("Byteobject write failed");
        }
        size -= wirteSize;
        buf += wirteSize;
    }
}
void readAll(int fd, uint8_t *buf, size_t size) {
    while (size > 0) {
        int readSize = read(fd, buf, size);
        if (readSize < 0) {
            perror("Byteobject read failed");
            throw std::runtime_error("Byteobject read failed");
        }
        size -= readSize;
        buf += readSize;
    }   
}


// ByteObjectStream
ByteObjectStream::ByteObjectStream(int fd, size_t iBuffer, size_t oBuffer) :
    _iBuffer(new uint8_t[iBuffer]),
    _iBufferSize(iBuffer),
    _oBuffer(new uint8_t[oBuffer]),
    _oBufferSize(oBuffer),
    _fd(fd)
{}

ByteObjectStream::~ByteObjectStream() {
    flush();
    delete[] _iBuffer;
    delete[] _oBuffer;
}

void ByteObjectStream::flush() {
    writeAll(_fd, _iBuffer+_icursorL, _icursorR-_icursorL);
    _icursorL = 0;
    _icursorR = 0;
}

void ByteObjectStream::writeBuffer(uint8_t const *buf, size_t size) {
    while (size) {
        if (_icursorR == 0 && size >= _iBufferSize) {
            writeAll(_fd, buf, size);
            size = 0;
        } else if (size > _iBufferSize - _icursorR) {
            flush();
        } else {
            memcpy(_iBuffer+_icursorR, buf, size);
            _icursorR += size;
            size = 0;
        }
    }
}

void ByteObjectStream::readBuffer(uint8_t *buf, size_t size) {
    if (size > _oBufferSize) { // if bigger than buffer empty buffer and directly read in obj
        int readSize = _ocursorR-_ocursorL;
        memcpy(buf, _oBuffer+_ocursorL, readSize);
        _ocursorR = 0;
        _ocursorL = 0;
        buf += readSize;
        size -= readSize;
        readAll(_fd, buf, size);
    } else {
        if (size <= (size_t)(_ocursorR-_ocursorL)) { // if entirelly contain in buffer just copy from buffer 
            memcpy(buf, _oBuffer+_ocursorL, size);
            _ocursorL += size;
            size = 0;
        } else { // partially contain in buffer
            int bufferDataSize = _ocursorR-_ocursorL;
            memcpy(buf, _oBuffer+_ocursorL, bufferDataSize); // copy buffer part
            size -= bufferDataSize;
            buf += bufferDataSize;
            _ocursorL = 0;
            _ocursorR = 0;
            while ((size_t)_ocursorR < size) { // fill buffer
                int ret = read(_fd, _oBuffer+_ocursorR, _oBufferSize-_ocursorR);
                if (ret < 0)
                    throw std::runtime_error("Byteobject read failed");
                _ocursorR +=  ret;
            }
            memcpy(buf, _oBuffer, size); // copy rest
            _ocursorL += size;
        }
    }


}


void ByteObjectStream::writeToByteObject(void const *thing, size_t size) {
    // writeAll(_fd, (uint8_t *)thing, size);
    writeBuffer((uint8_t const *)thing, size);
    _size += size;
};

void ByteObjectStream::readFromByteObject(void *thing, size_t size) {
    // readAll(_fd, (uint8_t*)thing, size);
    readBuffer((uint8_t*)thing, size);
    _cursor += size;
}
//