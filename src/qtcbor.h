#ifndef QTCBOR_H
#define QTCBOR_H

#include "qtcbor_global.h"
#include <QObject>
#include <QVariant>
#include <QByteArray>

namespace QCBOR {
    QTCBORSHARED_EXPORT QByteArray pack(const QVariant variant);
    QTCBORSHARED_EXPORT QVariant unpack(const QByteArray &data);
} // QCBOR

#endif // QTCBOR_H

