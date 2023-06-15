#include "updata.h"
#include "ui_updata.h"

static QString upDataFilePost = ".exe";
static QFile upDataFile;

upData::upData(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::upData)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() &~ Qt::WindowMaximizeButtonHint);
    timeSecs = startTimer(1000);
    QHostInfo::lookupHost("www.baidu.com", this, SLOT(lookedUp(QHostInfo)));
    ui->btn_checkUp->hide();
}

/**
* @funcname      timerEvent()
* @brief         定时器事件
* @param         ecevt-事件
* @return        无
* @author        Zhoucj
* @date          2018-11-19
***************************************************************/
void upData::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == timeSecs && connectOkFlag == true)
    {
        ui->lbl_speedNum->setText(unitChange(byteSpeed));
        byteSpeed = 0;
    }
}

/**
* @funcname      closeEvent()
* @brief         关闭应用事件
* @param         无
* @return        无
* @author        Zhoucj
* @date          2018-11-19
***************************************************************/
void upData::closeEvent(QCloseEvent *event)
{
    bool ok = false;
    if(downIng)
    {
        ok = QMessageBox::warning(this, "提示", "是否取消下载并退出？", "是", "否");
        if(ok == 0)
        {
            stopDownUp();
            checkMode = true;
            downIng = false;
            this->close();
        }
        else
            event->ignore();
    }
    else
        this->close();
}

upData::~upData()
{
    avatorReply->close();
    delete ui;
}

/**
* @funcname      getFilePath()
* @brief         获取文件储存路径
* @param         无
* @return        filePath-路径/Error-出错
* @author        Zhoucj
* @date          2018-11-19
***************************************************************/
QString upData::getFilePath(void)
{
    QString filePath = QFileDialog::getExistingDirectory(this, "请选择文件路径...", "./");
    if(!filePath.isEmpty())
        return filePath;
    else
    {
        return "Error";
    }
}
/**
* @funcname      checkNetwork()
* @brief         检测并返回当前网络状态
* @param         无
* @return        true-网络可用/flash-网络不可用
* @author        Zhoucj
* @date          2018-11-19
***************************************************************/
bool upData::checkNetwork(void)
{
    QHostInfo::lookupHost("www.baidu.com", this, SLOT(lookedUp(QHostInfo)));
    return connectOkFlag;
}

void upData::lookedUp(const QHostInfo &host)
{
    if(host.error() != QHostInfo::NoError)
    {
        connectOkFlag = false;
    }
    else
    {
//        qDebug() << host.addresses().first().toString();
        connectOkFlag = true;
    }
}


/**
* @funcname      upReset()
* @brief         界面初始化
* @param         无
* @return        无
* @author        Zhoucj
* @date          2018-11-19
***************************************************************/
void upData::upReset(void)
{
    ui->lbl_speedNumStr->setText("下载速度：");
    ui->lbl_byteValue->setText("0 MB");
    ui->lbl_speedNum->setText("0 KB/S");
    ui->pgb_upData->setValue(0);
    byteValue = 0;
    byteSpeed = 0;
}

/**
* @funcname      networkOk()
* @brief         实时检测网络状态
* @param         okFlag-传入参数
* @return        无
* @author        Zhoucj
* @date          2018-11-19
***************************************************************/
void upData::networkOk(QNetworkAccessManager::NetworkAccessibility okFlag)
{
    static unsigned char lastOkFlag = 0;
    if(lastOkFlag != connectOkFlag && downIng == true)
    {
        if(okFlag == QNetworkAccessManager::Accessible)
            connectOkFlag = true;
        else if(okFlag == QNetworkAccessManager::NotAccessible)
        {
            connectOkFlag = false;
            QMessageBox::warning(this, "警告", "网络连接中断！");
        }
        lastOkFlag = connectOkFlag;
    }
}

