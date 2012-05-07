#include "animatron.h"
#include <QtGui/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QTextCodec * codec = QTextCodec::codecForName("CP1251");
	QTextCodec::setCodecForCStrings(codec);

	QApplication a(argc, argv);
	animatron w;
	w.show();
	return a.exec();
}
