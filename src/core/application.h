//
// Created by Juan Perez on 10/8/23.
//

#pragma once

#include <SDL.h>
#include "utils/camera.h"
#include "rendering/renderer.h"

class Application {
public:
    Application(int width, int height);

    void mainLoop();

    void handleEvent();
    void updateFrame(void* data);
    void cleanUp();

    void handleMouseMovement(float xPosIn, float yPosIn);

    bool isOpen() { return m_isOpen; }

private:
    bool m_isOpen;
    int m_width, m_height;

    bool firstMouse;
    float lastX, lastY;

    Renderer m_renderer;
    Camera m_camera;

    SDL_Window* m_window;
    SDL_Renderer* m_sdlRenderer;
    SDL_Texture* m_windowTexture;
};

