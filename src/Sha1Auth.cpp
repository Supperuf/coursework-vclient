#include "Sha1Auth.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

std::string Sha1Auth::computeSHA1(const std::string& salt, const std::string& password) {
    std::string data = salt + password;
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);
    std::stringstream ss;
    for(int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        ss << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}
