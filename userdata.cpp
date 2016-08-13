#include "userdata.h"

UserData::UserData()
{

}

int UserData::ReadFromFile(QString filepath)
{
    QFile file(filepath);
    file.open(QIODevice::ReadOnly);
    QDataStream file_stream(&file);    // read the data serialized from the file

    // Read and check the header
    quint32 magic;
    file_stream >> magic;
    if (magic != 0x67EB67EB) {
        return 1; //bad file format
    }

    // Read the version
    quint8 version;
    file_stream >> version;

    return 0;
}
