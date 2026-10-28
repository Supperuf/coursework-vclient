#ifndef VECTORLOADER_H
#define VECTORLOADER_H
#include <string>
#include <vector>
class VectorLoader {
public:
    static std::vector<std::vector<double>> load(const std::string& filename);
};
#endif
