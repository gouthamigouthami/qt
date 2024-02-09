// aesmanager.cpp
#include "aesmanager.h"
#include "qdebug.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

AesManager::AesManager(QObject *parent)
    : QObject(parent)
{
    // Initialize AES key (you might want to implement key management securely)
    AES_set_encrypt_key(reinterpret_cast<const unsigned char*>("YourEncryptionKey"), 128, &m_aesKey);
}


QByteArray AesManager::deriveKeyFromPassword(const QString &password)
{
    QByteArray derivedKey;
    const EVP_MD *digest = EVP_sha256();

    // Use a Key Derivation Function (KDF) - PBKDF2 in this case
    int iterations = 10000; // You can adjust the number of iterations based on your security requirements

    derivedKey.resize(EVP_MD_size(digest));
    PKCS5_PBKDF2_HMAC(
        password.toUtf8().constData(), password.toUtf8().length(),
        nullptr, 0,
        iterations,
        digest,
        derivedKey.size(),
        reinterpret_cast<unsigned char *>(derivedKey.data())
        );

    return derivedKey;
}


QByteArray AesManager::encryptData(const QByteArray &data, const QByteArray &key)
{
    QByteArray encryptedData(data.size(), 0);
    AES_set_encrypt_key(reinterpret_cast<const unsigned char*>(key.constData()), 128, &m_aesKey);
    AES_encrypt(reinterpret_cast<const unsigned char*>(data.constData()),
                reinterpret_cast<unsigned char*>(encryptedData.data()), &m_aesKey);
    return encryptedData;
}

QByteArray AesManager::decryptData(const QByteArray &data, const QByteArray &key)
{
    QByteArray decryptedData(data.size(), 0);
    AES_set_decrypt_key(reinterpret_cast<const unsigned char*>(key.constData()), 128, &m_aesKey);
    AES_decrypt(reinterpret_cast<const unsigned char*>(data.constData()),
                reinterpret_cast<unsigned char*>(decryptedData.data()), &m_aesKey);
    return decryptedData;
}

