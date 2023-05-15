#include "custom.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>
#include <QFile>
#include <QScreen>
#include <QColor>

Custom::Custom(QWidget *parent) :
    QWidget(parent)
    ,ui(new Ui::Form)
{
    ui->setupUi(this);

    // FramelessWindowHint属性设置窗口去除边框;
    // WindowMinimizeButtonHint 属性设置在窗口最小化时，点击任务栏窗口可以显示出原窗口;
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint );
    setAttribute(Qt::WA_TranslucentBackground); // 设置窗口背景透明;
    QColor color=QColor(Qt::white);
    initTitleBar(color);                             // 初始化标题栏
}

Custom::~Custom()
{
    delete ui;
}

// 初始化标题栏
void Custom::initTitleBar(QColor& titlecolor)
{
    m_titleBar = new BaseTitleBar(this);
    m_titleBar->move(0, 0);                     // 移动到 左上角
    m_titleBar->setTitleRoll();
    m_titleBar->setBackgroundColor(titlecolor.red(),titlecolor.green(),titlecolor.blue());   // 设置 标题栏 的背景颜色
    m_titleBar->setTitleIcon(":/icon.png");
    m_titleBar->setTitleContent(QStringLiteral("自定义窗口-欢迎来到我的窗口"));
    m_titleBar->setButtonType(BaseTitleBar::MIN_MAX_BUTTON);
    m_titleBar->setTitleWidth(this->width());

    connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
    connect(m_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
    connect(m_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
    connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
}

void Custom::paintEvent(QPaintEvent* event)
{
    //设置背景色;
    QPainter painter(this);
    QPainterPath pathBack;
    pathBack.setFillRule(Qt::WindingFill);
    pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.fillPath(pathBack, QBrush(QColor(215, 221, 228)));

    return QWidget::paintEvent(event);
}

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
        hide();    //设置了Qt::Tool 如果调用showMinimized()则窗口就销毁了？？？
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
