#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include "userdata.h"
#include <QDialog>

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    PasswordDialog(QWidget *parent = 0);

private slots:
    void accept();

private:
    UserData *userdata;
};


#endif // PASSWORDDIALOG_H
