#include "rendering/renderer.h"

int main(int argc, char** argv) {
    const int width = 500, height = 500;
    Renderer renderer(width, height);

    renderer.handleRendering();
    renderer.cleanUp();

	return 0;
}