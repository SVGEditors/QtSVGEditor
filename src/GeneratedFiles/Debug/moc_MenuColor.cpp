/****************************************************************************
** Meta object code from reading C++ file 'MenuColor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MenuColor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MenuColor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CMenuColor_t {
    QByteArrayData data[14];
    char stringdata0[180];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CMenuColor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CMenuColor_t qt_meta_stringdata_CMenuColor = {
    {
QT_MOC_LITERAL(0, 0, 10), // "CMenuColor"
QT_MOC_LITERAL(1, 11, 13), // "colorSelected"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 20), // "colorSelectedRadient"
QT_MOC_LITERAL(4, 47, 12), // "std::wstring"
QT_MOC_LITERAL(5, 60, 20), // "colorSelectedPattern"
QT_MOC_LITERAL(6, 81, 20), // "colorSelectedPicture"
QT_MOC_LITERAL(7, 102, 15), // "OnColorSelected"
QT_MOC_LITERAL(8, 118, 12), // "OnTransprent"
QT_MOC_LITERAL(9, 131, 8), // "QAction*"
QT_MOC_LITERAL(10, 140, 7), // "pAction"
QT_MOC_LITERAL(11, 148, 11), // "OnMoreColor"
QT_MOC_LITERAL(12, 160, 9), // "OnRadient"
QT_MOC_LITERAL(13, 170, 9) // "OnPattern"

    },
    "CMenuColor\0colorSelected\0\0"
    "colorSelectedRadient\0std::wstring\0"
    "colorSelectedPattern\0colorSelectedPicture\0"
    "OnColorSelected\0OnTransprent\0QAction*\0"
    "pAction\0OnMoreColor\0OnRadient\0OnPattern"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CMenuColor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       3,    1,   62,    2, 0x06 /* Public */,
       5,    1,   65,    2, 0x06 /* Public */,
       6,    1,   68,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   71,    2, 0x0a /* Public */,
       8,    1,   72,    2, 0x0a /* Public */,
      11,    0,   75,    2, 0x0a /* Public */,
      12,    0,   76,    2, 0x0a /* Public */,
      13,    0,   77,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QColor,    2,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void, 0x80000000 | 4,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CMenuColor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CMenuColor *_t = static_cast<CMenuColor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->colorSelected((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 1: _t->colorSelectedRadient((*reinterpret_cast< const std::wstring(*)>(_a[1]))); break;
        case 2: _t->colorSelectedPattern((*reinterpret_cast< const std::wstring(*)>(_a[1]))); break;
        case 3: _t->colorSelectedPicture((*reinterpret_cast< const std::wstring(*)>(_a[1]))); break;
        case 4: _t->OnColorSelected(); break;
        case 5: _t->OnTransprent((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 6: _t->OnMoreColor(); break;
        case 7: _t->OnRadient(); break;
        case 8: _t->OnPattern(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CMenuColor::*_t)(QColor );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CMenuColor::colorSelected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CMenuColor::*_t)(const std::wstring & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CMenuColor::colorSelectedRadient)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (CMenuColor::*_t)(const std::wstring & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CMenuColor::colorSelectedPattern)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (CMenuColor::*_t)(const std::wstring & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CMenuColor::colorSelectedPicture)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject CMenuColor::staticMetaObject = {
    { &QMenu::staticMetaObject, qt_meta_stringdata_CMenuColor.data,
      qt_meta_data_CMenuColor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CMenuColor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CMenuColor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CMenuColor.stringdata0))
        return static_cast<void*>(const_cast< CMenuColor*>(this));
    return QMenu::qt_metacast(_clname);
}

int CMenuColor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMenu::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void CMenuColor::colorSelected(QColor _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CMenuColor::colorSelectedRadient(const std::wstring & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CMenuColor::colorSelectedPattern(const std::wstring & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CMenuColor::colorSelectedPicture(const std::wstring & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
