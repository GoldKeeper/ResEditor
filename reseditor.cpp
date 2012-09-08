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
    ui->treeWidget->sortByColumn(0, Qt::AscendingOrder);
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    dlgtexture = new DlgTexture(this, Qt::Window);

    prepeareActions();
    translator = new QTranslator(this);
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
        ui->actionUpdate_history->setVisible(false);
        //TODO: ui->actionUpdate_history !!!!  slotUpdateHistory()

        connect(ui->action_Qt, SIGNAL(triggered()), SLOT(slotAboutQt()));
        ui->action_Qt->setIcon(QPixmap(":/ResEditor/Resources/qt-logo.png"));

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

        actNormalaize=ui->mainToolBar->addAction(QPixmap(":/ResEditor/Resources/edit-list-order.png"), tr("Упорядочить узлы"));
        connect(actNormalaize, SIGNAL(triggered()), this, SLOT(slotNormalaize()));

        ui->mainToolBar->addSeparator();

        actAddTexture=ui->mainToolBar->addAction(QPixmap(":/ResEditor/Resources/image--plus.png"), tr("Добавить текстуру"));
        connect(actAddTexture,SIGNAL(triggered()),this,SLOT(slotAddTexture()));
        actDeletTexture=ui->mainToolBar->addAction(QPixmap(":/ResEditor/Resources/image--minus.png"), tr("Удалить текстуру"));
        connect(actDeletTexture,SIGNAL(triggered()),this,SLOT(slotDeletTexture()));
        actAddObject=ui->mainToolBar->addAction(QPixmap(":/ResEditor/Resources/folder--plus.png"), tr("Добавить объект"));
        connect(actAddObject,SIGNAL(triggered()),this,SLOT(slotAddObject()));
        actEditObject=ui->mainToolBar->addAction(QPixmap(":/ResEditor/Resources/folder--pencil.png"), tr("� едактировать объект"));
        connect(actEditObject,SIGNAL(triggered()),this,SLOT(slotEditObject()));
        actDeleteObject=ui->mainToolBar->addAction(QPixmap(":/ResEditor/Resources/folder--minus.png"), tr("Удалить объект"));
        connect(actDeleteObject,SIGNAL(triggered()),this,SLOT(slotDeleteObject()));
        actCopyObject=ui->mainToolBar->addAction(QPixmap(":/ResEditor/Resources/folders.png"), tr("Копировать объект"));
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
        textureMenu->addAction(actAddTexture);
        textureMenu->addAction(actDeletTexture);

        QActionGroup * actionGroup = new QActionGroup(this);
        QAction * action= NULL;

        action = ui->menuLang->addAction(QPixmap(":/ResEditor/Resources/flag_russia.png"), tr("� усский"));
        action->setCheckable(true);
        action->setChecked(true);
        actionGroup->addAction(action);
        tranlateFiles.insert(action, ":/lang/reseditor_ru.qm");

        action = ui->menuLang->addAction(QPixmap(":/ResEditor/Resources/flag_great_britain.png"), tr("English"));
        action->setCheckable(true);
        actionGroup->addAction(action);
        tranlateFiles.insert(action, ":/lang/reseditor_eng.qm");

        connect(actionGroup, SIGNAL(triggered(QAction*)), this, SLOT(slotChangeLang(QAction*)));
}

void ResEditor::retranslate()
{
    actNormalaize->setText(tr("Упорядочить узлы"));
    actAddTexture->setText(tr("Добавить текстуру"));
    actDeletTexture->setText(tr("Удалить текстуру"));
    actAddObject->setText(tr("Добавить объект"));
    actEditObject->setText(tr("� едактировать объект"));
    actDeleteObject->setText(tr("Удалить объект"));
    actCopyObject->setText(tr("Копировать объект"));

    setWindowTitleRE(windowTitle().right(1)=="*");

    retranslateTree();
}

void ResEditor::retranslateTree()
{
    if(!loaded) return;
    ResXmlParser::Instance().objectsItem->setText(0, QString(tr("Объекты")+" ("+QString::number(ResXmlParser::Instance().objectsItem->childCount())+")"));
    ResXmlParser::Instance().texturesItem->setText(0, QString(tr("Текстуры")+" ("+QString::number(ResXmlParser::Instance().texturesItem->childCount())+")"));
}


