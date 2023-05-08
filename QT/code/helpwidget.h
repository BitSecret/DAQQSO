#ifndef HELPWIDGET_H
#define HELPWIDGET_H
#include <QWidget>
#include <QLabel>
#include "datastructure.h"

class HelpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HelpWidget(QWidget *parent = nullptr, AccountInformation* ai = nullptr);

private:
    AccountInformation* accountInformation;    //用户信息

    QLabel* timeLabel;    //时间
    QLabel* welcomeLabel;    //欢迎语
    QLabel* titleLabel;    //欢迎语
    QLabel* introductionLabel;    //系统介绍
    QTimer* timer;    //定时器，用于更新时间

    void initWidget();    //初始化控件

private slots:
    void slotTimeChanged();    //实时更新时间
};

#endif // HELPWIDGET_H
