#include "reseditor.h"
#include "ui_reseditor.h"

#include <QToolBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QInputDialog>
#include <QTextStream>
#include "animatron/animatron.h"
#include <resxmlparser.h>

ResEditor::ResEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ResEditor)
{
    ui->setupUi(this);
    isEdited=false;

    xmlFileName="";
    xmlFileWorkDir="";
    loaded=false;
    edited=false;
    ui->treeWidget->sortByColumn(0, Qt::AscendingOrder);
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    dlgtexture = new DlgTexture(this, Qt::Window);

    prepeareActions();


}

ResEditor::~ResEditor()
{
    delete ui;
}


void ResEditor::prepeareActions()
{
        connect(ui->treeWidget, SIGNAL(	itemDoubleClicked ( QTreeWidgetItem * , int )), SLOT(slotSelectedNodeDBLCLK(QTreeWidgetItem *)));


        connect(ui->treeWidget, SIGNAL( customContextMenuRequested ( const QPoint & )), this, SLOT( slotContextMenu(const QPoint & )));

        connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));
        connect(ui->actionUpdate_history, SIGNAL(triggered()), this, SLOT(slotUpdateHistory()));

        ui->mainToolBar->addAction(ui->actionOpen);
        connect(ui->actionOpen, SIGNAL(triggered()), SLOT(slotOpenXml()));
        ui->actionOpen->setIcon(QPixmap(":/ResEditor/Resources/folder-horizontal-open.png"));
        connect(ui->treeWidget, SIGNAL(currentItemChanged ( QTreeWidgetItem *, QTreeWidgetItem *)), SLOT(slotSelectedNode(QTreeWidgetItem *)));

        ui->actionAbout->setIcon(QPixmap(":/ResEditor/Resources/information-frame.png"));
        ui->actionUpdate_history->setIcon(QPixmap(":/ResEditor/Resources/pencil.png"));

        ui->mainToolBar->addAction(ui->actionSave);
        ui->actionSave->setIcon(QPixmap(":/ResEditor/Resources/imgcuter/disk.png"));
        connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(slotSaveXml()));
        ui->actionExit->setIcon(QPixmap(":/ResEditor/Resources/door-open-in.png"));

        connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));

        actNormalaize=ui->mainToolBar->addAction(QPixmap(":/ResEditor/Resources/edit-list-order.png"), tr("����������� ����"));
        connect(actNormalaize, SIGNAL(triggered()), this, SLOT(slotNormalaize()));

        ui->mainToolBar->addSeparator();

        actAddTexture=ui->mainToolBar->addAction(QPixmap(":/ResEditor/Resources/image--plus.png"), tr("�������� ��������"));
        connect(actAddTexture,SIGNAL(triggered()),this,SLOT(slotAddTexture()));
        actDeletTexture=ui->mainToolBar->addAction(QPixmap(":/ResEditor/Resources/image--minus.png"), tr("������� ��������"));
        connect(actDeletTexture,SIGNAL(triggered()),this,SLOT(slotDeletTexture()));
        actAddObject=ui->mainToolBar->addAction(QPixmap(":/ResEditor/Resources/folder--plus.png"), tr("�������� ������"));
        connect(actAddObject,SIGNAL(triggered()),this,SLOT(slotAddObject()));
        actEditObject=ui->mainToolBar->addAction(QPixmap(":/ResEditor/Resources/folder--pencil.png"), tr("������������� ������"));
        connect(actEditObject,SIGNAL(triggered()),this,SLOT(slotEditObject()));
        actDeleteObject=ui->mainToolBar->addAction(QPixmap(":/ResEditor/Resources/folder--minus.png"), tr("������� ������"));
        connect(actDeleteObject,SIGNAL(triggered()),this,SLOT(slotDeleteObject()));
        actCopyObject=ui->mainToolBar->addAction(QPixmap(":/ResEditor/Resources/folders.png"), tr("���������� ������"));
        connect(actCopyObject,SIGNAL(triggered()),this,SLOT(slotCopyObject()));

        ui->actionSave->setEnabled(false);
        ui->actionSaveAs->setEnabled(false); ui->actionSaveAs->setVisible(false);
        actNormalaize->setEnabled(false);
        actAddTexture->setEnabled(false);
        actDeletTexture->setEnabled(false);
        actAddObject->setEnabled(false);
        actEditObject->setEnabled(false);
        actDeleteObject->setEnabled(false);
        actCopyObject->setEnabled(false);        

        objectMenu = new QMenu(this);
        objectMenu->addAction(actAddObject);
        objectMenu->addAction(actEditObject);
        objectMenu->addAction(actDeleteObject);
        objectMenu->addAction(actCopyObject);

        textureMenu = new QMenu(this);
}

