#include "utils/tgaimage.h"
#include "utils/model.h"
#include "rendering/buffer.h"
#include "utils/screen.h"
#include "utils/geometry.h"
#include "rendering/texture.h"

#include <vector>
#include <iostream>

const int width = 500, height = 500;
int pixelsDiscarded = 0;

void triangle(std::vector<glm::vec3> worldCoords, Buffer& zBuffer, TGAImage& image, TGAColor color) {
	std::vector<glm::ivec2> points(3);
	for (int i = 0; i < 3; i++)
		points[i] = convertToScreenCoords(width, height, worldCoords[i]);

    glm::ivec2 bboxmin(image.get_width() - 1, image.get_height() - 1);
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

			if (interpolatedZ <= zBuffer.at(P.x, P.y)) {
				pixelsDiscarded++;
				continue;
			}

            zBuffer.set(P.x, P.y, interpolatedZ);
			image.set(P.x, P.y, color);
		}
	}
}

int main(int argc, char** argv) {
	TGAImage image(width, height, TGAImage::RGBA);
	Buffer zBuffer(width, height);
	Model model("../resources/african_head.obj");
    Texture someTexture("../resources/african_head_diffuse.tga");

    std::vector<TGAColor> colors;

    Screen screen(width, height);

    while (screen.isOpen()) {
        screen.handleEvent();

        image.clear();
        zBuffer.clear();

        for (int i = 0; i < model.nfaces() / 3; i++) {
            std::vector<glm::vec3> worldCoords(3);
            for (int j = 0; j < 3; j++) {
                uint32_t face = model.face(i * 3 + j);
                worldCoords[j] = model.vert(face);
            }

            if (colors.size() <= i) {
                colors.emplace_back(rand() % 255, rand() % 255, rand() % 255, 255);
            }

            glm::vec2 texCoords = model.texCoords(model.face(i * 3));
            Color someColor = someTexture.getDataAt(texCoords);
            TGAColor convertedColor(someColor.r, someColor.g, someColor.b, someColor.a);
            triangle(worldCoords, zBuffer, image, convertedColor);
        }

        screen.updateFrame(image.buffer());
    }

    std::cout << pixelsDiscarded << "\n";

	image.flip_vertically();
	image.write_tga_file("output.tga");

	return 0;
}