//
// Created by Juan Perez on 10/8/23.
//

#include "geometry.h"

glm::ivec2 convertToScreenCoords(int width, int height, glm::vec3 worldCoords) {
    return {(worldCoords.x + 1.0f) * width / 2.0f, (worldCoords.y + 1.0f) * height / 2.0f};
}

glm::vec3 barycentricCoords(std::vector<glm::ivec2>& points, glm::ivec2& currentPoint) {
    glm::ivec2 v0 = points[2] - points[0], v1 = points[1] - points[0], v2 = points[0] - currentPoint;

    glm::vec3 u = glm::cross(glm::vec3(v0.x, v1.x, v2.x), glm::vec3(v0.y, v1.y, v2.y));

    if (std::fabs(u.z) < 0.01f) return glm::vec3(-1.0f, 1.0f, 1.0f);

    return glm::vec3(1.0f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

glm::mat4 lookAtMatrix(const glm::vec3& from, const glm::vec3& to, const glm::vec3& up) {
    glm::vec3 forward = glm::normalize(from - to);

    glm::vec3 arbitraryVector = glm::vec3(0, 1, 0);
    glm::vec3 right = glm::cross(arbitraryVector, forward);
    right = glm::normalize(right);

    glm::vec3 newUp = glm::cross(forward, right);


    return glm::mat4(right.x, newUp.y, forward.z, from.x,
                     right.x, newUp.y, forward.z, from.y,
                     right.x, newUp.y, forward.z, from.z,
                     0, 0, 0, 1);
}