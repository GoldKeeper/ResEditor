#ifndef DLGTEXTURE_H
#define DLGTEXTURE_H

#include "ui_dialogTextures.h"
#include <QDialog>

class DlgTexture : public QDialog
{
	Q_OBJECT

public:
	DlgTexture(QWidget *parent = 0, Qt::WFlags flags = 0): QDialog(parent, flags)
	{ui.setupUi(this);}
	~DlgTexture(){}

//private:
	Ui::DialogTextures ui;

};

#endif // DLGTEXTURE_H
