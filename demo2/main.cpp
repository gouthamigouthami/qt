#include <QCoreApplication>
#include <QByteArray>
#include <QDebug>

// Function to calculate CRC checksum
quint8 calculateCRC(const QByteArray &data) {
    quint8 crc = 0;
    for (char byte : data) {
        crc ^= byte;
        for (int i = 0; i < 8; ++i) {
            if (crc & 0x01)
                crc = (crc >> 1) ^ 0x8C; // CRC-8 polynomial (0x8C)
            else
                crc >>= 1;
        }
    }
    return crc;
}

// Function to validate CRC checksum
bool validateCRC(const QByteArray &dataWithCRC) {
    if (dataWithCRC.size() < 2) // Minimum size for data + CRC
        return false;

    QByteArray data = QByteArray::fromHex(dataWithCRC.mid(2, dataWithCRC.size() - 4)); // Convert HEX string to bytes
    quint8 receivedCRC = static_cast<quint8>(dataWithCRC.mid(dataWithCRC.size() - 2, 2).toInt(nullptr, 16)); // Convert last two characters of HEX string to quint8
    quint8 calculatedCRC = calculateCRC(data);
    qDebug()<<"rec:"<<receivedCRC;

    return receivedCRC == calculatedCRC;
}

int main() {
    QByteArray frame = QByteArray::fromHex("S00600004844521B"); // Frame
    QByteArray data = frame.mid(1, frame.size() - 3); // Extract data from frame

    // Calculate CRC checksum
    quint8 crc = calculateCRC(data);

    qDebug() << "Calculated CRC:" << crc;

    // Validate CRC checksum
    bool isValid = validateCRC(frame);
    if (isValid) {
        qDebug() << "CRC checksum is valid.";
    } else {
        qDebug() << "CRC checksum is not valid.";
    }

    return 0;
}
