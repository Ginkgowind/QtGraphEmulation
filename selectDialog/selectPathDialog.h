#pragma once

#include <QDialog>
#include "ui_selectPathDialog.h"

class QButtonGroup;

QT_BEGIN_NAMESPACE
namespace Ui { class selectPathDialogClass; };
QT_END_NAMESPACE

class selectPathDialog : public QDialog
{
	Q_OBJECT

public:
	selectPathDialog(int number, QWidget *parent = nullptr);
	~selectPathDialog();
	QButtonGroup* pathBtnGroup;

private:
	Ui::selectPathDialogClass *ui;
	int num;
};
