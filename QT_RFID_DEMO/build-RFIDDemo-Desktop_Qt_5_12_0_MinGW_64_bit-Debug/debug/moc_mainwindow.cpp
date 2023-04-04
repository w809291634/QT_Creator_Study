/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../RFIDDemo-src/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[86];
    char stringdata0[1241];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 7), // "Get_Com"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 16), // "OpenUart_Clicked"
QT_MOC_LITERAL(4, 37, 8), // "Get_Time"
QT_MOC_LITERAL(5, 46, 8), // "ReadUart"
QT_MOC_LITERAL(6, 55, 9), // "Show_Data"
QT_MOC_LITERAL(7, 65, 13), // "Set_Show_Mode"
QT_MOC_LITERAL(8, 79, 8), // "Stop_Rec"
QT_MOC_LITERAL(9, 88, 7), // "Del_Num"
QT_MOC_LITERAL(10, 96, 9), // "Change_ID"
QT_MOC_LITERAL(11, 106, 9), // "Change_IC"
QT_MOC_LITERAL(12, 116, 10), // "Change_ETC"
QT_MOC_LITERAL(13, 127, 10), // "Change_24G"
QT_MOC_LITERAL(14, 138, 8), // "About_up"
QT_MOC_LITERAL(15, 147, 7), // "Message"
QT_MOC_LITERAL(16, 155, 9), // "Time_Save"
QT_MOC_LITERAL(17, 165, 14), // "ID_Change_Mode"
QT_MOC_LITERAL(18, 180, 12), // "ID_Open_Door"
QT_MOC_LITERAL(19, 193, 9), // "ID_EM4100"
QT_MOC_LITERAL(20, 203, 12), // "ID_Read_Card"
QT_MOC_LITERAL(21, 216, 13), // "ID_Write_Card"
QT_MOC_LITERAL(22, 230, 11), // "IC_Del_Card"
QT_MOC_LITERAL(23, 242, 12), // "IC_Send_Look"
QT_MOC_LITERAL(24, 255, 13), // "IC_Begin_Look"
QT_MOC_LITERAL(25, 269, 12), // "IC_Data_Look"
QT_MOC_LITERAL(26, 282, 14), // "IC_Pass_Change"
QT_MOC_LITERAL(27, 297, 15), // "IC_Write_Region"
QT_MOC_LITERAL(28, 313, 14), // "IC_Read_Region"
QT_MOC_LITERAL(29, 328, 12), // "IC_Read_Mess"
QT_MOC_LITERAL(30, 341, 11), // "Mode_Change"
QT_MOC_LITERAL(31, 353, 17), // "IC_Read_Mess_flag"
QT_MOC_LITERAL(32, 371, 8), // "Money_Up"
QT_MOC_LITERAL(33, 380, 15), // "Down_Money_Save"
QT_MOC_LITERAL(34, 396, 15), // "IC_Ticket_Reset"
QT_MOC_LITERAL(35, 412, 18), // "IC_App_Ticket_Save"
QT_MOC_LITERAL(36, 431, 15), // "IC_App_Up_Money"
QT_MOC_LITERAL(37, 447, 19), // "IC_App_Send_Balance"
QT_MOC_LITERAL(38, 467, 17), // "IC_App_Down_Money"
QT_MOC_LITERAL(39, 485, 14), // "IC_Change_Pass"
QT_MOC_LITERAL(40, 500, 14), // "IC_Switch_Mode"
QT_MOC_LITERAL(41, 515, 17), // "ETC_Momery_Change"
QT_MOC_LITERAL(42, 533, 14), // "ETC_Look_Begin"
QT_MOC_LITERAL(43, 548, 17), // "ETC_Data_Analysis"
QT_MOC_LITERAL(44, 566, 13), // "Del_Look_Data"
QT_MOC_LITERAL(45, 580, 15), // "ETC_Change_Mode"
QT_MOC_LITERAL(46, 596, 13), // "ETC_Read_Data"
QT_MOC_LITERAL(47, 610, 14), // "ETC_Get_Region"
QT_MOC_LITERAL(48, 625, 14), // "ETC_Set_Region"
QT_MOC_LITERAL(49, 640, 10), // "ETC_Get_RF"
QT_MOC_LITERAL(50, 651, 10), // "ETC_Set_RF"
QT_MOC_LITERAL(51, 662, 12), // "ETC_Get_TxLv"
QT_MOC_LITERAL(52, 675, 12), // "ETC_Set_TxLv"
QT_MOC_LITERAL(53, 688, 12), // "ETC_Get_RSSI"
QT_MOC_LITERAL(54, 701, 14), // "ETC_Write_Data"
QT_MOC_LITERAL(55, 716, 12), // "ETC_One_Look"
QT_MOC_LITERAL(56, 729, 19), // "ETC_Read_A_UserData"
QT_MOC_LITERAL(57, 749, 14), // "ETC_A_Up_Money"
QT_MOC_LITERAL(58, 764, 16), // "ETC_A_Down_Money"
QT_MOC_LITERAL(59, 781, 10), // "ETC_B_Mode"
QT_MOC_LITERAL(60, 792, 14), // "ETC_B_Del_Time"
QT_MOC_LITERAL(61, 807, 17), // "ETC_B_Up_Handrail"
QT_MOC_LITERAL(62, 825, 19), // "ETC_B_Down_Handrail"
QT_MOC_LITERAL(63, 845, 22), // "ETC_B_Handrail_AllOpen"
QT_MOC_LITERAL(64, 868, 14), // "ETC_B_Del_User"
QT_MOC_LITERAL(65, 883, 14), // "ETC_B_Up_Money"
QT_MOC_LITERAL(66, 898, 16), // "ETC_B_Down_Money"
QT_MOC_LITERAL(67, 915, 16), // "ETC_B_Price_Save"
QT_MOC_LITERAL(68, 932, 13), // "G_Change_Mode"
QT_MOC_LITERAL(69, 946, 10), // "G_Analysis"
QT_MOC_LITERAL(70, 957, 11), // "G_Del_Cargo"
QT_MOC_LITERAL(71, 969, 12), // "G_Read_Panid"
QT_MOC_LITERAL(72, 982, 13), // "G_Write_Panid"
QT_MOC_LITERAL(73, 996, 11), // "G_Read_Data"
QT_MOC_LITERAL(74, 1008, 12), // "G_Write_Data"
QT_MOC_LITERAL(75, 1021, 9), // "G_Get_Row"
QT_MOC_LITERAL(76, 1031, 15), // "G_Read_CardData"
QT_MOC_LITERAL(77, 1047, 16), // "G_Write_CardData"
QT_MOC_LITERAL(78, 1064, 11), // "G_Look_Card"
QT_MOC_LITERAL(79, 1076, 10), // "G_Label_ID"
QT_MOC_LITERAL(80, 1087, 32), // "on_IC_radioButton_Pass_B_clicked"
QT_MOC_LITERAL(81, 1120, 7), // "checked"
QT_MOC_LITERAL(82, 1128, 32), // "on_IC_radioButton_Pass_A_clicked"
QT_MOC_LITERAL(83, 1161, 36), // "on_IC_lineEdit_Sys_Write_text..."
QT_MOC_LITERAL(84, 1198, 4), // "arg1"
QT_MOC_LITERAL(85, 1203, 37) // "on_ETC_lineEdit_WriteData_tex..."

    },
    "MainWindow\0Get_Com\0\0OpenUart_Clicked\0"
    "Get_Time\0ReadUart\0Show_Data\0Set_Show_Mode\0"
    "Stop_Rec\0Del_Num\0Change_ID\0Change_IC\0"
    "Change_ETC\0Change_24G\0About_up\0Message\0"
    "Time_Save\0ID_Change_Mode\0ID_Open_Door\0"
    "ID_EM4100\0ID_Read_Card\0ID_Write_Card\0"
    "IC_Del_Card\0IC_Send_Look\0IC_Begin_Look\0"
    "IC_Data_Look\0IC_Pass_Change\0IC_Write_Region\0"
    "IC_Read_Region\0IC_Read_Mess\0Mode_Change\0"
    "IC_Read_Mess_flag\0Money_Up\0Down_Money_Save\0"
    "IC_Ticket_Reset\0IC_App_Ticket_Save\0"
    "IC_App_Up_Money\0IC_App_Send_Balance\0"
    "IC_App_Down_Money\0IC_Change_Pass\0"
    "IC_Switch_Mode\0ETC_Momery_Change\0"
    "ETC_Look_Begin\0ETC_Data_Analysis\0"
    "Del_Look_Data\0ETC_Change_Mode\0"
    "ETC_Read_Data\0ETC_Get_Region\0"
    "ETC_Set_Region\0ETC_Get_RF\0ETC_Set_RF\0"
    "ETC_Get_TxLv\0ETC_Set_TxLv\0ETC_Get_RSSI\0"
    "ETC_Write_Data\0ETC_One_Look\0"
    "ETC_Read_A_UserData\0ETC_A_Up_Money\0"
    "ETC_A_Down_Money\0ETC_B_Mode\0ETC_B_Del_Time\0"
    "ETC_B_Up_Handrail\0ETC_B_Down_Handrail\0"
    "ETC_B_Handrail_AllOpen\0ETC_B_Del_User\0"
    "ETC_B_Up_Money\0ETC_B_Down_Money\0"
    "ETC_B_Price_Save\0G_Change_Mode\0"
    "G_Analysis\0G_Del_Cargo\0G_Read_Panid\0"
    "G_Write_Panid\0G_Read_Data\0G_Write_Data\0"
    "G_Get_Row\0G_Read_CardData\0G_Write_CardData\0"
    "G_Look_Card\0G_Label_ID\0"
    "on_IC_radioButton_Pass_B_clicked\0"
    "checked\0on_IC_radioButton_Pass_A_clicked\0"
    "on_IC_lineEdit_Sys_Write_textChanged\0"
    "arg1\0on_ETC_lineEdit_WriteData_textChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      82,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  424,    2, 0x08 /* Private */,
       3,    0,  425,    2, 0x08 /* Private */,
       4,    0,  426,    2, 0x08 /* Private */,
       5,    0,  427,    2, 0x08 /* Private */,
       6,    0,  428,    2, 0x08 /* Private */,
       7,    0,  429,    2, 0x08 /* Private */,
       8,    0,  430,    2, 0x08 /* Private */,
       9,    0,  431,    2, 0x08 /* Private */,
      10,    0,  432,    2, 0x08 /* Private */,
      11,    0,  433,    2, 0x08 /* Private */,
      12,    0,  434,    2, 0x08 /* Private */,
      13,    0,  435,    2, 0x08 /* Private */,
      14,    0,  436,    2, 0x08 /* Private */,
      15,    0,  437,    2, 0x08 /* Private */,
      16,    0,  438,    2, 0x08 /* Private */,
      17,    0,  439,    2, 0x08 /* Private */,
      18,    0,  440,    2, 0x08 /* Private */,
      19,    0,  441,    2, 0x08 /* Private */,
      20,    0,  442,    2, 0x08 /* Private */,
      21,    0,  443,    2, 0x08 /* Private */,
      22,    0,  444,    2, 0x08 /* Private */,
      23,    0,  445,    2, 0x08 /* Private */,
      24,    0,  446,    2, 0x08 /* Private */,
      25,    0,  447,    2, 0x08 /* Private */,
      26,    0,  448,    2, 0x08 /* Private */,
      27,    0,  449,    2, 0x08 /* Private */,
      28,    0,  450,    2, 0x08 /* Private */,
      29,    0,  451,    2, 0x08 /* Private */,
      30,    0,  452,    2, 0x08 /* Private */,
      31,    0,  453,    2, 0x08 /* Private */,
      32,    0,  454,    2, 0x08 /* Private */,
      33,    0,  455,    2, 0x08 /* Private */,
      34,    0,  456,    2, 0x08 /* Private */,
      35,    0,  457,    2, 0x08 /* Private */,
      36,    0,  458,    2, 0x08 /* Private */,
      37,    0,  459,    2, 0x08 /* Private */,
      38,    0,  460,    2, 0x08 /* Private */,
      39,    0,  461,    2, 0x08 /* Private */,
      40,    0,  462,    2, 0x08 /* Private */,
      41,    0,  463,    2, 0x08 /* Private */,
      42,    0,  464,    2, 0x08 /* Private */,
      43,    0,  465,    2, 0x08 /* Private */,
      44,    0,  466,    2, 0x08 /* Private */,
      45,    0,  467,    2, 0x08 /* Private */,
      46,    0,  468,    2, 0x08 /* Private */,
      47,    0,  469,    2, 0x08 /* Private */,
      48,    0,  470,    2, 0x08 /* Private */,
      49,    0,  471,    2, 0x08 /* Private */,
      50,    0,  472,    2, 0x08 /* Private */,
      51,    0,  473,    2, 0x08 /* Private */,
      52,    0,  474,    2, 0x08 /* Private */,
      53,    0,  475,    2, 0x08 /* Private */,
      54,    0,  476,    2, 0x08 /* Private */,
      55,    0,  477,    2, 0x08 /* Private */,
      56,    0,  478,    2, 0x08 /* Private */,
      57,    0,  479,    2, 0x08 /* Private */,
      58,    0,  480,    2, 0x08 /* Private */,
      59,    0,  481,    2, 0x08 /* Private */,
      60,    0,  482,    2, 0x08 /* Private */,
      61,    0,  483,    2, 0x08 /* Private */,
      62,    0,  484,    2, 0x08 /* Private */,
      63,    0,  485,    2, 0x08 /* Private */,
      64,    0,  486,    2, 0x08 /* Private */,
      65,    0,  487,    2, 0x08 /* Private */,
      66,    0,  488,    2, 0x08 /* Private */,
      67,    0,  489,    2, 0x08 /* Private */,
      68,    0,  490,    2, 0x08 /* Private */,
      69,    0,  491,    2, 0x08 /* Private */,
      70,    0,  492,    2, 0x08 /* Private */,
      71,    0,  493,    2, 0x08 /* Private */,
      72,    0,  494,    2, 0x08 /* Private */,
      73,    0,  495,    2, 0x08 /* Private */,
      74,    0,  496,    2, 0x08 /* Private */,
      75,    0,  497,    2, 0x08 /* Private */,
      76,    0,  498,    2, 0x08 /* Private */,
      77,    0,  499,    2, 0x08 /* Private */,
      78,    0,  500,    2, 0x08 /* Private */,
      79,    0,  501,    2, 0x08 /* Private */,
      80,    1,  502,    2, 0x08 /* Private */,
      82,    1,  505,    2, 0x08 /* Private */,
      83,    1,  508,    2, 0x08 /* Private */,
      85,    1,  511,    2, 0x08 /* Private */,

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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Long,
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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   81,
    QMetaType::Void, QMetaType::Bool,   81,
    QMetaType::Void, QMetaType::QString,   84,
    QMetaType::Void, QMetaType::QString,   84,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Get_Com(); break;
        case 1: _t->OpenUart_Clicked(); break;
        case 2: _t->Get_Time(); break;
        case 3: _t->ReadUart(); break;
        case 4: _t->Show_Data(); break;
        case 5: _t->Set_Show_Mode(); break;
        case 6: _t->Stop_Rec(); break;
        case 7: _t->Del_Num(); break;
        case 8: _t->Change_ID(); break;
        case 9: _t->Change_IC(); break;
        case 10: _t->Change_ETC(); break;
        case 11: _t->Change_24G(); break;
        case 12: _t->About_up(); break;
        case 13: { bool _r = _t->Message();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 14: _t->Time_Save(); break;
        case 15: _t->ID_Change_Mode(); break;
        case 16: _t->ID_Open_Door(); break;
        case 17: _t->ID_EM4100(); break;
        case 18: _t->ID_Read_Card(); break;
        case 19: _t->ID_Write_Card(); break;
        case 20: _t->IC_Del_Card(); break;
        case 21: _t->IC_Send_Look(); break;
        case 22: _t->IC_Begin_Look(); break;
        case 23: _t->IC_Data_Look(); break;
        case 24: _t->IC_Pass_Change(); break;
        case 25: _t->IC_Write_Region(); break;
        case 26: _t->IC_Read_Region(); break;
        case 27: _t->IC_Read_Mess(); break;
        case 28: _t->Mode_Change(); break;
        case 29: _t->IC_Read_Mess_flag(); break;
        case 30: _t->Money_Up(); break;
        case 31: _t->Down_Money_Save(); break;
        case 32: _t->IC_Ticket_Reset(); break;
        case 33: _t->IC_App_Ticket_Save(); break;
        case 34: _t->IC_App_Up_Money(); break;
        case 35: _t->IC_App_Send_Balance(); break;
        case 36: _t->IC_App_Down_Money(); break;
        case 37: _t->IC_Change_Pass(); break;
        case 38: _t->IC_Switch_Mode(); break;
        case 39: _t->ETC_Momery_Change(); break;
        case 40: _t->ETC_Look_Begin(); break;
        case 41: _t->ETC_Data_Analysis(); break;
        case 42: _t->Del_Look_Data(); break;
        case 43: _t->ETC_Change_Mode(); break;
        case 44: _t->ETC_Read_Data(); break;
        case 45: _t->ETC_Get_Region(); break;
        case 46: _t->ETC_Set_Region(); break;
        case 47: _t->ETC_Get_RF(); break;
        case 48: _t->ETC_Set_RF(); break;
        case 49: _t->ETC_Get_TxLv(); break;
        case 50: _t->ETC_Set_TxLv(); break;
        case 51: _t->ETC_Get_RSSI(); break;
        case 52: _t->ETC_Write_Data(); break;
        case 53: _t->ETC_One_Look(); break;
        case 54: _t->ETC_Read_A_UserData(); break;
        case 55: _t->ETC_A_Up_Money(); break;
        case 56: _t->ETC_A_Down_Money(); break;
        case 57: _t->ETC_B_Mode(); break;
        case 58: _t->ETC_B_Del_Time(); break;
        case 59: _t->ETC_B_Up_Handrail(); break;
        case 60: _t->ETC_B_Down_Handrail(); break;
        case 61: _t->ETC_B_Handrail_AllOpen(); break;
        case 62: _t->ETC_B_Del_User(); break;
        case 63: _t->ETC_B_Up_Money(); break;
        case 64: { long _r = _t->ETC_B_Down_Money();
            if (_a[0]) *reinterpret_cast< long*>(_a[0]) = std::move(_r); }  break;
        case 65: _t->ETC_B_Price_Save(); break;
        case 66: _t->G_Change_Mode(); break;
        case 67: _t->G_Analysis(); break;
        case 68: _t->G_Del_Cargo(); break;
        case 69: _t->G_Read_Panid(); break;
        case 70: _t->G_Write_Panid(); break;
        case 71: _t->G_Read_Data(); break;
        case 72: _t->G_Write_Data(); break;
        case 73: _t->G_Get_Row(); break;
        case 74: _t->G_Read_CardData(); break;
        case 75: _t->G_Write_CardData(); break;
        case 76: _t->G_Look_Card(); break;
        case 77: _t->G_Label_ID(); break;
        case 78: _t->on_IC_radioButton_Pass_B_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 79: _t->on_IC_radioButton_Pass_A_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 80: _t->on_IC_lineEdit_Sys_Write_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 81: _t->on_ETC_lineEdit_WriteData_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
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
        if (_id < 82)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 82;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 82)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 82;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
