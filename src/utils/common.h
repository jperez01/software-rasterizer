//
// Created by Juan Perez on 10/8/23.
//
#pragma once

#include <vector>
#include <string>

enum Format {
    GRAYSCALE = 1, RGB = 3, RGBA = 4
};

struct Color {
    unsigned char r, g, b, a;
    int bytespp;

    Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A);
    Color(Color& color);

    ~Color();
};

unsigned int toUInt(const std::string& str);

std::vector<std::string> tokenize(const std::string& s, const std::string& delim = ", ", bool includeEmpty = false);
