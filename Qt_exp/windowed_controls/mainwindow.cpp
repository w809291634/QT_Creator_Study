#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initInterface();
    inttControl();
    CreateMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* 初始化控件属性 */
void MainWindow::initInterface()
{
    /* 主窗口默认状态 */
    ui->dockWidget->setVisible(true);
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_menu->setCurrentIndex(0);
    ui->stackedWidget_demo->setCurrentIndex(0);

    /* page_widget 子页 */
    ui->groupBox_demo->setCheckable(true);          // 设置 QGroupBox 带有切换功能
    ui->groupBox_demo->setChecked(false);           // 设置 QGroupBox 当前没有被激活
    // page_widget 子页对应的 dockWidget 控制窗口
    QStringList shapelist;
    shapelist<<"NoFrame"<<"Box"<<"Panel"<<"WinPanel"<<"HLine"<<"VLine"<<"StyledPanel";
    ui->cbox_frame_Shape->addItems(shapelist);

    QStringList shadowlist;
    shadowlist<<"Plain"<<"Raised"<<"Sunken";
    ui->cbox_frame_Shadow->addItems(shadowlist);

    QStringList alignmentlist;
    alignmentlist<<"AlignLeft"<<"AlignRight"<<"AlignHCenter"<<"AlignJustify";
    ui->cbox_demo_alignment->addItems(alignmentlist);

    /* page_tabWidget 子页 */
    ui->tabWidget->setCurrentIndex(0);
    QStringList list;
    list<<"North"<<"South"<<"West"<<"East";
    ui->cbox_tabWidget->addItems(list);

//    QStringList Alignlist;
//    Alignlist<<"AlignLeft"<<"AlignHCenter"<<"AlignRight"<<"AlignTop"<<"AlignVCenter"<<"AlignBottom";
//    ui->cbox_scrollArea_property->addItems(Alignlist);

    /* page_mdiArea 子页 */
    QStringList layoutlist;
    layoutlist<<"级联展开"<<"平铺展开";
    ui->cbox_mdiArea_layout->addItems(layoutlist);
    QStringList modelist;
    modelist<<"窗口模式"<<"tab模式";
    ui->cbox_mdiArea_model->addItems(modelist);

    /* page_listWidget 子页 */
    QStringList stylelist;
    stylelist<<"style"<<"style 1"<<"style 2"<<"style 3";
    ui->cbox_listWidget_style->addItems(stylelist);

    /* cbox_tableWidget 子页 */
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

/* 初始化控件控制 */
void MainWindow::inttControl()
{
    QList<QAction*> actionlist = this->findChildren<QAction*>(QString(),Qt::FindDirectChildrenOnly);
    foreach(auto action, actionlist)
    {
        connect(action,SIGNAL(triggered()),this,SLOT(on_setCurrentIndex()));
    }
}

/* 创建自定义菜单 */
void MainWindow::CreateMenu()
{
    /* 建立一个一级菜单 */
    QMenu *mainMenu = new QMenu(this);              // 建立一级菜单类，父类为主窗口
    mainMenu->setTitle("mainMenu");

    QAction *action1 = new QAction(mainMenu);
    action1->setObjectName("action1");
    action1->setText("action1");

    QAction *action2 = new QAction(mainMenu);
    action2->setObjectName("action2");
    action2->setText("action2");

    QList<QAction *> actionlist;
    actionlist << action1 << action2;

    /* 建立一个二级菜单 */
    QMenu *nextMenu = new QMenu(mainMenu);          // 建立二级菜单类，父类为mainMenu
    nextMenu->setTitle("nextMenu&N");               // 设置快捷键

    QAction *action3 = new QAction(nextMenu);
    action3->setObjectName("action3");
    action3->setText("action3");

    QAction *action4 = new QAction(nextMenu);
    action4->setObjectName("action4");
    action4->setText("action4");

    /* 一级菜单添加项目 */
    mainMenu->addActions(actionlist);
    mainMenu->addMenu(nextMenu);
    /* 二级菜单添加项目 */
    nextMenu->addAction(action3);
    nextMenu->addAction(action4);
    /* 主窗口添加菜单子项 */
    ui->menuBar->addMenu(mainMenu);

    /* 设置如上所有action的连接槽函数 */
    QList<QString> action_name_list;
    action_name_list << "action1" << "action2" << "action3" << "action4";
    QList<QAction*> actionlist_1 = mainMenu->findChildren<QAction*>();
    foreach(auto action, actionlist_1)
    {
        for(auto name:action_name_list){
            if(action->objectName()==name){
//                qDebug()<<action->objectName();
                connect(action,SIGNAL(triggered()),this,SLOT(menu_clicked()));
            }
        }
    }
}

/* 菜单栏的回调函数 */
void MainWindow::on_setCurrentIndex()
{
    QAction* action= qobject_cast<QAction*>(sender());

    /* Widget 菜单 */
    if(action->objectName()=="actionQWidget")
    {
        ui->stackedWidget->setCurrentIndex(0);
        ui->stackedWidget_menu->setCurrentIndex(0);
    }
    else if(action->objectName()=="actionQScrollArea")
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->stackedWidget_menu->setCurrentIndex(1);
    }
    else if(action->objectName()=="actionQStackedWidget")
    {
        ui->stackedWidget->setCurrentIndex(2);
        ui->stackedWidget_menu->setCurrentIndex(2);
    }
    else if(action->objectName()=="actionQTabWidget")
    {
        ui->stackedWidget->setCurrentIndex(3);
        ui->stackedWidget_menu->setCurrentIndex(3);
    }
    else if(action->objectName()=="actionQListWidget")
    {
        ui->stackedWidget->setCurrentIndex(4);
        ui->stackedWidget_menu->setCurrentIndex(4);
    }
    else if(action->objectName()=="actionQTreeWidget")
    {
        ui->stackedWidget->setCurrentIndex(5);
        ui->stackedWidget_menu->setCurrentIndex(5);
    }
    else if(action->objectName()=="actionQTableWidget")
    {
        ui->stackedWidget->setCurrentIndex(6);
        ui->stackedWidget_menu->setCurrentIndex(6);
    }

    /* Etc 菜单 */
    else if(action->objectName()=="actionQDockWidget")
    {
        bool visible;
        visible=!ui->dockWidget->isVisible();       // 获取当前 dockWidget 的显示状态，根据状态取反

        ui->dockWidget->setVisible(visible);
    }
    else if(action->objectName()=="actionQMdiArea")
    {
        ui->stackedWidget->setCurrentIndex(7);
        ui->stackedWidget_menu->setCurrentIndex(7);
    }
    else if(action->objectName()=="actionOpenGWidget"){}
    else if(action->objectName()=="actionQuickWidget"){}

    /* About 菜单 */
    else if(action->objectName()=="actionAbout_Qt"){
        QMessageBox::aboutQt(this,"about Qt");
    }

    ui->statusBar->showMessage(QString("切换到:%1页").arg(action->text()));
}

