/*****************************************************
>File Name: terix_field.cpp
>Author: 庾吉飞
>Mail: yujihui.xidian@gmail.com
>Created Time: 2014年01月30日 星期四 02时11分45秒
****************************************************/

#include "terix_field.h"
#include "config.h"
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <QPen>
#include <QRect>
#include <QBrush>
#include <QPainter>
#include <QPainterPath>


TerixField::TerixField(QWidget* parent): QWidget(parent)
{
	m_state = TerixGameStateUnstart;
	m_mode  = TerixGameModeSimple;
	m_time  = TerixInitInterval;
	init(TerixDefaultWidth, TerixDefaultHeight);
}

TerixField::~TerixField()
{
	for (int i = 0; i < TerixShapeNum; ++i)
		delete m_shapes[i];
}

QSize TerixField::sizeHint() const
{
	return TerixDefaultSize;
}

QSize TerixField::minimumSizeHint() const
{
	return TerixMinimumSize;
}

void TerixField::init(int width, int height)
{
	m_map.init(width, height);

	m_shapes[TerixShapeTypeBox]       = new TerixShapeBox;
	m_shapes[TerixShapeTypeL]         = new TerixShapeL;
	m_shapes[TerixShapeTypeAntiL]     = new TerixShapeAntiL;
	m_shapes[TerixShapeTypeZ]         = new TerixShapeZ;
	m_shapes[TerixShapeTypeAntiZ]     = new TerixShapeAntiZ;
	m_shapes[TerixShapeTypeStick]     = new TerixShapeStick;
	m_shapes[TerixShapeTypeShortT]    = new TerixShapeShortT;
	m_shapes[TerixShapeTypeT]         = new TerixShapeT;
	m_shapes[TerixShapeTypeLongZ]     = new TerixShapeLongZ;
	m_shapes[TerixShapeTypeAntiLongZ] = new TerixShapeAntiLongZ;
	m_shapes[TerixShapeTypeCross]     = new TerixShapeCross;
	m_shapes[TerixShapeTypeCorner]    = new TerixShapeCorner;
	m_shapes[TerixShapeTypeAdder]     = new TerixShapeAdder;
	m_shapes[TerixShapeTypeDistroyer] = new TerixShapeDistroyer;
	m_shapes[TerixShapeTypeBomb]      = new TerixShapeBomb;
	m_shapes[TerixShapeTypeGoThrough] = new TerixShapeGoThrough;

	m_state = TerixGameStateUnstart;
	m_mode  = TerixGameModeSimple;

	buildTerixGrids(NULL);
	for (int i = 0; i < TerixShapeNum; ++i)
	{
		m_shapes[i]->attachMap(&m_map);
		m_shapes[i]->init();
	}
	emit gameReady();
}

const TerixMap& TerixField::getDocument() const
{
	return m_map;
}

int TerixField::width() const
{
	return m_map.getWidth();
}

int TerixField::height() const
{
	return m_map.getHeight();
}

void TerixField::setTime(double tm)
{
	m_time = tm;
	m_timer.setInterval(tm * 1000);
	if (m_state == TerixGameStateStart)
		m_timer.start();
}

TerixField::TerixGameState TerixField::state() const
{
	return m_state;
}

void TerixField::setState(TerixGameState state)
{
	m_state = state;
}

TerixField::TerixGameMode TerixField::mode() const
{
	return m_mode;
}

void TerixField::setMode(TerixGameMode mode)
{
	m_mode = mode;
}

ITerixShape* TerixField::getPreview() const
{
	return m_nextShape;
}

void TerixField::start()
{
	if (m_state != TerixGameStateUnstart)
		return;

	m_state = TerixGameStateStart;
	srand(time(NULL));
	int shapeId = rand() % m_mode;
	m_currentShape = m_shapes[shapeId];
	shapeId = rand() % m_mode;
	m_nextShape = m_shapes[shapeId];
	m_map.setShape(m_currentShape);
	m_timer.setInterval(m_time * 1000);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(moveShapeDown()));
	m_timer.start();
	update();
	emit gameStart();
	emit newShape();
}

void TerixField::pause()
{
	if (m_state != TerixGameStateStart)
		return;

	m_state = TerixGameStatePause;
	m_timer.stop();
	update();
	emit gamePause();
}

void TerixField::restart()
{
	if (m_state != TerixGameStatePause)
		return;

	m_state = TerixGameStateStart;
	m_timer.start();
	update();
	emit gameRestart();
}

void TerixField::stop()
{
	if (m_state != TerixGameStateStart)
		return;

	m_state = TerixGameStateUnstart;
	m_timer.stop();
	m_currentShape = NULL;
	m_map.clear();
	update();
	emit gameStop();
}

void TerixField::moveShapeLeft()
{
	if (m_state != TerixGameStateStart)
		return;

	m_currentShape->moveLeft();
	update();
}

void TerixField::moveShapeRight()
{
	if (m_state != TerixGameStateStart)
		return;

	m_currentShape->moveRight();
	update();
}