/**
* @funcname      downUpData()
* @brief         urlStr-下载链接
* @param         无
* @return        无
* @author        Zhoucj
* @date          2018-11-19
***************************************************************/
void upData::downData(QString urlStr)
{
    if(checkNetwork() == true)
    {
        upReset();
        if(checkMode == false)
        {
            upFilePath = getFilePath();
            if(upFilePath != "Error")
            {
                upDataFile.setFileName(upFilePath + "/" + upDataFile_TEST + upDataFilePost);
                if(upDataFile.exists())
                {
                    upDataFile.remove();
                }
            }
            else
            {
                QMessageBox::warning(this, "提示", "选择路径出错，请重新选择！");
                this->close();
                return;
            }
        }
        downIng = true;
        QUrl serviceUrl = QUrl(urlStr);
        avatorManager = new QNetworkAccessManager(this);
        avatorReply = avatorManager->get(QNetworkRequest(serviceUrl));
        QObject::connect(avatorReply, SIGNAL(readyRead()), this, SLOT(dataHandle()));
        QObject::connect(avatorReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(set_progress(qint64,qint64)));
        QObject::connect(avatorReply, SIGNAL(finished()), this, SLOT(downUpFinish()));
        QObject::connect(avatorManager, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)),
                         this, SLOT(networkOk(QNetworkAccessManager::NetworkAccessibility)));

    }
    else
    {
        if(putMessBox)
        {
            QMessageBox::warning(this, "提示", "请检查网络连接！");
        }
    }
}

/**
* @funcname      downUpFinish()
* @brief         下载安装完成
* @param         无
* @return        无
* @author        Zhoucj
* @date          2018-11-19
***************************************************************/
void upData::downUpFinish(void)
{
    bool renameOkFlag = 0;
    downIng = false;
    if(checkMode == false)
    {
        if(upDataFile.exists())
        {
            upFilePath = upFilePath.replace("/", "\\");
            renameOkFlag = upDataFile.rename(upFilePath + "\\" + upDataFileName + upDataFilePost);
            if(!renameOkFlag)
            {
                renameOkFlag = upDataFile.rename(upFilePath + "\\" + upDataFileName + '_' + severVersion + upDataFilePost);
                if(!renameOkFlag)
                {
                    upDataFile.remove();
                    QMessageBox::warning(this, "提示", "文件下载路径已包含当前最新版本！");
                }
            }
        }
        this->close();
    }
}

/**
* @funcname      unitChange()
* @brief         速度单位换算
* @param         date-速度
* @return        speedStr-单位换算
* @author        Zhoucj
* @date          2018-11-19
***************************************************************/
QString upData::unitChange(qint64 date)
{
    QString speedStr;
    double speedFloat = 0;
    speedFloat = (date / 104857.6) * 0.1;
    if(date < 1024)
        speedStr = QString::number(date, 10) + "B/S";
    else if(date >= 104857.6)
        speedStr = QString::number(speedFloat, '.', 2) + "MB/S";
    else
        speedStr = QString::number(date / 1024, 10) + "KB/S";
    return speedStr;
}

/**
* @funcname      dataHandle()
* @brief         处理下载的数据
* @param         无
* @return        无
* @author        Zhoucj
* @date          2018-11-19
***************************************************************/
void upData::dataHandle(void)
{
    // 数据处理
    if(checkMode == false)
    {
        if (upDataFile.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            upDataFile.write(avatorReply->readAll());
        }
        upDataFile.close();
    }
    else
    {
        QByteArray byteArray = avatorReply->readAll();
        int checkNum = byteArray.indexOf(upDataFileName) + upDataFileName.length() + 4;
        if(checkNum <= 12)
        {
            if(putMessBox)
            {
                QMessageBox::warning(this, "提示", "获取版本信息失败！");
            }
        }
        else
        {
            byteArray.remove(0, checkNum);
            checkNum = byteArray.indexOf('}');
            byteArray.remove(checkNum + 1, byteArray.length());
            downIng = false;
            jsonAnalysis(byteArray);
        }
    }
}

