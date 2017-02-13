/****************************************************************************
** Meta object code from reading C++ file 'CCameraData.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/CCameraData.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CCameraData.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CCameraData_t {
    QByteArrayData data[6];
    char stringdata0[66];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CCameraData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CCameraData_t qt_meta_stringdata_CCameraData = {
    {
QT_MOC_LITERAL(0, 0, 11), // "CCameraData"
QT_MOC_LITERAL(1, 12, 7), // "capture"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 32), // "rho::apiGenerator::CMethodResult"
QT_MOC_LITERAL(4, 54, 7), // "oResult"
QT_MOC_LITERAL(5, 62, 3) // "run"

    },
    "CCameraData\0capture\0\0"
    "rho::apiGenerator::CMethodResult\0"
    "oResult\0run"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CCameraData[] = {

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
       5,    0,   27,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void CCameraData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CCameraData *_t = static_cast<CCameraData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->capture((*reinterpret_cast< rho::apiGenerator::CMethodResult(*)>(_a[1]))); break;
        case 1: _t->run(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CCameraData::*_t)(rho::apiGenerator::CMethodResult );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CCameraData::capture)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CCameraData::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CCameraData.data,
      qt_meta_data_CCameraData,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CCameraData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CCameraData::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CCameraData.stringdata0))
        return static_cast<void*>(const_cast< CCameraData*>(this));
    return QThread::qt_metacast(_clname);
}

int CCameraData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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
void CCameraData::capture(rho::apiGenerator::CMethodResult _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
