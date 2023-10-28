//
// Created by Juan Perez on 10/9/23.
//

#include "renderer.h"

Renderer::Renderer(int width, int height) : m_screen(width, height), m_framebuffer(width, height),
m_model("../resources/african_head.obj"), m_albedo("../resources/african_head_diffuse.tga") {
}

void Renderer::handleRendering() {
    while (m_screen.isOpen()) {
        m_screen.handleEvent();

        m_framebuffer.clear();

        for (int i = 0; i < m_model.nfaces() / 3; i++) {
            std::vector<glm::vec3> worldCoords;
            worldCoords.resize(3);
            for (int j = 0; j < 3; j++) {
                uint32_t face = m_model.face(i * 3 + j);
                worldCoords[j] = m_model.vert(face);
            }

            glm::vec2 texCoords = m_model.texCoords(m_model.face(i * 3));
            Color someColor = m_albedo.getDataAt(texCoords);
            triangle(worldCoords, someColor);
        }

        m_screen.updateFrame(m_framebuffer.getColorBuffer());
    }
}

void Renderer::triangle(std::vector<glm::vec3> worldCoords, const Color& color) {
    std::vector<glm::ivec2> points(3);
    for (int i = 0; i < 3; i++)
        points[i] = convertToScreenCoords(m_framebuffer.getWidth(), m_framebuffer.getHeight(), worldCoords[i]);

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

            float interpolatedZ = baryCoords.x * worldCoords[0].z + baryCoords.y * worldCoords[1].z +
                                  baryCoords.z * worldCoords[2].z;

            if (interpolatedZ <= m_framebuffer.zValueAt(P.x, P.y)) {
                continue;
            }

            m_framebuffer.setZValue(P.x, P.y, interpolatedZ);
            m_framebuffer.setColorValue(P.x, P.y, color);
        }
    }
}

void Renderer::cleanUp() {
    m_screen.cleanUp();
}
