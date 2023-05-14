#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFileDialog>
#include <QTextStream>
#include <QList>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_openFile_clicked(){
    QString curPath=QDir::currentPath();//获取应用程序的路径
    QString dlgTitle="选择文件"; //对话框标题
    QString filter="文本文件(*.txt);;图片文件(*.jpg *.gif *.png);;所有文件(*.*)"; //文件过滤器
    QString fileName=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->statusBar->showMessage("打开文件失败");
        return;
    }
    ui->statusBar->showMessage("打开文件:"+fileName);

    QTextStream in(&file);
    ui->textEdit->clear();
    ui->textEdit->setPlainText(in.readAll());
    file.close();
}
void MainWindow::on_btn_openFiles_clicked(){
    QString curPath=QDir::currentPath();//获取应用程序的路径
    QString dlgTitle="选择多个文件"; //对话框标题
    QString filter="文本文件(*.txt);;图片文件(*.jpg *.gif *.png);;所有文件(*.*)"; //文件过滤器
    QStringList filelist = QFileDialog::getOpenFileNames(this,dlgTitle,curPath,filter);

    ui->textEdit->clear();
    foreach(QString str,filelist)
        ui->textEdit->append(str);

}
void MainWindow::on_btn_openDir_clicked(){

}
void MainWindow::on_btn_save_clicked(){

}
