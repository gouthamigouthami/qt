#ifndef BUFFERHANDLER_H
#define BUFFERHANDLER_H

#include <QObject>

class BufferHandler : public QObject
{
    Q_OBJECT
public:
    explicit BufferHandler(QObject *parent = nullptr);
    QByteArray extractData(const QByteArray &buffer);
    QByteArray senddataToBuffer(const QByteArray &buffer,quint8);
    bool  validateCRC(const QByteArray &frame);
    QByteArray sendFrame(const quint8 startId, const quint8 sequenceNo, const quint8 messageLength, const QByteArray &data);


signals:
};

#endif // BUFFERHANDLER_H
