#include "helpwidget.h"
#include <QVBoxLayout>
#include <QLabel>

HelpWidget::HelpWidget(QWidget *parent, AccountInformation* ai) : QWidget(parent)
{
    accountInformation = ai;
    initWidget();
}

void HelpWidget::initWidget()
{
    timer = new QTimer(this);    //计时器
    timer->start(1000);   //1000ms为周期启动定时器
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimeChanged()));    //连接计时器槽函数

    QFont font;    //字体
    timeLabel = new QLabel(QDateTime::currentDateTime().toString("yyyy年MM月dd日 hh:mm:ss"));
    timeLabel->setAlignment(Qt::AlignRight);
    welcomeLabel = new QLabel(QString("%1，下午好，欢迎您登陆系统！").arg(accountInformation->username));
    welcomeLabel->setFixedHeight(75);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    font.setPointSize(30);
    welcomeLabel->setFont(font);
    titleLabel = new QLabel("帮助");
    titleLabel->setFixedHeight(25);
    titleLabel->setAlignment(Qt::AlignCenter);
    font.setPointSize(20);
    titleLabel->setFont(font);
    introductionLabel = new QLabel("\n"
                                   "当您第一次登录系统时，请查看此新手指引，这将会极大地缩减您使用系统的时间。\n"
                                   "本系统上方有三个菜单，分别是操作菜单、权限菜单和关于菜单。\n"
                                   "\n"
                                   "---------------------------------------------------------------------------------------------------"
                                   "\n"
                                   "当您需要新建项目时，点击操作菜单-->新建项目。输入您项目的各项信息，点击确定，即可建立一项新的项目。\n"
                                   "如果您需要将此项目保存到数据库，请在退出系统前点击项目页面的保存按钮。\n"
                                   "下次登录系统时，点击操作菜单-->打开项目，即可打开之前保存的项目。\n"
                                   "如果您要通过文件的方式导入项目，请点击操作菜单-->下载，下载导入模板。\n"
                                   "然后点击操作菜单-->导入，导入项目。点击操作菜单-->当前项目切换到当前项目。\n"
                                   "\n"
                                   "---------------------------------------------------------------------------------------------------"
                                   "\n"
                                   "在权限菜单，您可以修改密码，切换用户或退出系统。\n"
                                   "如果您的研究是多人共享一个账号，那推荐您针对不同的研究，注册不同的账号。\n"
                                   "这样可以保证不同之间项目的隔离和安全性。\n"
                                   "\n"
                                   "---------------------------------------------------------------------------------------------------"
                                   "\n"
                                   "如果您对软件的使用有问题，请点击关于菜单，联系我们。\n"
                                   "另外，此页面也可以通过关于菜单-->帮助，调出.\n"
                                   "祝您使用愉快！\n");
    introductionLabel->setFixedHeight(480);
    introductionLabel->setFixedWidth(1000);

    introductionLabel->setFrameStyle(QLabel::StyledPanel);

    introductionLabel->setAlignment(Qt::AlignLeft);
    font.setPointSize(15);
    introductionLabel->setFont(font);


    QVBoxLayout* mainLayout = new QVBoxLayout(this);    //布局
    mainLayout->setSpacing(5);
    mainLayout->addWidget(timeLabel);
    mainLayout->addStretch(0);
    mainLayout->addWidget(welcomeLabel);
    mainLayout->addStretch(0);
    mainLayout->addWidget(introductionLabel, 0, Qt::AlignCenter);    //控件居中
    mainLayout->addStretch(0);
}


void HelpWidget::slotTimeChanged()    //实时更新时间
{
    timeLabel->setText(QDateTime::currentDateTime().toString("yyyy年MM月dd日 hh:mm:ss"));
}
