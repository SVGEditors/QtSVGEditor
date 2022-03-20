/****************************************************************************
** Meta object code from reading C++ file 'PanePropertyTableView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../PanePropertyTableView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PanePropertyTableView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CTextItem_t {
    QByteArrayData data[7];
    char stringdata0[60];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CTextItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CTextItem_t qt_meta_stringdata_CTextItem = {
    {
QT_MOC_LITERAL(0, 0, 9), // "CTextItem"
QT_MOC_LITERAL(1, 10, 11), // "textChanged"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 4), // "nrow"
QT_MOC_LITERAL(4, 28, 3), // "str"
QT_MOC_LITERAL(5, 32, 17), // "OneditingFinished"
QT_MOC_LITERAL(6, 50, 9) // "OnFocusIn"

    },
    "CTextItem\0textChanged\0\0nrow\0str\0"
    "OneditingFinished\0OnFocusIn"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CTextItem[] = {

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
       1,    2,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   34,    2, 0x0a /* Public */,
       6,    0,   35,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CTextItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CTextItem *_t = static_cast<CTextItem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->textChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->OneditingFinished(); break;
        case 2: _t->OnFocusIn(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CTextItem::*_t)(int , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CTextItem::textChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject CTextItem::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CTextItem.data,
      qt_meta_data_CTextItem,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CTextItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CTextItem::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CTextItem.stringdata0))
        return static_cast<void*>(const_cast< CTextItem*>(this));
    return QWidget::qt_metacast(_clname);
}

int CTextItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void CTextItem::textChanged(int _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_CColorItem_t {
    QByteArrayData data[6];
    char stringdata0[64];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CColorItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CColorItem_t qt_meta_stringdata_CColorItem = {
    {
QT_MOC_LITERAL(0, 0, 10), // "CColorItem"
QT_MOC_LITERAL(1, 11, 13), // "OnButtonClick"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 15), // "OncolorSelected"
QT_MOC_LITERAL(4, 42, 3), // "clr"
QT_MOC_LITERAL(5, 46, 17) // "OneditingFinished"

    },
    "CColorItem\0OnButtonClick\0\0OncolorSelected\0"
    "clr\0OneditingFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CColorItem[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a /* Public */,
       3,    1,   30,    2, 0x0a /* Public */,
       5,    0,   33,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QColor,    4,
    QMetaType::Void,

       0        // eod
};

void CColorItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CColorItem *_t = static_cast<CColorItem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnButtonClick(); break;
        case 1: _t->OncolorSelected((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 2: _t->OneditingFinished(); break;
        default: ;
        }
    }
}

const QMetaObject CColorItem::staticMetaObject = {
    { &CTextItem::staticMetaObject, qt_meta_stringdata_CColorItem.data,
      qt_meta_data_CColorItem,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CColorItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CColorItem::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CColorItem.stringdata0))
        return static_cast<void*>(const_cast< CColorItem*>(this));
    return CTextItem::qt_metacast(_clname);
}

int CColorItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CTextItem::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_CChooseItem_t {
    QByteArrayData data[5];
    char stringdata0[44];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CChooseItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CChooseItem_t qt_meta_stringdata_CChooseItem = {
    {
QT_MOC_LITERAL(0, 0, 11), // "CChooseItem"
QT_MOC_LITERAL(1, 12, 11), // "ButtonClick"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 4), // "nrow"
QT_MOC_LITERAL(4, 30, 13) // "OnButtonClick"

    },
    "CChooseItem\0ButtonClick\0\0nrow\0"
    "OnButtonClick"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CChooseItem[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   27,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void CChooseItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CChooseItem *_t = static_cast<CChooseItem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ButtonClick((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->OnButtonClick(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CChooseItem::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CChooseItem::ButtonClick)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject CChooseItem::staticMetaObject = {
    { &CTextItem::staticMetaObject, qt_meta_stringdata_CChooseItem.data,
      qt_meta_data_CChooseItem,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CChooseItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CChooseItem::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CChooseItem.stringdata0))
        return static_cast<void*>(const_cast< CChooseItem*>(this));
    return CTextItem::qt_metacast(_clname);
}

int CChooseItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CTextItem::qt_metacall(_c, _id, _a);
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
void CChooseItem::ButtonClick(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_CSliderItem_t {
    QByteArrayData data[6];
    char stringdata0[75];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CSliderItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CSliderItem_t qt_meta_stringdata_CSliderItem = {
    {
QT_MOC_LITERAL(0, 0, 11), // "CSliderItem"
QT_MOC_LITERAL(1, 12, 20), // "OnSlidervalueChanged"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 5), // "value"
QT_MOC_LITERAL(4, 40, 17), // "OneditingFinished"
QT_MOC_LITERAL(5, 58, 16) // "OnsliderReleased"

    },
    "CSliderItem\0OnSlidervalueChanged\0\0"
    "value\0OneditingFinished\0OnsliderReleased"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CSliderItem[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x0a /* Public */,
       4,    0,   32,    2, 0x0a /* Public */,
       5,    0,   33,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CSliderItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CSliderItem *_t = static_cast<CSliderItem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnSlidervalueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->OneditingFinished(); break;
        case 2: _t->OnsliderReleased(); break;
        default: ;
        }
    }
}

const QMetaObject CSliderItem::staticMetaObject = {
    { &CTextItem::staticMetaObject, qt_meta_stringdata_CSliderItem.data,
      qt_meta_data_CSliderItem,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CSliderItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CSliderItem::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CSliderItem.stringdata0))
        return static_cast<void*>(const_cast< CSliderItem*>(this));
    return CTextItem::qt_metacast(_clname);
}

int CSliderItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CTextItem::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_CComboxItem_t {
    QByteArrayData data[6];
    char stringdata0[45];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CComboxItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CComboxItem_t qt_meta_stringdata_CComboxItem = {
    {
QT_MOC_LITERAL(0, 0, 11), // "CComboxItem"
QT_MOC_LITERAL(1, 12, 11), // "textChanged"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 4), // "nrow"
QT_MOC_LITERAL(4, 30, 3), // "str"
QT_MOC_LITERAL(5, 34, 10) // "OnEndFocus"

    },
    "CComboxItem\0textChanged\0\0nrow\0str\0"
    "OnEndFocus"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CComboxItem[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   29,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    4,

       0        // eod
};

void CComboxItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CComboxItem *_t = static_cast<CComboxItem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->textChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->OnEndFocus((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CComboxItem::*_t)(int , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CComboxItem::textChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject CComboxItem::staticMetaObject = {
    { &CTextItem::staticMetaObject, qt_meta_stringdata_CComboxItem.data,
      qt_meta_data_CComboxItem,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CComboxItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CComboxItem::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CComboxItem.stringdata0))
        return static_cast<void*>(const_cast< CComboxItem*>(this));
    return CTextItem::qt_metacast(_clname);
}

int CComboxItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CTextItem::qt_metacall(_c, _id, _a);
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
void CComboxItem::textChanged(int _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_CPanePropertyTableView_t {
    QByteArrayData data[9];
    char stringdata0[99];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CPanePropertyTableView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CPanePropertyTableView_t qt_meta_stringdata_CPanePropertyTableView = {
    {
QT_MOC_LITERAL(0, 0, 22), // "CPanePropertyTableView"
QT_MOC_LITERAL(1, 23, 11), // "textChanged"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 7), // "strName"
QT_MOC_LITERAL(4, 44, 8), // "strValue"
QT_MOC_LITERAL(5, 53, 22), // "OnitemSelectionChanged"
QT_MOC_LITERAL(6, 76, 13), // "OntextChanged"
QT_MOC_LITERAL(7, 90, 4), // "nrow"
QT_MOC_LITERAL(8, 95, 3) // "str"

    },
    "CPanePropertyTableView\0textChanged\0\0"
    "strName\0strValue\0OnitemSelectionChanged\0"
    "OntextChanged\0nrow\0str"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CPanePropertyTableView[] = {

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
       1,    2,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   34,    2, 0x0a /* Public */,
       6,    2,   35,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    7,    8,

       0        // eod
};

void CPanePropertyTableView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CPanePropertyTableView *_t = static_cast<CPanePropertyTableView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->textChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->OnitemSelectionChanged(); break;
        case 2: _t->OntextChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CPanePropertyTableView::*_t)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CPanePropertyTableView::textChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject CPanePropertyTableView::staticMetaObject = {
    { &QTableWidget::staticMetaObject, qt_meta_stringdata_CPanePropertyTableView.data,
      qt_meta_data_CPanePropertyTableView,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CPanePropertyTableView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CPanePropertyTableView::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CPanePropertyTableView.stringdata0))
        return static_cast<void*>(const_cast< CPanePropertyTableView*>(this));
    return QTableWidget::qt_metacast(_clname);
}

int CPanePropertyTableView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableWidget::qt_metacall(_c, _id, _a);
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
void CPanePropertyTableView::textChanged(const QString & _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
