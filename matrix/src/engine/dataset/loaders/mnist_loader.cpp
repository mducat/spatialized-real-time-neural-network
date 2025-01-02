//
// Created by thornhill on 02/01/25.
//

#include <fstream>

#include "mnist_loader.hpp"
#include "debug.hpp"
#include "ByteObject.hpp"
#include "ByteObject.tcc"

int reverse(int i)
{
    unsigned int val = i;

    if (sizeof(val) == 4)
        val = ((val & 0xff) << 24) | ((val & 0xff00) << 8) | ((val & 0xff0000) >> 8) | ((val & 0xff000000) >> 24);
    return val;
}

int readFile(const char *&filename, ByteObject *obj) {
    auto file = std::ifstream(filename);

    if (!file.is_open()) {
        qDebug() << "Unable to open " << filename;
        return 1;
    }

    std::ifstream getsize(filename, std::ifstream::ate);
    auto size = getsize.tellg();

    obj->_data.resize(size);
    file.read((char *)&(obj->_data[0]), size);
    return 0;
}

std::vector<std::shared_ptr<Tensor>> loadMNIST(const char *filename, int expected_magic, std::shared_ptr<Tensor::Shape> shape) {
    ByteObject obj;
    std::vector<std::shared_ptr<Tensor>> items;

    if (readFile(filename, &obj)) {
        return items;
    }

    int32_t magic;
    int32_t qty;

    obj >> magic;
    obj >> qty;

    magic = reverse(magic);
    qty = reverse(qty);

    if (magic != expected_magic) {
        std::string const details = "(" + std::to_string(magic) + " != " + std::to_string(expected_magic) + ")";
        throw std::runtime_error("MNIST file does not match expected magic number " + details);
    }

    std::shared_ptr<DataSet> ds;

    std::size_t shape_count = shape->count();

    for (int i = 0; i < qty; i++) {
        std::vector<float> values;

        for (std::size_t index = 0; index < shape_count; index++) {
            uint8_t byte;

            obj >> byte;
            values.push_back(byte);
        }
        auto tensor = std::make_shared<Tensor>(std::move(values), shape);

        items.push_back(tensor);
    }

    // @todo add warning left data

    return items;
}