/**
* @funcname      verCompare()
* @brief         当前版本与服务器版本对比
* @param         currVer-当前版本/secerVer-服务器版本
* @return        true-需要更新/flash-不用更新
* @author        Zhoucj
* @date          2018-11-19
***************************************************************/
bool upData::verCompare(QString currVer, QString serverVer)
{
    bool ok = false;
    if(currVer < serverVer)
    {
        ok = QMessageBox::information(this, "提示", "当前版本："+ currVer + "\n最新版本："+ serverVer +"\n检测到新版本！是否更新？", "更新", "取消");
        if(ok == 0)
            return true;
        else
        {
            this->close();
            return false;
        }
    }
    else
    {
        if(putMessBox)
            QMessageBox::about(this, "提示", "当前版本："+ currVer + "\n最新版本："+ serverVer +"\n当前已是最新版本！");
        return false;
    }
}

/**
* @funcname      jsonAnalysis()
* @brief         josn数据解析
* @param         data-服务器版本数据
* @return        无
* @author        Zhoucj
* @date          2018-11-19
***************************************************************/
void upData::jsonAnalysis(QByteArray data)
{
    QJsonParseError josnError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data, &josnError);
    if(josnError.error == QJsonParseError::NoError)
    {
        if(jsonDocument.isObject())
        {
            QJsonObject obj = jsonDocument.object();
            if(obj.contains("url"))
            {
                QJsonValue version_value = obj.take("url");
                if(version_value.isString())
                {
                    upDataByte = version_value.toString();
                }
            }
            if(obj.contains("ver"))
            {
                QJsonValue name_value = obj.take("ver");
                if(name_value.isString())
                {
                    severVersion = name_value.toString();
                    if(verCompare(upVersion, severVersion))
                        downUpData();
                }
            }
        }
    }
}

/**
* @funcname      downUpData()
* @brief         下载文件
* @param         无
* @return        无
* @author        Zhoucj
* @date          2018-11-19
***************************************************************/
void upData::downUpData(void)
{
    if(!upDataByte.isEmpty() && downIng == false)
    {
        this->show();
        this->setWindowModality(Qt::ApplicationModal);//其他窗口不可用
        checkMode = false;
        downData(upDataByte);
    }
    else
    {
        if(putMessBox)
        {
            QMessageBox::warning(this, "提示", "获取版本信息失败！");
        }
    }
}

/**
* @funcname      set_progress
* @brief         更新进度条
* @param         bytesRead-获取的数据量/totalBytes-文件总大小
* @return        无
* @author        Zhoucj
* @date          2018-11-15
***************************************************************/
void upData::set_progress(qint64 bytesRead, qint64 totalBytes)
{
    static qint64 lastByte = 0;
    byteValue = totalBytes;
    ui->lbl_byteValue->setText(unitChange(byteValue).replace(QString("/S"), QString("")));
    ui->pgb_upData->setMaximum(totalBytes);
    ui->pgb_upData->setValue(bytesRead);
    byteSpeed += bytesRead - lastByte;
    lastByte = bytesRead;
}

/**
* @funcname      checkUp()
* @brief         获取服务器版本号
* @param         无
* @return        无
* @author        Zhoucj
* @date          2018-11-19
***************************************************************/
void upData::checkUp(void)
{
    checkMode = true;
    downData(upDataText);
}

/**
* @funcname      stopDownUp()
* @brief         停止下载
* @param         无
* @return        无
* @author        Zhoucj
* @date          2018-11-19
***************************************************************/
void upData::stopDownUp(void)
{
    if(downIng == true)
    {
        downIng = false;
        QObject::disconnect(avatorReply, SIGNAL(readyRead()), this, SLOT(dataHandle()));
        QObject::disconnect(avatorReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(set_progress(qint64,qint64)));
        QObject::disconnect(avatorReply, SIGNAL(finished()), this, SLOT(downUpFinish()));
        QObject::disconnect(avatorManager, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)),
                            this, SLOT(networkOk(QNetworkAccessManager::NetworkAccessibility)));
        if(upDataFile.exists())
        {
            upDataFile.remove();
        }
        upReset();
    }
    this->close();
}

QString upData::backVersion()
{
    return this->upVersion;
}

void upData::on_btn_checkUp_clicked()
{
    putMessBox = true;
    checkUp();
}

void upData::on_btn_disConnect_clicked()
{
    stopDownUp();
}
