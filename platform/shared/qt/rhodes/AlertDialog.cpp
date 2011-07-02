#include "AlertDialog.h"
#include "ui_AlertDialog.h"

AlertDialog::AlertDialog(CDateTimeMessage* msg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlertDialog)
{
    ui->setupUi(this);
}

AlertDialog::~AlertDialog()
{
    delete ui;
}
