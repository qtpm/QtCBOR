#ifndef QTCBOR_GLOBAL_H
#define QTCBOR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTCBOR_LIBRARY)
#  define QTCBORSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTCBORSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTCBOR_GLOBAL_H

