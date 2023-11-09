//
// Created by Juan Perez on 10/9/23.
//

#include "renderer.h"
#include <glm/gtx/string_cast.hpp>

Renderer::Renderer(int width, int height, Camera& camera) : m_camera(camera), m_framebuffer(width, height),
    m_model("../resources/african_head.obj"), m_albedo("../resources/african_head_diffuse.tga") {
    m_program.onFragment(fragFunction);
    m_program.onVertex(vertFunction);
}

void Renderer::handleRendering() {
    m_framebuffer.clear();
    glm::mat4 viewMatrix = m_camera.getViewMatrix();

    Uniforms uniforms{};
    uniforms.albedo = &m_albedo;
    uniforms.view = viewMatrix;
    uniforms.model = glm::mat4(1.0f);
    uniforms.proj = glm::mat4(1.0f);

    m_program.setUniforms(uniforms);

    for (int i = 0; i < m_model.nfaces() / 3; i++) {
        std::vector<Varying> vertOutputs(3);
        for (int j = 0; j < 3; j++) {
            uint32_t face = m_model.face(i * 3 + j);

            Inputs inputs{};
            inputs.texCoords = m_model.texCoords(face);
            inputs.normal = glm::vec3(1.0f);
            inputs.pos = m_model.vert(face);

            Varying output{};
            m_program.doVertexShader(inputs, output);

            vertOutputs[j] = output;
        }
        triangle(vertOutputs);
    }
}

void Renderer::triangle(std::vector<Varying> vertOutputs) {
    std::vector<glm::ivec2> points(3);
    for (int i = 0; i < 3; i++)
        points[i] = convertToScreenCoords(m_framebuffer.getWidth(), m_framebuffer.getHeight(), vertOutputs[i].position);

    glm::ivec2 bboxmin(m_framebuffer.getWidth() - 1, m_framebuffer.getHeight() - 1);
    glm::ivec2 bboxmax(0, 0);
    glm::ivec2 clamp = bboxmin;

    for (int i = 0; i < 3; i++) {
        bboxmin.x = std::max(0, std::min(bboxmin.x, points[i].x));
        bboxmin.y = std::max(0, std::min(bboxmin.y, points[i].y));

        bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, points[i].x));
        bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, points[i].y));
    }

    glm::ivec2 P;
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
            glm::vec3 baryCoords = barycentricCoords(points, P);
            if (baryCoords.x < 0 || baryCoords.y < 0 || baryCoords.z < 0) continue;

            float interpolatedZ = baryCoords.x * vertOutputs[0].position.z + baryCoords.y * vertOutputs[1].position.z +
                                  baryCoords.z * vertOutputs[2].position.z;

            if (interpolatedZ <= m_framebuffer.zValueAt(P.x, P.y)) {
                continue;
            }

            Varying interpolatedValues{};
            interpolatedValues.texCoords = baryCoords.x * vertOutputs[0].texCoords + baryCoords.y * vertOutputs[1].texCoords +
                                           baryCoords.z * vertOutputs[2].texCoords;
            Color foundColor = m_program.doFragmentShader(interpolatedValues);

            m_framebuffer.setZValue(P.x, P.y, interpolatedZ);
            m_framebuffer.setColorValue(P.x, P.y, foundColor);
        }
    }
}