#include "datatransfer.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>

DataTransfer::DataTransfer(QObject *parent) : QObject(parent) {}

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
        static int frame = 1;

        QByteArray lineBuffer = in.readLine().toLatin1(); // Convert QString to QByteArray

        // Skip empty lines or lines that don't start with 'S' followed by a digit
        if (lineBuffer.isEmpty() || !(lineBuffer.startsWith('S') && lineBuffer[1] >= '0' && lineBuffer[1] <= '9'))
            continue;

        QByteArray transferFrame = createTransferFrame(lineBuffer);
        if (!writeToTransferFile(transferFrame, frame)) {
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
    QByteArray data = line.mid(8, line.length() - 10);
    QByteArray sequenceNo = data.mid(0, 2);
    QByteArray messageLength = data.mid(2, 2);
    QByteArray messageData = data.mid(4);
    QByteArray crc = line.mid(line.length() - 2, 2);

    transferFrame = "[RX] [CAN] " + getCurrentDate().toUtf8() + " " + getCurrentTime().toUtf8() + " "
                    + sequenceNo + " " + messageLength + " " + messageData + " " + crc;
    return transferFrame;
}

bool DataTransfer::writeToTransferFile(const QByteArray &line, int frame) {
    QFile transferFile("transfer.txt");
    if (!transferFile.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Error: Failed to open transfer file!";
        return false;
    }

    QTextStream out(&transferFile);
    int size = line.size();
    qDebug() << "Size of the frame " << frame << " is: " << size;

    // Encrypt the line before writing to the transfer file
    QByteArray key = "my_secret_key"; // Your encryption key
    QByteArray password = "my_password"; // Your password
    QByteArray encryptedLine = encrypt(line, key, password);

    out << "KEY: " << key << "\n";
    out << "PASSWORD: " << password << "\n";
    out << encryptedLine << "\n";

    transferFile.close();
    return true;
}

QByteArray DataTransfer::encrypt(const QByteArray &data, const QByteArray &key, const QByteArray &password) {
    // XOR encryption algorithm
    QByteArray encryptedData;
    for (int i = 0; i < data.size(); ++i) {
        encryptedData.append(data.at(i) ^ key.at(i % key.size()) ^ password.at(i % password.size()));
    }

    return encryptedData;
}

QByteArray DataTransfer::decrypt(const QByteArray &data, const QByteArray &key, const QByteArray &password) {
    // XOR decryption algorithm
    QByteArray decryptedData;
    for (int i = 0; i < data.size(); ++i) {
        decryptedData.append(data.at(i) ^ key.at(i % key.size()) ^ password.at(i % password.size()));
    }

    return decryptedData;
}

void DataTransfer::openTransferFile() {
    QFile transferFile("transfer.txt");
    if (!transferFile.exists()) {
        qDebug() << "Error: Transfer file does not exist!";
        return;
    }

    QByteArray key, password;

    if (!transferFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Failed to open transfer file!";
        return;
    }

    QTextStream in(&transferFile);
    while (!in.atEnd()) {
        QByteArray line = in.readLine().toLatin1();
        if (line.startsWith("KEY: ")) {
            key = line.mid(5).trimmed();
        } else if (line.startsWith("PASSWORD: ")) {
            password = line.mid(10).trimmed();
        }
    }

    transferFile.close();

    QFile decryptedFile("decrypted.txt");
    if (!decryptedFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error: Failed to create decrypted file!";
        return;
    }

    if (!transferFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Failed to open transfer file!";
        return;
    }

    QTextStream out(&decryptedFile);
    while (!in.atEnd()) {
        QByteArray line = in.readLine().toLatin1();
        if (!line.startsWith("KEY: ") && !line.startsWith("PASSWORD: ")) {
            QByteArray encryptedLine = line;
            QByteArray decryptedLine = decrypt(encryptedLine, key, password);
            out << decryptedLine << "\n";
        }
    }

    transferFile.close();
decryptedFile.close();
}