void ResEditor::setWindowTitleRE(const bool &isEdited)
{
    setWindowTitle( tr("� едактор ресурсов: ") + xmlFileName + (isEdited?"*":""));
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
        fileToOpen = QFileDialog::getOpenFileName(this, tr("Открыть XML"), "", tr("XML-файлы")+" (*.xml)");
    }
    else
    {
        QFileInfo qfi(startFile);
        if (qfi.exists())
        {
            if (qfi.fileName().endsWith(".xml", Qt::CaseInsensitive))
                fileToOpen = qfi.absoluteFilePath ();
            else
                QMessageBox::critical(0,0, tr("Неверное расширение файла"));
        }
        else
          QMessageBox::critical(0,0, tr("Файл не найден"));
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
            retranslateTree();


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
        case ResXmlParser::OTHER: break;
        }
        actAddTexture->setEnabled(true);
        actAddObject->setEnabled(true);
}

void ResEditor::slotAddTexture()
{
        bool bOK;
        QString newName= QInputDialog::getText(this,
                                               tr( "Название текстуры"),
                                               tr("Название:"),
                                                QLineEdit::Normal,
                                               "new_texture",
                                                &bOK);
        if (bOK)
        {
            QString fileToOpen = QFileDialog::getOpenFileName(this, tr("Файл текстуры"), "", tr("PNG-файлы")+" (*.png)");

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
                                ResXmlParser::Instance().texturesItem->setText(0,tr("Текстуры")+" ("+QString::number(ResXmlParser::Instance().document.documentElement().elementsByTagName("texture").count())+")");
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
        ResXmlParser::Instance().texturesItem->setText(0,tr("Текстуры")+" ("+QString::number(ResXmlParser::Instance().document.documentElement().elementsByTagName("texture").count())+")");
        treeNodes.remove(ui->treeWidget->currentItem());
        delete ui->treeWidget->currentItem();

        isEdited=true;
        setWindowTitleRE(true);
}

void ResEditor::slotAddObject()
{
    bool bOK;
    QString newName= QInputDialog::getText(this,
                                           tr( "Название объекта"),
                                           tr("Название:"),
                                            QLineEdit::Normal,
                                           "new_draw",
                                            &bOK);
    if(bOK)
    {
        QDomNode newNode= ResXmlParser::Instance().document.createElement("draw");
        newNode.toElement().setAttribute("name", newName);
        newNode.toElement().setAttribute("size", "0,0");
        newNode.toElement().setAttribute("offset", "0,0");
        ResXmlParser::Instance().document.documentElement().appendChild(newNode);
        QTreeWidgetItem* newObject= ResXmlParser::Instance().addObject( ResXmlParser::Instance().objectsItem, newNode);
        ResXmlParser::Instance().objectsItem->setText(0,tr("Объекты")+" ("+QString::number( ResXmlParser::Instance().document.documentElement().elementsByTagName("draw").count())+")");
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
            QString translFile="";
                    QHashIterator<QAction*, QString> iter(tranlateFiles);
                    while(iter.hasNext())
                    {
                        iter.next();
                        if(iter.key()->isChecked())
                            translFile = iter.value();
                    }                    
                    animatronObj = new animatron(this, Qt::Dialog, translFile);
                    animatronObj->setWindowModality(Qt::WindowModal);
                    connect(animatronObj, SIGNAL(objectSaved()), this, SLOT(objectSaved()));
                    connect(animatronObj, SIGNAL(destroyed()),SLOT(setCurrentWindowActive()));
                    animatronObj->allTextures.unite(ResXmlParser::Instance().allTextures);
                    animatronObj->imgCtr->mapTextures.unite(ResXmlParser::Instance().allTextures);
                   animatronObj->dir=xmlFileWorkDir;
                   animatronObj->imgCtr->directory=xmlFileWorkDir;
                   animatronObj->imgCtr->updateTextureMap();                   
                   animatronObj->setSettings(treeNodes.value(ui->treeWidget->currentItem()));
                   animatronObj->show();
         }
    }
}

