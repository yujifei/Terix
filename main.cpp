/*****************************************************
>File Name: main.cpp
>Author: 庾吉飞
>Mail: yujihui.xidian@gmail.com
>Created Time: 2014年02月02日 星期日 21时16分53秒
****************************************************/

#include "terix_main_window.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

	TerixMainWindow mWin;
	mWin.show();
	return app.exec();
}

