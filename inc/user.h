#ifndef USER_H
#define USER_H

#include <stdint.h>
#include <QString>
#include <QByteArray>
#include <QVector>
#include "../inc/pwentry.h"

class User
{

public:
    User();
    User(QString username,
         QString password);
    User(QString username,
         QByteArray auth_salt,
         QByteArray key_salt,
         QByteArray iv,
         QByteArray auth_hash,
         QVector<PwEntry> password_entries);

    QString username;
    QByteArray auth_salt;
    QByteArray key_salt;
    QByteArray iv;
    QByteArray auth_hash;

    QVector<PwEntry> password_entries;

    int Authenticate(QString password);

private:
    void DecryptAllPwEntries(QByteArray key);
};

#endif // USER_H
