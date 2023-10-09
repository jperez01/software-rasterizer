//
// Created by Juan Perez on 10/8/23.
//

#pragma once

#include <SDL2/SDL.h>

class Screen {
public:
    Screen(int width, int height);

    void handleEvent();
    void updateFrame(void* data);
    bool isOpen() { return m_isOpen; }

private:
    bool m_isOpen;
    int m_width, m_height;

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Texture* m_windowTexture;
};

