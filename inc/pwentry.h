#ifndef PWENTRY_H
#define PWENTRY_H

#include <QString>
#include <QByteArray>

class PwEntry
{

public:
    PwEntry();
    PwEntry(QString username, QString password, QString notes);

private:
    QString username;
    QString password;
    QString notes;
    bool decrypted;

    QByteArray data;

};

#endif // PWENTRY_H
