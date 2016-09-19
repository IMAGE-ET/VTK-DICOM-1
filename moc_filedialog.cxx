/****************************************************************************
** Meta object code from reading C++ file 'filedialog.h'
**
** Created: Fri Aug 5 15:28:52 2016
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "filedialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'filedialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FileDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      36,   11,   11,   11, 0x08,
      67,   62,   11,   11, 0x08,
     105,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FileDialog[] = {
    "FileDialog\0\0on_pushButton_clicked()\0"
    "on_pushButton_2_clicked()\0arg1\0"
    "on_doubleSpinBox_valueChanged(double)\0"
    "on_pushButton_3_clicked()\0"
};

void FileDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FileDialog *_t = static_cast<FileDialog *>(_o);
        switch (_id) {
        case 0: _t->on_pushButton_clicked(); break;
        case 1: _t->on_pushButton_2_clicked(); break;
        case 2: _t->on_doubleSpinBox_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->on_pushButton_3_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FileDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FileDialog::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_FileDialog,
      qt_meta_data_FileDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FileDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FileDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FileDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FileDialog))
        return static_cast<void*>(const_cast< FileDialog*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int FileDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
