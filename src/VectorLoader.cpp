#include "VectorLoader.h"
#include <fstream>
#include <stdexcept>

std::vector<std::vector<double>> VectorLoader::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Не удалось открыть файл: " + filename);
    std::vector<std::vector<double>> vectors;
    unsigned int num_vectors;
    if (!(file >> num_vectors)) throw std::runtime_error("Ошибка чтения количества");
    for (unsigned int i = 0; i < num_vectors; ++i) {
        unsigned int size;
        if (!(file >> size)) throw std::runtime_error("Ошибка чтения размера");
        std::vector<double> vec(size);
        for (unsigned int j = 0; j < size; ++j) {
            if (!(file >> vec[j])) throw std::runtime_error("Ошибка чтения элемента");
        }
        vectors.push_back(vec);
    }
    return vectors;
}