/* 自定义 菜单栏 槽函数 */
void MainWindow::menu_clicked()
{
    QAction* action= qobject_cast<QAction*>(sender());
    ui->statusBar->showMessage(QString("%1被点击").arg(action->objectName()));
}

/* page_widget */
void MainWindow::on_cbox_frame_Shadow_currentIndexChanged(int index)
{
    switch(index){
    case 0:
        ui->frame->setFrameShadow(QFrame::Shadow::Plain);
        break;
    case 1:
        ui->frame->setFrameShadow(QFrame::Shadow::Raised);
        break;
    case 2:
        ui->frame->setFrameShadow(QFrame::Shadow::Sunken);
        break;
    default:
        qDebug()<<"index error";
        break;
    }
}

void MainWindow::on_cbox_frame_Shape_currentIndexChanged(int index)
{
    ui->frame->setFrameShape(QFrame::Shape(index));
}

void MainWindow::on_spinBox_lineWidth_valueChanged(int arg1)
{
    ui->frame->setLineWidth(arg1);
}

void MainWindow::on_spinBox_midLineWidth_valueChanged(int arg1)
{
    ui->frame->setMidLineWidth(arg1);
}

void MainWindow::on_cbox_demo_alignment_currentIndexChanged(int index)
{
    switch (index) {
    case 0:    ui->groupBox_demo->setAlignment(Qt::AlignLeft);
        break;
    case 1:    ui->groupBox_demo->setAlignment(Qt::AlignRight);
        break;
    case 2:    ui->groupBox_demo->setAlignment(Qt::AlignCenter);
        break;
    default:    ui->groupBox_demo->setAlignment(Qt::AlignJustify);
        break;
    }
}

