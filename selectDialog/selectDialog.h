#pragma once

#include <QDialog>
#include "ui_selectDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class selectDialogClass; };
QT_END_NAMESPACE

class selectDialog : public QDialog
{
	Q_OBJECT

public:
	selectDialog(QWidget *parent = nullptr);
	~selectDialog();

//private:
	Ui::selectDialogClass *ui;
};
