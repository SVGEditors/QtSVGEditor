/****************************************************************************
** Meta object code from reading C++ file 'DlgGradient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DlgGradient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DlgGradient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CDlgGradient_t {
    QByteArrayData data[17];
    char stringdata0[177];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CDlgGradient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CDlgGradient_t qt_meta_stringdata_CDlgGradient = {
    {
QT_MOC_LITERAL(0, 0, 12), // "CDlgGradient"
QT_MOC_LITERAL(1, 13, 11), // "OnColorMenu"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 7), // "OnColor"
QT_MOC_LITERAL(4, 34, 3), // "clr"
QT_MOC_LITERAL(5, 38, 20), // "OnSlidervalueChanged"
QT_MOC_LITERAL(6, 59, 5), // "value"
QT_MOC_LITERAL(7, 65, 28), // "OncurrentHistoryIndexChanged"
QT_MOC_LITERAL(8, 94, 5), // "index"
QT_MOC_LITERAL(9, 100, 12), // "OnHeadToTail"
QT_MOC_LITERAL(10, 113, 9), // "OnButton1"
QT_MOC_LITERAL(11, 123, 9), // "OnButton2"
QT_MOC_LITERAL(12, 133, 9), // "OnButton3"
QT_MOC_LITERAL(13, 143, 9), // "OnButton4"
QT_MOC_LITERAL(14, 153, 9), // "OnButton5"
QT_MOC_LITERAL(15, 163, 4), // "OnOk"
QT_MOC_LITERAL(16, 168, 8) // "OnCancel"

    },
    "CDlgGradient\0OnColorMenu\0\0OnColor\0clr\0"
    "OnSlidervalueChanged\0value\0"
    "OncurrentHistoryIndexChanged\0index\0"
    "OnHeadToTail\0OnButton1\0OnButton2\0"
    "OnButton3\0OnButton4\0OnButton5\0OnOk\0"
    "OnCancel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CDlgGradient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x0a /* Public */,
       3,    1,   75,    2, 0x0a /* Public */,
       5,    1,   78,    2, 0x0a /* Public */,
       7,    1,   81,    2, 0x0a /* Public */,
       9,    0,   84,    2, 0x0a /* Public */,
      10,    0,   85,    2, 0x0a /* Public */,
      11,    0,   86,    2, 0x0a /* Public */,
      12,    0,   87,    2, 0x0a /* Public */,
      13,    0,   88,    2, 0x0a /* Public */,
      14,    0,   89,    2, 0x0a /* Public */,
      15,    0,   90,    2, 0x0a /* Public */,
      16,    0,   91,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QColor,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CDlgGradient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CDlgGradient *_t = static_cast<CDlgGradient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnColorMenu(); break;
        case 1: _t->OnColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 2: _t->OnSlidervalueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->OncurrentHistoryIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->OnHeadToTail(); break;
        case 5: _t->OnButton1(); break;
        case 6: _t->OnButton2(); break;
        case 7: _t->OnButton3(); break;
        case 8: _t->OnButton4(); break;
        case 9: _t->OnButton5(); break;
        case 10: _t->OnOk(); break;
        case 11: _t->OnCancel(); break;
        default: ;
        }
    }
}

const QMetaObject CDlgGradient::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CDlgGradient.data,
      qt_meta_data_CDlgGradient,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CDlgGradient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CDlgGradient::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CDlgGradient.stringdata0))
        return static_cast<void*>(const_cast< CDlgGradient*>(this));
    return QDialog::qt_metacast(_clname);
}

int CDlgGradient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