void MainWindow::on_checkBox_demo_clicked(bool checked)
{
    ui->groupBox_demo->setChecked(checked);
}

void MainWindow::on_checkBox_demo_flat_clicked(bool checked)
{
    ui->groupBox_demo->setFlat(checked);
}

/* stackedWidget */
void MainWindow::on_btn_stackedWidget_delete_clicked()
{
    ui->stackedWidget_demo->removeWidget(ui->stackedWidget_demo->currentWidget());
}

void MainWindow::on_btn_stackedWidget_insert_clicked()
{
    QLabel *label=new QLabel();
    label->setText(QString("page%1").arg(ui->stackedWidget_demo->count()+1));
    label->setAlignment(Qt::AlignCenter);
    ui->stackedWidget_demo->addWidget(label);
    ui->stackedWidget_demo->setCurrentIndex(ui->stackedWidget_demo->indexOf(label));    // 切换到
}

void MainWindow::on_btn_stackedWidget_toggle_clicked()
{
    int page=ui->stackedWidget_demo->currentIndex();
    if(++page>=ui->stackedWidget_demo->count())
        page=0;
    ui->stackedWidget_demo->setCurrentIndex(page);
}

/* tabWidget */
void MainWindow::on_btn_tabWidget_delete_clicked()
{
    ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
}

void MainWindow::on_btn_tabWidget_insert_clicked()
{
    QLabel *label= new QLabel();
    QString str=QString("Tab %1").arg(ui->tabWidget->count()+1);
    label->setText(str);
    label->setAlignment(Qt::AlignCenter);
    ui->tabWidget->addTab(label,str);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(label));
}

void MainWindow::on_btn_tabWidget_toggle_clicked()
{
    int page=ui->tabWidget->currentIndex();
    if(++page>=ui->tabWidget->count())
        page=0;

    ui->tabWidget->setCurrentIndex(page);
}

void MainWindow::on_cbox_tabWidget_currentIndexChanged(int index)
{
    ui->tabWidget->setTabPosition(QTabWidget::TabPosition(index));
}

