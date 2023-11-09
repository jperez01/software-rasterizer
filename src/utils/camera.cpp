//
// Created by Juan Perez on 10/28/23.
//

#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : m_position(position), m_worldUp(up), m_yaw(yaw), m_pitch(pitch) {
    updateVectors();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 Camera::getProjectionMatrix() {
    return glm::perspective(glm::radians(m_zoom), 1.0f, 1.0f, 100.0f);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    switch (direction) {
        case FORWARD:
            m_position += deltaTime * m_front;
            break;
        case BACKWARD:
            m_position -= deltaTime * m_front;
            break;
        case RIGHT:
            m_position += deltaTime * m_right;
            break;
        case LEFT:
            m_position -= deltaTime * m_right;
            break;
        default:
            break;
    }
}

void Camera::proccessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= SENSITIVITY;
    yoffset *= SENSITIVITY;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (constrainPitch) {
        if (m_pitch > 89.0f) m_pitch = 89.0f;
        if (m_pitch < -89.0f) m_pitch = -89.0f;
    }

    updateVectors();
}

void Camera::processMouseScroll(float yoffset) {
    m_zoom -= yoffset;

    if (m_zoom < 1.0f) m_zoom = 1.0f;
    if (m_zoom > 45.0f) m_zoom = 45.0f;
}

void Camera::updateVectors() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    direction.y = sin(glm::radians(m_pitch));
    direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_front = glm::normalize(direction);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
