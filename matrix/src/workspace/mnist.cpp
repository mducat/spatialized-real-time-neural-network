//
// Created by thornhill on 31/12/24.
//

#include <fstream>
#include <launcher.hpp>

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

int main(int ac, char **av) {
    launcher::init();
    ByteObject test;

    auto path = "../data/mnist/train-labels-idx1-ubyte";

    std::ifstream ifs(path);

    if (!ifs.is_open()) {
        qDebug() << "Unable to open file";
        return 0;
    }

    std::ifstream getsize(path, std::ifstream::ate);
    auto size = getsize.tellg();

    test._data.resize(size);
    ifs.read((char *)&(test._data[0]), size);

    // ifs >> test;

    int32_t magic;
    int32_t qty;

    test >> magic;
    test >> qty;

    magic = reverse(magic);
    qty = reverse(qty);

    uint8_t label;
    test >> label;
    test >> label;
    test >> label;

    qDebug() << "Magic: " << magic;
    qDebug() << "Qty: " << qty;
    qDebug() << "Label: " << label;
}
