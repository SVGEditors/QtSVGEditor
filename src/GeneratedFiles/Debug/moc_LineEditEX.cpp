/****************************************************************************
** Meta object code from reading C++ file 'LineEditEX.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../LineEditEX.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LineEditEX.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CLineEditEX_t {
    QByteArrayData data[4];
    char stringdata0[29];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CLineEditEX_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CLineEditEX_t qt_meta_stringdata_CLineEditEX = {
    {
QT_MOC_LITERAL(0, 0, 11), // "CLineEditEX"
QT_MOC_LITERAL(1, 12, 7), // "DBClick"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 7) // "focusIn"

    },
    "CLineEditEX\0DBClick\0\0focusIn"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CLineEditEX[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,
       3,    0,   25,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CLineEditEX::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CLineEditEX *_t = static_cast<CLineEditEX *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->DBClick(); break;
        case 1: _t->focusIn(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CLineEditEX::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CLineEditEX::DBClick)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CLineEditEX::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CLineEditEX::focusIn)) {
                *result = 1;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CLineEditEX::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_CLineEditEX.data,
      qt_meta_data_CLineEditEX,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CLineEditEX::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CLineEditEX::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CLineEditEX.stringdata0))
        return static_cast<void*>(const_cast< CLineEditEX*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int CLineEditEX::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void CLineEditEX::DBClick()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void CLineEditEX::focusIn()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