void ResEditor::slotDeleteObject()
{
    ResXmlParser::Instance().document.documentElement().removeChild(treeNodes.value(ui->treeWidget->currentItem()));
    ResXmlParser::Instance().objectsItem->setText(0,"Объекты ("+QString::number(ResXmlParser::Instance().document.documentElement().elementsByTagName("draw").count())+")");
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

         QApplication::setActiveWindow(this);
}

void ResEditor::slotAbout()
{
    QString ver= "v 1.4.5";
    QString text="";
    text+= tr("Редактор ресурсов") + ver + "\n" + tr("для проекта")+ " ORIGIN-WORLD\n";
    text+="http://origin-world.com\n\n";
    text+="made by GoldKeeper\n";
    text+="Email: prod3500@gmail.com\n";
    text+="GitHub repository: https://github.com/GoldKeeper/ResEditor";

    QMessageBox::information(this, "About", text);
}

void ResEditor::slotUpdateHistory()
{
    QString v1_1="";
    v1_1+="ver. 1.1:\n";
    v1_1+="1)добавлено контекстное меню;\n";
    v1_1+="2)добавлена возможность копирования объектов;\n";
    v1_1+="3)корректное отображение миниатюры спрайта(фрейма);\n";
    v1_1+="4)исправлено сохранение с учётом новой структуры анимации;\n";
    
    v1_1+="ver. 1.2:\n";
        v1_1+="1)запуск из консоли с параметром - имя файла\n";
        v1_1+="2)возможнотсь открытия другого файла не закрывая программу\n";
        v1_1+="3)предупреждение о несохранённости (пока только в главном окне)\n";
        v1_1+="4)переработан парсер XML, и теперь показывает не валидное место\n";

    v1_1+="ver. 1.3:\n";
        v1_1+="1)Изменена кодировка с CP-1251 на UTF-8(теперь везде должен быть виден текст)\n";
        v1_1+="2)Проведена интернационализация\n";
        v1_1+="3)Добавлено контекстное меню для текстур\n";
        v1_1+="4)Поправлено добавление нового фрейма в анимацию\n";
    v1_1+="ver. 1.3.1:\n";
        v1_1+="1)Поправлена масштабируемость миниатюр\n";
        v1_1+="2)Испралена ошибка при создании нового элемента (спрайт/фрейм) при нарезке текстуры\n";
        v1_1+="3)Инофрмация о версии Qt\n";
    v1_1+="ver. 1.4:\n";
        v1_1+="1)Добавлено отображение уже использованных в объекте областей на текутуре\n";
        v1_1+="2)Испралена ошибка при выходе выделяемой области текстуры заэкран\n";
    v1_1+="ver. 1.4.1:\n";
        v1_1+="1)Исправлено редактирование спрайтов\n";


    QMessageBox::information(this, tr("История обновлений"), v1_1);
}

void ResEditor::slotContextMenu(const QPoint & qp)
{
    QTreeWidgetItem * qtwi =  ui->treeWidget->itemAt(qp);
    if (qtwi>0)
    {
        switch(ui->treeWidget->currentItem()->type())
        {
        case ResXmlParser::TEXTURE:
                textureMenu->popup(QCursor::pos()); //дописать меню для текстуры
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
    ResXmlParser::Instance().objectsItem->setText(0,tr("Объекты")+" ("+QString::number(ResXmlParser::Instance().document.documentElement().elementsByTagName("draw").count())+")");
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
                          tr("Внимание"),
                          tr("Внесены изменения."),
                          tr("Сохранить"),
                          tr("Не сохранять"),
                          tr("Отмена"),
                          0,
                          1
                          );

    switch(n)
    {
        case 0: slotSaveXml(); return true;
        case 1: return true;
        case 2: return false;
    }

    return false;
}

void ResEditor::slotChangeLang(QAction * act)
{
    translator->load(tranlateFiles.value(act));
    qApp->installTranslator(translator);
    ui->retranslateUi(this);
}

void ResEditor::changeEvent(QEvent *event)
{
    if(event->type ()==QEvent::LanguageChange)
    {
        retranslate ();
    }
    QMainWindow::changeEvent (event);
}

void ResEditor::slotAboutQt()
{
    QMessageBox::aboutQt(this);
}

void ResEditor::setCurrentWindowActive()
{
    QApplication::setActiveWindow(this);
}



