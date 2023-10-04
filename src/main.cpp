#include "utils/tgaimage.h"
#include "utils/model.h"
#include "rendering/buffer.h"

#include <vector>
#include <iostream>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const int width = 200, height = 200;
int pixelsDiscarded = 0;

Vec2i convertToScreenCoords(Vec3f worldCoords) {
	return Vec2i((worldCoords.x + 1.0f) * width / 2.0f, (worldCoords.y + 1.0f) * height / 2.0f);
}

Vec3f barycentricCoords(std::vector<Vec2i>& points, Vec2i& currentPoint) {
	Vec2i v0 = points[1] - points[0], v1 = points[2] - points[0], v2 = points[0] - currentPoint;

	Vec3f u = Vec3f(v0.x, v1.x, v2.x) ^ Vec3f(v0.y, v1.y, v2.y);

	if (std::abs(u.z) < 1) return Vec3f(-1, 1, 1);

	return Vec3f(1.0f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

void line(int x0, int x1, int y0, int y1, TGAImage& image, TGAColor color) {
	bool steep = false;

	// If line is steep, transpose the image
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	// Make for loop go from lowest to highest x values
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx = x1 - x0, dy = y1 - y0;
	int derror = std::abs(dy) * 2, error = 0;
	int y = y0;
	const int yincr = (y1 > y0) ? 1 : -1;

	for (int x = x0; x <= x1; x++) {
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}

		error += derror;
		if (error > dx) {
			y += yincr;
			error -= dx * 2;
		}
	}
}

void triangle(std::vector<Vec3f> worldCoords, Buffer& zBuffer, TGAImage& image, TGAColor color) {
	std::vector<Vec2i> points(3);
	for (int i = 0; i < 3; i++)
		points[i] = convertToScreenCoords(worldCoords[i]);

	Vec2i bboxmin(image.get_width() - 1, image.get_height() - 1);
	Vec2i bboxmax(0, 0);
	Vec2i clamp = bboxmin;

	for (int i = 0; i < 3; i++) {
		bboxmin.x = std::max(0, std::min(bboxmin.x, points[i].x));
		bboxmin.y = std::max(0, std::min(bboxmin.y, points[i].y));

		bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, points[i].x));
		bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, points[i].y));
	}

	Vec2i P;
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
			Vec3f baryCoords = barycentricCoords(points, P);
			if (baryCoords.x < 0 || baryCoords.y < 0 || baryCoords.z < 0) continue;

			float interpolatedZ = baryCoords.x * worldCoords[0].z + baryCoords.y * worldCoords[1].z +
				baryCoords.z * worldCoords[2].z;

			if (interpolatedZ < zBuffer.at(P.x, P.y)) {
				pixelsDiscarded++;
				continue;
			}

			zBuffer.set(P.x, P.y, interpolatedZ);
			image.set(P.x, P.y, color);
		}
	}
}

int main(int argc, char** argv) {
	TGAImage image(width, height, TGAImage::RGB);
	Buffer zBuffer(width, height);
	Model model("../../../resources/african_head.obj");

	Vec2i screenDimensions(width, height);

	for (int i = 0; i < model.nfaces(); i++) {
		std::vector<int> face = model.face(i);

		std::vector<Vec3f> worldCoords(3);
		for (int j = 0; j < 3; j++) {
			worldCoords[j] = model.vert(face.at(j));
		}

		triangle(worldCoords, zBuffer, image, TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
	}

	std::cout << pixelsDiscarded << "\n";

	image.flip_vertically();
	image.write_tga_file("output.tga");

	return 0;
}