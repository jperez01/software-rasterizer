//
// Created by Juan Perez on 11/8/23.
//

#ifndef SOFTWARE_RASTERIZER_SIMPLESHADER_H
#define SOFTWARE_RASTERIZER_SIMPLESHADER_H

#include "shaders/shader.h"

struct Inputs {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Varying {
    glm::vec4 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Uniforms {
    Texture* albedo;

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

auto vertFunction = [](const Uniforms& uniforms, const Inputs& in, Varying& out) {
    out.position = uniforms.proj * uniforms.view * uniforms.model * glm::vec4(in.pos, 1.0f);
    out.texCoords = in.texCoords;
    out.normal = in.normal;
};

auto fragFunction = [](const Uniforms& uniforms, const Varying& in) -> Color {
    Color albedoColor = uniforms.albedo->getDataAt(in.texCoords);

    return uniforms.albedo->getDataAt(in.texCoords);
};

#endif //SOFTWARE_RASTERIZER_SIMPLESHADER_H
