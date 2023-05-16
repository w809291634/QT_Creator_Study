#ifndef CUSTOM_H
#define CUSTOM_H

#include "basetitlebar.h"
#include <QWidget>
#include "ui_custom.h"

class Custom : public QWidget
{
    Q_OBJECT
public:
    explicit Custom(QWidget *parent = nullptr);
    ~Custom();

private slots:
    void onButtonMinClicked();
    void onButtonRestoreClicked();
    void onButtonMaxClicked();
    void onButtonCloseClicked();

private:
    void initTitleBar(QColor& titlecolor,
                      QString TitleIcon_path=QStringLiteral(":/icon.png"),
                      QString Title=QStringLiteral("自定义窗口-欢迎来到我的窗口"),
                      BaseTitleBar::ButtonType btn_type=BaseTitleBar::MIN_MAX_BUTTON,
                      QString StyleSheet_Path=":/MyTitle.css");
    void paintEvent(QPaintEvent *event);
    void loadStyleSheet(const QString &sheetName);

    // ui文件的指针
    Ui::Form *ui;
    // 自定义标题栏指针
    BaseTitleBar* m_titleBar;
    // 标题窗口的背景颜色
    QColor m_titlecolor;
    // 窗口的背景颜色
    QColor m_win_backgroundcolor;
};

#endif // CUSTOM_H
