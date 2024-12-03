#include "sobredialog.h"
#include "ui_sobredialog.h"

sobreDialog::sobreDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sobreDialog)
{
    ui->setupUi(this);
}

sobreDialog::~sobreDialog()
{
    delete ui;
}
