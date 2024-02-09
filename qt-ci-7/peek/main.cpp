#include <QByteArray>
#include <QDebug>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QBuffer>
#include <QTextStream>

int main() {
    QByteArray byteArray("Hello, Qt!");

    // Peeking at the first character
    const char& firstChar = byteArray.peek(0);

    qDebug() << "First character:" << firstChar;

    // No modification to the original data
    qDebug() << "Original QByteArray:" << byteArray;

    return 0;
}
