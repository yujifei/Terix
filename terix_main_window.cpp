/*****************************************************
>File Name: terix_main_window.cpp
>Author: 庾吉飞
>Mail: yujihui.xidian@gmail.com
>Created Time: 2014年02月01日 星期六 01时16分38秒
****************************************************/

#include "terix_main_window.h"
#include "terix_field.h"
#include "terix_preview.h"
#include "config.h"
#include <cassert>
#include <QAction>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QLabel>
#include <QToolBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QtCore>

TerixMainWindow::TerixMainWindow(QWidget* parent): QDialog(parent)
{
	m_score = 0;
	m_level = 1;

	m_newAction = new QAction(tr("新建"), NULL);
	m_newAction->setIcon(QIcon(":/icons/new.png"));
	m_newAction->setToolTip(tr("新建游戏"));
	connect(m_newAction, SIGNAL(triggered()), this, SLOT(onNewGameClicked()));
	m_pauseAction = new QAction(tr("开始"), NULL);
	m_pauseAction->setIcon(QIcon(":/icons/start.png"));
	m_pauseAction->setToolTip("开始游戏");
	connect(m_pauseAction, SIGNAL(triggered()), this, SLOT(onPauseClicked()));
	m_pauseAction->setEnabled(false);
	m_toolBar = new QToolBar;
	m_toolBar->addAction(m_newAction);
	m_toolBar->addAction(m_pauseAction);

	m_terixField = new TerixField;
	m_terixField->setMode(TerixField::TerixGameModeSimple);
	connect(m_terixField, SIGNAL(gameScore(int)), this, SLOT(onAddScore(int)));
	connect(m_terixField, SIGNAL(gameStart()), this, SLOT(onGameStart()));
	connect(m_terixField, SIGNAL(gameStop()), this, SLOT(onGameStop()));
	connect(m_terixField, SIGNAL(gamePause()), this, SLOT(onGamePause()));
	connect(m_terixField, SIGNAL(gameRestart()), this, SLOT(onGameRestart()));
	connect(m_terixField, SIGNAL(gameOver()), this, SLOT(onGameOver()));
	connect(m_terixField, SIGNAL(gameReady()), this, SLOT(onGameReady()));
	connect(m_terixField, SIGNAL(newShape()), this, SLOT(onNewShape()));

	m_terixPreview = new TerixShapePreview;

	m_modeSimpleRadio = new QRadioButton(tr("简化模式(7种基本形状)"));
	m_modeHighRadio = new QRadioButton(tr("高级模式(更多形状)"));
	m_modeSimpleRadio->setFocusPolicy(Qt::NoFocus);
	m_modeHighRadio->setFocusPolicy(Qt::NoFocus);
	QGroupBox* modeGroupBox= new QGroupBox(tr("游戏模式"));
	connect(m_modeSimpleRadio, SIGNAL(clicked()), this, SLOT(onSwitchMode()));
	connect(m_modeHighRadio, SIGNAL(clicked()), this, SLOT(onSwitchMode()));
	QVBoxLayout* radioWrapper = new QVBoxLayout;
	radioWrapper->addWidget(m_modeSimpleRadio);
	radioWrapper->addWidget(m_modeHighRadio);
	m_modeSimpleRadio->setChecked(true);
	modeGroupBox->setLayout(radioWrapper);

	m_scoreLabel = new QLabel;
	m_levelLabel = new QLabel;

	m_leftButton = new QPushButton;
	m_leftButton->setIcon(QIcon(":/icons/left.png"));
	m_leftButton->setFocusPolicy(Qt::NoFocus);
	connect(m_leftButton, SIGNAL(clicked()), this, SLOT(onLeftClicked()));
	m_downButton = new QPushButton;
	m_downButton->setIcon(QIcon(":/icons/down.png"));
	m_downButton->setFocusPolicy(Qt::NoFocus);
	connect(m_downButton, SIGNAL(pressed()), this, SLOT(onDownPressed()));
	connect(m_downButton, SIGNAL(released()), this, SLOT(onDownReleased()));
	m_rightButton = new QPushButton;
	m_rightButton->setIcon(QIcon(":/icons/right.png"));
	m_rightButton->setFocusPolicy(Qt::NoFocus);
	connect(m_rightButton, SIGNAL(clicked()), this, SLOT(onRightClicked()));
	m_rotateButton = new QPushButton;
	m_rotateButton->setIcon(QIcon(":/icons/rotate.png"));
	m_rotateButton->setFocusPolicy(Qt::NoFocus);
	connect(m_rotateButton, SIGNAL(clicked()), this, SLOT(onRotateClicked()));

	QHBoxLayout* rotateButtonWrapper = new QHBoxLayout;
	rotateButtonWrapper->addStretch();
	rotateButtonWrapper->addWidget(m_rotateButton);
	rotateButtonWrapper->addStretch();
	QHBoxLayout* rlButtonWrapper = new QHBoxLayout;
	rlButtonWrapper->addWidget(m_leftButton);
	rlButtonWrapper->addStretch();
	rlButtonWrapper->addWidget(m_rightButton);
	QHBoxLayout* downButtonWrapper = new QHBoxLayout;
	downButtonWrapper->addStretch();
	downButtonWrapper->addWidget(m_downButton);
	downButtonWrapper->addStretch();

	QVBoxLayout* rightPart = new QVBoxLayout;
	rightPart->addWidget(m_terixPreview);
	rightPart->addWidget(modeGroupBox);
	rightPart->addSpacing(1);
	rightPart->addWidget(m_scoreLabel);
	rightPart->addWidget(m_levelLabel);
	rightPart->addSpacing(1);
	rightPart->addLayout(rotateButtonWrapper);
	rightPart->addLayout(rlButtonWrapper);
	rightPart->addLayout(downButtonWrapper);

	QHBoxLayout* widgetLayout = new QHBoxLayout;
	widgetLayout->addWidget(m_terixField);
	widgetLayout->addLayout(rightPart);
	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(m_toolBar);
	mainLayout->addLayout(widgetLayout);

	setSizeGripEnabled(true);
	setWindowTitle(tr("俄罗斯方块"));
	setLayout(mainLayout);
	adjustSize();

	showScore();
	showLevel();
}

