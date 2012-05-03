#include "resxmlparser.h"
#include <QMessageBox>

bool ResXmlParser::peraseXml(const QString &file, QTreeWidget *pqTreeWidgetee, QHash<QTreeWidgetItem*, QDomNode> * _treeNodes)
{
    treeNodes=_treeNodes;
    //������ ����� � ������� ��� ���������� � ��������
    QFile qf(file);
    if (!qf.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                    QString MSGcaption= "READ ERROR";
                    QString MSGcontent = QString("������ �������� �����\n%1").arg(file);
                    QMessageBox::critical(0, MSGcaption, MSGcontent);
                    return false;
            }
    QString domErr;
    int domLineErr, domColErr;
    if (!document.setContent(qf.readAll(), &domErr, &domLineErr, &domColErr))
            {
                    QString MSGcaption="SET ERROR";
                    QString MSGcontent =QString("������ ��������� ����������� �����\n%1\n� DOM document.\n ������ %2 ������� %3.\n����� ������:\n%4")
                            .arg(file)
                            .arg(domLineErr)
                            .arg(domColErr)
                            .arg(domErr);
                    QMessageBox::critical(0, MSGcaption, MSGcontent);
                    return false;
            }


    //������ �������� ������� � ��������� �������
    rootNode = document.documentElement();
    rootItem = new QTreeWidgetItem(pqTreeWidgetee, QStringList("root"), ResXmlParser::OTHER);
    rootItem->setIcon(0, QPixmap(":/ResEditor/Resources/document-code.png"));
    rootItem->setExpanded(true);
    treeNodes->insert(rootItem,rootNode);
    rootItem->setText(0,"<" + rootNode.toElement().tagName()+"   name = \""+rootNode.toElement().attribute("name")+"\">");
    addAllres();

   return true;
}

void ResXmlParser::addAllres()
{
    //���������� ������� <texture>
    texturesItem = new QTreeWidgetItem(rootItem, QStringList("�������� ("+QString::number(rootNode.toElement().elementsByTagName("texture").count())+")"), ResXmlParser::OTHER);
    texturesItem->setIcon(0,QPixmap(":/ResEditor/Resources/images-stack.png"));
    QDomNodeList texturesNodeList = rootNode.toElement().elementsByTagName("texture");
    for (int tnli=0; tnli<texturesNodeList.count(); tnli++)
    {
            addTexture(texturesItem, texturesNodeList.at(tnli));
    }

    //���������� �������� <draw>
    objectsItem = new QTreeWidgetItem(rootItem, QStringList("������� ("+QString::number(rootNode.toElement().elementsByTagName("draw").count())+")"),ResXmlParser::OTHER);
    objectsItem->setIcon(0,QPixmap(":/ResEditor/Resources/folder-open-image.png"));
    QDomNodeList objectsNodeList = rootNode.toElement().elementsByTagName("draw");
    for (int onli=0; onli<objectsNodeList.count(); onli++)
    {
            addObject(objectsItem, objectsNodeList.at(onli));
    }

    rootItem->treeWidget()->sortByColumn(0, Qt::AscendingOrder);
    rootItem->treeWidget()->resizeColumnToContents(0);

}

QTreeWidgetItem * ResXmlParser::addTexture(QTreeWidgetItem *_parentItem, QDomNode _node)
{
    QTreeWidgetItem * textureItem = new QTreeWidgetItem(_parentItem, QStringList(_node.toElement().attribute("name")), ResXmlParser::TEXTURE);
    textureItem->setIcon(0,QPixmap(":/ResEditor/Resources/image.png"));
    textureItem->setText(1,_node.toElement().attribute("srcfile"));
    textureItem->setText(2,"node");
    textureItem->setText(3,_node.toElement().attribute("name"));
    treeNodes->insert(textureItem,_node);
    allTextures.insert(_node.toElement().attribute("name"), _node.toElement().attribute("srcfile"));
    return textureItem;
}

QTreeWidgetItem * ResXmlParser::addObject(QTreeWidgetItem *_parentItem, QDomNode _node)
{
    QTreeWidgetItem * objectItem = new QTreeWidgetItem(_parentItem, QStringList(_node.toElement().attribute("name")),ResXmlParser::OBJECT);
    objectItem->setIcon(0,QPixmap(":/ResEditor/Resources/document-film"));
    objectItem->setText(2,"node");
    objectItem->setText(3,_node.toElement().attribute("name"));
    treeNodes->insert(objectItem,_node);
    return objectItem;
}
