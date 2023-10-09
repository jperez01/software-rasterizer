//
// Created by Juan Perez on 10/9/23.
//

#pragma once

#include "buffer.h"
#include "utils/common.h"

#include <iostream>

class Framebuffer {
public:
    Framebuffer(int width, int height);

    void clear();

    void* getColorBuffer() { return colorBuffer.get(); }
    void* getZBuffer() { return zBuffer.get(); }

    int getWidth() { return m_width; }
    int getHeight() { return m_height; }

    float zValueAt(int x, int y);
    Color colorAt(int x, int y);

    bool setZValue(int x, int y, float value);
    bool setColorValue(int x, int y, const Color& color);
private:
    std::unique_ptr<float[]> zBuffer;
    std::unique_ptr<unsigned char[]> colorBuffer;

    int m_width, m_height;
};
