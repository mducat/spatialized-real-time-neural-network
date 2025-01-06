//
// Created by thornhill on 31/12/24.
//

#include <dataset_entry.hpp>
#include <integrate_fire.hpp>
#include <launcher.hpp>
#include <project.hpp>

#include "debug.hpp"
#include "loss.hpp"
#include "data/dataset.hpp"
#include "dataset/loaders/mnist_loader.hpp"

int main(int ac, char **av) {
    launcher::init();

    auto const path = "../data/mnist/train-labels-idx1-ubyte";
    auto const labels_shape = std::make_shared<Tensor::Shape>(1);
    auto labels_loaded = loadMNIST(path, 2049, labels_shape);

    qDebug() << "labels count:" << labels_loaded.size();

    auto const path_imgs = "../data/mnist/train-images-idx3-ubyte";
    auto const image_shape = std::make_shared<Tensor::Shape>(7, 7);
    auto images_loaded = loadMNIST(path_imgs, 2051, image_shape);

    qDebug() << "images count:" << images_loaded.size();

    auto const &project = std::make_shared<Project>();

    auto const &data_layer = project->createLayer(LayerType::DATA);
    auto const &network_layer = project->createLayer(LayerType::NETWORK);

    auto const &ds = data_layer->create<Dataset>(images_loaded, labels_loaded);

    std::vector<std::shared_ptr<DatasetEntry>> inputs;

    for (int index = 0; index < image_shape->count(); index++) {
        auto const &input = network_layer->create<DatasetEntry>(ds, index);

        inputs.push_back(input);
    }

    std::vector<std::shared_ptr<IntegrateFire>> layer_1;

    int const layer_1_size = 12;
    for (int i = 0; i < layer_1_size; i++) {
        auto const &node = network_layer->create<IntegrateFire>();

        for (int index = 0; index < image_shape->count(); index++) {
            node->addInput(inputs[index]);
        }

        layer_1.push_back(node);
    }

    for (int index = 0; index < image_shape->count(); index++) {
        auto const &input = network_layer->create<DatasetEntry>(ds, index);

        inputs.push_back(input);
    }

    auto const &output = network_layer->create<DatasetEntry>(ds, 0, 1);

    auto const &loss = network_layer->create<Loss<QuadraticLoss>>(output, layer_1[0]);

    project->init();
    project->step(1);

    qDebug() << "READ INPUT:" << inputs[3]->value();
    qDebug() << "READ LAYER:" << layer_1[3]->value();

    project->step(1000);
    project->step(1000);
    project->step(1000);
    qDebug() << "READ INPUT:" << inputs[3]->value();
    qDebug() << "READ LAYER:" << layer_1[3]->value();
    qDebug() << "LOSS VALUE:" << loss->value();

}
