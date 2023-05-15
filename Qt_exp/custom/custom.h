#ifndef CUSTOM_H
#define CUSTOM_H

#include "BaseTitleBar.h"
#include <QWidget>
#include "ui_Custom.h"

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
    void initTitleBar(QColor& titlecolor);
    void paintEvent(QPaintEvent *event);
    void loadStyleSheet(const QString &sheetName);
    BaseTitleBar* m_titleBar;
    Ui::Form *ui;
};

#endif // CUSTOM_H
