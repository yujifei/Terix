/*****************************************************
>File Name: terix_main_window.h
>Author: 庾吉飞
>Mail: yujihui.xidian@gmail.com
>Created Time: 2014年01月30日 星期四 23时06分02秒
****************************************************/

class QWidget;
class QKeyEvent;
class QToolBar;
class QPushButton;
class QRadioButton;
class QAction;
class QLabel;
class TerixField;
class TerixShapePreview;

#include <QDialog>

class TerixMainWindow: public QDialog
{
	Q_OBJECT

public:
	TerixMainWindow(QWidget* parent = NULL);

public slots:
	void onAddScore(int nLine);
	void onGameStart();
	void onGameStop();
	void onGamePause();
	void onGameRestart();
	void onGameOver();
	void onGameReady();
	void onNewShape();
	void onNewGameClicked();
	void onPauseClicked();
	void onLeftClicked();
	void onDownPressed();
	void onDownReleased();
	void onRightClicked();
	void onRotateClicked();
	void onSwitchMode();

protected:
	void keyPressEvent(QKeyEvent* event);
	void keyReleaseEvent(QKeyEvent* event);

private:
	void updateSpeed();
	void showScore();
	void showLevel();

private:
	unsigned long m_score;
	unsigned long m_level;
	QToolBar* m_toolBar;
	QAction*  m_newAction;
	QAction*  m_pauseAction;
	QPushButton* m_leftButton;
	QPushButton* m_downButton;
	QPushButton* m_rightButton;
	QPushButton* m_rotateButton;
	QRadioButton* m_modeSimpleRadio;
	QRadioButton* m_modeHighRadio;
	QLabel* m_scoreLabel;
	QLabel* m_levelLabel;
	TerixField* m_terixField;
	TerixShapePreview* m_terixPreview;
};
