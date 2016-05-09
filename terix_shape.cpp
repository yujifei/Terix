/*****************************************************
>File Name: terix_shape.cpp
>Author: 庾吉飞
>Mail: yujihui.xidian@gmail.com
>Created Time: 2014年01月28日 星期二 00时03分15秒
****************************************************/

#include "terix_shape.h"
#include <cstdlib>
#include <cassert>
#include <ctime>
#include <climits>
#include <cmath>
#include <cstdio>

void TerixShapeBase::init()
{
	srand(time(0));
	int angle = rand() % 4;
	while (angle--)
		reshape();
	assert(m_terixMap->getWidth() > 0);
	translate(m_terixMap->getWidth() / 2, 0);

	int maxy = -100;
	for (PosIter iter = m_points.begin(); iter != m_points.end(); ++iter)
		if (iter->y() > maxy)
			maxy = iter->y();
	if (maxy >= 0)
		for (PosIter iter = m_points.begin(); iter != m_points.end(); ++iter)
			iter->m_y -= maxy + 1;
}

void TerixShapeBase::attachMap(TerixMap* map)
{
	m_terixMap = map;
}

void TerixShapeBase::moveDown()
{
	for (PosIter iter = m_points.begin(); iter != m_points.end(); ++iter)
		iter->m_y += 1;
}

void TerixShapeBase::moveLeft()
{
	for (PosIter iter = m_points.begin(); iter != m_points.end(); ++iter)
	{
		if (m_terixMap->get(iter->x() - 1, iter->y()))
			return;
	}

	for (PosIter iter = m_points.begin(); iter != m_points.end(); ++iter)
		iter->m_x -= 1;
}

void TerixShapeBase::moveRight()
{
	for (PosIter iter = m_points.begin(); iter != m_points.end(); ++iter)
	{
		if (m_terixMap->get(iter->x() + 1, iter->y()))
			return;
	}

	for (PosIter iter = m_points.begin(); iter != m_points.end(); ++iter)
		iter->m_x += 1;
}

void TerixShapeBase::reshape()
{
	bool success = rotate(90);
	if (success)
	{
	}
}

bool TerixShapeBase::stop()
{
	PosIter iter = m_points.begin();
	bool bStop = false;

	while (!bStop && iter != m_points.end())
	{
		bStop = bStop || iter->y() + 1 >= m_terixMap->getHeight();
		bStop = bStop || m_terixMap->get(iter->x(), iter->y() + 1);
		++iter;
	}

	return bStop;
}

bool TerixShapeBase::freeze()
{
	bool result = true;
	for (PosIter iter = m_points.begin(); iter != m_points.end(); ++iter)
	{
		if (iter->x() < 0)
			result = false;
		m_terixMap->set(iter->x(), iter->y());
	}
	return result;
}

std::vector<TerixPosition> TerixShapeBase::getPointEnum() const
{
	return m_points;
}

TerixPosition TerixShapeBase::center() const
{
	double minx = m_terixMap->getWidth(), maxx = 0;
    double miny = m_terixMap->getHeight(), maxy = 0;

	for (auto iter = m_points.begin(); iter != m_points.end(); ++iter)
	{
		if (iter->m_x > maxx)
			maxx = iter->m_x;
		if (iter->m_x < minx)
			minx = iter->m_x;
		if (iter->m_y > maxy)
			maxy = iter->m_y;
		if (iter->m_y < miny)
			miny = iter->m_y;
	}

	return TerixPosition((minx + maxx)/ 2, (miny + maxy) / 2);
}

void TerixShapeBase::translate(double dx, double dy)
{
	for (PosIter iter = m_points.begin(); iter != m_points.end(); ++iter)
	{ 
		iter->m_x += dx;
		iter->m_y += dy;
	}
}

