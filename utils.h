#ifndef PISKVORKY_UTILS_H
#define PISKVORKY_UTILS_H


#include <string>
#include <sstream>

// https://gist.github.com/shanehsu/a45eb3e4bc8a09145bce
template <typename T>
std::string to_string(const T& value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

#endif //PISKVORKY_UTILS_H
