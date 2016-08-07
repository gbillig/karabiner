#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>
#include <QRadioButton>

class QCheckBox;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;

class NewDialog : public QDialog
{
    Q_OBJECT

public:
    NewDialog(QWidget *parent = 0);

private:
    QLabel *nameLabel;
    QLineEdit *nameEdit;
    QGroupBox *cryptoBox;
    QRadioButton *Aes128RadioButton;
    QRadioButton *Aes192RadioButton;
    QRadioButton *Aes256RadioButton;
    QGroupBox *hashBox;
    QRadioButton *Sha256RadioButton;
    QRadioButton *Sha384RadioButton;
    QRadioButton *Sha512RadioButton;

    QDialogButtonBox *buttonBox;
    QPushButton *doneButton;
    QPushButton *cancelButton;
};


#endif // NEWDIALOG_H