bool TerixShapeBase::rotate(double angle)
{
	TerixPosition centPos = center();
	double cosTheta = cos(angle / 180 * PI);
	double sinTheta = sin(angle / 180 * PI);

	std::vector<TerixPosition> points = m_points;
	translate(-centPos.x(), -centPos.y());
	for (auto iter = m_points.begin(); iter != m_points.end(); ++iter)
	{
		double x = iter->m_x, y = iter->m_y;
		iter->m_x = x * cosTheta - y * sinTheta;
		iter->m_y = y * cosTheta + x * sinTheta;
	}
	translate(centPos.x(), centPos.y());

	bool failed = false;
	for (auto iter = m_points.begin(); iter != m_points.end(); ++iter)
	{
		failed = failed || (iter->y() < 0);
		failed = failed || (iter->y() >= m_terixMap->getWidth());
		failed = failed || (iter->x() >= m_terixMap->getHeight());
		if (iter->x() >= 0 && iter->x() < m_terixMap->getHeight())
			failed = failed || (m_terixMap->get(iter->x(), iter->y()));
	}
	if (failed)
		m_points = points;
	return !failed;
}

void TerixShapeBox::init()
{
	m_points.clear();
	m_points.push_back(TerixPosition(-0, -2));
	m_points.push_back(TerixPosition(-0, -1));
	m_points.push_back(TerixPosition(-1, -2));
	m_points.push_back(TerixPosition(-1, -1));

	TerixShapeBase::init();
}

void TerixShapeBox::reshape()
{
	//Do nothing
}

void TerixShapeL::init()
{
	m_points.clear();
	m_points.push_back(TerixPosition(-0, -2));
	m_points.push_back(TerixPosition(-0, -1));
	m_points.push_back(TerixPosition(-1, -1));
	m_points.push_back(TerixPosition(-2, -1));
	
	TerixShapeBase::init();
}

void TerixShapeAntiL::init()
{
	m_points.clear();
	m_points.push_back(TerixPosition(+1, -1));
	m_points.push_back(TerixPosition(-0, -1));
	m_points.push_back(TerixPosition(-1, -1));
	m_points.push_back(TerixPosition(-1, -2));

	TerixShapeBase::init();
}

void TerixShapeZ::init()
{
	m_points.clear();
	m_points.push_back(TerixPosition(+1, -1));
	m_points.push_back(TerixPosition(-0, -1));
	m_points.push_back(TerixPosition(-0, -2));
	m_points.push_back(TerixPosition(-1, -2));

	TerixShapeBase::init();
}

void TerixShapeAntiZ::init()
{
	m_points.clear();
	m_points.push_back(TerixPosition(+1, -2));
	m_points.push_back(TerixPosition(-0, -2));
	m_points.push_back(TerixPosition(-0, -1));
	m_points.push_back(TerixPosition(-1, -1));

	TerixShapeBase::init();
}

void TerixShapeStick::init()
{
	m_points.clear();
	m_points.push_back(TerixPosition(0, -1));
	m_points.push_back(TerixPosition(0, -2));
	m_points.push_back(TerixPosition(0, -3));
	m_points.push_back(TerixPosition(0, -4));

	TerixShapeBase::init();
}

void TerixShapeShortT::init()
{
	m_points.clear();
	m_points.push_back(TerixPosition(+1, -1));
	m_points.push_back(TerixPosition(-0, -1));
	m_points.push_back(TerixPosition(-0, -2));
	m_points.push_back(TerixPosition(-1, -1));

	TerixShapeBase::init();
}

void TerixShapeT::init()
{
	m_points.clear();
	m_points.push_back(TerixPosition(+1, -1));
	m_points.push_back(TerixPosition(-0, -1));
	m_points.push_back(TerixPosition(-0, -2));
	m_points.push_back(TerixPosition(-0, -3));
	m_points.push_back(TerixPosition(-1, -1));

	TerixShapeBase::init();
}

void TerixShapeLongZ::init()
{
	m_points.clear();
	m_points.push_back(TerixPosition(+1, -1));
	m_points.push_back(TerixPosition(-0, -1));
	m_points.push_back(TerixPosition(-0, -2));
	m_points.push_back(TerixPosition(-0, -3));
	m_points.push_back(TerixPosition(-1, -3));

	TerixShapeBase::init();
}

