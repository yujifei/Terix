/*****************************************************
>File Name: terix_map.h
>Author: 庾吉飞
>Mail: yujihui.xidian@gmail.com
>Created Time: 2014年01月27日 星期一 02时00分56秒
****************************************************/

#ifndef __FILE_TERIX_MAP__
#define __FILE_TERIX_MAP__

#include<cstdint>
#include<vector>

class ITerixShape;
typedef uint64_t TerixLineType;

class TerixMap
{
public:
	TerixMap();
	~TerixMap(){}
	void init(int width, int height);
	int reduce();
	bool get(int row, int col) const;
	bool getForDraw(int row, int col) const;
	bool set(int row, int col);
	bool clear(int row, int col);
	void clear();
	int getWidth() const;
	int getHeight() const;
	void setShape(ITerixShape* shape);
	bool freezeShape();

private:
	std::vector<TerixLineType> m_terixMap;
	int m_width;
	int m_height;
	ITerixShape* m_currentShape;
};

#endif
