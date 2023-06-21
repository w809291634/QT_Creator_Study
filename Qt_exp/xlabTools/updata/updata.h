#ifndef UPDATA_H
#define UPDATA_H

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

#include <QMainWindow>
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QStandardPaths>
#include <QNetworkConfigurationManager>
#include <QTimer>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QCloseEvent>
#include <QHostInfo>


#define upDataFile_TEST "xLabTools_down"
#define upDataText      "http://demo.zhiyun360.com/appstore/update.txt"

namespace Ui {
class upData;
}

class upData : public QMainWindow
{
    Q_OBJECT

public:
    explicit upData(QMainWindow *parent = nullptr);
    ~upData();
    void timerEvent(QTimerEvent *event);
    void closeEvent(QCloseEvent *event);
    QString unitChange(qint64 date);
    void downData(QString urlStr);
    bool checkNetwork();
    void jsonAnalysis(QByteArray data);
    void upReset();
    bool verCompare(QString currVer, QString serverVer);
    void downUpData();
    QString getFilePath();
    void stopDownUp();
    QString backVersion();
public slots:
    void lookedUp(const QHostInfo &host);
private:
    Ui::upData *ui;
    QNetworkReply *avatorReply;
    QNetworkAccessManager *avatorManager;
    QString severVersion;
    QString upVersion = "v1.4.230310";                          // 当前软件版本
    QString upDataFileName = "xLabTools";                       // 下载文件名
    QString upDataByte;                                         // 下载链接
    QString upFilePath;
    bool firstFlag = false;                                     // 软件第一次开启检测更新
    bool putMessBox = false;                                    // 弹出弹窗
    bool checkMode = true;
    bool downIng = false;                                       // 下载过程中
    bool connectOkFlag = false;                                 // 网络是否可用
    int timeSecs;                                     // 1秒定时器ID
    qint64 byteValue = 0;                                // 下载总大小
    qint64 byteSpeed = 0;                                // 下载速度
private slots:
    void dataHandle(void);
    void set_progress(qint64 bytesRead, qint64 totalBytes);
    void downUpFinish();
    void on_btn_checkUp_clicked();
    void networkOk(QNetworkAccessManager::NetworkAccessibility okFlag);
    void on_btn_disConnect_clicked();
    void checkUp();
};

#endif // UPDATA_H
