// aesmanager.h
#ifndef AESMANAGER_H
#define AESMANAGER_H

#include <QObject>
#include <QByteArray>
#include <openssl/aes.h>

class AesManager : public QObject
{
    Q_OBJECT
public:
    explicit AesManager(QObject *parent = nullptr);

    // Function to derive an encryption key from a password
    QByteArray deriveKeyFromPassword(const QString &password);

    QByteArray encryptData(const QByteArray &data, const QByteArray &key);
    QByteArray decryptData(const QByteArray &data, const QByteArray &key);

private:
    AES_KEY m_aesKey;
    // m_encryptionKey;
};

#endif // AESMANAGER_H
