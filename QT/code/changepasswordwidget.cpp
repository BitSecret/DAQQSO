#include "changepasswordwidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

ChangePasswordWidget::ChangePasswordWidget(QWidget *parent, AccountInformation* ai) : QWidget(parent)
{
    accountInformation = ai;


    titleLabel = new QLabel("修改密码");
    titleLabel->setFixedHeight(40);
    titleLabel->setFrameStyle(QLabel::StyledPanel);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont font;
    font.setPointSize(25);
    titleLabel->setFont(font);

    changeLabel = new QLabel("请输入新密码");    //新密码
    changeLabel->setFixedHeight(60);
    font.setPointSize(22);
    changeLabel->setFont(font);
    changeLabel->setAlignment(Qt::AlignCenter);

    newPasswordLabel = new QLabel("新密码：");
    newPasswordLabel->setFixedHeight(60);
    newPasswordLabel->setAlignment(Qt::AlignHCenter);
    newPasswordLineEdit = new QLineEdit;

    newPasswordLineEdit->setFixedHeight(60);
    newPasswordLineEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordLabel = new QLabel("再输一次：");    //确认新密码
    confirmPasswordLabel->setAlignment(Qt::AlignHCenter);
    confirmPasswordLabel->setFixedHeight(60);
    confirmPasswordLineEdit = new QLineEdit;
    confirmPasswordLineEdit->setFixedHeight(60);
    confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);
    confirmHintLabel = new QLabel("");

    okButton = new QPushButton("确认");
    okButton->setFixedHeight(60);
    okButton->setEnabled(false);

    font.setPointSize(22);
    newPasswordLabel->setFont(font);
    newPasswordLineEdit->setFont(font);
    confirmPasswordLabel->setFont(font);
    confirmPasswordLineEdit->setFont(font);
    confirmHintLabel->setFont(font);
    okButton->setFont(font);

    /********布局管理*********/
    QGridLayout* changeLayout = new QGridLayout;
    changeLayout->addWidget(changeLabel, 1, 1, 1, 9);

    changeLayout->addWidget(newPasswordLabel, 2, 1, 1, 2);
    changeLayout->addWidget(newPasswordLineEdit, 2, 3, 1, 6);

    changeLayout->addWidget(confirmPasswordLabel, 3, 1, 1, 2);
    changeLayout->addWidget(confirmPasswordLineEdit, 3, 3, 1, 6);
    changeLayout->addWidget(confirmHintLabel, 3, 9, 1, 1);

    changeLayout->addWidget(okButton, 4, 1, 1, 9);


    QVBoxLayout* topLayout = new QVBoxLayout(this);
    topLayout->addSpacing(0);
    topLayout->addWidget(titleLabel);
    topLayout->addStretch(0);
    topLayout->addLayout(changeLayout);
    topLayout->addStretch(0);

    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(slotOkButton()));    //关联信号和槽
    connect(newPasswordLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotChangeLineEditTextChange()));
    connect(confirmPasswordLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotChangeLineEditTextChange()));
}
void ChangePasswordWidget::slotOkButton()    //确认按钮的响应函数
{
    QString sql = QString("update account set password='%1' where user_id=%2").arg(newPasswordLineEdit->text()).arg(accountInformation->user_id);
    qDebug() << sql;
    QSqlQuery query(accountInformation->db);
    if(query.exec(sql)){
        QMessageBox::warning(this, "提示", "修改成功！", QMessageBox::Ok);
    }
    else {
        QMessageBox::warning(this, "提示", "修改失败，可能原因：与数据库连接已断开！", QMessageBox::Ok);
    }
}


void ChangePasswordWidget::slotChangeLineEditTextChange()    //保证只有输入正确时按钮才可以点
{
    if(!newPasswordLineEdit->text().isEmpty() && !confirmPasswordLineEdit->text().isEmpty())
    {
        if(newPasswordLineEdit->text() == confirmPasswordLineEdit->text())
        {
            okButton->setEnabled(true);
            confirmHintLabel->setText("✔");
            confirmHintLabel->setStyleSheet("color:green;");
        }
        else
        {
            okButton->setEnabled(false);
            confirmHintLabel->setText("✖");
            confirmHintLabel->setStyleSheet("color:red;");
        }

    }
    else    //有空的
    {
        okButton->setEnabled(false);
        confirmHintLabel->setText("");
    }
}
