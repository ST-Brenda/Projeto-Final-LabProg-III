#ifndef SOBREDIALOG_H
#define SOBREDIALOG_H

#include <QDialog>

namespace Ui {
class sobreDialog;
}

class sobreDialog : public QDialog
{
    Q_OBJECT

public:
    explicit sobreDialog(QWidget *parent = nullptr);
    ~sobreDialog();

private:
    Ui::sobreDialog *ui;
};

#endif // SOBREDIALOG_H