void TerixShapeAntiLongZ::init()
{
	m_points.clear();
	m_points.push_back(TerixPosition(+1, -3));
	m_points.push_back(TerixPosition(-0, -3));
	m_points.push_back(TerixPosition(-0, -2));
	m_points.push_back(TerixPosition(-0, -1));
	m_points.push_back(TerixPosition(-1, -1));

	TerixShapeBase::init();
}

void TerixShapeCross::init()
{
	m_points.clear();
	m_points.push_back(TerixPosition(+1, -2));
	m_points.push_back(TerixPosition(-0, -1));
	m_points.push_back(TerixPosition(-0, -2));
	m_points.push_back(TerixPosition(-0, -3));
	m_points.push_back(TerixPosition(-1, -2));

	TerixShapeBase::init();
}

void TerixShapeCorner::init()
{
	m_points.clear();
	m_points.push_back(TerixPosition(+1, -1));
	m_points.push_back(TerixPosition(-0, -1));
	m_points.push_back(TerixPosition(-0, -2));

	TerixShapeBase::init();
}

void TerixShapeBomb::init()
{
	m_points.clear();
	m_points.push_back(TerixPosition(+1, -3));
	m_points.push_back(TerixPosition(-0, -1));
	m_points.push_back(TerixPosition(-0, -2));
	m_points.push_back(TerixPosition(-1, -1));
	m_points.push_back(TerixPosition(-1, -2));
	m_points.push_back(TerixPosition(-2, -3));

	TerixShapeBase::init();
}

void TerixShapeBomb::reshape()
{
}

bool TerixShapeBomb::freeze()
{
	int minx = INT_MAX, maxx = INT_MIN, miny = INT_MAX, maxy = INT_MIN;
	for (PosIter iter = m_points.begin(); iter != m_points.end(); ++iter)
	{
		if (iter->x() < minx)
			minx = iter->x();
		if (iter->x() > maxx)
			maxx = iter->x();
		if (iter->y() < miny)
			miny = iter->y();
		if (iter->y() > maxy)
			maxy = iter->y();
	}

	for (int i = minx - 1; i <= maxx + 1; ++i)
		for (int j = miny - 1; j  <= maxy + 1; ++j)
			m_terixMap->clear(i, j);
	return true;
}

void TerixShapeAdder::init()
{
	m_points.clear();
	m_points.push_back(TerixPosition(0, -1));
	m_points.push_back(TerixPosition(0, -2));
	m_points.push_back(TerixPosition(0, -3));

	TerixShapeBase::init();
}

void TerixShapeAdder::reshape()
{
	if (m_points.back().y() < 0)
		return;

	int x = m_points.back().x();
	int y = 0;
	for (;y < m_terixMap->getHeight(); ++y)
	{
		if (m_terixMap->get(x, y))
			break;
	}
	m_terixMap->set(x, y - 1);
}

bool TerixShapeAdder::freeze()
{
	//Do Nothing
	return true;
}

void TerixShapeDistroyer::init()
{
	m_points.clear();
	m_points.push_back(TerixPosition(0, -1));
	m_points.push_back(TerixPosition(0, -2));

	TerixShapeBase::init();
}

void TerixShapeDistroyer::reshape()
{
	if (m_points.back().y() < 0)
		return;

	for (int y = 0; y < m_terixMap->getHeight(); ++y)
	{
		if (m_terixMap->get(m_points.back().x(), y))
		{
			m_terixMap->clear(m_points.back().x(), y);
			break;
		}
	}
}

bool TerixShapeDistroyer::freeze()
{
	//Do Nothing
	return true;
}

void TerixShapeGoThrough::init()
{
	m_points.clear();
	m_points.push_back(TerixPosition(0, -1));

	TerixShapeBase::init();
}

void TerixShapeGoThrough::reshape()
{
	//Do Nothing
}

bool TerixShapeGoThrough::stop()
{
	int x = m_points.front().x();
	int y = m_points.front().y();

	int yy = -1;
	for(yy = m_terixMap->getHeight() - 1; yy >= 0; --yy)
		if (!m_terixMap->get(x, yy))
			break;

	if (yy < 0 || y == yy)
		return true;
	return false;
}
