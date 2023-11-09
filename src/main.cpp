#include "core/application.h"

int main(int argc, char** argv) {
    const int width = 500, height = 500;
    Application app(width, height);

    app.mainLoop();
    app.cleanUp();

	return 0;
}