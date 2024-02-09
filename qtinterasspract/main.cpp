#include <QCoreApplication>
#include "datatransfer.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DataTransfer transfer;
    if (transfer.createExampleFile()) {
        qDebug() << "Example file created successfully.";
    } else {
        qDebug() << "Failed to create example file!";
        return -1;
    }

    if (transfer.transferData()) {
        qDebug() << "Data transferred successfully.";
    } else {
        qDebug() << "Failed to transfer data!";
        return -1;
    }

    transfer.openTransferFile();

    return a.exec();
}
