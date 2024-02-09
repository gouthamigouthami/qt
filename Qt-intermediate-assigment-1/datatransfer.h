
#ifndef DATATRANSFER_H
#define DATATRANSFER_H

#include <QDateTime>
#include <QByteArray>

class DataTransfer {
public:
    DataTransfer();
    bool createExampleFile();
    bool transferData();

private:
    QByteArray m_hexDataBuffer;
    QByteArray m_transferBuffer;
    QDateTime m_currentDateTime;
    QDateTime currentTime;

    bool readFile(const QString &fileName);
    QString getCurrentTime();
    QString getCurrentDate();
    QString convertToSREC(const QByteArray &data);
    QByteArray createTransferFrame(const QByteArray &data);
    bool writeToTransferFile(const QByteArray &data,int);
};

#endif // DATATRANSFER_H
