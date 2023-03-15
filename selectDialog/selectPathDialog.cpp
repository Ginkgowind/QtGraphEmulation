#include "selectPathDialog.h"
#include "../Config/config.h"
#include <QPushButton>
#include <QButtonGroup>

selectPathDialog::selectPathDialog(int number, QWidget *parent)
	: num(number), QDialog(parent)
	, ui(new Ui::selectPathDialogClass())
{
	ui->setupUi(this);

	QSize thisSize = size();
	resize(thisSize + QSize{0, num * 30});

	pathBtnGroup = new QButtonGroup(this);
	for (int i = 0; i < num; i++) {
		QPushButton* btn = new QPushButton(QString(i + 1), this);
		btn->setText(QString("µÚ%1ÌõÂ·¾¶").arg(i + 1));
		pathBtnGroup->addButton(btn, i);
		btn->resize(240, 25);
		btn->move(30, thisSize.rheight() + i * 30);
	}

	setWindowModality(Qt::WindowModal);
	setAttribute(Qt::WA_DeleteOnClose);
	show();
}

selectPathDialog::~selectPathDialog()
{
	delete ui;
}
