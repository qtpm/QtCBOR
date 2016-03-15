QtCBOR
=================================

`CBOR <http://cbor.io/>`_ (RFC 7049 Concise Binary Object Representation) decoder/eoncoder functions for Qt's QVariant.

You can encode/decode the following types:

* ``int``
* ``double``
* ``bool``
* ``null(empty QVariant)``
* ``QString``
* ``QVariantList(QList<QVariant>)``
* ``QStringList``
* ``QMap<QString, QVariant>``

Install
--------------

This is a `qtpm <https://github.com/qtpm/qtpm>`_ package.

.. code-block:: bash

   $ qtpm get github.com/qtpm/QtCBOR

Headers and a library file are installed under ``vendor/include`` and ``vendor/lib`` directory.

Example
--------------

.. code-block:: cpp

   #include <iostream>
   #include <QtCBOR/qtcbor.h>
   #include <QStringList>


   int main() {
       QStringList array;
       array.append("string1");
       array.append("string2");
       array.append("string3");

       // You can store this byte array in file or send via socket
       QByteArray binary = QCBOR::pack(array);

       // Decoding is easy too
       QVariant decoded = QCBOR::unpack(binary);
       auto array2 = decoded.toStringList();
   }

Reference
--------------

* ``QByteArray QCBOR::pack(const QVariant variant)``

  ``pack`` function converts ``variant`` (``QVariant``) into ``QByteArray`` that has CBOR formated data.

* ``QVariant QCBOR::unpack(const QByteArray &data)``

  ``unpack`` function converts CBOR formated data into ``QVariant``.

Develop
--------------

``qtpm`` command helps your development.

.. code-block:: bash

   $ git clone git@github.com:qtpm/QtCBOR.git
   $ cd QtCBOR

   # Run the following command once if this package has required packages
   # -u updates modules they are already downloaded
   $ qtpm get [-u]

   # Build and run test
   $ qtpm test

   # Try install to ``vendor`` folder
   $ qtpm build


Folder Structure and Naming Convention
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: none

   + src/           // Header files under this folder will be published.
   |  |             // Don't add `main.cpp`. qtpm assumes this package is an application, not library.
   |  + private/    // Files under this folder will hide from outer modules.
   + test/          // Each _test.cpp file is built into executable and launched by qtpm test.
   |                // Use `qtpm add test`` to add test case.
   |                // Other .cpp files are used as utility code. They are linked to test modules.
   + doc/           // (not working yet) QDoc setting file and extra image files and so on.
   + html/          // (not working yet) HTML output of QDoc.
   + vendor/        // External libraries (source and binary)
   + build/         // Work folder.
   + CMakeExtra.txt // This file is included from created CMakeLists.txt.
                    // You can add any extra build options here.

Author
--------------

* shibukawa.yoshiki

License
--------------

The MIT License (MIT)

History
--------------

* 3/8/2016: First version 
