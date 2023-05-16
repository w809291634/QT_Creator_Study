#include "custom.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>
#include <QFile>
#include <QScreen>
#include <QColor>

Custom::Custom(QWidget *parent):
    QWidget(parent)
    ,ui(new Ui::Form)
    ,m_titlecolor(QColor(Qt::lightGray))                    // 标题窗口的背景颜色
    ,m_win_backgroundcolor(QColor(230,234,255))             // 窗体的背景色
{
    ui->setupUi(this);
    // 设置 自定义 窗体的属性
    this->setWindowFlags(Qt::FramelessWindowHint            // FramelessWindowHint属性设置窗口去除边框;
                         | Qt::WindowMinimizeButtonHint);   // WindowMinimizeButtonHint 属性设置在窗口最小化时，点击任务栏窗口可以显示出原窗口;
    setAttribute(Qt::WA_TranslucentBackground);             // 设置窗口背景透明
    // 初始化 标题栏
    initTitleBar(m_titlecolor,                              // 标题栏 背景颜色
                 QStringLiteral(":/icon/logo100.png"),      // 标题 图标
                 QStringLiteral("我的自定义窗口"),            // 自定义窗口文本
                 BaseTitleBar::MIN_MAX_BUTTON,              // 启动 最小化、最大化和关闭按钮
                 QStringLiteral(":/MyTitle.css"));          // 样式表css 路径
}

Custom::~Custom()
{
    delete ui;
}

// 初始化标题栏
// titlecolor:标题栏的背景色
// TitleIcon_path:标题栏图标的路径
// Title
//
void Custom::initTitleBar(QColor& titlecolor,
                          QString TitleIcon_path,
                          QString Title,
                          BaseTitleBar::ButtonType btn_type,
                          QString StyleSheet_Path)
{
    m_titleBar = new BaseTitleBar(this,StyleSheet_Path);
    m_titleBar->move(0, 0);                                 // 移动到 左上角
    m_titleBar->setTitleRoll();
    m_titleBar->setBackgroundColor(titlecolor.red(),titlecolor.green(),titlecolor.blue());   // 设置 标题栏 的背景颜色
    m_titleBar->setTitleIcon(TitleIcon_path);               // 初始化 标题栏 图标
    m_titleBar->setTitleContent(Title);                     // 初始化 标题栏 文字
    m_titleBar->setButtonType(btn_type);                    // 初始化 标题栏 按钮类型
    m_titleBar->setTitleWidth(this->width());

    // 进行 标题和窗口 的信号连接
    connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
    connect(m_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
    connect(m_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
    connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
}

// 窗体绘制事件
void Custom::paintEvent(QPaintEvent* event)
{
    //设置背景色;
    QPainter painter(this);
    QPainterPath pathBack;
    pathBack.setFillRule(Qt::WindingFill);
    pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.fillPath(pathBack, QBrush(m_win_backgroundcolor));

    return QWidget::paintEvent(event);
}

// 窗体加载样式表
void Custom::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/Resources/" + sheetName + ".css");
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }
}

// 窗口最小化
void Custom::onButtonMinClicked()
{
    if (Qt::Tool == (windowFlags() & Qt::Tool))
        hide();                                     //设置了Qt::Tool 如果调用showMinimized()则窗口就销毁了
    else
        showMinimized();
}

// 窗口恢复
void Custom::onButtonRestoreClicked()
{
    QPoint windowPos;
    QSize windowSize;
    m_titleBar->getRestoreInfo(windowPos, windowSize);
    this->setGeometry(QRect(windowPos, windowSize));
}

// 窗口最大化
void Custom::onButtonMaxClicked()
{
    m_titleBar->saveRestoreInfo(this->pos(), QSize(this->width(), this->height()));
//    QRect desktopRect = QApplication::desktop()->availableGeometry();     // 新版本废弃
    QScreen* Screen=QGuiApplication::primaryScreen();                       // 获取当前使用的首要屏幕
    QRect desktopRect=Screen->geometry();                                   // 获取当前的尺寸
    QRect FactRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
    setGeometry(FactRect);                                                  // 设置窗口为最大尺寸
}

// 窗口关闭
void Custom::onButtonCloseClicked()
{
    close();
}
