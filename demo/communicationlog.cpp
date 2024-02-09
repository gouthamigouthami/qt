#include "communicationlog.h"
#include "openssl/evp.h"
#include <openssl/rand.h>
#include <openssl/sha.h>
//#include "transfer.h"

Communicationlog::Communicationlog(QObject *parent)
    : QObject{parent}
{}
bool Communicationlog::openLogFile(const QString &fileName)
{
    QFile exampleFile(fileName);
    if (exampleFile.exists()) {
        qDebug() << "example.txt already exists.";
        return true;
    }


    if (!exampleFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error: Failed to create example file!";
        return false;
    }

    exampleFile.close();
    return true;
}

void Communicationlog::closeLogFile()
{
    m_logFile.close();
}
QString Communicationlog::getCurrentTime() {
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timeString = currentTime.toString("HH:mm:ss.zzz");
    return timeString;
}

QString Communicationlog::getCurrentDate() {
    QDateTime currentTime = QDateTime::currentDateTime();
    QString dateString = currentTime.toString("yy-MM-dd");
    return dateString;
}
bool Communicationlog::logData(const QString &data,const QString &fileName,int datalength)
{
    QFile transferFile(fileName);
    if (!transferFile.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Error: Failed to open transfer file!";
        return false;
    }
    QTextStream out(&transferFile);

    //qDebug() << "Size of the frame  "<<frame <<"is: " << size;
    out << "[RX] [CAN] " + getCurrentDate().toUtf8() + " " + getCurrentTime().toUtf8() + " "+ data << "\n";



    transferFile.close();
    return true;

}

void Communicationlog::setEncryptionKey(const QByteArray &key)
{
    m_encryptionKey = key;
}

// QByteArray Communicationlog::readDataFromFile(const QString &fileName)
// {
//     QFile file(fileName);
//     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         qDebug() << "Error: Failed to open file " << fileName;
//         return QByteArray();
//     }

//     QByteArray data = file.readAll();
//     file.close();
//     return data;
// }


QByteArray Communicationlog::encryptData(const QByteArray &data, const QByteArray &key)
{
    QByteArray encryptedData;

    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    // Create and initialize the context
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        qDebug() << "Error: Failed to create encryption context.";
        return encryptedData;
    }

    // Initialize the encryption operation
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, reinterpret_cast<const unsigned char*>(key.constData()), NULL) != 1) {
        qDebug() << "Error: Failed to initialize encryption operation.";
        EVP_CIPHER_CTX_free(ctx);
        return encryptedData;
    }

    encryptedData.resize(data.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc()));

    // Perform the encryption
    if (EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char*>(encryptedData.data()), &len, reinterpret_cast<const unsigned char*>(data.constData()), data.size()) != 1) {
        qDebug() << "Error: Failed to perform encryption.";
        EVP_CIPHER_CTX_free(ctx);
        return encryptedData;
    }

    ciphertext_len = len;

    // Finalize the encryption
    if (EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(encryptedData.data()) + len, &len) != 1) {
        qDebug() << "Error: Failed to finalize encryption.";
        EVP_CIPHER_CTX_free(ctx);
        return encryptedData;
    }

    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    // Resize the result to the actual ciphertext length
    encryptedData.resize(ciphertext_len);

    return encryptedData;
}


QByteArray Communicationlog::decryptData(const QByteArray &data, const QByteArray &key)
{
    QByteArray decryptedData;

    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;

    // Create and initialize the context
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        qDebug() << "Error: Failed to create decryption context.";
        return decryptedData;
    }

    // Initialize the decryption operation
    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, reinterpret_cast<const unsigned char*>(key.constData()), NULL) != 1) {
        qDebug() << "Error: Failed to initialize decryption operation.";
        EVP_CIPHER_CTX_free(ctx);
        return decryptedData;
    }

    decryptedData.resize(data.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc()));

    // Perform the decryption
    if (EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(decryptedData.data()), &len, reinterpret_cast<const unsigned char*>(data.constData()), data.size()) != 1) {
        qDebug() << "Error: Failed to perform decryption.";
        EVP_CIPHER_CTX_free(ctx);
        return decryptedData;
    }

    plaintext_len = len;

    // Finalize the decryption
    if (EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(decryptedData.data()) + len, &len) != 1) {
        qDebug() << "Error: Failed to finalize decryption.";
        EVP_CIPHER_CTX_free(ctx);
        return decryptedData;
    }

    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    // Resize the result to the actual plaintext length
    decryptedData.resize(plaintext_len);

    return decryptedData;
}

QByteArray Communicationlog::readDataFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Error: Failed to open file for reading.";
        return QByteArray();
    }

    QByteArray data = file.readAll();
    file.close();

    return data;
}



QByteArray Communicationlog::readKeyFromFile(const QString &fileName)
{
    QFile keyFile(fileName);

    if (keyFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&keyFile);
        QString key = in.readLine().trimmed();
        keyFile.close();

        if (!key.isEmpty()) {
            return key.toUtf8();
        } else {
            qDebug() << "Error: Key read from file " << fileName << " is empty.";
        }
    } else {
        qDebug() << "Error: Failed to open key file for reading - " << fileName;
    }

    return QByteArray();
}


bool Communicationlog::validateKey(const QString &enteredKey, const QString &storedKey)
{
    return (enteredKey == storedKey);
}
