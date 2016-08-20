#include "../inc/user.h"
#include "../inc/glbcrypto/misc.h"

User::User()
{

}

User::User(QString username,
           QByteArray auth_salt,
           QByteArray key_salt,
           QByteArray iv,
           QByteArray auth_hash)
    : username(username),
      auth_salt(auth_salt),
      key_salt(key_salt),
      iv(iv),
      auth_hash(auth_hash)
{

}

User::User(QString username, QString password)
    : username(username)
{
    // create auth_salt and key_salt
    // generate crypto secure pseudo random number for iv
    // calculate auth_hash with auth_salt + password

    auth_salt = QByteArray("", 32);
    get_random((unsigned char*) auth_salt.data(), 32);

    key_salt = QByteArray("", 32);
    get_random((unsigned char*) key_salt.data(), 32);

    iv = QByteArray("", 32);
    get_random((unsigned char*) iv.data(), 32);


}

void User::SetUsername(QString new_username) {
    this->username = new_username;
}

QString User::GetUsername() {
    return this->username;
}

void User::SetAuthSalt(QByteArray new_auth_salt) {
    this->auth_salt = new_auth_salt;
}

QByteArray User::GetAuthSalt() {
    return this->auth_salt;
}

void User::SetKeySalt(QByteArray new_key_salt) {
    this->key_salt = new_key_salt;
}

QByteArray User::GetKeySalt() {
    return this->key_salt;
}

void User::SetIV(QByteArray new_iv) {
    this->iv = new_iv;
}

QByteArray User::GetIV() {
    return this->iv;
}

void User::SetAuthHash(QByteArray new_auth_hash) {
    this->auth_hash = new_auth_hash;
}

QByteArray User::GetPasswordHash() {
    return this->auth_hash;
}
