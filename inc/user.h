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

    void SetUsername(QString new_username);
    QString GetUsername();
    void SetAuthSalt(QByteArray new_auth_salt);
    QByteArray GetAuthSalt();
    void SetKeySalt(QByteArray new_key_salt);
    QByteArray GetKeySalt();
    void SetIV(QByteArray new_iv);
    QByteArray GetIV();
    void SetAuthHash(QByteArray new_auth_hash);
    QByteArray GetAuthHash();

private:
    QString username;
    QByteArray auth_salt;
    QByteArray key_salt;
    QByteArray iv;
    QByteArray auth_hash;

    QVector<PwEntry> password_entries;
};

#endif // USER_H
