/****************************************************************************
** Meta object code from reading C++ file 'RovCameraWidget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "RovCameraWidget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RovCameraWidget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RovCameraWidget_t {
    QByteArrayData data[12];
    char stringdata0[155];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RovCameraWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RovCameraWidget_t qt_meta_stringdata_RovCameraWidget = {
    {
QT_MOC_LITERAL(0, 0, 15), // "RovCameraWidget"
QT_MOC_LITERAL(1, 16, 12), // "startCapture"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 11), // "stopCapture"
QT_MOC_LITERAL(4, 42, 12), // "cameraStatus"
QT_MOC_LITERAL(5, 55, 15), // "QCamera::Status"
QT_MOC_LITERAL(6, 71, 15), // "takeMosaicPhoto"
QT_MOC_LITERAL(7, 87, 15), // "mosaicPhotoBack"
QT_MOC_LITERAL(8, 103, 13), // "shipPhotoBack"
QT_MOC_LITERAL(9, 117, 9), // "countShip"
QT_MOC_LITERAL(10, 127, 13), // "takeFishPhoto"
QT_MOC_LITERAL(11, 141, 13) // "takeShipPhoto"

    },
    "RovCameraWidget\0startCapture\0\0stopCapture\0"
    "cameraStatus\0QCamera::Status\0"
    "takeMosaicPhoto\0mosaicPhotoBack\0"
    "shipPhotoBack\0countShip\0takeFishPhoto\0"
    "takeShipPhoto"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RovCameraWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x0a /* Public */,
       3,    0,   60,    2, 0x0a /* Public */,
       4,    0,   61,    2, 0x0a /* Public */,
       6,    0,   62,    2, 0x0a /* Public */,
       7,    0,   63,    2, 0x0a /* Public */,
       8,    0,   64,    2, 0x0a /* Public */,
       9,    0,   65,    2, 0x0a /* Public */,
      10,    0,   66,    2, 0x0a /* Public */,
      11,    0,   67,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RovCameraWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RovCameraWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startCapture(); break;
        case 1: _t->stopCapture(); break;
        case 2: { QCamera::Status _r = _t->cameraStatus();
            if (_a[0]) *reinterpret_cast< QCamera::Status*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->takeMosaicPhoto(); break;
        case 4: _t->mosaicPhotoBack(); break;
        case 5: _t->shipPhotoBack(); break;
        case 6: _t->countShip(); break;
        case 7: _t->takeFishPhoto(); break;
        case 8: _t->takeShipPhoto(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject RovCameraWidget::staticMetaObject = { {
    &QStackedWidget::staticMetaObject,
    qt_meta_stringdata_RovCameraWidget.data,
    qt_meta_data_RovCameraWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RovCameraWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RovCameraWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RovCameraWidget.stringdata0))
        return static_cast<void*>(this);
    return QStackedWidget::qt_metacast(_clname);
}

int RovCameraWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QStackedWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
