#ifndef RESEDITOR_H
#define RESEDITOR_H

#include <QMainWindow>
#include <QAction>
#include <QTreeWidget>
#include <QMenu>
#include <QEvent>
#include <QTranslator>

#include "resxmlparser.h"
#include "dlgtexture.h"
#include "imgcuter/imgcuter.h"
#include "animatron/animatron.h"

namespace Ui {
    class ResEditor;
}

class ResEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit ResEditor(QWidget *parent = 0);
    ~ResEditor();

    void closeEvent(QCloseEvent *);
    QString startFile;

    private:
    Ui::ResEditor *ui;

    void changeEvent (QEvent *event);
    void retranslate ();
    void retranslateTree();


public slots:
        void slotOpenXml();//prepeare init app from XML-file //подготовка к инициализации из XML файла
        void slotSaveXml(const QString& fileName = ""); //Save QDomDocument to XML file//сохранение DOM документа в файл
        void slotExit();//slot react on "Exit" button //слот выхода при клике на "выйход"
        void slotNormalaize();//sort dom nodes  //слот нормализации - упорядочивание узлов в документе - коменты снуржи узлов - улетают вниз
        void objectSaved();//save edited object //сохранение объекта
        void slotSelectedNode(QTreeWidgetItem *);//slot react on select tree item //слот реагирует на смену текущего итема
        void slotSelectedNodeDBLCLK(QTreeWidgetItem *);//Duble click on tree item //двойной клик на итем

private:
        void prepeareActions();//prepeare Actions and connects //подготовка Экшенов и коннектов
        QString xmlFileName;//work file name // имя рабочего файла
        QString xmlFileWorkDir;//xml File Work Directory //путь к Xml файлу
        bool loaded;//state of load XML file //состоянике - загружен ли XML файл
        DlgTexture * dlgtexture; //pointer to texture viewer //указатель на окно с текстурой
        QPoint dlgTextPos;//last texture viewer position
        void setWindowTitleRE(const bool& isEdited);//add "*" in title of window when true//добавляет "*" в заголовок если параметр true
        bool tryExitNotSaved();//check on Edited when try exit//проверка на наличие изменений при попытке закрыть программу

        QHash<QAction*, QString> tranlateFiles;
        QTranslator * translator;

protected:
        bool isEdited; //state of edited XML file //состоянике - изменён ли XML файл
        //Actions:
        QAction *actNormalaize;
        QAction *actAddTexture;
        QAction *actDeletTexture;
        QAction *actAddObject;
        QAction *actCopyObject;
        QAction *actEditObject;
        QAction *actDeleteObject;

        QMenu * objectMenu;
        QMenu * textureMenu;
        void enablingActions();//check and set what actions is enabled and what is disables //проверяет и устанавливает активны экшены или нет

        QHash<QTreeWidgetItem*, QDomNode> treeNodes;//mapping of QTreeWidgetItem* and QDomNode //соответствие триитемам и нодам документа
        animatron * animatronObj;//object editor // DELETE SELF WHEN CLOSED// редактор объекта // удаляется сам при закрытии

protected slots:
        //slots for actions:
        void slotAddTexture();
        void slotDeletTexture();
        void slotAddObject();
        void slotEditObject();
        void slotDeleteObject();
        void slotAbout();
        void slotUpdateHistory();
        void slotContextMenu(const QPoint & );
        void slotCopyObject();

        void slotChangeLang(QAction *);
};

#endif // RESEDITOR_H
