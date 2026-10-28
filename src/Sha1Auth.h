#ifndef SHA1AUTH_H
#define SHA1AUTH_H
#include <string>
class Sha1Auth {
public:
    static std::string computeSHA1(const std::string& salt, const std::string& password);
};
#endif
