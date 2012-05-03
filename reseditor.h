#ifndef RESEDITOR_H
#define RESEDITOR_H

#include <QMainWindow>
#include <QAction>
#include <QTreeWidget>
#include <QMenu>
#include <QEvent>

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

    //private:
    Ui::ResEditor *ui;

    void closeEvent(QCloseEvent *);
    QString startFile;


public slots:
        void slotOpenXml();
        void slotSaveXml(const QString& fileName = "");
        void slotExit();
        void slotNormalaize();
        void objectSaved();
        void slotSelectedNode(QTreeWidgetItem *);
        void slotSelectedNodeDBLCLK(QTreeWidgetItem *);

private:
        void prepeareActions();
        QString xmlFileName;
        QString xmlFileWorkDir;
        bool loaded;
        bool edited;
        DlgTexture * dlgtexture;
        QPoint dlgTextPos;
        void setWindowTitleRE(const bool& isEdited);
        bool tryExitNotSaved();

protected:
        bool isEdited;
        QAction *actNormalaize;
        QAction *actAddTexture;
        QAction *actDeletTexture;
        QAction *actAddObject;
        QAction *actCopyObject;
        QAction *actEditObject;
        QAction *actDeleteObject;

        QMenu * objectMenu;
        QMenu * textureMenu;
        void enablingActions();

        QHash<QTreeWidgetItem*, QDomNode> treeNodes;
        animatron * animatronObj;

protected slots:
        void slotFirstLoad();
        void slotAddTexture();
        void slotDeletTexture();
        void slotAddObject();
        void slotEditObject();
        void slotDeleteObject();
        void slotAbout();
        void slotUpdateHistory();
        void slotContextMenu(const QPoint & );
        void slotCopyObject();
};

#endif // RESEDITOR_H
