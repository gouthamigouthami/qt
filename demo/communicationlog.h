#ifndef COMMUNICATIONLOG_H
#define COMMUNICATIONLOG_H

#include "aesmanager.h"
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

class Communicationlog : public QObject
{
    Q_OBJECT
public:
    explicit Communicationlog(QObject *parent = nullptr);
    bool openLogFile(const QString &fileName);
    void closeLogFile();
    bool logData(const QString &data, const QString &fileName, int);
    QString getCurrentTime();
    QString getCurrentDate();
    QByteArray encryptData(const QByteArray &data, const QByteArray &key);
    QByteArray decryptData(const QByteArray &data, const QByteArray &key);
    void setEncryptionKey(const QByteArray &key);
    QByteArray readDataFromFile(const QString &fileName);
    QByteArray readKeyFromFile(const QString &fileName);
    bool validateKey(const QString &enteredKey, const QString &storedKey);;

signals:
private:
    QFile m_logFile;
    QTextStream m_logStream;
    // AesManager m_aesManager;
    QByteArray m_encryptionKey;
};

#endif // COMMUNICATIONLOG_H

