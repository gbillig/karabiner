#include "../inc/pwentry.h"
#include "../inc/glbcrypto/aes.h"

PwEntry::PwEntry()
{
    decrypted = false;
}

PwEntry::PwEntry(QString username, QString password, QString notes)
    : username(username),
      password(password),
      notes(notes)
{
    decrypted = true;
}

int PwEntry::EncryptEntry(QByteArray key, QByteArray iv) {

    // username and password fields cannot be empty
    if (!username.length() || !password.length()) {
        return 1;
    }

    // clear encrypted data if it isn't empty
    if (encrypted_data.length()) {
        encrypted_data.clear();
    }

    QByteArray plaintext_data;
    QDataStream data_stream(&plaintext_data, QIODevice::WriteOnly);

    data_stream << username;
    data_stream << password;
    data_stream << notes;

    aes_256_cbc((uint8_t*) encrypted_data.data(), (uint8_t*) plaintext_data.data(), plaintext_data.size(),
                (uint8_t*) iv.data(), (uint8_t*) key.data(), key.size(), 0);

    return 0;
}
