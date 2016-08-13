#ifndef USER_H
#define USER_H

#include <stdint.h>
#include <QString>
#include <QByteArray>

class User
{

public:
    User();
    User(QString username,
         QByteArray auth_salt,
         QByteArray key_salt,
         QByteArray iv,
         QByteArray auth_hash);

    void SetUsername(QString new_username);
    QString GetUsername();
    void SetAuthSalt(QByteArray new_auth_salt);
    QByteArray GetAuthSalt();
    void SetKeySalt(QByteArray new_key_salt);
    QByteArray GetKeySalt();
    void SetIV(QByteArray new_iv);
    QByteArray GetIV();
    void SetAuthHash(QByteArray new_auth_hash);
    QByteArray GetPasswordHash();

private:
    QString username;
    QByteArray auth_salt;
    QByteArray key_salt;
    QByteArray iv;
    QByteArray auth_hash;
};

#endif // USER_H\
