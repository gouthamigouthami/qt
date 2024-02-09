#include "transfer.h"
#include "aesmanager.h"
#include "communicationlog.h"
#include "BufferHandler.h"

AesManager aesManager;

Transfer::Transfer(QObject *parent)
    : QObject{parent}
{}

bool Transfer::loadingdataToBuffer(const QString &fileName){
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Failed to open file " << fileName;
        return false;
    }

    QByteArray buffer;
    while (!file.atEnd()) {
        QByteArray lineBuffer = file.readLine();
        buffer.append(lineBuffer);

        // Check if buffer size exceeds a certain threshold
        if (buffer.size() >= 1024) { // Adjust buffer size as needed
            loadingData(buffer);
            buffer.clear(); // Clear the buffer after processing
        }
    }
    file.close();
    return true;
}

bool Transfer::loadingData(const QByteArray &buffer) {
    Communicationlog com;
    BufferHandler buff;
    QList<QByteArray> lines = buffer.split('\n');
    for (const QByteArray &line : lines) {
        // Skip empty lines or lines that don't start with 'S' followed by a digit
        if (line.isEmpty() || !(line.startsWith('S') && line[1] >= '0' && line[1] <= '9'))
            continue;
        QByteArray transferFrame = buff.extractData(line);
        int datalength = Tranferdatasize(transferFrame);
        QString filename = "transfer.txt";

        com.logData(transferFrame, filename, datalength);
        //encryptAndLog(filename, "YourEncryptionKey", transferFrame);
    }

    return true;
}

int Transfer::Tranferdatasize(const QByteArray &line){
    QByteArray data = line.mid(8, line.length() - 10);
    QByteArray messageData = data.mid(4);
    int datasize = messageData.size();
    return datasize;
}

void Transfer::encryptAndLog(const QString &logFilePath, const QString &key, const QString &data)
{
    Communicationlog com;
    QByteArray dataToLog = data.toUtf8();
    QByteArray encryptedData = aesManager.encryptData(dataToLog, key.toUtf8());

    // com.logData(encryptedData, logFilePath, 0);
    QFile logFile(logFilePath);
    if (!logFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Error: Failed to open log file for writing.";
        return;
    }

    logFile.write(encryptedData);
    logFile.close();
}

QString Transfer::decryptAndRead(const QString &logFilePath, const QString &key, const QString &outputFilePath)
{
    AesManager aesManager;
    QByteArray derivedKey = aesManager.deriveKeyFromPassword(key.toUtf8());

    QFile logFile(logFilePath);
    if (!logFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Error: Failed to open log file for reading.";
        return "";
    }

    QByteArray encryptedData = logFile.readAll();
    QByteArray decryptedData = aesManager.decryptData(encryptedData, derivedKey);

    logFile.close();

    QString decryptedText = QString::fromUtf8(decryptedData);
    qDebug() << "Data decrypted and read successfully.";

    // Save decrypted data to output file
    QFile outputFile(outputFilePath);
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error: Failed to open output file for writing.";
        return "";
    }

    QTextStream out(&outputFile);
    out << decryptedText;
    outputFile.close();

    qDebug() << "Decrypted data saved to " << outputFilePath;

    return decryptedText;
}