void TerixField::moveShapeDown()
{
	if (m_state != TerixGameStateStart)
		return;
	
	if(m_currentShape->stop())
	{ 
		if(!m_map.freezeShape())
	 	{
			m_timer.stop();
			m_state = TerixGameStateOver;
			emit gameOver();
		}
		else
		{
			int nLine = m_map.reduce();
			if (nLine > 0)
				emit gameScore(nLine);
			int shapeId = rand() % m_mode;
			m_currentShape = m_nextShape;
			m_nextShape = m_shapes[shapeId];
			m_currentShape->init();
			m_map.setShape(m_currentShape);
			emit newShape();
	 	}
	}
	else
	{
		m_currentShape->moveDown();
	}
	update();
}

void TerixField::reshape()
{
	if (m_state != TerixGameStateStart)
		return;

	m_currentShape->reshape();
	int nLine = m_map.reduce();
	if (nLine)
		emit gameScore(nLine);
	update();
}

void TerixField::paintEvent(QPaintEvent* event)
{
	drawBackground();
//	drawOutterRect();
	drawInnerRect();
	drawGrids();
	QWidget::paintEvent(event);
}

void TerixField::resizeEvent(QResizeEvent* event)
{
	buildTerixGrids(event);
	update();
	QWidget::resizeEvent(event);
}

void TerixField::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_Space:
		if (m_state == TerixGameStatePause)
			restart();
		else if (m_state != TerixGameStateStart)
			start();
		else
			pause();
		break;

	case Qt::Key_Left:
		moveShapeLeft();
		break;

	case Qt::Key_Right:
		moveShapeRight();
		break;

	case Qt::Key_Down:
		m_timer.setInterval(TerixDownFast * 1000);
		m_timer.start();
		break;

	case Qt::Key_Up:
		reshape();
		break;

	case Qt::Key_Escape:
		stop();
		break;
	}
	QWidget::keyPressEvent(event);
}

void TerixField::keyReleaseEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_Down:
		m_timer.setInterval(m_time * 1000);
		m_timer.start();
		break;
	}
	QWidget::keyReleaseEvent(event);
}

void TerixField::buildTerixGrids(QResizeEvent* event)
{
	m_terixGrid.clear();
	m_terixGrid.reserve(width() * height());

	QSize winSize(rect().width(), rect().height());
	if (event != NULL)
		winSize = event->size();

	double w = winSize.width() - TerixMarginLeft - TerixMarginRight;
	double h = winSize.height() - TerixMarginTop - TerixMarginBottom;
	int gridWidth = qMin((int)(w / width()), (int)(h / height()));
	int actualW = gridWidth * width();
	int actualH = gridWidth * height();
	int xOffset = (int)((winSize.width() - actualW) / 2.0 + 0.5);
	int yOffset = (int)((winSize.height() - actualH) / 2.0 + 0.5);

	m_innerRect = QRect(xOffset - 1, yOffset - 1, actualW + 2, actualH + 2);
	for (int y = 0; y < height(); ++y)
	{
		for (int x = 0; x < width(); ++x)
		{
			m_terixGrid.push_back(QRect(xOffset + x * gridWidth, 
										yOffset + y * gridWidth, 
										gridWidth - 1, gridWidth - 1));
	 	}
	} 
}

void TerixField::drawBackground()
{
	QPainter painter(this);
	painter.fillRect(m_innerRect, QBrush(TerixBackgroundColor));
}

void TerixField::drawOutterRect()
{
	QPen pen;
	pen.setWidth(TerixOutterBorderWidth);
	pen.setColor(TerixOutterBorderColor);
	
	QRect outterRect = rect();
	int adj = (int)(TerixOutterBorderWidth / 2.0 + 0.5);
	outterRect.adjusted(adj, adj, -adj, -adj);

	QPainter painter(this);
	painter.drawRect(outterRect);
}

void TerixField::drawInnerRect()
{
	QPen pen;
	pen.setWidth(TerixInnerBorderWidth);
	pen.setColor(TerixInnerBorderColor);

	QRect innerRect = m_innerRect;
	int adj = (int)(TerixInnerBorderWidth / 2.0 + 0.5);
	innerRect.adjusted(-adj, -adj, adj, adj);

	QPainter painter(this);
	painter.drawRect(innerRect);
}

void TerixField::drawGrids()
{
	for (int y = 0; y < height(); ++y)
	{
		for (int x = 0; x < width(); ++x)
		{
			int idx = y * width() + x;
			assert(idx >= 0 && idx < (int)m_terixGrid.size());
			if (m_map.getForDraw(x, y))
				drawSingleGrid(m_terixGrid.at(idx));
		}
	}
}

void TerixField::drawSingleGrid(const QRect& r)
{	
	QPen pen(TerixGridColor);
	pen.setWidth(1);
	QBrush brush(TerixGridColor, TerixGridStyle);
	QPainter painter(this);
	painter.setPen(pen);

	painter.fillRect(r, brush);
	painter.drawRect(r);
}
