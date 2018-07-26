#include "moydialog.h"
#include "ui_moydialog.h"

MoyDialog::MoyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MoyDialog)
{
    ui->setupUi(this);
}

MoyDialog::~MoyDialog()
{
    delete ui;
}

QString MoyDialog::getIP_N9000()
{
    return ui->lineEdit->text();
}

QString MoyDialog::getIP_HMP2020()
{
    return ui->lineEdit_6->text();
}

QString MoyDialog::getIP_Micran()
{
    return ui->lineEdit_4->text();
}
