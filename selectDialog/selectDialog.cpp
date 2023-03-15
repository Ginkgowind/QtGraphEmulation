#include "selectDialog.h"

selectDialog::selectDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::selectDialogClass())
{
	ui->setupUi(this);

	setFixedSize(size());

	connect(ui->btn_ok, &QPushButton::clicked, [this]() {close(); });
}

selectDialog::~selectDialog()
{
	delete ui;
}
