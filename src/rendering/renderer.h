//
// Created by Juan Perez on 10/9/23.
//

#pragma once

#include "framebuffer.h"
#include "utils/model.h"
#include "texture.h"
#include "utils/camera.h"
#include "shaders/simpleShader.h"

class Renderer {
public:
    Renderer(int width, int height, Camera& camera);

    void handleRendering();
    void triangle(std::vector<Varying> vertOutputs);

    void* getFramebufferData() { return m_framebuffer.getColorBuffer(); }

private:
    Framebuffer m_framebuffer;
    Camera& m_camera;

    Model m_model;
    Texture m_albedo;

    Program<Inputs, Varying, Uniforms> m_program;
};

