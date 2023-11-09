//
// Created by Juan Perez on 10/8/23.
//

#include "application.h"

#include <iostream>

Application::Application(int width, int height) : m_renderer(width, height, m_camera), m_width(width), m_height(height), m_isOpen(true) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Error: " << SDL_GetError() << "\n";
    }
    m_window = SDL_CreateWindow("Software Rasterizer", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, width, height, 0);

    Uint32 renderFlags = SDL_RENDERER_ACCELERATED;
    m_sdlRenderer = SDL_CreateRenderer(m_window, -1, renderFlags);

    m_windowTexture = SDL_CreateTexture(m_sdlRenderer,
                                        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
}

void Application::mainLoop() {
    while (m_isOpen) {
        handleEvent();

        m_renderer.handleRendering();

        updateFrame(m_renderer.getFramebufferData());
    }
}

void Application::handleEvent() {
    SDL_Event event;
    SDL_Keycode type;
    float deltaTime = 0.01f;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            m_isOpen = false;
        } else if (event.type == SDL_KEYDOWN) {
            type = event.key.keysym.sym;

            if (type == SDLK_UP)
                m_camera.processKeyboard(FORWARD, deltaTime);
            if (type == SDLK_DOWN)
                m_camera.processKeyboard(BACKWARD, deltaTime);
            if (type == SDLK_LEFT)
                m_camera.processKeyboard(LEFT, deltaTime);
            if (type == SDLK_RIGHT)
                m_camera.processKeyboard(RIGHT, deltaTime);
        } else if (event.type == SDL_MOUSEWHEEL) {
            m_camera.processMouseScroll(event.wheel.y);
        } else if (event.type == SDL_MOUSEMOTION) {
            handleMouseMovement(event.motion.x, event.motion.y);
        }
    }
}

void Application::updateFrame(void* data) {
    SDL_RenderClear(m_sdlRenderer);

    SDL_UpdateTexture(m_windowTexture, nullptr, data, m_width * 4 * sizeof(char));
    SDL_RenderCopyEx(m_sdlRenderer, m_windowTexture, nullptr, nullptr, 0, nullptr, SDL_FLIP_VERTICAL);
    SDL_RenderPresent(m_sdlRenderer);
}

void Application::cleanUp() {
    SDL_DestroyTexture(m_windowTexture);
    SDL_DestroyRenderer(m_sdlRenderer);
    SDL_DestroyWindow(m_window);
}

void Application::handleMouseMovement(float xPosIn, float yPosIn) {
    if (firstMouse) {
        lastX = xPosIn;
        lastY = yPosIn;
        firstMouse = false;
    }

    float xOffset = xPosIn - lastX;
    float yOffset = lastY - yPosIn;
    lastX = xPosIn;
    lastY = yPosIn;

    m_camera.proccessMouseMovement(xOffset, yOffset);
}
