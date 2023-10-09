#include "buffer.h"

void Buffer::set(int x, int y, float value)
{
	data[x + m_width * y] = value;
}

void Buffer::clear() {
    memset(data.data(), 0, m_width * m_height * sizeof(float));
}
