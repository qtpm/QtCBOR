#pragma once

#include <QByteArray>
#include <QObject>
#include <QVariant>

namespace QCBOR
{
QByteArray pack(const QVariant variant);
QVariant unpack(const QByteArray& data);
}
