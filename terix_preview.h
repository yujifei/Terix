/***************************************************
>Author: 庾吉飞
>Mail: yujihui.xidian@gmail.com
>Created Time: 2014年01月31日 星期五 04时35分08秒
****************************************************/

#include "terix_shape.h"
#include <QWidget>
#include <QPaintEvent>

class TerixShapePreview: public QWidget
{
	Q_OBJECT
public:
	TerixShapePreview(QWidget* parent = NULL);
	void setShape(const ITerixShape* shape);

protected:
	void paintEvent(QPaintEvent* event);
	void resizeEvent(QResizeEvent* event);

private:
	std::vector<TerixPosition> m_posEnum;
};
