#ifndef BUFFERHANDLER_H
#define BUFFERHANDLER_H

#include <QByteArray>

class BufferHandler
{
public:
    BufferHandler();
    QByteArray extractAndAppend(const QByteArray &buffer);

private:
    QByteArray extractData(const QByteArray &buffer);
    quint8 calculateCRC(const QByteArray &data);
};

#endif // BUFFERHANDLER_H