void ResEditor::slotFirstLoad()
{
}


void ResEditor::setWindowTitleRE(const bool &isEdited)
{
    setWindowTitle( tr("�������� ��������: ") + xmlFileName + (isEdited?"*":""));
}

void ResEditor::slotOpenXml()
{
    if(isEdited)
    {
        if(!tryExitNotSaved()){return;}
    }

    QString fileToOpen;
    if (startFile.length()==0)
    {
        fileToOpen = QFileDialog::getOpenFileName(this, tr("������� XML"), "", tr("XML-�����")+" (*.xml)");
    }
    else
    {
        QFileInfo qfi(startFile);
        if (qfi.exists())
        {
            if (qfi.fileName().endsWith(".xml", Qt::CaseInsensitive))
                fileToOpen = qfi.absoluteFilePath ();
            else
                QMessageBox::critical(0,0, tr("�������� ���������� �����"));
        }
        else
          QMessageBox::critical(0,0, tr("���� �� ������"));
        startFile=QString();
    }

    if (fileToOpen.length()>0)
    {
        if(loaded)
        {
            ui->treeWidget->blockSignals(true);
            ui->treeWidget->clear();
            ui->treeWidget->blockSignals(false);

            treeNodes.clear();            
        }

        if(ResXmlParser::Instance().peraseXml(fileToOpen, ui->treeWidget, &treeNodes))
        {
            loaded=true;

            QFileInfo qfi(fileToOpen);
            QString TMPxmlFileName=qfi.fileName();
            QString TMPxmlFileWorkDir=qfi.path();

                xmlFileName=TMPxmlFileName;
                xmlFileWorkDir=TMPxmlFileWorkDir;

                isEdited=false;
                setWindowTitleRE(false);

                ui->actionSave->setEnabled(true);
                ui->actionSaveAs->setEnabled(true);
                actNormalaize->setEnabled(true);
        }
    }
}

void ResEditor::slotSelectedNode(QTreeWidgetItem * qtwi)
{
        dlgTextPos=dlgtexture-> pos();
        dlgtexture->setVisible(false);
        enablingActions();
}

void ResEditor::slotSelectedNodeDBLCLK(QTreeWidgetItem *qtwi)
{
    if ((qtwi->type()==ResXmlParser::TEXTURE))
    {
        if(!dlgtexture->isVisible())
        dlgtexture->move(dlgTextPos);
        dlgtexture->setVisible(true);
        dlgtexture->setWindowTitle(xmlFileWorkDir +"/" + qtwi->text(1));
        activateWindow();

        dlgtexture->ui.label->setPixmap(QPixmap(xmlFileWorkDir +"/" + qtwi->text(1)));

    }
    else if ((qtwi->type()==ResXmlParser::OBJECT))
    {
        slotEditObject();
    }
    else
    {
        dlgTextPos=dlgtexture-> pos();
        dlgtexture->setVisible(false);
    }
    enablingActions();
}

void ResEditor::enablingActions()
{
        actAddTexture->setEnabled(false);
        actDeletTexture->setEnabled(false);
        actAddObject->setEnabled(false);
        actEditObject->setEnabled(false);
        actDeleteObject->setEnabled(false);
        actCopyObject->setEnabled(false);
        switch(ui->treeWidget->currentItem()->type())
        {
        case ResXmlParser::TEXTURE:
                actDeletTexture->setEnabled(true);
                break;
        case ResXmlParser::OBJECT:
                actEditObject->setEnabled(true);
                actDeleteObject->setEnabled(true);
                actCopyObject->setEnabled(true);
                break;
        case ResXmlParser::SPRITE: break; //��� �� ����������� ����� ������
        case ResXmlParser::TILE: break; //��� �� ����������� ����� ������
        case ResXmlParser::OTHER: break;
        }
        actAddTexture->setEnabled(true);
        actAddObject->setEnabled(true);
}

void ResEditor::slotAddTexture()
{
        bool bOK;
        QString newName= QInputDialog::getText(this,
                                               tr( "�������� ��������"),
                                               tr("��������:"),
                                                QLineEdit::Normal,
                                               tr("new_texture"),
                                                &bOK);
        if (bOK)
        {
            QString fileToOpen = QFileDialog::getOpenFileName(this, tr("���� ��������"), "", tr("PNG-�����")+" (*.png)");

                if(fileToOpen!="")
                {
                        QStringList qstrl = fileToOpen.split(xmlFileWorkDir+"/");
                        if(qstrl.count()==2)
                        {
                                QDomNode newNode= ResXmlParser::Instance().document.createElement("texture");
                                newNode.toElement().setAttribute("name", newName);
                                newNode.toElement().setAttribute("srcfile", qstrl[1]);
                                ResXmlParser::Instance().document.documentElement().appendChild(newNode);
                                QTreeWidgetItem * newTextureItem =ResXmlParser::Instance().addTexture(ResXmlParser::Instance().texturesItem, newNode);
                                ResXmlParser::Instance().texturesItem->setText(0,tr("��������")+" ("+QString::number(ResXmlParser::Instance().document.documentElement().elementsByTagName("texture").count())+")");
                                ui->treeWidget->setCurrentItem(newTextureItem);
                                isEdited=true;
                                setWindowTitleRE(true);
                        }
                }
        }
}

