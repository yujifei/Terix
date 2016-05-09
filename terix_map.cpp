/*****************************************************
>File Name: terix_map.cpp
>Author: 庾吉飞
>Mail: yujihui.xidian@gmail.com
>Created Time: 2014年01月27日 星期一 02时54分23秒
****************************************************/

#include "terix_map.h"
#include "terix_shape.h"
#include <cassert>

TerixMap::TerixMap(): m_currentShape(0)
{
}

void TerixMap::init(int width, int height)
{
	assert(width < 64);
	m_width = width;
	m_height = height;
	m_terixMap.resize(height);
	clear();
}

int TerixMap::reduce()
{
	int lineCount = 0;
	std::vector<TerixLineType>::iterator iter = m_terixMap.begin();
	while (iter != m_terixMap.end())
	{
		if ((*iter + 1) & ~(1 << m_width))
		{
			++iter;
			continue;
		}

		auto it = iter;
		for (; it != m_terixMap.begin(); --it)
			*it = *(it - 1);
		*it = 0;
		++lineCount;
	}

	return lineCount;
}

bool TerixMap::get(int x, int y) const
{
	if (x < 0)
		return true;
	if (x >= m_width)
		return true;
	if (y < 0)
		return false;
	if (y >= m_height)
		return true;
	return m_terixMap[y] & (1 << x);
}

bool TerixMap::set(int x, int y)
{
	if (y >= 0 && x >= 0 && y < m_height && x < m_width)
	{
		m_terixMap[y] |= 1 << x;
		return true;
	}
	return false;
}

bool TerixMap::clear(int x, int y)
{
	if (y >= 0 && x >= 0 && y < m_height && x < m_width)
	{
		m_terixMap[y] &= ~(1 << x);
		return true;
	}
	return false;
}

void TerixMap::clear()
{
	for (int i = 0; i < (int)m_terixMap.size(); ++i)
		m_terixMap[i] = 0;
}

int TerixMap::getWidth() const
{
	return m_width;
}

int TerixMap::getHeight() const
{
	return m_height;
}

bool TerixMap::getForDraw(int x, int y) const
{
	if (m_currentShape)
	 {
		std::vector<TerixPosition> positions = m_currentShape->getPointEnum();
		for (auto iter = positions.begin(); iter != positions.end(); ++iter)
	 	{
			if (iter->x() == x && iter->y() == y)
				return !get(x, y);
		}
	}
	return get(x, y);
}

void TerixMap::setShape(ITerixShape* shape)
{
	assert(shape);
	m_currentShape = shape;
}

bool TerixMap::freezeShape()
{
	bool result = m_currentShape->freeze();
	m_currentShape = 0;
	return result;
}
