//
// Created by Juan Perez on 10/28/23.
//

#ifndef SOFTWARE_RASTERIZER_CAMERA_H
#define SOFTWARE_RASTERIZER_CAMERA_H

#include <glm/glm.hpp>

const float YAW = -90.0f, PITCH = 0.0f, SENSITIVITY = 0.1f;

enum CameraMovement {
    FORWARD, BACKWARD, LEFT, RIGHT
};

class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW, float pitch = PITCH);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void processKeyboard(CameraMovement direction, float deltaTime);
    void proccessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void processMouseScroll(float yoffset);

    void updateVectors();

private:
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_right;
    glm::vec3 m_up;

    glm::vec3 m_worldUp;

    float m_yaw, m_pitch, m_zoom;
};


#endif //SOFTWARE_RASTERIZER_CAMERA_H
