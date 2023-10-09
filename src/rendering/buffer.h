#pragma once

#include <vector>

class Buffer {
public:
	Buffer(int width, int height) : m_width(width), m_height(height) {
		data = std::vector<float>(width * height);
	}

	inline float at(int x, int y) {
		return data.at(x + m_width * y);
	}

	void set(int x, int y, float value);
    void clear();

private:
	int m_width, m_height;
	std::vector<float> data;
};