void TerixMainWindow:: onAddScore(int nLine)
{
	assert(nLine <= TerixMaxShapeSize);
	static int scoreMap[TerixMaxShapeSize] = {10, 30, 60, 100, 150};
	static int levelMap[TerixMaxLevel] = {1000, 3000, 6000, 10000, 15000, 25000, 50000};
	
	m_score += scoreMap[nLine - 1];
	if ((int)m_level <= (int)TerixMaxLevel && (int)m_score > levelMap[m_level - 1])
		++m_level;

	updateSpeed();
	showScore();
	showLevel();
}

void TerixMainWindow::onGameStart()
{
	m_newAction->setEnabled(false);
	m_pauseAction->setIcon(QIcon(":/icons/pause.png"));
	m_pauseAction->setText(tr("暂停"));
	m_pauseAction->setToolTip(tr("暂停游戏"));
	m_pauseAction->setEnabled(true);

	showScore();
	showLevel();
}

void TerixMainWindow::onGameStop()
{
	m_newAction->setEnabled(true);
	m_pauseAction->setIcon(QIcon(":/icons/start.png"));
	m_pauseAction->setText(tr("开始"));
	m_pauseAction->setText(tr("开始游戏"));
	m_pauseAction->setEnabled(false);
}

void TerixMainWindow::onGamePause()
{
	m_pauseAction->setIcon(QIcon(":/icons/start.png"));
	m_pauseAction->setText(tr("恢复"));
	m_pauseAction->setText(tr("恢复游戏"));
}

void TerixMainWindow::onGameRestart()
{
	m_pauseAction->setIcon(QIcon(":/icons/pause.png"));
	m_pauseAction->setText(tr("暂停"));
	m_pauseAction->setText(tr("暂停游戏"));
}

void TerixMainWindow::onGameOver()
{
	QMessageBox::information(this, tr("俄罗斯方块"), tr("游戏结束"));
	m_terixField->stop();
}

void TerixMainWindow::onGameReady()
{
}

void TerixMainWindow::onNewShape()
{
	m_terixPreview->setShape(m_terixField->getPreview());
}

void TerixMainWindow::onNewGameClicked()
{
	m_terixField->stop();
	m_score = 0;
	m_level = 1;
	updateSpeed();
	m_terixField->start();
}

void TerixMainWindow::onPauseClicked()
{
	if (m_terixField->state() == TerixField::TerixGameStateStart)
		m_terixField->pause();
	else if (m_terixField->state() == TerixField::TerixGameStatePause)
		m_terixField->restart();
}

void TerixMainWindow::onLeftClicked()
{
	m_terixField->moveShapeLeft();
}

void TerixMainWindow::onDownPressed()
{
	m_terixField->setTime(TerixDownFast);
}

void TerixMainWindow::onDownReleased()
{
	updateSpeed();
}

void TerixMainWindow::onRightClicked()
{
	m_terixField->moveShapeRight();
}

void TerixMainWindow::onRotateClicked()
{
	m_terixField->reshape();
}

void TerixMainWindow::onSwitchMode()
{
	if (m_modeHighRadio->isChecked())
		m_terixField->setMode(TerixField::TerixGameModeHighLevel);
	else
		m_terixField->setMode(TerixField::TerixGameModeSimple);
}

void TerixMainWindow::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_Space:
		if (m_terixField->state() == TerixField::TerixGameStatePause)
			m_terixField->restart();
		else if (m_terixField->state() == TerixField::TerixGameStateStart)
			m_terixField->pause();
		else
			m_terixField->start();
		break;

	case Qt::Key_Left:
	case Qt::Key_A:
		m_terixField->moveShapeLeft();
		break;

	case Qt::Key_Down:
	case Qt::Key_S:
		m_terixField->setTime(TerixDownFast);
		break;

	case Qt::Key_Right:
	case Qt::Key_D:
		m_terixField->moveShapeRight();
		break;
	
	case Qt::Key_Up:
	case Qt::Key_W:
		m_terixField->reshape();
		break;

	case Qt::Key_Escape:
		m_terixField->stop();
		break;
	}

	QDialog::keyPressEvent(event);
}

void TerixMainWindow::keyReleaseEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_Down:
	case Qt::Key_S:
		updateSpeed();
		break;
	}

	QDialog::keyReleaseEvent(event);
}

void TerixMainWindow::updateSpeed()
{
	const double shinkRatio = 0.76;
	double curInterval = TerixInitInterval * qPow(shinkRatio, m_level - 1);
	m_terixField->setTime(curInterval);
}

void TerixMainWindow::showScore()
{
	QString scoreString = tr("得分: %1").arg(m_score);
	m_scoreLabel->setText(scoreString);
}

void TerixMainWindow::showLevel()
{
	QString levelString = tr("级别: %1").arg(m_level);
	m_levelLabel->setText(levelString);
}
