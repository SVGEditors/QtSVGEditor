/****************************************************************************
** Meta object code from reading C++ file 'DlgRotateSlider.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DlgRotateSlider.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DlgRotateSlider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CDlgRotateSlider_t {
    QByteArrayData data[7];
    char stringdata0[84];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CDlgRotateSlider_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CDlgRotateSlider_t qt_meta_stringdata_CDlgRotateSlider = {
    {
QT_MOC_LITERAL(0, 0, 16), // "CDlgRotateSlider"
QT_MOC_LITERAL(1, 17, 18), // "RotatevalueChanged"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 6), // "nValue"
QT_MOC_LITERAL(4, 44, 14), // "OnvalueChanged"
QT_MOC_LITERAL(5, 59, 5), // "value"
QT_MOC_LITERAL(6, 65, 18) // "OnSpinvalueChanged"

    },
    "CDlgRotateSlider\0RotatevalueChanged\0"
    "\0nValue\0OnvalueChanged\0value\0"
    "OnSpinvalueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CDlgRotateSlider[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   32,    2, 0x0a /* Public */,
       6,    1,   35,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,

       0        // eod
};

void CDlgRotateSlider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CDlgRotateSlider *_t = static_cast<CDlgRotateSlider *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->RotatevalueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->OnvalueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->OnSpinvalueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CDlgRotateSlider::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CDlgRotateSlider::RotatevalueChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject CDlgRotateSlider::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CDlgRotateSlider.data,
      qt_meta_data_CDlgRotateSlider,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CDlgRotateSlider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CDlgRotateSlider::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CDlgRotateSlider.stringdata0))
        return static_cast<void*>(const_cast< CDlgRotateSlider*>(this));
    return QDialog::qt_metacast(_clname);
}

int CDlgRotateSlider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void CDlgRotateSlider::RotatevalueChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
