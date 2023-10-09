//
// Created by Juan Perez on 10/9/23.
//

#include "framebuffer.h"

Framebuffer::Framebuffer(int width, int height) {
    m_width = width;
    m_height = height;

    zBuffer = std::make_unique<float[]>(width * height);
    colorBuffer = std::make_unique<unsigned char[]>(width * height * 4);
}

float Framebuffer::zValueAt(int x, int y) {
    if (zBuffer) {
        return zBuffer.get()[x + y * m_width];
    }

    return 0.0f;
}

Color Framebuffer::colorAt(int x, int y) {
    if (colorBuffer) {
        unsigned char* pointer = colorBuffer.get();
        int index = (x + y * m_width) * 4;
        return {
                pointer[index],
                pointer[index+1],
                pointer[index+2],
                pointer[index+3]
        };
    }

    return { 0, 0, 0, 0};
}

bool Framebuffer::setZValue(int x, int y, float value) {
    if (!zBuffer || x<0 || y<0 || x>=m_width || y>=m_height) {
        return false;
    }
    zBuffer.get()[x + y * m_width] = value;

    return true;
}

bool Framebuffer::setColorValue(int x, int y, const Color& color) {
    if (!colorBuffer || x<0 || y<0 || x>=m_width || y>=m_height) {
        return false;
    }

    unsigned char rawData[4];
    rawData[0] = color.r;
    rawData[1] = color.g;
    rawData[2] = color.b;
    rawData[3] = color.a;
    memcpy(colorBuffer.get()+(x+y*m_width)*4, rawData, 4);

    return true;
}

void Framebuffer::clear() {
    memset((void *)zBuffer.get(), 0, m_width*m_height * 4);
    memset(colorBuffer.get(), 0, m_width * m_height * 4);
}
