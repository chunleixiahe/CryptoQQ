#ifndef QIODEVICESOURCE_H
#define QIODEVICESOURCE_H

#include <QIODevice>
#include <QByteArray>
#include <QBuffer>

#include <cryptopp/filters.h>

namespace CryptoQQ {

class CRYPTO_QQ_EXPORT QIODeviceStore : public CryptoPP::Store, private CryptoPP::FilterPutSpaceHelper, public CryptoPP::NotCopyable
{
public:
#if CRYPTOPP_VERSION >= 600
	using byte = CryptoPP::byte;
#endif

	static const char * const DeviceParameter;

	class Err : public CryptoPP::Exception
	{
	public:
		Err(const QString &s);
	};

	QIODeviceStore();
	QIODeviceStore(QIODevice *device);

	CryptoPP::lword MaxRetrievable() const override;
	size_t TransferTo2(CryptoPP::BufferedTransformation &target, CryptoPP::lword &transferBytes, const std::string &channel, bool blocking) override;
	size_t CopyRangeTo2(CryptoPP::BufferedTransformation &target, CryptoPP::lword &begin, CryptoPP::lword end, const std::string &channel, bool blocking) const override;
	CryptoPP::lword Skip(CryptoPP::lword skipMax=ULONG_MAX) override;

	QIODevice *device() const;

protected:
	void StoreInitialize(const CryptoPP::NameValuePairs &parameters) override;

private:
	QIODevice *_device;
	byte *_space;
	qint64 _len;
	bool _waiting;
};

class CRYPTO_QQ_EXPORT QIODeviceSource : public CryptoPP::SourceTemplate<QIODeviceStore>
{
public:
	QIODeviceSource(CryptoPP::BufferedTransformation *attachment = nullptr);
	QIODeviceSource(QIODevice *device, bool pumpAll, CryptoPP::BufferedTransformation *attachment = nullptr);

	QIODevice *device() const;
};

class CRYPTO_QQ_EXPORT QByteArraySource : public QIODeviceSource
{
public:
	QByteArraySource(CryptoPP::BufferedTransformation *attachment = nullptr);
	QByteArraySource(const QByteArray &source, bool pumpAll, CryptoPP::BufferedTransformation *attachment = nullptr);

	const QByteArray &buffer() const;

private:
	QBuffer _buffer;
};

}

#endif // QIODEVICESOURCE_H
