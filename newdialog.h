#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>

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
    QCheckBox *caseCheckBox;
    QCheckBox *fromStartCheckBox;
    QCheckBox *wholeWordsCheckBox;
    QCheckBox *searchSelectionCheckBox;
    QCheckBox *backwardCheckBox;
    QDialogButtonBox *buttonBox;
    QPushButton *doneButton;
    QPushButton *cancelButton;
    QWidget *extension;
};


#endif // NEWDIALOG_H
