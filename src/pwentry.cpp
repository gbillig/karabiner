#include "../inc/pwentry.h"

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

