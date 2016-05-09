/*****************************************************
>File Name: terix_preview.cpp
>Author: 庾吉飞
>Mail: yujihui.xidian@gmail.com
>Created Time: 2014年01月31日 星期五 05时05分26秒
****************************************************/

#include "terix_preview.h"
#include "config.h"
#include <climits>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QRect>
#include <QSize>
#include <QWidget>

#include <cstdio>

TerixShapePreview::TerixShapePreview(QWidget* parent): QWidget(parent)
{
}

void TerixShapePreview::setShape(const ITerixShape* shape)
{
	TerixPosition p = shape->center();
	m_posEnum =shape->getPointEnum();
	for (int i = 0; i < (int)m_posEnum.size(); ++i)
	{
		m_posEnum[i].m_x += TerixPreviewWidth / 2.0 - p.m_x;
		m_posEnum[i].m_y += TerixPreviewHeight / 2.0 - p.m_y;
	}
	update();
}

void TerixShapePreview::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.fillRect(rect(), TerixPreviewBackground);

	int gridWidth = qMin((int)(rect().width() / TerixPreviewWidth), 
						 (int)(rect().height() / TerixPreviewHeight));
	int xOffset = (rect().width() - gridWidth * TerixPreviewWidth) / 2.0 + 0.5;
	int yOffset = (rect().height() - gridWidth * TerixPreviewHeight) / 2.0 + 0.5;

	QPen pen(TerixPreviewGridBorderColor);
	pen.setWidth(1);
	painter.setPen(pen);
	QBrush brush(TerixPreviewGridBorderColor, TerixPreviewGridStyle);

	for (int i = 0; i < (int)m_posEnum.size(); ++i)
	{
		int x = m_posEnum[i].x();
		int y = m_posEnum[i].y();
		QRect r(xOffset + x * gridWidth, yOffset + y * gridWidth, gridWidth, gridWidth);
		painter.fillRect(r, brush);
		painter.drawRect(r);
	}

	QWidget::paintEvent(event);
}

void TerixShapePreview::resizeEvent(QResizeEvent* event)
{
	int newWidth = qMin(event->size().width(), event->size().height());
	resize(QSize(newWidth, newWidth));
	QWidget::resizeEvent(event);
}
