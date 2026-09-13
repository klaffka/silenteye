//  This file is part of SilentEye.
//
//  SilentEye is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  SilentEye is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SilentEye.  If not, see <http://www.gnu.org/licenses/>.

#include "secryptoaes.h"
#include "math.h"

namespace SECryptoAES {

    SECryptoAESModule::SECryptoAESModule()
    {
        this->setObjectName("SECryptoAES128");
        m_logger = new Logger(this);
        m_isQcaCompatible = false;
    }

    SECryptoAESModule::~SECryptoAESModule()
    {
        delete m_logger;
    }

    void SECryptoAESModule::init()
    {
        m_isQcaCompatible = QCA::isSupported("aes128-cbc-pkcs7");

        if( m_isQcaCompatible )
        {
            m_logger->info(name() + ": aes128-cbc-pkcs7 supported by system [OK]");
        }
        else
        {
            m_logger->warning(name() + ": aes128-cbc-pkcs7 supported by system [KO]");
            QList<QCA::Provider*> prov = QCA::providers();
            m_logger->debug("> QCA2: " + QString::number(prov.size()) + " providers found");
            for(int i = 0; i<prov.size(); i++)
                m_logger->debug(prov.at(i)->name());

            m_logger->debug(QCA::pluginDiagnosticText());
        }

    }


    QString SECryptoAESModule::name() const
    {
        return QString("Silent Eye Encryption "+typeSupported());
    }

    QString SECryptoAESModule::version() const
    {
        return QString("1.1");
    }

    QString SECryptoAESModule::status()
    {
        QCA::Initializer qcaInit;
        init();
        if(m_isQcaCompatible)
            return "OK|aes128-cbc-pkcs7 supported by system";
        else
            return "KO|'aes128-cbc-pkcs7' not supported by system.\nPlease check your 'libqca2-plugin-ossl' installation.";
    }

    QString SECryptoAESModule::typeSupported() const
    {
        return QString("AES128");
    }

    QCA::SecureArray SECryptoAESModule::initializationVector(QString key)
    {
        QByteArray hash = QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Md5);
        QString md5 = hash.toHex();
        QString value;
        for (int i=0; i<floor(128 / md5.size()); i++)
        {
            value += md5;
        }
        m_logger->debug("encrypted key: " + value);
        return QCA::InitializationVector( QCA::SecureArray(value.toUtf8()) );
    }

    QPointer<EncodedData> SECryptoAESModule::encode(QString key, QPointer<EncodedData> msg)
    {
        QCA::Initializer qcaInit;
        init();
        if(!m_isQcaCompatible){
            throw ModuleException("aes128-cbc-pkcs7 is not supported by the system.");
        }

        QCA::SymmetricKey cipherKey( QString(name()+"%/.?!:;]{[}&").toUtf8() );
        QCA::InitializationVector iv = initializationVector(key);

        // create a 128 bit AES cipher object using Cipher Block Chaining (CBC) mode
        QCA::Cipher cipher(QString("aes128"),QCA::Cipher::CBC,
                           // use Default padding, which is equivalent to PKCS7 for CBC
                           QCA::Cipher::DefaultPadding,
                           // this object will encrypt
                           QCA::Encode,
                           cipherKey, iv);

        // we use the cipher object to encrypt the argument we passed in
        // the result of that is returned - note that if there is less than
        // 1 block, then nothing will be returned - it is buffered
        // update() can be called as many times as required.
        QCA::SecureArray u = cipher.update( QCA::SecureArray(msg->bytes()) );

        if (!cipher.ok())
            throw ModuleException("An error occured during the encryption process.",
                                  "An error occured during the cipher update !");

        if(msg->format() != Data::FILE)
            m_logger->debug("AES128 non-final encryption of " + QCA::arrayToHex(msg->bytes()) + " is " + QCA::arrayToHex(u.toByteArray()) );

        // Because we are using PKCS7 padding, we need to output the final (padded) block
        // Note that we should always call final() even with no padding, to clean up
        QCA::SecureArray f = cipher.final();

        if (!cipher.ok())
            throw ModuleException("An error occured during the encryption process.",
                                  "An error occured during the finalization of the cipher !");

        return new EncodedData(u.append(f).toByteArray(), Data::BYTES, false);
    }

    QPointer<EncodedData> SECryptoAESModule::decode(QString key, QPointer<EncodedData> data)
    {
        QCA::Initializer qcaInit;
        init();
        if(!m_isQcaCompatible){
            throw ModuleException("aes128-cbc-pkcs7 is not supported by the system.");
        }

        QCA::SymmetricKey cipherKey( QString(name()+"%/.?!:;]{[}&").toUtf8() );
        QCA::InitializationVector iv = initializationVector(key);

        // create a 128 bit AES cipher object using Cipher Block Chaining (CBC) mode
        QCA::Cipher cipher(QString("aes128"),QCA::Cipher::CBC,
                           // use Default padding, which is equivalent to PKCS7 for CBC
                           QCA::Cipher::DefaultPadding,
                           // this object will encrypt
                           QCA::Decode,
                           cipherKey, iv);

        // take that cipher text, and decrypt it
        QCA::SecureArray plainText = cipher.update( QCA::SecureArray(data->toData()->data()) );

        // check if the update() call worked
        if (!cipher.ok())
            throw ModuleException("An error occured during the decryption process. (wrong password key?)",
                                  "An error occured during the cipher update !");

        // Again we need to call final(), to get the last block (with its padding removed)
        plainText += cipher.final();

        // check if the final() call worked
        if (!cipher.ok())
            throw ModuleException("An error occured during the decryption process. (wrong password key?)",
                                  "An error occured during the finalization of the cipher !");

        QByteArray plain = plainText.toByteArray();
        // The encrypted payload may be wrapped in a qCompress envelope
        // ([4-byte big-endian size][zlib stream]) when compression was enabled
        // before encryption. Its leading byte is not a valid format marker,
        // so unwrap the envelope before handing it over to EncodedData.
        QPointer<EncodedData> result;
        if (plain.size() > 5 && plain.at(0) == '\0' && (uchar)plain.at(4) == 0x78)
            result = new EncodedData(qUncompress(plain), Data::F_UNDEF, false);
        else
            result = new EncodedData(plain, Data::F_UNDEF, false);
        if(result->format() != Data::FILE)
            m_logger->debug("AES128 decryption of " + QCA::arrayToHex(data->toData()->data()) + " is "  + result->toString());

        return result;
    }

}
