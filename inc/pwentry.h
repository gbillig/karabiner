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
    PwEntry(QString service_name, QString username, QString password, QString notes);
    PwEntry(QByteArray encrypted_data);

    QString service_name;
    QString username;
    QString password;
    QString notes;

    QByteArray encrypted_data;

    int EncryptEntry(QByteArray key, QByteArray iv);
    int DecryptEntry(QByteArray key, QByteArray iv);

    int ClearPlaintext();
    int ClearCiphertext();

private:
    bool encrypted;
    bool decrypted;
};

#endif // PWENTRY_H
