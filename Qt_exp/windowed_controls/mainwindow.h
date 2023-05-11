#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMdiArea>
#include <QTreeWidgetItem>
#include <QFileInfoList>
#include <QTableWidgetItem>

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
    void initInterface();
//    void inttControl();
//    void CreateMenu();

private slots:




};

#endif // MAINWINDOW_H
