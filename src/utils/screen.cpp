//
// Created by Juan Perez on 10/8/23.
//

#include "screen.h"

#include <iostream>

Screen::Screen(int width, int height) : m_width(width), m_height(height), m_isOpen(true) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Error: " << SDL_GetError() << "\n";
    }
    m_window = SDL_CreateWindow("Software Rasterizer", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, width, height, 0);

    Uint32 renderFlags = SDL_RENDERER_ACCELERATED;
    m_renderer = SDL_CreateRenderer(m_window, -1, renderFlags);

    m_windowTexture = SDL_CreateTexture(m_renderer,
                                                   SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
}

void Screen::handleEvent() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: {
                m_isOpen = false;
                break;
            }
            default:
                break;
        }
    }
}

void Screen::updateFrame(void* data) {
    SDL_RenderClear(m_renderer);

    SDL_UpdateTexture(m_windowTexture, nullptr, data, m_width * 4 * sizeof(char));
    SDL_RenderCopyEx(m_renderer, m_windowTexture, nullptr, nullptr, 0, nullptr, SDL_FLIP_VERTICAL);
    SDL_RenderPresent(m_renderer);
}

void Screen::cleanUp() {
    SDL_DestroyTexture(m_windowTexture);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
}
