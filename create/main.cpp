#include <QCoreApplication>
#include <QDebug>
#include "BufferHandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QByteArray bufferToProcess = QByteArray::fromHex("S31900018000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF79");

    BufferHandler handler;
    QByteArray resultBuffer = handler.extractAndAppend(bufferToProcess);

    qDebug() << "Result buffer:" << resultBuffer;

    return a.exec();
}
