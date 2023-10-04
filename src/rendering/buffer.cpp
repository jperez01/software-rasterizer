#include "buffer.h"

void Buffer::set(int x, int y, float value)
{
	data[x + m_width * y] = value;
}
