/****************************************************************************
** Meta object code from reading C++ file 'ToolButtonColorMenu.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ToolButtonColorMenu.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ToolButtonColorMenu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CToolButtonColorMenu_t {
    QByteArrayData data[13];
    char stringdata0[203];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CToolButtonColorMenu_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CToolButtonColorMenu_t qt_meta_stringdata_CToolButtonColorMenu = {
    {
QT_MOC_LITERAL(0, 0, 20), // "CToolButtonColorMenu"
QT_MOC_LITERAL(1, 21, 13), // "ColorSelected"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 3), // "clr"
QT_MOC_LITERAL(4, 40, 20), // "colorSelectedRadient"
QT_MOC_LITERAL(5, 61, 12), // "std::wstring"
QT_MOC_LITERAL(6, 74, 20), // "colorSelectedPattern"
QT_MOC_LITERAL(7, 95, 20), // "colorSelectedPicture"
QT_MOC_LITERAL(8, 116, 13), // "OnColorSelect"
QT_MOC_LITERAL(9, 130, 22), // "OncolorSelectedRadient"
QT_MOC_LITERAL(10, 153, 3), // "str"
QT_MOC_LITERAL(11, 157, 22), // "OncolorSelectedPattern"
QT_MOC_LITERAL(12, 180, 22) // "OncolorSelectedPicture"

    },
    "CToolButtonColorMenu\0ColorSelected\0\0"
    "clr\0colorSelectedRadient\0std::wstring\0"
    "colorSelectedPattern\0colorSelectedPicture\0"
    "OnColorSelect\0OncolorSelectedRadient\0"
    "str\0OncolorSelectedPattern\0"
    "OncolorSelectedPicture"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CToolButtonColorMenu[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    1,   57,    2, 0x06 /* Public */,
       6,    1,   60,    2, 0x06 /* Public */,
       7,    1,   63,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   66,    2, 0x0a /* Public */,
       9,    1,   69,    2, 0x0a /* Public */,
      11,    1,   72,    2, 0x0a /* Public */,
      12,    1,   75,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QColor,    3,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, 0x80000000 | 5,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QColor,    3,
    QMetaType::Void, 0x80000000 | 5,   10,
    QMetaType::Void, 0x80000000 | 5,   10,
    QMetaType::Void, 0x80000000 | 5,   10,

       0        // eod
};

void CToolButtonColorMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CToolButtonColorMenu *_t = static_cast<CToolButtonColorMenu *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ColorSelected((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 1: _t->colorSelectedRadient((*reinterpret_cast< const std::wstring(*)>(_a[1]))); break;
        case 2: _t->colorSelectedPattern((*reinterpret_cast< const std::wstring(*)>(_a[1]))); break;
        case 3: _t->colorSelectedPicture((*reinterpret_cast< const std::wstring(*)>(_a[1]))); break;
        case 4: _t->OnColorSelect((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 5: _t->OncolorSelectedRadient((*reinterpret_cast< const std::wstring(*)>(_a[1]))); break;
        case 6: _t->OncolorSelectedPattern((*reinterpret_cast< const std::wstring(*)>(_a[1]))); break;
        case 7: _t->OncolorSelectedPicture((*reinterpret_cast< const std::wstring(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CToolButtonColorMenu::*_t)(QColor );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CToolButtonColorMenu::ColorSelected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CToolButtonColorMenu::*_t)(const std::wstring & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CToolButtonColorMenu::colorSelectedRadient)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (CToolButtonColorMenu::*_t)(const std::wstring & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CToolButtonColorMenu::colorSelectedPattern)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (CToolButtonColorMenu::*_t)(const std::wstring & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CToolButtonColorMenu::colorSelectedPicture)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject CToolButtonColorMenu::staticMetaObject = {
    { &QToolButton::staticMetaObject, qt_meta_stringdata_CToolButtonColorMenu.data,
      qt_meta_data_CToolButtonColorMenu,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CToolButtonColorMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CToolButtonColorMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CToolButtonColorMenu.stringdata0))
        return static_cast<void*>(const_cast< CToolButtonColorMenu*>(this));
    return QToolButton::qt_metacast(_clname);
}

int CToolButtonColorMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void CToolButtonColorMenu::ColorSelected(QColor _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CToolButtonColorMenu::colorSelectedRadient(const std::wstring & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CToolButtonColorMenu::colorSelectedPattern(const std::wstring & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CToolButtonColorMenu::colorSelectedPicture(const std::wstring & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
