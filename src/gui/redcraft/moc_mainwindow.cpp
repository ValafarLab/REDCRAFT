/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[63];
    char stringdata0[1580];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 16), // "updateStage0Text"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 21), // "updateStage0TextError"
QT_MOC_LITERAL(4, 51, 16), // "updateStage1Text"
QT_MOC_LITERAL(5, 68, 21), // "updateStage1TextError"
QT_MOC_LITERAL(6, 90, 16), // "updateStage2Text"
QT_MOC_LITERAL(7, 107, 21), // "updateStage2TextError"
QT_MOC_LITERAL(8, 129, 14), // "updateRMSDText"
QT_MOC_LITERAL(9, 144, 23), // "on_actionExit_triggered"
QT_MOC_LITERAL(10, 168, 30), // "on_actionOpen_Folder_triggered"
QT_MOC_LITERAL(11, 199, 28), // "on_actionOpen_RMSD_triggered"
QT_MOC_LITERAL(12, 228, 30), // "on_actionProcess_RDC_triggered"
QT_MOC_LITERAL(13, 259, 20), // "on_leRDC_textChanged"
QT_MOC_LITERAL(14, 280, 4), // "arg1"
QT_MOC_LITERAL(15, 285, 20), // "on_btFindRDC_clicked"
QT_MOC_LITERAL(16, 306, 28), // "on_btClearAngleFiles_clicked"
QT_MOC_LITERAL(17, 335, 31), // "on_actionGenerate_PDB_triggered"
QT_MOC_LITERAL(18, 367, 22), // "on_btRunStage1_clicked"
QT_MOC_LITERAL(19, 390, 26), // "on_sbRLNonGly_valueChanged"
QT_MOC_LITERAL(20, 417, 23), // "on_sbRLGly_valueChanged"
QT_MOC_LITERAL(21, 441, 23), // "on_leCutoff_textChanged"
QT_MOC_LITERAL(22, 465, 41), // "on_actionREDCRAFT_Documentati..."
QT_MOC_LITERAL(23, 507, 30), // "on_sbStartResidue_valueChanged"
QT_MOC_LITERAL(24, 538, 29), // "on_sbStopResidue_valueChanged"
QT_MOC_LITERAL(25, 568, 23), // "on_sbMedia_valueChanged"
QT_MOC_LITERAL(26, 592, 28), // "on_leSearchDepth_textChanged"
QT_MOC_LITERAL(27, 621, 24), // "on_btClearOutPDB_clicked"
QT_MOC_LITERAL(28, 646, 22), // "on_btRunStage2_clicked"
QT_MOC_LITERAL(29, 669, 25), // "on_cbRunType_stateChanged"
QT_MOC_LITERAL(30, 695, 31), // "on_lwFileList_itemDoubleClicked"
QT_MOC_LITERAL(31, 727, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(32, 744, 4), // "item"
QT_MOC_LITERAL(33, 749, 25), // "on_btRefreshFiles_clicked"
QT_MOC_LITERAL(34, 775, 41), // "on_actionConvert_old_RDC_to_n..."
QT_MOC_LITERAL(35, 817, 33), // "on_actionAbout_REDCRAFT_trigg..."
QT_MOC_LITERAL(36, 851, 27), // "on_btUseAllResidues_clicked"
QT_MOC_LITERAL(37, 879, 23), // "on_btStopStage1_clicked"
QT_MOC_LITERAL(38, 903, 23), // "on_btStopStage2_clicked"
QT_MOC_LITERAL(39, 927, 34), // "on_cbEnableDecimation_stateCh..."
QT_MOC_LITERAL(40, 962, 34), // "on_cbEnableRefinement_stateCh..."
QT_MOC_LITERAL(41, 997, 33), // "on_leRefinementScript_textCha..."
QT_MOC_LITERAL(42, 1031, 27), // "on_btAddSearchDepth_clicked"
QT_MOC_LITERAL(43, 1059, 34), // "on_twDepthSearch_cellDoubleCl..."
QT_MOC_LITERAL(44, 1094, 3), // "row"
QT_MOC_LITERAL(45, 1098, 6), // "column"
QT_MOC_LITERAL(46, 1105, 26), // "on_pbOpenDirectory_clicked"
QT_MOC_LITERAL(47, 1132, 26), // "on_PDBFinderButton_clicked"
QT_MOC_LITERAL(48, 1159, 18), // "on_runRMSD_clicked"
QT_MOC_LITERAL(49, 1178, 35), // "on_sbClusterSensitivity_textC..."
QT_MOC_LITERAL(50, 1214, 29), // "on_sbClusterCount_textChanged"
QT_MOC_LITERAL(51, 1244, 32), // "on_cbConsolidateRes_stateChanged"
QT_MOC_LITERAL(52, 1277, 28), // "on_sbLJThreshold_textChanged"
QT_MOC_LITERAL(53, 1306, 26), // "on_btExecuteStage0_clicked"
QT_MOC_LITERAL(54, 1333, 26), // "on_btInferSequence_clicked"
QT_MOC_LITERAL(55, 1360, 25), // "on_btRemoveFilter_clicked"
QT_MOC_LITERAL(56, 1386, 22), // "on_btAddFilter_clicked"
QT_MOC_LITERAL(57, 1409, 23), // "on_btStopStage0_clicked"
QT_MOC_LITERAL(58, 1433, 27), // "on_cbRamAngles_stateChanged"
QT_MOC_LITERAL(59, 1461, 33), // "on_cbLoadFromTarball_stateCha..."
QT_MOC_LITERAL(60, 1495, 28), // "on_btLoadFromTarball_clicked"
QT_MOC_LITERAL(61, 1524, 27), // "on_btGenerateREDCAT_clicked"
QT_MOC_LITERAL(62, 1552, 27) // "on_btClearRamAngles_clicked"

    },
    "MainWindow\0updateStage0Text\0\0"
    "updateStage0TextError\0updateStage1Text\0"
    "updateStage1TextError\0updateStage2Text\0"
    "updateStage2TextError\0updateRMSDText\0"
    "on_actionExit_triggered\0"
    "on_actionOpen_Folder_triggered\0"
    "on_actionOpen_RMSD_triggered\0"
    "on_actionProcess_RDC_triggered\0"
    "on_leRDC_textChanged\0arg1\0"
    "on_btFindRDC_clicked\0on_btClearAngleFiles_clicked\0"
    "on_actionGenerate_PDB_triggered\0"
    "on_btRunStage1_clicked\0"
    "on_sbRLNonGly_valueChanged\0"
    "on_sbRLGly_valueChanged\0on_leCutoff_textChanged\0"
    "on_actionREDCRAFT_Documentation_triggered\0"
    "on_sbStartResidue_valueChanged\0"
    "on_sbStopResidue_valueChanged\0"
    "on_sbMedia_valueChanged\0"
    "on_leSearchDepth_textChanged\0"
    "on_btClearOutPDB_clicked\0"
    "on_btRunStage2_clicked\0on_cbRunType_stateChanged\0"
    "on_lwFileList_itemDoubleClicked\0"
    "QListWidgetItem*\0item\0on_btRefreshFiles_clicked\0"
    "on_actionConvert_old_RDC_to_new_triggered\0"
    "on_actionAbout_REDCRAFT_triggered\0"
    "on_btUseAllResidues_clicked\0"
    "on_btStopStage1_clicked\0on_btStopStage2_clicked\0"
    "on_cbEnableDecimation_stateChanged\0"
    "on_cbEnableRefinement_stateChanged\0"
    "on_leRefinementScript_textChanged\0"
    "on_btAddSearchDepth_clicked\0"
    "on_twDepthSearch_cellDoubleClicked\0"
    "row\0column\0on_pbOpenDirectory_clicked\0"
    "on_PDBFinderButton_clicked\0"
    "on_runRMSD_clicked\0"
    "on_sbClusterSensitivity_textChanged\0"
    "on_sbClusterCount_textChanged\0"
    "on_cbConsolidateRes_stateChanged\0"
    "on_sbLJThreshold_textChanged\0"
    "on_btExecuteStage0_clicked\0"
    "on_btInferSequence_clicked\0"
    "on_btRemoveFilter_clicked\0"
    "on_btAddFilter_clicked\0on_btStopStage0_clicked\0"
    "on_cbRamAngles_stateChanged\0"
    "on_cbLoadFromTarball_stateChanged\0"
    "on_btLoadFromTarball_clicked\0"
    "on_btGenerateREDCAT_clicked\0"
    "on_btClearRamAngles_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      56,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  294,    2, 0x08 /* Private */,
       3,    0,  295,    2, 0x08 /* Private */,
       4,    0,  296,    2, 0x08 /* Private */,
       5,    0,  297,    2, 0x08 /* Private */,
       6,    0,  298,    2, 0x08 /* Private */,
       7,    0,  299,    2, 0x08 /* Private */,
       8,    0,  300,    2, 0x08 /* Private */,
       9,    0,  301,    2, 0x08 /* Private */,
      10,    0,  302,    2, 0x08 /* Private */,
      11,    0,  303,    2, 0x08 /* Private */,
      12,    0,  304,    2, 0x08 /* Private */,
      13,    1,  305,    2, 0x08 /* Private */,
      15,    0,  308,    2, 0x08 /* Private */,
      16,    0,  309,    2, 0x08 /* Private */,
      17,    0,  310,    2, 0x08 /* Private */,
      18,    0,  311,    2, 0x08 /* Private */,
      19,    1,  312,    2, 0x08 /* Private */,
      20,    1,  315,    2, 0x08 /* Private */,
      21,    1,  318,    2, 0x08 /* Private */,
      22,    0,  321,    2, 0x08 /* Private */,
      23,    1,  322,    2, 0x08 /* Private */,
      24,    1,  325,    2, 0x08 /* Private */,
      25,    1,  328,    2, 0x08 /* Private */,
      26,    1,  331,    2, 0x08 /* Private */,
      27,    0,  334,    2, 0x08 /* Private */,
      28,    0,  335,    2, 0x08 /* Private */,
      29,    1,  336,    2, 0x08 /* Private */,
      30,    1,  339,    2, 0x08 /* Private */,
      33,    0,  342,    2, 0x08 /* Private */,
      34,    0,  343,    2, 0x08 /* Private */,
      35,    0,  344,    2, 0x08 /* Private */,
      36,    0,  345,    2, 0x08 /* Private */,
      37,    0,  346,    2, 0x08 /* Private */,
      38,    0,  347,    2, 0x08 /* Private */,
      39,    1,  348,    2, 0x08 /* Private */,
      40,    1,  351,    2, 0x08 /* Private */,
      41,    1,  354,    2, 0x08 /* Private */,
      42,    0,  357,    2, 0x08 /* Private */,
      43,    2,  358,    2, 0x08 /* Private */,
      46,    0,  363,    2, 0x08 /* Private */,
      47,    0,  364,    2, 0x08 /* Private */,
      48,    0,  365,    2, 0x08 /* Private */,
      49,    1,  366,    2, 0x08 /* Private */,
      50,    1,  369,    2, 0x08 /* Private */,
      51,    1,  372,    2, 0x08 /* Private */,
      52,    1,  375,    2, 0x08 /* Private */,
      53,    0,  378,    2, 0x08 /* Private */,
      54,    0,  379,    2, 0x08 /* Private */,
      55,    0,  380,    2, 0x08 /* Private */,
      56,    0,  381,    2, 0x08 /* Private */,
      57,    0,  382,    2, 0x08 /* Private */,
      58,    1,  383,    2, 0x08 /* Private */,
      59,    1,  386,    2, 0x08 /* Private */,
      60,    0,  389,    2, 0x08 /* Private */,
      61,    0,  390,    2, 0x08 /* Private */,
      62,    0,  391,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, 0x80000000 | 31,   32,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   44,   45,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateStage0Text(); break;
        case 1: _t->updateStage0TextError(); break;
        case 2: _t->updateStage1Text(); break;
        case 3: _t->updateStage1TextError(); break;
        case 4: _t->updateStage2Text(); break;
        case 5: _t->updateStage2TextError(); break;
        case 6: _t->updateRMSDText(); break;
        case 7: _t->on_actionExit_triggered(); break;
        case 8: _t->on_actionOpen_Folder_triggered(); break;
        case 9: _t->on_actionOpen_RMSD_triggered(); break;
        case 10: _t->on_actionProcess_RDC_triggered(); break;
        case 11: _t->on_leRDC_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: _t->on_btFindRDC_clicked(); break;
        case 13: _t->on_btClearAngleFiles_clicked(); break;
        case 14: _t->on_actionGenerate_PDB_triggered(); break;
        case 15: _t->on_btRunStage1_clicked(); break;
        case 16: _t->on_sbRLNonGly_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->on_sbRLGly_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->on_leCutoff_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 19: _t->on_actionREDCRAFT_Documentation_triggered(); break;
        case 20: _t->on_sbStartResidue_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->on_sbStopResidue_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->on_sbMedia_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->on_leSearchDepth_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 24: _t->on_btClearOutPDB_clicked(); break;
        case 25: _t->on_btRunStage2_clicked(); break;
        case 26: _t->on_cbRunType_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->on_lwFileList_itemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 28: _t->on_btRefreshFiles_clicked(); break;
        case 29: _t->on_actionConvert_old_RDC_to_new_triggered(); break;
        case 30: _t->on_actionAbout_REDCRAFT_triggered(); break;
        case 31: _t->on_btUseAllResidues_clicked(); break;
        case 32: _t->on_btStopStage1_clicked(); break;
        case 33: _t->on_btStopStage2_clicked(); break;
        case 34: _t->on_cbEnableDecimation_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 35: _t->on_cbEnableRefinement_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 36: _t->on_leRefinementScript_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 37: _t->on_btAddSearchDepth_clicked(); break;
        case 38: _t->on_twDepthSearch_cellDoubleClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 39: _t->on_pbOpenDirectory_clicked(); break;
        case 40: _t->on_PDBFinderButton_clicked(); break;
        case 41: _t->on_runRMSD_clicked(); break;
        case 42: _t->on_sbClusterSensitivity_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 43: _t->on_sbClusterCount_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 44: _t->on_cbConsolidateRes_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 45: _t->on_sbLJThreshold_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 46: _t->on_btExecuteStage0_clicked(); break;
        case 47: _t->on_btInferSequence_clicked(); break;
        case 48: _t->on_btRemoveFilter_clicked(); break;
        case 49: _t->on_btAddFilter_clicked(); break;
        case 50: _t->on_btStopStage0_clicked(); break;
        case 51: _t->on_cbRamAngles_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 52: _t->on_cbLoadFromTarball_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 53: _t->on_btLoadFromTarball_clicked(); break;
        case 54: _t->on_btGenerateREDCAT_clicked(); break;
        case 55: _t->on_btClearRamAngles_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 56)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 56;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 56)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 56;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
