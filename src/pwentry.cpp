#include "../inc/pwentry.h"
#include "../inc/glbcrypto/aes.h"
#include "../inc/glbcrypto/misc.h"

PwEntry::PwEntry()
{
    encrypted = false;
    decrypted = false;
}

PwEntry::PwEntry(QString username, QString password, QString notes)
    : username(username),
      password(password),
      notes(notes)
{
    encrypted = false;
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

    // to serialize QString, it takes a quint32 (4 bytes) for size and string in UTF-16 (2 bytes per char)
    int plaintext_data_length = 4 + 2 * username.length() +
                                4 + 2 * password.length() +
                                4 + 2 * notes.length();

    // round up to to the nearest multiple of 16
    int plaintext_data_length_with_pad = (plaintext_data_length / 16) * 16;

    QByteArray plaintext_data = QByteArray("", plaintext_data_length);
    QByteArray plaintext_data_pad = QByteArray("", plaintext_data_length_with_pad);

    QDataStream data_stream(&plaintext_data, QIODevice::WriteOnly);

    data_stream << username;
    data_stream << password;
    data_stream << notes;

    pad_message((uint8_t*) plaintext_data_pad.data(), (uint8_t*) plaintext_data.data(), plaintext_data_length, 16);

    encrypted_data = QByteArray("", 32);

    aes_256_cbc((uint8_t*) encrypted_data.data(), (uint8_t*) plaintext_data_pad.data(), plaintext_data_pad.size(),
                (uint8_t*) iv.data(), (uint8_t*) key.data(), key.size(), 0);

    encrypted = true;
    decrypted = true;

    return 0;
}

int PwEntry::DecryptEntry(QByteArray key, QByteArray iv) {

    return 0;
}

int PwEntry::ClearPlaintext() {
    username.clear();
    password.clear();
    notes.clear();

    decrypted = false;

    return 0;
}

int PwEntry::ClearCryptotext() {
    encrypted_data.clear();

    encrypted = false;

    return 0;
}

