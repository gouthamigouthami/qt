#ifndef TRANSFER_H
#define TRANSFER_H

#include <QObject>

class Transfer : public QObject
{
    Q_OBJECT
public:
    explicit Transfer(QObject *parent = nullptr);
   // QByteArray srecord(const QByteArray &data);
    QByteArray senddataToBuffer(const QByteArray &line);
    int Tranferdatasize(const QByteArray &data);
    bool loadingData(const QByteArray &buffer);
    bool loadingdataToBuffer(const QString &fileName);

    void encryptAndLog(const QString &logFilePath, const QString &key, const QString &data);
    QString decryptAndRead(const QString &logFilePath, const QString &key, const QString &outputFilePath);
signals:

};

#endif // TRANSFER_H
