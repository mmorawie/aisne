/****************************************************************************
** Meta object code from reading C++ file 'aisne.cpp'
**
** Created: ?r 26. maj 22:22:13 2021
**      by: The Qt Meta Object Compiler version 58 (Qt 4.0.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'aisne.cpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 58
#error "This file was generated using the moc from 4.0.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_Form1[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Form1[] = {
    "Form1\0\0run()\0"
};

const QMetaObject Form1::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Form1,
      qt_meta_data_Form1, 0 }
};

const QMetaObject *Form1::metaObject() const
{
    return &staticMetaObject;
}

void *Form1::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Form1))
	return static_cast<void*>(const_cast<Form1*>(this));
    return QWidget::qt_metacast(_clname);
}

int Form1::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: run(); break;
        }
        _id -= 1;
    }
    return _id;
}
