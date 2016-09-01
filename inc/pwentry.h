#ifndef PWENTRY_H
#define PWENTRY_H

#include <QString>
#include <QByteArray>
#include <QIODevice>
#include <QDataStream>

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

    QByteArray encrypted_data;

    int EncryptEntry(QByteArray key, QByteArray iv);
};

#endif // PWENTRY_H
