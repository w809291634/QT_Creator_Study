#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QTreeWidgetItem>
#include <QFileInfoList>
#include <QTableWidgetItem>
#include <QMdiSubWindow>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void initInterface(void);
    void inttControl(void);
    void CreateMenu(void);
    enum color
    {
        RED=2,
        GREEN,  //默认值是3，比前一个多1
        BLUE=7
    };
private slots:
    /* 页切换 */
    void menu_clicked(void);
    void on_setCurrentIndex(void);

    /* page_widget */
    void on_cbox_frame_Shadow_currentIndexChanged(int index);
    void on_cbox_frame_Shape_currentIndexChanged(int index);
    void on_spinBox_lineWidth_valueChanged(int arg1);
    void on_spinBox_midLineWidth_valueChanged(int arg1);
    void on_cbox_demo_alignment_currentIndexChanged(int index);
    void on_checkBox_demo_clicked(bool checked);
    void on_checkBox_demo_flat_clicked(bool checked);

    /* stackedWidget */
    void on_btn_stackedWidget_delete_clicked();
    void on_btn_stackedWidget_insert_clicked();
    void on_btn_stackedWidget_toggle_clicked();

    /* tabWidget */
    void on_btn_tabWidget_delete_clicked();
    void on_btn_tabWidget_insert_clicked();
    void on_btn_tabWidget_toggle_clicked();
    void on_cbox_tabWidget_currentIndexChanged(int index);

    /* listWidget */
    void on_btn_listWidget_add_clicked();
    void on_btn_listWidget_clear_clicked();
    void on_btn_listWidget_delete_clicked();
    void on_btn_listWidget_insert_clicked();

    /* treeWidget */
    void on_btn_treeWidget_delete_clicked();
    void on_btn_treeWidget_path_clicked();
    QFileInfoList searchFile(QTreeWidgetItem* root,QString path);

    /* tableWidget */
    void on_btn_tableWidget_init_clicked();
    void on_btn_tableWidget_addcol_clicked();
    void on_btn_tableWidget_addrow_clicked();
    void on_btn_tableWidget_delcol_clicked();
    void on_btn_tableWidget_delrow_clicked();
    void on_btn_tableWidget_cleardate_clicked();
    void on_btn_tableWidget_cleartable_clicked();
    void on_btn_tableWidget_adjust_clicked();
    void on_btn_tableWidget_insertitem_clicked();
    void on_cbox_tableWidget_EditTrigger_currentIndexChanged(int index);
    void on_cbox_tableWidget_SelectionBehavior_currentIndexChanged(int index);
    void on_cbox_tableWidget_SelectionMode_currentIndexChanged(int index);
    void on_cbox_table_title_clicked(bool checked);
    void on_tableWidget_cellClicked(int row, int column);
    void on_tableWidget_itemClicked(QTableWidgetItem *item);

    /* mdiArea */
    void on_btn_mdiArea_add_clicked();
    void on_btn_mdiArea_close_clicked();
    void on_btn_mdiArea_closeAll_clicked();
    void on_cbox_mdiArea_layout_currentIndexChanged(int index);
    void on_cbox_mdiArea_model_currentIndexChanged(int index);
    void on_mdiArea_subWindowActivated(QMdiSubWindow *arg1);
};

#endif // MAINWINDOW_H
