#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include "../inc/userdata.h"
#include "../inc/newpassworddialog.h"
#include "../inc/mainwindow.h"

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>

class NewPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    NewPasswordDialog(QWidget *parent = 0);

private slots:
    void accept();

private:
    UserData *userdata;

    QLineEdit *serviceNameEdit;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
};


#endif // PASSWORDDIALOG_H
