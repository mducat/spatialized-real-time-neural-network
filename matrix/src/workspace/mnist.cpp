//
// Created by thornhill on 31/12/24.
//

#include <launcher.hpp>

#include "debug.hpp"
#include "ByteObject.hpp"
#include "ByteObject.tcc"
#include "dataset/loaders/mnist_loader.hpp"

int main(int ac, char **av) {
    launcher::init();

    auto const path = "../data/mnist/train-labels-idx1-ubyte";
    auto const labels_shape = std::make_shared<Tensor::Shape>(1);
    auto test = loadMNIST(path, 2049, labels_shape);

    qDebug() << "test size:" << test.size();

    qDebug() << "first label:" << test.at(0)->data()[0];
    qDebug() << "second label:" << test.at(1)->data()[0];
    qDebug() << "third label:" << test.at(2)->data()[0];
    qDebug() << "fo label:" << test.at(3)->data()[0];
    qDebug() << "fi label:" << test.at(4)->data()[0];

    auto const path_imgs = "../data/mnist/train-images-idx3-ubyte";
    auto const image_shape = std::make_shared<Tensor::Shape>(7, 7);
    auto test2 = loadMNIST(path_imgs, 2051, image_shape);

    qDebug() << "test2 size:" << test2.size();
    qDebug() << "test2 shape:" << test2.at(0)->shape()->count();

    // ifs >> test;

    // qDebug() << "Magic: " << magic;
    // qDebug() << "Qty: " << qty;
    // qDebug() << "Label: " << label;
}
