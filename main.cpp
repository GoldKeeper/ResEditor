#include <QtGui/QApplication>
#include "reseditor.h"
#include <QTextCodec>

//#include <QTranslator>

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);


    QApplication a(argc, argv);

    //QTranslator translator;
    //translator.load(":/lang/reseditor_ru.qm");
    //a.installTranslator(&translator);

    ResEditor w;

    w.show();
    if(argc>1)
    {
        w.startFile = QApplication::arguments().at(1);
        w.slotOpenXml();
    }

    return a.exec();
}
