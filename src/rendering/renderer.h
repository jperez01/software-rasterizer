//
// Created by Juan Perez on 10/9/23.
//

#pragma once

#include "framebuffer.h"
#include "utils/screen.h"
#include "utils/model.h"
#include "texture.h"

class Renderer {
public:
    Renderer(int width, int height);

    void cleanUp();
    void handleRendering();
    void triangle(std::vector<glm::vec3> worldCoords, const Color& color);
private:
    Framebuffer m_framebuffer;
    Screen m_screen;

    Model m_model;
    Texture m_albedo;
};

