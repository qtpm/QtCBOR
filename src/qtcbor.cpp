#include "qtcbor.h"
#include "./private/cbor.h"
#include <iostream>
#include <QStack>
#include <QList>
#include <QMap>

/*!
 * \namespace QCBOR
 *
 * \brief QCBOR namespaces contains converter functions related to \l{http://cbor.io/}{CBOR(RFC 7049 Concise Binary Object Representation)} format.
 *
 * This module supports the structured data that contains the following types:
 *
 * \list
 * \li int
 * \li double
 * \li bool
 * \li null(empty QVariant)
 * \li QString
 * \li QVariantList(QList<QVariant>)
 * \li QStringList
 * \li QMap<QString, QVariant>
 * \endlist
 *
 * \inmodule QCBOR
 */
namespace QCBOR {

}

void encode(cbor::encoder& encoder, const QVariant &variant) {
    switch (variant.type()) {
    case QVariant::Int:
        encoder.write_int(variant.toInt());
        break;
    case QVariant::Double:
        encoder.write_double(variant.toDouble());
        break;
    case QVariant::Bool:
        encoder.write_bool(variant.toBool());
        break;
    case QVariant::ByteArray:
        {
            auto buffer = variant.toByteArray();
            encoder.write_bytes((const unsigned char *)buffer.constData(), buffer.size());
        }
        break;
    case QVariant::String:
        {
            auto string = variant.toString().toUtf8();
            encoder.write_string(string.constData(), string.size());
        }
        break;
    case QVariant::List:
        {
            auto list = variant.toList();
            encoder.write_array(list.size());
            for (auto &el : list) {
                encode(encoder, el);
            }
        }
        break;
    case QVariant::StringList:
        {
            auto list = variant.toStringList();
            encoder.write_array(list.size());
            for (auto &el : list) {
                auto string = el.toUtf8();
                encoder.write_string(string.constData(), string.size());
            }
        }
        break;
    case QVariant::Map:
        {
            auto map = variant.toMap();
            encoder.write_map(map.size());
            for (auto i = map.begin(); i != map.end(); i++) {
                auto string = i.key().toUtf8();
                encoder.write_string(string.constData(), string.size());
                encode(encoder, i.value());
            }
        }
        break;
    default:
        if (variant.isNull()) {
            encoder.write_null();
        } else {
            std::cout << "no support type:" << variant.typeName() << std::endl;
        }
    }
}

class Listener : public cbor::listener {
    QStack<QMap<QString, QVariant>*> mapStack;
    QStack<QVariantList*> listStack;
    QStack<bool>  isMapStack;
    QStack<int>   remainedItemsStack;
    QStack<QString> keyStack;

    QString       key;
    int           remainedItems;

    QVariant*     result;
    bool          waitingKey;
    const char*   error;

public:
    explicit Listener() : result(nullptr), waitingKey(false), error(nullptr) {}
    ~Listener() {
        if (this->result != nullptr) {
            delete this->result;
        }
    }

    bool hasError() {
        return this->error != nullptr;
    }

    QVariant variant() {
        return *(this->result);
    }

    void pushToParent(QVariant value) {
        if (this->isMapStack.last()) {
            auto currentMap = this->mapStack.last();
            currentMap->insert(this->key, value);
            this->waitingKey = true;
            for (;;) {
                this->remainedItems--;
                if (this->remainedItems == 0) {
                    auto parent = this->mapStack.pop();
                    this->isMapStack.pop();
                    this->remainedItems = this->remainedItemsStack.pop();
                    if (this->isMapStack.empty()) {
                        this->result = new QVariant(*parent);
                        break;
                    } else {
                        if (this->isMapStack.last()) {
                            this->waitingKey = false;
                            this->key = this->keyStack.pop();
                        }
                        this->pushToParent(*parent);
                    }
                    delete parent;
                } else {
                    break;
                }
            }
        } else {
            auto list = this->listStack.last();
            list->append(value);
            for (;;) {
                this->remainedItems--;
                if (this->remainedItems == 0) {
                    auto parent = this->listStack.pop();
                    this->isMapStack.pop();
                    this->remainedItems = this->remainedItemsStack.pop();
                    if (this->isMapStack.empty()) {
                        this->result = new QVariant(*parent);
                        break;
                    } else {
                        if (this->isMapStack.last()) {
                            this->waitingKey = false;
                            this->key = this->keyStack.pop();
                        }
                        this->pushToParent(*parent);
                    }
                    delete parent;
                } else {
                    break;
                }
            }
        }
    }

