#include "bufferhandler.h"

BufferHandler::BufferHandler(QObject *parent)
    : QObject{parent}
{}
QByteArray BufferHandler::extractData(const QByteArray &line){
    QByteArray transferFrame;

    // Extract startId, sequenceNo, messageLength, and crc
    quint8 startId = static_cast<quint8>(line.at(0));
    quint8 sequenceNo = static_cast<quint8>(line.at(1));
    quint8 messageLength =  static_cast<quint8>(line.at(2));


    // Extract data
    QByteArray data = senddataToBuffer(line,messageLength );


    // Construct frame
    transferFrame = sendFrame(startId, sequenceNo, messageLength, data);

    return transferFrame;
}

QByteArray BufferHandler::senddataToBuffer(const QByteArray &line, quint8 messageLength) {
    // Extract data from the line
    QByteArray messageData = line.mid(4, (messageLength - 1));
    quint8 Byte = 123;  // Assuming this is the chunk size

    QByteArray crc = line.mid(line.length() - 2, 2);


    // Send data in chunks based on Byte size
    QByteArray sendData;
    for (int i = 0; i < messageData.size(); i += Byte) {
        QByteArray chunk = messageData.mid(i, Byte);
        sendData.append(chunk);
    }
    if(validateCRC(crc)){
    // Add CRC to the final result
        sendData.append(crc);}

    return sendData;
}

QByteArray BufferHandler::sendFrame(const quint8 startId, const quint8 sequenceNo, const quint8 messageLength, const QByteArray &data){
    // Construct result buffer
    QByteArray resultBuffer;
    resultBuffer.append(startId);
    resultBuffer.append(sequenceNo);
    resultBuffer.append(messageLength);
    resultBuffer.append(data);
    return resultBuffer;
}
bool  BufferHandler::validateCRC(const QByteArray &frame) {
    // Compute CRC over the frame (excluding the last 2 bytes which are the CRC itself)
    quint16 crc16 = qChecksum(frame.constData(), frame.size() - 2);

    // Extract CRC from the frame
    quint16 receivedCRC = static_cast<quint8>(frame.at(frame.size() - 2)) |
                          static_cast<quint8>(frame.at(frame.size() - 1)) << 8;

    // Compare computed CRC with received CRC
    return crc16 == receivedCRC;
}
