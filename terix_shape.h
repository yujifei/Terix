/*****************************************************
>File Name: terix_shape.h
>Author: 庾吉飞
>Mail: yujihui.xidian@gmail.com
>Created Time: 2014年01月27日 星期一 02时10分34秒
****************************************************/

#ifndef __FILE_TERIX_SHAPE__
#define __FILE_TERIX_SHAPE__

#include "terix_map.h"
#include <vector>

const double PI = 3.14159265358979;

struct TerixPosition
{
	TerixPosition(double _x, double _y)
	{
		this->m_x = _x;
		this->m_y = _y;
	}
	int x() {return int(m_x + 0.5);}
	int y() {return int(m_y + 0.5);}

	double m_x, m_y;
};

enum TerixShapeType
{
	TerixShapeTypeBox,       //4个点的小方形
	TerixShapeTypeL,         //L形
	TerixShapeTypeAntiL,     //反L形
	TerixShapeTypeZ,         //Z形
	TerixShapeTypeAntiZ,     //反Z形
	TerixShapeTypeStick,     //4个点的杆
	TerixShapeTypeShortT,    //短T形
	TerixShapeTypeT,         //T形
	TerixShapeTypeLongZ,     //长Z形
	TerixShapeTypeAntiLongZ, //反长Z形
	TerixShapeTypeCross,     //十字形
	TerixShapeTypeCorner,    //三点组成的拐角
	TerixShapeTypeAdder,     //可以给当前列增加方块
	TerixShapeTypeDistroyer, //可以消掉当前列方块
	TerixShapeTypeBomb,      //可以炸掉堆积的方块
	TerixShapeTypeGoThrough, //具有穿透能力的小点
	TerixModeSimple = TerixShapeTypeT,
	TerixModeHighLevel = TerixShapeTypeGoThrough + 1,
	TerixShapeNum = TerixModeHighLevel
};

class ITerixShape
{
public:
	virtual void init() = 0;
	virtual void attachMap(TerixMap*) = 0;
	virtual void moveDown() = 0;
	virtual void moveLeft() = 0;
	virtual void moveRight() = 0;
	virtual void reshape() = 0;
	virtual bool stop() = 0;
	virtual bool freeze() = 0;
	virtual std::vector<TerixPosition> getPointEnum() const = 0;
	virtual TerixPosition center() const = 0;
};

class TerixShapeBase: public ITerixShape
{
public:
	typedef std::vector<TerixPosition>::iterator PosIter;

public:
	void init();
	void attachMap(TerixMap* map);
	void moveDown();
	void moveLeft();
	void moveRight();
	void reshape();
	bool stop();
	bool freeze();
	std::vector<TerixPosition> getPointEnum() const;
	TerixPosition center() const;

private:
	void translate(double dx, double dy);
	bool rotate(double angle);

public:
	std::vector<TerixPosition> m_points;
	TerixMap* m_terixMap;
};

class TerixShapeBox: public TerixShapeBase
{
public:
	void init();
	void reshape();
};

class TerixShapeL: public TerixShapeBase
{
public:
	void init();
};

class TerixShapeAntiL: public TerixShapeBase
{
public:
	void init();
};

class TerixShapeZ: public TerixShapeBase
{
public:
	void init();
};

class TerixShapeAntiZ: public TerixShapeBase
{
public:
	void init();
};

class TerixShapeStick: public TerixShapeBase
{
public:
	void init();
};

class TerixShapeShortT: public TerixShapeBase
{
public:
	void init();
};

class TerixShapeT: public TerixShapeBase
{
public:
	void init();
};

class TerixShapeLongZ: public TerixShapeBase
{
public:
	void init();
};

class TerixShapeAntiLongZ: public TerixShapeBase
{
public:
	void init();
};

class TerixShapeCross: public TerixShapeBase
{
public:
	void init();
};

class TerixShapeCorner: public TerixShapeBase
{
public:
	void init();
};

class TerixShapeBomb: public TerixShapeBase
{
public:
	void init();
	void reshape();
	bool freeze();
};

class TerixShapeAdder: public TerixShapeBase
{
public:
	void init();
	void reshape();
	bool freeze();
};

class TerixShapeDistroyer: public TerixShapeBase
{
public:
	void init();
	void reshape();
	bool freeze();
};

class TerixShapeGoThrough: public TerixShapeBase
{
public:
	void init();
	void reshape();
	bool stop();
};

#endif
