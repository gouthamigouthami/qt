 #include <QCoreApplication>
#include <QDebug>
#include "datatransfer.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Step 1: Create example.txt and store the provided hexadecimal data
    DataTransfer transfer;
    if (!transfer.createExampleFile()) {
        qWarning() << "Error: Failed to create example file!";
        return -1;
    }else{
        qDebug()<<"successfully created example.txt file\n";
    }

    // Step 2: Transfer only relevant information to transfer.txt
    if (!transfer.transferData()) {
        qWarning() << "Error: Failed to transfer data!";
        return -1;
    }else{
        qDebug()<<"successfull transfer of file\n";
    }

    return a.exec();
}