void ResEditor::slotDeletTexture()
{
        ResXmlParser::Instance().document.documentElement().removeChild(treeNodes.value(ui->treeWidget->currentItem()));
        ResXmlParser::Instance().texturesItem->setText(0,tr("��������")+" ("+QString::number(ResXmlParser::Instance().document.documentElement().elementsByTagName("texture").count())+")");
        treeNodes.remove(ui->treeWidget->currentItem());
        delete ui->treeWidget->currentItem();

        isEdited=true;
        setWindowTitleRE(true);
}

void ResEditor::slotAddObject()
{
    bool bOK;
    QString newName= QInputDialog::getText(this,
                                           tr( "�������� �������"),
                                           tr("��������:"),
                                            QLineEdit::Normal,
                                           tr("new_draw"),
                                            &bOK);
    if(bOK)
    {
        QDomNode newNode= ResXmlParser::Instance().document.createElement("draw");
        newNode.toElement().setAttribute("name", newName);
        newNode.toElement().setAttribute("size", "0,0");
        newNode.toElement().setAttribute("offset", "0,0");
        ResXmlParser::Instance().document.documentElement().appendChild(newNode);
        QTreeWidgetItem* newObject= ResXmlParser::Instance().addObject( ResXmlParser::Instance().objectsItem, newNode);
        ResXmlParser::Instance().objectsItem->setText(0,tr("�������")+" ("+QString::number( ResXmlParser::Instance().document.documentElement().elementsByTagName("draw").count())+")");
        ui->treeWidget->setCurrentItem(newObject);
        isEdited=true;
        setWindowTitleRE(true);
    }
}

void ResEditor::slotEditObject()
{
    QTreeWidgetItem * twi= ui->treeWidget->currentItem();
    if(twi->type()==ResXmlParser::OBJECT)
    {
        {
                    animatronObj = new animatron(this);
                    animatronObj->setWindowModality(Qt::WindowModal);
                    connect(animatronObj, SIGNAL(objectSaved()), this, SLOT(objectSaved()));
                    animatronObj->allTextures.unite(ResXmlParser::Instance().allTextures);
                    animatronObj->imgCtr->mapTextures.unite(ResXmlParser::Instance().allTextures);
                   animatronObj->dir=xmlFileWorkDir;
                   animatronObj->imgCtr->directory=xmlFileWorkDir;
                   animatronObj->imgCtr->updateTextureMap();
                   animatronObj->setSettings();
                   animatronObj->setSettings(treeNodes.value(ui->treeWidget->currentItem()));
                   animatronObj->show();
         }
    }
}

void ResEditor::slotDeleteObject()
{
    ResXmlParser::Instance().document.documentElement().removeChild(treeNodes.value(ui->treeWidget->currentItem()));
    ResXmlParser::Instance().objectsItem->setText(0,"������� ("+QString::number(ResXmlParser::Instance().document.documentElement().elementsByTagName("draw").count())+")");
    treeNodes.remove(ui->treeWidget->currentItem());
    delete ui->treeWidget->currentItem();

    isEdited=true;
    setWindowTitleRE(true);
}

void ResEditor::slotSaveXml(const QString& fileName)
{
        if(xmlFileName!="")
        {
            QFile* file = new QFile(xmlFileWorkDir+"/"+xmlFileName);
            file->open(QIODevice::WriteOnly);

            QTextStream out(file);            
            ResXmlParser::Instance().document.save(out,4);
            file->flush();
            file->close();
            delete file;

            isEdited=false;
            setWindowTitleRE(false);
        }
}

