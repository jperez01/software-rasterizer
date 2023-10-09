//
// Created by Juan Perez on 10/8/23.
//

#include "texture.h"
#include "stb_image.h"

#include <iostream>

Texture::Texture(std::string path) {
    int width, height, nrComponents;

    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        m_data = data;
        m_width = width;
        m_height = height;
        m_numComponents = nrComponents;
    } else {
        std::cout << "Texture failed to load at path: " << path << "\n";
        stbi_image_free(data);
    }
}

Color Texture::getDataAt(glm::vec2 coords) {
    int convertedX = coords.x * m_width, convertedY = coords.y * m_height;

    int startIndex = convertedX + convertedY * m_width;

    Color resultColor(m_data[startIndex], m_data[startIndex+1], m_data[startIndex+2], m_data[startIndex+3]);

    return resultColor;
}
