#include "../inc/user.h"
#include "../inc/glbcrypto/misc.h"
#include "../inc/glbcrypto/sha.h"

User::User()
{

}

User::User(QString username,
           QByteArray auth_salt,
           QByteArray key_salt,
           QByteArray iv,
           QByteArray auth_hash,
           QVector<PwEntry> password_entries)
    : username(username),
      auth_salt(auth_salt),
      key_salt(key_salt),
      iv(iv),
      auth_hash(auth_hash),
      password_entries(password_entries)
{

}

User::User(QString username, QString password)
    : username(username)
{
    // create auth_salt and key_salt
    // generate crypto secure pseudo random number for iv
    // calculate auth_hash with auth_salt + password

    auth_salt = QByteArray("", 32);
    get_random((uint8_t*) auth_salt.data(), 32);

    key_salt = QByteArray("", 32);
    get_random((uint8_t*) key_salt.data(), 32);

    iv = QByteArray("", 32);
    get_random((uint8_t*) iv.data(), 32);

    QByteArray salted_password = password.toLatin1();
    salted_password.prepend(auth_salt);

    auth_hash = QByteArray("", 32);
    sha_256((uint8_t*) auth_hash.data(), (uint8_t*) salted_password.data(), salted_password.length() * 8);
}

void User::SetUsername(QString new_username) {
    username = new_username;
}

QString User::GetUsername() {
    return username;
}

void User::SetAuthSalt(QByteArray new_auth_salt) {
    auth_salt = new_auth_salt;
}

QByteArray User::GetAuthSalt() {
    return auth_salt;
}

void User::SetKeySalt(QByteArray new_key_salt) {
    key_salt = new_key_salt;
}

QByteArray User::GetKeySalt() {
    return key_salt;
}

void User::SetIV(QByteArray new_iv) {
    iv = new_iv;
}

QByteArray User::GetIV() {
    return iv;
}

void User::SetAuthHash(QByteArray new_auth_hash) {
    auth_hash = new_auth_hash;
}

QByteArray User::GetAuthHash() {
    return auth_hash;
}
