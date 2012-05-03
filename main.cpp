#include <QtGui/QApplication>
#include "reseditor.h"
#include <QTextCodec>

#include <QMessageBox>

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("CP1251");
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);



    QApplication a(argc, argv);
    ResEditor w;


    w.show();
    if(argc>1)
    {
        w.startFile = QApplication::arguments().at(1);
        w.slotOpenXml();
    }



    return a.exec();
}
