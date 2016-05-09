/*****************************************************
>File Name: config.h
>Author: 庾吉飞
>Mail: yujihui.xidian@gmail.com
>Created Time: 2014年02月02日 星期日 20时02分52秒
****************************************************/

#ifndef __FILE_CONFIG_H__
#define __FILE_CONFIG_H__

#include <QSize>
#include <QColor>

const int TerixDefaultWidth  = 15;
const int TerixDefaultHeight = 30;
const QSize TerixDefaultSize(300, 600);
const QSize TerixMinimumSize(150, 300);
const double TerixDownFast = 0.05;
const int TerixMarginLeft = 0;
const int TerixMarginRight = 0;
const int TerixMarginTop = 0;
const int TerixMarginBottom = 0;
const QColor TerixBackgroundColor(5, 5, 5);
const int TerixOutterBorderWidth = 1;
const QColor TerixOutterBorderColor(100, 100, 100);
const int TerixInnerBorderWidth = 1;
const QColor TerixInnerBorderColor(0, 0, 0);
const QColor TerixGridColor(250, 250, 250);
const Qt::BrushStyle TerixGridStyle = Qt::Dense3Pattern;
const int TerixMaxShapeSize = 5;
const int TerixMaxLevel = 7;
const double TerixInitInterval = 1.0;
const int TerixPreviewWidth = 5;
const int TerixPreviewHeight = 5;
const QColor TerixPreviewBackground(5, 5, 5);
const QColor TerixPreviewGridBorderColor(250, 250, 250);
const Qt::BrushStyle TerixPreviewGridStyle = Qt::Dense3Pattern;

#endif