/* listWidget */
void MainWindow::on_btn_listWidget_add_clicked()
{
    QListWidgetItem *item=new QListWidgetItem();
    item->setText(QString("当前列:%1").arg(ui->listWidget->count()));
    if(ui->cbox_listWidget_style->currentText()=="style 1"){
        item->setIcon(QIcon(":/icon/icon/Industry_and_academia.png"));
    }
    else if(ui->cbox_listWidget_style->currentText()=="style 2"){
        item->setIcon(QIcon(":/icon/icon/innovate.png"));
        item->setCheckState(Qt::Unchecked);
    }
    else if(ui->cbox_listWidget_style->currentText()=="style 3"){
        item->setIcon(QIcon(":/icon/icon/mortarboard.png"));
        item->setCheckState(Qt::Checked);
    }
    if (ui->cbox_edit->isChecked())
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
    else
        item->setFlags(Qt::ItemIsSelectable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
    ui->listWidget->addItem(item);
}

void MainWindow::on_btn_listWidget_insert_clicked()
{
    QListWidgetItem *item=new QListWidgetItem();
    item->setText(QString("当前列:%1").arg(ui->listWidget->count()));
    if(ui->cbox_listWidget_style->currentText()=="style 1"){
        item->setIcon(QIcon(":/icon/icon/产学.png"));
    }
    else if(ui->cbox_listWidget_style->currentText()=="style 2"){
        item->setIcon(QIcon(":/icon/icon/创新.png"));
        item->setCheckState(Qt::Unchecked);
    }
    else if(ui->cbox_listWidget_style->currentText()=="style 3"){
        item->setIcon(QIcon(":/icon/icon/学士帽.png"));
        item->setCheckState(Qt::Checked);
    }
    if (ui->cbox_edit->isChecked())
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
    else
        item->setFlags(Qt::ItemIsSelectable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);

    ui->listWidget->insertItem(ui->listWidget->currentRow(),item);
}

void MainWindow::on_btn_listWidget_clear_clicked()
{
    ui->listWidget->clear();
}

void MainWindow::on_btn_listWidget_delete_clicked()
{
    QListWidgetItem* item=ui->listWidget->takeItem(ui->listWidget->currentRow());
    delete item;
}

/* treeWidget */
void MainWindow::on_btn_treeWidget_path_clicked()
{
    QString curPath=QDir::currentPath();
    QString dlgTitle="选择文件夹";
    QString rootpath = QFileDialog::getExistingDirectory(this,dlgTitle,curPath);        // 打开文件对话框，选择一个文件路径
    if (rootpath.isEmpty())
        return;

    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList()<<rootpath);
    root->setIcon(0, QIcon(":/images/notepad/ic_file.png"));
    root->setCheckState(1, Qt::Checked);
    searchFile(root,rootpath);

    ui->treeWidget->addTopLevelItem(root);
}

QFileInfoList MainWindow::searchFile(QTreeWidgetItem *root,QString path)
{
    QDir dir(path);
    QDir file(path);

    file.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    file.setSorting(QDir::Size | QDir::Reversed);
    QFileInfoList fileList = file.entryInfoList();

    //当前目录的file
    foreach(auto file, fileList)
    {
        QTreeWidgetItem* child = new QTreeWidgetItem(QStringList()<<file.fileName());
        child->setIcon(0, QIcon(":/images/notepad/ic_setting.png"));
        child->setCheckState(1, Qt::Checked);
        root->addChild(child);
    }

    QFileInfoList fileListCur=dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    //当前目录的dir
    foreach(auto folder, folderList)
    {
        QTreeWidgetItem* childroot = new QTreeWidgetItem(QStringList()<<folder.fileName());
        childroot->setIcon(0, QIcon(":/images/notepad/menu_icon.png"));
        childroot->setCheckState(1, Qt::Checked);
        root->addChild(childroot);
        QFileInfoList fileListChild = searchFile(childroot,folder.absoluteFilePath());
        fileListCur.append(fileListChild);
        fileListCur.append(folder.fileName());
    }

    return fileListCur;
}

void MainWindow::on_btn_treeWidget_delete_clicked()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    delete item;
}

/* tableWidget */
void MainWindow::on_btn_tableWidget_init_clicked()
{
    ui->tableWidget->setRowCount(ui->spinBox_row->value());
    ui->tableWidget->setColumnCount(ui->spinBox_col->value());
}

void MainWindow::on_btn_tableWidget_addrow_clicked()
{
    int curRow=ui->tableWidget->currentRow();
    ui->tableWidget->insertRow(curRow);
}

void MainWindow::on_btn_tableWidget_addcol_clicked()
{
    int curCol=ui->tableWidget->currentColumn();
    ui->tableWidget->insertColumn(curCol);
}

void MainWindow::on_btn_tableWidget_delrow_clicked()
{
    int curRow=ui->tableWidget->currentRow();
    ui->tableWidget->removeRow(curRow);
}

