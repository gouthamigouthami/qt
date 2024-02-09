#ifndef DATATRANSFER_H
#define DATATRANSFER_H

#include <QObject>

class DataTransfer : public QObject
{
    Q_OBJECT
public:
    explicit DataTransfer(QObject *parent = nullptr);

    bool createExampleFile();
    bool transferData();
    void openTransferFile();

private:
    QString getCurrentTime();
    QString getCurrentDate();
    QByteArray createTransferFrame(const QByteArray &line);
    bool writeToTransferFile(const QByteArray &line, int frame);
    QByteArray encrypt(const QByteArray &data, const QByteArray &key, const QByteArray &password);
    QByteArray decrypt(const QByteArray &data, const QByteArray &key, const QByteArray &password);
};

#endif // DATATRANSFER_H
