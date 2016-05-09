/*****************************************************
>File Name: terix_field.h
>Author: 庾吉飞
>Mail: yujihui.xidian@gmail.com
>Created Time: 2014年01月30日 星期四 00时46分17秒
****************************************************/

#include"terix_map.h"
#include"terix_shape.h"
#include<QWidget>
#include<QSize>
#include<QTimer>
#include<QPaintEvent>
#include<QResizeEvent>
#include<QKeyEvent>


class TerixField: public QWidget
{
	Q_OBJECT
public:
	enum TerixGameState 
	{ 
		TerixGameStateUnstart, 
		TerixGameStateReady,
		TerixGameStateStart, 
		TerixGameStatePause, 
		TerixGameStateOver
	};
	enum TerixGameMode
	{ 
		TerixGameModeSimple = TerixModeSimple,
		TerixGameModeHighLevel = TerixModeHighLevel
	};

public:
	TerixField(QWidget* parent = NULL);
	~TerixField();
	QSize sizeHint() const;
	QSize minimumSizeHint() const;
	void init(int width, int height);
	const TerixMap& getDocument() const;

public:
	int width() const;
	int height() const;
	void setTime(double tm);
	TerixGameState state() const;
	void setState(TerixGameState state);
	TerixGameMode mode() const;
	void setMode(TerixGameMode mode);
	ITerixShape* getPreview() const;

public slots:
	void start();
	void pause();
	void restart();
	void stop();
	void moveShapeLeft();
	void moveShapeRight();
	void moveShapeDown();
	void reshape();

signals:
	void gameScore(int n);
	void gameStart();
	void gameStop();
	void gameOver();
	void gamePause();
	void gameRestart();
	void gameReady();
	void newShape();

protected:
	void paintEvent(QPaintEvent* event);
	void resizeEvent(QResizeEvent* event);
	void keyPressEvent(QKeyEvent* event);
	void keyReleaseEvent(QKeyEvent* event);

private:
	TerixField(const TerixField&);
	TerixField& operator=(const TerixField&);
	void buildTerixGrids(QResizeEvent*);
	void drawBackground();
	void drawOutterRect();
	void drawInnerRect();
	void drawGrids();
	void drawSingleGrid(const QRect& r);

private:
	ITerixShape* m_shapes[TerixShapeNum];
	ITerixShape* m_currentShape;
	ITerixShape* m_nextShape; //for preview
	TerixMap     m_map;
	double		 m_time;
	QTimer       m_timer;
	std::vector<QRect> m_terixGrid;
	QRect        m_innerRect;
	TerixGameState m_state;
	TerixGameMode  m_mode;
};
