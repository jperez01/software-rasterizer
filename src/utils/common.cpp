//
// Created by Juan Perez on 10/8/23.
//

#include "common.h"
#include <iostream>

Color::Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A) :
        r(R), g(G), b(B), a(A), bytespp(Format::RGBA) { }

Color::Color(Color &color) : r(color.r), g(color.g), b(color.b), a(color.a), bytespp(color.bytespp) {}

Color::~Color() {}

unsigned int toUInt(const std::string& str) {
    char* end_ptr = nullptr;

    auto val = (unsigned int) strtol(str.c_str(), &end_ptr, 10);

    if (*end_ptr != '\0')
        std::cout << "Could not parse floating point from string \n";

    return val;
}

std::vector<std::string> tokenize(const std::string& s, const std::string& delim, bool includeEmpty) {
    std::vector<std::string> tokens;

    std::string::size_type lastPos = 0, position = s.find_first_of(delim, lastPos);
    while (lastPos != std::string::npos) {
        if (position != lastPos || includeEmpty) {
            tokens.push_back(s.substr(lastPos, position - lastPos));
        }
        lastPos = position;
        if (lastPos != std::string::npos) {
            lastPos += 1;
            position = s.find_first_of(delim, lastPos);
        }
    }

    return tokens;
}
