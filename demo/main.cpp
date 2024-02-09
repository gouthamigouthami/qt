#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include "transfer.h"
#include "communicationlog.h"



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString filename = "example.txt";
    QString transFile="transfer.txt";
    QString decryptFile="decrypt.txt";
    Communicationlog comm;

    QString encryptionKey = comm.readKeyFromFile("encryptionkey.txt");

    // Check if the key is valid
    if (encryptionKey.isEmpty()) {
        qDebug() << "Error: Encryption key is empty or couldn't be read.";
        return 1;  // or handle the error appropriately
    }

    Transfer trans;

    if (trans.loadingdataToBuffer(filename)) {
        qDebug() << "Successful transfer of data\n";

        // Prompt for decryption key
        qDebug() << "Enter decryption key:";
        QString enteredDecryptionKey;
        QTextStream stream(stdin);
        stream >> enteredDecryptionKey;
        enteredDecryptionKey = enteredDecryptionKey.trimmed();

        // Decrypt and verify data
        QString storedDecryptionKey = comm.readKeyFromFile("decryptionkey.txt");
        if (comm.validateKey(enteredDecryptionKey, storedDecryptionKey)) {
            qDebug() << "Decryption key is correct. Decrypting data...";
            QString decryptedText = trans.decryptAndRead(transFile, enteredDecryptionKey, decryptFile);
            //qDebug() << "Decrypted Data: " << decryptedText;


        } else {
            qDebug() << "Error: Wrong decryption key. Please try again.";
        }
    } else {
        qDebug() << "No successful transfer of data\n";
    }
    return a.exec();
}