void ResEditor::slotNormalaize()
{
        QDomElement root = ResXmlParser::Instance().document.documentElement();
        QDomNodeList texturesNodeList = root.toElement().elementsByTagName("texture");
        QDomNode lastNode = QDomNode();
        for (int tnli=0; tnli<texturesNodeList.count(); tnli++)
                {
                        QDomNode texturNode = texturesNodeList.at(tnli);
                        if(tnli==0)
                        { root.insertBefore(texturNode, lastNode);
                                lastNode = texturNode;
                        }
                        else
                        {
                          root.insertAfter(texturNode, lastNode);
                                lastNode = texturNode;
                        }
                }
        QDomNodeList objectsNodeList = root.toElement().elementsByTagName("draw");
        for (int onli=0; onli<objectsNodeList.count(); onli++)
                {
                        QDomNode objectNode = objectsNodeList.at(onli);
                        if(onli==0)
                        { root.insertBefore(objectNode, lastNode);
                                lastNode = objectNode;
                        }
                        else
                        {
                          root.insertAfter(objectNode, lastNode);
                                lastNode = objectNode;
                        }
                }
        QDomNodeList tilesNodeList = root.toElement().elementsByTagName("tile");
        for (int tlnli=0; tlnli<tilesNodeList.count(); tlnli++)
                {
                        QDomNode tileNode = tilesNodeList.at(tlnli);
                        if(tlnli==0)
                        { root.insertBefore(tileNode, lastNode);
                        lastNode = tileNode;
                        }
                        else
                        {	root.insertAfter(tileNode, lastNode);
                        lastNode = tileNode;
                }
        }
}

void ResEditor::objectSaved()
{
        QString newObjName= treeNodes.value(ui->treeWidget->currentItem()).toElement().attribute("name");
        ui->treeWidget->currentItem()->setText(0, newObjName);
        ui->treeWidget->currentItem()->setText(3, newObjName);        
        isEdited=true;
        setWindowTitleRE(true);
}

void ResEditor::slotAbout()
{
    QString text="";
            text+="Resourse editor v 1.2 \nfor project ORIGIN-WORLD\n";
            text+="http://origin-world.com\n\n";
            text+="made by GoldKeeper\n";
            text+="Email: prod3500@gmail.com";

    QMessageBox::information(this, "About", text);
}

void ResEditor::slotUpdateHistory()
{
    QString v1_1="";
    v1_1+="ver. 1.1:\n";
    v1_1+="1)��������� ����������� ����;\n";
    v1_1+="2)��������� ����������� ����������� ��������;\n";
    v1_1+="3)���������� ����������� ��������� �������(������);\n";
    v1_1+="4)���������� ���������� � ������ ����� ��������� ��������;\n";
    
    v1_1+="ver. 1.2:\n";
	v1_1+="������ �� ������� � ���������� - ��� �����\n";
	v1_1+="����������� �������� ������� ����� �� �������� ���������\n";
	v1_1+="�������������� � �������������� (���� ������ � ������� ����)\n";

	QMessageBox::information(this, "Update history", v1_1);
}

void ResEditor::slotContextMenu(const QPoint & qp)
{
    QTreeWidgetItem * qtwi =  ui->treeWidget->itemAt(qp);
    if (qtwi>0)
    {
        switch(ui->treeWidget->currentItem()->type())
        {
        case ResXmlParser::TEXTURE:
                //textureMenu->popup(QCursor::pos()); //�������� ���� ��� ��������
                break;
        case ResXmlParser::OBJECT:
                objectMenu->popup(QCursor::pos());
                break;
        }
    }
}

void ResEditor::slotCopyObject()
{
    if (ui->treeWidget->currentItem()->type()!= ResXmlParser::OBJECT)return;
    QDomNode newNode= treeNodes.value(ui->treeWidget->currentItem()).cloneNode(true);
    ResXmlParser::Instance().document.documentElement().appendChild(newNode);
    QTreeWidgetItem* newObject= ResXmlParser::Instance().addObject(ResXmlParser::Instance().objectsItem, newNode);
    ResXmlParser::Instance().objectsItem->setText(0,tr("�������")+" ("+QString::number(ResXmlParser::Instance().document.documentElement().elementsByTagName("draw").count())+")");
    ui->treeWidget->setCurrentItem(newObject);

    isEdited=true;
    setWindowTitleRE(true);
}

void ResEditor::closeEvent(QCloseEvent *e)
{
    if(isEdited)
    {        
        if(tryExitNotSaved())
        e->accept();
           else
        e->ignore();
    }
}

void ResEditor::slotExit()
{   
    if(tryExitNotSaved())
    exit(0);
}

bool ResEditor::tryExitNotSaved()
{
    int n = QMessageBox::warning(0,
                          tr("��������"),
                          tr("������� ���������."),
                          tr("���������"),
                          tr("�� ���������"),
                          tr("������"),
                          0,
                          1
                          );

    switch(n)
    {
        case 0: slotSaveXml(); return true; break;
        case 1: return true; break;
        case 2: return false; break;
    }

    return false;
}

