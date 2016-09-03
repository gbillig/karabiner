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
    PwEntry(QByteArray encrypted_data);

    QString username;
    QString password;
    QString notes;

    QByteArray encrypted_data;

private:
    bool encrypted;
    bool decrypted;

    int EncryptEntry(QByteArray key, QByteArray iv);
    int DecryptEntry(QByteArray key, QByteArray iv);

    int ClearPlaintext();
    int ClearCiphertext();
};

#endif // PWENTRY_H
