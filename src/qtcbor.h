#ifndef QTCBOR_H
#define QTCBOR_H

#include "qtcbor_global.h"
#include <QObject>
#include <QVariant>
#include <QByteArray>

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
 */
namespace QCBOR {
    /*!
     * \fn QByteArray pack(const QVariant variant)
     *
     * \brief pack function converts QVariant into QByteArray that has CBOR formated data.
     */
    QTCBORSHARED_EXPORT QByteArray pack(const QVariant variant);
    /*!
     * \fn QVariant unpack(const QByteArray &data)
     *
     * \brief unpack function converts CBOR formated data into QVariant
     */
    QTCBORSHARED_EXPORT QVariant unpack(const QByteArray &data);
} // QCBOR

#endif // QTCBOR_H