void MainWindow::on_btn_tableWidget_delcol_clicked()
{
    int curCol=ui->tableWidget->currentColumn();
    ui->tableWidget->removeColumn(curCol);
}

void MainWindow::on_btn_tableWidget_cleartable_clicked()
{
    ui->tableWidget->clear();
}

void MainWindow::on_btn_tableWidget_cleardate_clicked()
{
    ui->tableWidget->clearContents();
}

void MainWindow::on_btn_tableWidget_adjust_clicked()
{
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
}

void MainWindow::on_btn_tableWidget_insertitem_clicked()
{
    ui->tableWidget->clearContents();
    for(int i=0; i< ui->tableWidget->rowCount();i++ )
    {
        for(int j=0; j<ui->tableWidget->columnCount(); j++)
        {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setText(QString("item(%1,%2)").arg(i+1).arg(j+1));
            ui->tableWidget->setItem(i,j,item);
        }
    }
}

void MainWindow::on_cbox_table_title_clicked(bool checked)
{
    ui->tableWidget->verticalHeader()->setVisible(checked);
    ui->tableWidget->horizontalHeader()->setVisible(checked);
}

void MainWindow::on_cbox_tableWidget_EditTrigger_currentIndexChanged(int index)
{
    switch(index){
    case 0:ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);break;
    case 1:ui->tableWidget->setEditTriggers(QAbstractItemView::CurrentChanged);break;
    case 2:ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);break;
    case 3:ui->tableWidget->setEditTriggers(QAbstractItemView::SelectedClicked);break;
    case 4:ui->tableWidget->setEditTriggers(QAbstractItemView::EditKeyPressed);break;
    case 5:ui->tableWidget->setEditTriggers(QAbstractItemView::AnyKeyPressed);break;
    case 6:ui->tableWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);break;
    default:qDebug()<<"index error";break;
    }
}

void MainWindow::on_cbox_tableWidget_SelectionBehavior_currentIndexChanged(int index)
{
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectionBehavior(index));
}

void MainWindow::on_cbox_tableWidget_SelectionMode_currentIndexChanged(int index)
{
    ui->tableWidget->setSelectionMode(QAbstractItemView::SelectionMode(index));
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    ui->lab_pos_x->setText(QString::number(row+1));
    ui->lab_pos_y->setText(QString::number(column+1));
}

void MainWindow::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    ui->lab_content->setText(item->text());
}

/* mdiArea */
void MainWindow::on_btn_mdiArea_add_clicked()
{
    QLabel* label=new QLabel();
    QString str(QString("subwindow %1").arg(ui->mdiArea->subWindowList().count()));
    label->setObjectName(str);
    label->setWindowTitle(str);
    label->setText(str);
    label->setAlignment(Qt::AlignCenter);
    ui->mdiArea->addSubWindow(label);
    label->show();
}

void MainWindow::on_btn_mdiArea_close_clicked()
{
    ui->mdiArea->closeActiveSubWindow();
}

void MainWindow::on_btn_mdiArea_closeAll_clicked()
{
    ui->mdiArea->closeAllSubWindows();
}

void MainWindow::on_cbox_mdiArea_layout_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        ui->mdiArea->cascadeSubWindows();
        break;
    case 1:
        ui->mdiArea->tileSubWindows();
        break;
    default:
        ui->mdiArea->cascadeSubWindows();
        break;
    }
}

void MainWindow::on_cbox_mdiArea_model_currentIndexChanged(int index)
{
    ui->mdiArea->setViewMode(QMdiArea::ViewMode(index));
}

void MainWindow::on_mdiArea_subWindowActivated(QMdiSubWindow *arg1)
{
    (void)arg1;
    if (ui->mdiArea->subWindowList().count()==0)
        ui->statusBar->showMessage("所有窗口均被关闭");
    else
    {
        QLabel *lab = static_cast<QLabel*>(ui->mdiArea->currentSubWindow()->widget());
        ui->statusBar->showMessage(lab->objectName());
        qDebug()<<lab->text();
    }
}
