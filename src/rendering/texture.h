//
// Created by Juan Perez on 10/8/23.
//
#pragma once

#include "utils/common.h"
#include "utils/geometry.h"

#include <string>

class Texture {
public:
    Texture(std::string path);

    Color getDataAt(glm::vec2);

private:
    int m_width, m_height, m_numComponents;
    unsigned char* m_data;
};
