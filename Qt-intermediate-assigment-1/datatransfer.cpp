
#include "datatransfer.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>

DataTransfer::DataTransfer() {}


bool DataTransfer::createExampleFile() {
    QFile exampleFile("example.txt");
    if (exampleFile.exists()) {
        qDebug() << "example.txt already exists. Skipping creation.";
        return true;
    }

    if (!exampleFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error: Failed to create example file!";
        return false;
    }

    QTextStream out(&exampleFile);
    out << "S31900018000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF79\n";
    out << "S31900018028FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF51\n";
    // Add more lines as needed

    exampleFile.close();
    return true;
}
bool DataTransfer::transferData() {
    QFile file("example.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Failed to open file example.txt";
        return false;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        static int frame=1;

        QByteArray lineBuffer = in.readLine().toLatin1(); // Convert QString to QByteArray

        // Skip empty lines or lines that don't start with 'S' followed by a digit
        if (lineBuffer.isEmpty() || !(lineBuffer.startsWith('S') && lineBuffer[1] >= '0' && lineBuffer[1] <= '9'))
            continue;

        QByteArray transferFrame = createTransferFrame(lineBuffer);
        if (!writeToTransferFile(transferFrame,frame)) {
            qDebug() << "Error: Failed to write to transfer file!";
            file.close();
            return false;
        }
        frame++;
    }

    file.close();

    return true;
}

QString DataTransfer::getCurrentTime() {
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timeString = currentTime.toString("HH:mm:ss.zzz");
    return timeString;
}

QString DataTransfer::getCurrentDate() {
    QDateTime currentTime = QDateTime::currentDateTime();
    QString dateString = currentTime.toString("yy-MM-dd");
    return dateString;
}

QByteArray DataTransfer::createTransferFrame(const QByteArray &line) {
    QByteArray transferFrame;
    int size=line.size();
    QByteArray data = line.mid(8, line.length() - 10);
    QByteArray sequenceNo = data.mid(0, 2);
    QByteArray messageLength = data.mid(2, 2);
    QByteArray messageData = data.mid(4);
    QByteArray crc = line.mid(line.length() - 2, 2);

    transferFrame = "[RX] [CAN] " + getCurrentDate().toUtf8() + " " + getCurrentTime().toUtf8() + " "
                    + sequenceNo + " " + messageLength + " " + messageData + " " + crc;
    return transferFrame;
}

bool DataTransfer::writeToTransferFile(const QByteArray &line,int frame) {
    QFile transferFile("transfer.txt");
    if (!transferFile.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Error: Failed to open transfer file!";
        return false;
    }

    QTextStream out(&transferFile);
    int size = line.size();
    qDebug() << "Size of the frame  "<<frame <<"is: " << size;
    out << line << "\n";

    transferFile.close();
    return true;
}
