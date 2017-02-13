/****************************************************************************
** Meta object code from reading C++ file 'CCapturer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/CCapturer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CCapturer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CCapturer_t {
    QByteArrayData data[9];
    char stringdata0[88];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CCapturer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CCapturer_t qt_meta_stringdata_CCapturer = {
    {
QT_MOC_LITERAL(0, 0, 9), // "CCapturer"
QT_MOC_LITERAL(1, 10, 10), // "imageSaved"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 2), // "id"
QT_MOC_LITERAL(4, 25, 8), // "fileName"
QT_MOC_LITERAL(5, 34, 5), // "error"
QT_MOC_LITERAL(6, 40, 7), // "capture"
QT_MOC_LITERAL(7, 48, 32), // "rho::apiGenerator::CMethodResult"
QT_MOC_LITERAL(8, 81, 6) // "result"

    },
    "CCapturer\0imageSaved\0\0id\0fileName\0"
    "error\0capture\0rho::apiGenerator::CMethodResult\0"
    "result"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CCapturer[] = {

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
       1,    2,   29,    2, 0x08 /* Private */,
       5,    0,   34,    2, 0x08 /* Private */,
       6,    1,   35,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    3,    4,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void CCapturer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CCapturer *_t = static_cast<CCapturer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->imageSaved((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->error(); break;
        case 2: _t->capture((*reinterpret_cast< rho::apiGenerator::CMethodResult(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CCapturer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CCapturer.data,
      qt_meta_data_CCapturer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CCapturer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CCapturer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CCapturer.stringdata0))
        return static_cast<void*>(const_cast< CCapturer*>(this));
    return QObject::qt_metacast(_clname);
}

int CCapturer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
