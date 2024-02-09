#include "BufferHandler.h"

BufferHandler::BufferHandler() {}

QByteArray BufferHandler::extractData(const QByteArray &buffer) {
    // Check if the buffer has sufficient length
    if (buffer.length() < 5)
        return QByteArray(); // Return empty QByteArray if buffer is too short

    // Extract data
    QByteArray extractedData;
    extractedData.append(buffer[0]); // Start ID
    extractedData.append(buffer[1]); // Sequence no
    quint8 messageLength = buffer[2];
    extractedData.append(messageLength); // Message length

    // Extract data bytes
    for (int i = 0; i < messageLength; ++i) {
        extractedData.append(buffer[3 + i]);
    }

    return extractedData;
}

quint8 BufferHandler::calculateCRC(const QByteArray &data) {
    // CRC calculation logic goes here (not provided in the question)
    // Placeholder implementation assuming CRC is 0x00
    return 0x00;
}

QByteArray BufferHandler::extractAndAppend(const QByteArray &buffer) {
    QByteArray extractedData = extractData(buffer);
    if (extractedData.isEmpty())
        return QByteArray(); // Return empty QByteArray if extraction failed

    // Calculate CRC
    quint8 crc = calculateCRC(extractedData);

    // Append CRC to the extracted data
    extractedData.append(crc);

    return extractedData;
}