    bool empty() {
        return (this->isMapStack.empty());
    }

    virtual void on_integer(int value) {
        if (this->empty()) {
            this->result = new QVariant(value);
        } else {
            this->pushToParent(value);
        }
    }

    virtual void on_bool(bool value) {
        if (this->empty()) {
            this->result = new QVariant(value);
        } else {
            this->pushToParent(value);
        }
    }
    virtual void on_null() {
        if (this->empty()) {
            this->result = new QVariant();
        } else {
            this->pushToParent(QVariant());
        }
    }

    virtual void on_undefined() {
        if (this->empty()) {
            this->result = new QVariant();
        } else {
            this->pushToParent(QVariant());
        }
    }

    virtual void on_half(float value) {
        if (this->empty()) {
            this->result = new QVariant(value);
        } else {
            this->pushToParent(value);
        }
    }

    virtual void on_float(float value) {
        if (this->empty()) {
            this->result = new QVariant(value);
        } else {
            this->pushToParent(value);
        }
    }

    virtual void on_double(double value) {
        if (this->empty()) {
            this->result = new QVariant(value);
        } else {
            this->pushToParent(value);
        }
    }

    virtual void on_bytes(unsigned char *data, int size) {
        if (this->empty()) {
            this->result = new QVariant(QByteArray((const char*)data, size));
        } else {
            this->pushToParent(QByteArray((const char*)data, size));
        }
    }

    virtual void on_string(std::string &str) {
        if (this->waitingKey) {
            this->key = QString::fromUtf8(str.c_str(), str.size());
            this->waitingKey = false;
        } else if (this->empty()) {
            this->result = new QVariant(QString::fromUtf8(str.c_str(), str.size()));
        } else {
            this->pushToParent(QString::fromUtf8(str.c_str(), str.size()));
        }
    }

    virtual void on_array(int size) {
        this->listStack.append(new QVariantList());
        this->isMapStack.append(false);
        if (!this->isMapStack.empty() && this->isMapStack.last()) {
            this->keyStack.append(this->key);
        }
        this->remainedItemsStack.append(this->remainedItems);
        this->remainedItems = size;
    }

    virtual void on_map(int size)  {
        this->mapStack.append(new QMap<QString, QVariant>());
        if (!this->isMapStack.empty() && this->isMapStack.last()) {
            this->keyStack.append(this->key);
        }
        this->isMapStack.append(true);
        this->remainedItemsStack.append(this->remainedItems);
        this->remainedItems = size;
        this->waitingKey = true;
    }

    virtual void on_tag(unsigned int tag)  {

    }

    virtual void on_special(unsigned int code)  {

    }

    virtual void on_error(const char *error)  {

    }
};

/*!
 * \fn QByteArray QCBOR::pack(const QVariant variant)
 *
 * \brief pack function converts \a variant (QVariant) into QByteArray that has CBOR formated data.
 */
QByteArray QCBOR::pack(const QVariant variant)
{
    cbor::output_dynamic output;
    cbor::encoder encoder(output);

    encode(encoder, variant);

    return QByteArray((const char *)output.data(), output.size());
}


/*!
 * \fn QVariant QCBOR::unpack(const QByteArray &data)
 *
 * \brief unpack function converts CBOR formated \a data into QVariant
 */
QVariant QCBOR::unpack(const QByteArray &data) {
    Listener listener;
    cbor::input input((void*)data.data(), data.size());
    cbor::decoder decoder(input, listener);
    decoder.run();
    return listener.variant();
}
