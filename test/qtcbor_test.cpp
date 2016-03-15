#include <QString>
#include <QtTest>
#include "qtcbor.h"

class TestQtCBOR : public QObject {
    Q_OBJECT

public:
    TestQtCBOR() {}

private Q_SLOTS:
    void testPackUnpackInt() {
        auto binary = QCBOR::pack(10);
        auto decoded = QCBOR::unpack(binary);
        QCOMPARE(decoded.toInt(), 10);
    }

    void testPackUnpackFloat() {
        auto binary = QCBOR::pack(10.5);
        auto decoded = QCBOR::unpack(binary);
        QCOMPARE(decoded.toDouble(), 10.5);
    }

    void testPackUnpackBool() {
        auto binary = QCBOR::pack(true);
        auto decoded = QCBOR::unpack(binary);
        QCOMPARE(decoded.toBool(), true);
    }

    void testPackUnpackNull() {
        auto var = QVariant();
        QCOMPARE(var.isNull(), true);
        auto binary = QCBOR::pack(var);
        auto decoded = QCBOR::unpack(binary);
        QCOMPARE(decoded.isNull(), true);
    }

    void testPackUnpackBytes() {
        auto buffer = QByteArray("bytes");
        auto binary = QCBOR::pack(buffer);
        auto decoded = QCBOR::unpack(binary);
        QCOMPARE(decoded.toByteArray(), buffer);
    }

    void testPackUnpackString() {
        auto binary = QCBOR::pack("string");
        auto decoded = QCBOR::unpack(binary);
        QCOMPARE(decoded.toString(), QString("string"));
    }

    void testPackUnpackArray() {
        QVariantList array;
        array.append(10);
        array.append(true);
        array.append(10.5);
        array.append("string");

        auto binary = QCBOR::pack(array);
        auto decoded = QCBOR::unpack(binary);
        QCOMPARE(decoded.toList(), array);
    }

    void testPackUnpackNestedArray() {
        QVariantList array{10, true, 10.5, QVariantList{"string"}};

        auto binary = QCBOR::pack(array);
        auto decoded = QCBOR::unpack(binary);
        QCOMPARE(decoded.toList(), array);
    }

    void testPackUnpackStringList() {
        QStringList array{"string1", "string2", "string3"};

        auto binary = QCBOR::pack(array);
        auto decoded = QCBOR::unpack(binary);
        QCOMPARE(decoded.toStringList(), array);
    }

    void testPackUnpackMap() {
        QMap<QString, QVariant> map{
            {"integer", 10},
            {"bool", true},
            {"double", 10.5},
            {"string", "string"}};

        auto binary = QCBOR::pack(map);
        auto decoded = QCBOR::unpack(binary);
        QCOMPARE(decoded.toMap(), map);
    }

    void testPackUnpackNestedMap() {
        QMap<QString, QVariant> map{
            {"integer", 10},
            {"bool", true},
            {"double", 10.5},
            {"map", QMap<QString, QVariant>{
                {"string", "string"}
            }}
        };

        auto binary = QCBOR::pack(map);
        auto decoded = QCBOR::unpack(binary);
        QCOMPARE(decoded.toMap(), map);
    }

    void testPackUnpackNestedMapAndList() {
        QMap<QString, QVariant> map{
            {"path", "/test"},
            {"method", "method"},
            {"params", QVariantList{"abc.png", 0777}}
        };

        auto binary = QCBOR::pack(map);
        auto decoded = QCBOR::unpack(binary);
        QCOMPARE(decoded.toMap(), map);
    }
};

QTEST_APPLESS_MAIN(TestQtCBOR)

#include "qtcbor_test.moc"
