#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initInterface();
//    inttControl();
//    CreateMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initInterface()
{
    /* 主窗口默认状态 */
    ui->dockWidget->setVisible(true);
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_menu->setCurrentIndex(0);
    ui->stackedWidget_demo->setCurrentIndex(0);

    /* QWidget 子页 */
    ui->groupBox_demo->setCheckable(true);
    ui->groupBox_demo->setChecked(false);

    QStringList shapelist;
    shapelist<<"NoFrame"<<"Box"<<"Panel"<<"WinPanel"<<"HLine"<<"VLine"<<"StyledPanel";
    ui->cbox_frame_Shape->addItems(shapelist);

    QStringList shadowlist;
    shadowlist<<"Plain"<<"Raised"<<"Sunken";
    ui->cbox_frame_Shadow->addItems(shadowlist);

    QStringList alignmentlist;
    alignmentlist<<"AlignLeft"<<"AlignRight"<<"AlignHCenter"<<"AlignJustify";
    ui->cbox_demo_alignment->addItems(alignmentlist);

    ui->tabWidget->setCurrentIndex(0);
    QStringList list;
    list<<"North"<<"South"<<"West"<<"East";
    ui->cbox_tabWidget->addItems(list);

//    QStringList Alignlist;
//    Alignlist<<"AlignLeft"<<"AlignHCenter"<<"AlignRight"<<"AlignTop"<<"AlignVCenter"<<"AlignBottom";
//    ui->cbox_scrollArea_property->addItems(Alignlist);

    QStringList layoutlist;
    layoutlist<<"级联展开"<<"平铺展开";
    ui->cbox_mdiArea_layout->addItems(layoutlist);
    QStringList modelist;
    modelist<<"窗口模式"<<"tab模式";
    ui->cbox_mdiArea_model->addItems(modelist);

    QStringList stylelist;
    stylelist<<"style"<<"style 1"<<"style 2"<<"style 3";
    ui->cbox_listWidget_style->addItems(stylelist);

    QStringList edittriggerlist;
    edittriggerlist<<"NoEditTriggers"<<"CurrentChanged"<<"DoubleClicked"<<"SelectedClicked"<<"EditKeyPressed"<<"AnyKeyPressed"<<"AllEditTriggers";
    ui->cbox_tableWidget_EditTrigger->addItems(edittriggerlist);

    QStringList selectlist;
    selectlist<<"SelectItems"<<"SelectRows"<<"SelectColumns";
    ui->cbox_tableWidget_SelectionBehavior->addItems(selectlist);

    QStringList selectionModelist;
    selectionModelist<<"NoSelection"<<"SingleSelection"<<"MultiSelection"<<"ExtendedSelection"<<"ContiguousSelection";
    ui->cbox_tableWidget_SelectionMode->addItems(selectionModelist);
}
