#include "logindialog.h"
#include <QFile>
#include <QTextStream>
/*登录和注册界面*/

LoginDialog::LoginDialog(QWidget *parent, AccountInformation* ai) : QDialog(parent)
{
    accountInformation = ai;
    setFixedSize(500, 350);
    setWindowTitle("登录");
    setWindowFlags(windowFlags() &~ Qt::WindowContextHelpButtonHint);

    /*登录界面weight*/
    loginLayoutWidget = new QWidget;
    loginLayoutWidget->setFixedSize(500, 350);
    welcomeLabel = new QLabel("欢迎使用DAQQ产品外观设计方案评价系统");
    welcomeLabel->setFixedHeight(75);
    welcomeLabel->setFrameStyle(QLabel::StyledPanel);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    QFont font;
    font.setPointSize(16);
    welcomeLabel->setFont(font);

    font.setPointSize(12);
    usernameLabel = new QLabel("账号");
    usernameLabel->setFont(font);
    usernameLineEdit = new QLineEdit;
    usernameLineEdit->setText("zhang");
    usernameLineEdit->setFont(font);

    passwordLabel = new QLabel("密码");
    passwordLabel->setFont(font);
    passwordLineEdit = new QLineEdit;
    passwordLineEdit->setText("123456");
    passwordLineEdit->setEchoMode(QLineEdit::Password);    //密码变成小黑点
    passwordLineEdit->setFont(font);

    okButton = new QPushButton("登录");
    registerButton = new QPushButton("注册");
    cancelButton = new QPushButton("退出");
    QGridLayout* loginLayout = new QGridLayout(loginLayoutWidget);
    loginLayout->setContentsMargins(0, 0, 0, 0);
    loginLayout->addWidget(welcomeLabel, 1, 0, 1, 7);
    loginLayout->addWidget(usernameLabel, 2, 1, 1, 1);
    loginLayout->addWidget(usernameLineEdit, 2, 2, 1, 3);
    loginLayout->addWidget(passwordLabel, 3, 1, 1, 1);
    loginLayout->addWidget(passwordLineEdit, 3, 2, 1, 3);
    loginLayout->addWidget(okButton, 4, 2, 1, 1);
    loginLayout->addWidget(registerButton, 4, 3, 1, 1);
    loginLayout->addWidget(cancelButton, 4, 4, 1, 1);
    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(slotOkButton()));
    connect(registerButton, SIGNAL(clicked(bool)), this, SLOT(slotRegisterButton()));
    connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(slotCancelButton()));


    /*注册界面weight*/
    registerLayoutWidget = new QWidget;
    registerLayoutWidget->setFixedSize(500, 350);
    welcomeLabelForRegister = new QLabel("注册新的账号");
    welcomeLabelForRegister->setFixedHeight(75);
    welcomeLabelForRegister->setFrameStyle(QLabel::StyledPanel);
    welcomeLabelForRegister->setAlignment(Qt::AlignCenter);
    font.setPointSize(16);
    welcomeLabelForRegister->setFont(font);

    font.setPointSize(12);
    usernameLabelForRegister = new QLabel("账号");
    usernameLabelForRegister->setFont(font);
    usernameLineEditForRegister = new QLineEdit;
    usernameLineEditForRegister->setText("");
    usernameLineEditForRegister->setFont(font);

    passwordLabelForRegister = new QLabel("密码");
    passwordLabelForRegister->setFont(font);
    passwordLineEditForRegister = new QLineEdit;
    passwordLineEditForRegister->setText("");
    passwordLineEditForRegister->setEchoMode(QLineEdit::Password);    //密码变成小黑点
    passwordLineEditForRegister->setFont(font);

    passwordConfirmLabel = new QLabel("密码");
    passwordConfirmLabel->setFont(font);
    passwordConfirmLineEdit = new QLineEdit;
    passwordConfirmLineEdit->setText("");
    passwordConfirmLineEdit->setEchoMode(QLineEdit::Password);    //密码变成小黑点
    passwordConfirmLineEdit->setFont(font);

    usernameTips = new QLabel("");
    usernameTips->setFont(font);
    passwordTips = new QLabel("");
    passwordTips->setFont(font);
    passwordConfirmTips = new QLabel("");
    passwordConfirmTips->setFont(font);

    registerButtonForRegister = new QPushButton("注册");
    registerButtonForRegister->setEnabled(false);
    backButtonForRegister = new QPushButton("返回");
    cancelButtonForRegister = new QPushButton("退出");
    QGridLayout* loginLayoutForRegister = new QGridLayout(registerLayoutWidget);
    loginLayoutForRegister->setContentsMargins(0, 0, 0, 0);
    loginLayoutForRegister->addWidget(welcomeLabelForRegister, 1, 0, 1, 7);
    loginLayoutForRegister->addWidget(usernameLabelForRegister, 2, 1, 1, 1);
    loginLayoutForRegister->addWidget(usernameLineEditForRegister, 2, 2, 1, 3);
    loginLayoutForRegister->addWidget(usernameTips, 2, 5, 1, 1);


    loginLayoutForRegister->addWidget(passwordLabelForRegister, 3, 1, 1, 1);
    loginLayoutForRegister->addWidget(passwordLineEditForRegister, 3, 2, 1, 3);
    loginLayoutForRegister->addWidget(passwordTips, 3, 5, 1, 1);


    loginLayoutForRegister->addWidget(passwordConfirmLabel, 4, 1, 1, 1);
    loginLayoutForRegister->addWidget(passwordConfirmLineEdit, 4, 2, 1, 3);
    loginLayoutForRegister->addWidget(passwordConfirmTips, 4, 5, 1, 1);


    loginLayoutForRegister->addWidget(registerButtonForRegister, 5, 2, 1, 1);
    loginLayoutForRegister->addWidget(backButtonForRegister, 5, 3, 1, 1);
    loginLayoutForRegister->addWidget(cancelButtonForRegister, 5, 4, 1, 1);
    connect(registerButtonForRegister, SIGNAL(clicked(bool)), this, SLOT(slotRegisterButtonForRegister()));
    connect(backButtonForRegister, SIGNAL(clicked(bool)), this, SLOT(slotBackButtonForRegister()));
    connect(cancelButtonForRegister, SIGNAL(clicked(bool)), this, SLOT(slotCancelButton()));

    connect(usernameLineEditForRegister, SIGNAL(textChanged(QString)), this, SLOT(slotPasswordLineEditTextChange()));
    connect(passwordLineEditForRegister, SIGNAL(textChanged(QString)), this, SLOT(slotPasswordLineEditTextChange()));
    connect(passwordConfirmLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotPasswordLineEditTextChange()));

    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(loginLayoutWidget);    //添加登录和注册的widegt到栈窗口
    stackedWidget->addWidget(registerLayoutWidget);
    stackedWidget->setCurrentIndex(0);    //设置显示的首页面

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(stackedWidget);
}

void LoginDialog::slotCancelButton()
{
    QDialog::reject();
}

void LoginDialog::slotRegisterButton()
{
    stackedWidget->setCurrentIndex(1);
}

void LoginDialog::slotOkButton()    //确定按钮的响应函数
{
    if(passwordLineEdit->text().isEmpty() || usernameLineEdit->text().isEmpty()){
        QMessageBox::information(this, "提示", "用户名或密码不能为空！", QMessageBox::Ok);
    }
    else{
        QString usernameInput = usernameLineEdit->text();    //用户输入
        QString passwordInput = passwordLineEdit->text();
        QString passwordStored = "";

        QSqlQuery query(accountInformation->db);    //初始化数据库连接

        QString sql = QString("select password from account where username='%1'").arg(usernameInput);    //查询密码
        query.exec(sql);
        if(query.next())
        {
            if(passwordInput == query.value(0).toString())    //验证成功，返回上层accept信号
            {
                accountInformation->username = usernameInput;    //存储用户名
                sql = QString("select user_id from account where username='%1'").arg(usernameInput);   //查询id
                query.exec(sql);
                if(query.next())
                {
                    accountInformation->user_id = query.value(0).toInt();
                }
                QDialog::accept();
            }
            else{
                QMessageBox::warning(this, "提示", "请确认输入密码是否正确！", QMessageBox::Ok);    //验证失败
            }
        }
        else{
            QMessageBox::warning(this, "提示", "不存在此用户！", QMessageBox::Ok);    //没查到
        }
    }
}



void LoginDialog::slotBackButtonForRegister()     //返回登录页面
{
    stackedWidget->setCurrentIndex(0);
}

void LoginDialog::slotRegisterButtonForRegister()     //返回登录页面
{
    QString sql = QString("insert into account values(null, '%1', %2)").arg(usernameLineEditForRegister->text()).arg(passwordLineEditForRegister->text());
    QSqlQuery query(accountInformation->db);    //查询密码
    if(query.exec(sql)){
        QMessageBox::information(this, "提示", "注册成功", QMessageBox::Ok);
        registerButtonForRegister->setEnabled(false);
        usernameLineEdit->setText(usernameLineEditForRegister->text());
        passwordLineEdit->setText(passwordLineEditForRegister->text());
        stackedWidget->setCurrentIndex(0);
    }
    else{
        QMessageBox::information(this, "提示", "注册失败", QMessageBox::Ok);
    }
}

void LoginDialog::slotPasswordLineEditTextChange()    //保证只有输入正确时按钮才可以点
{
    bool nameValid = false;
    bool passwordValid = false;

    QString sql = QString("select user_id from account where username='%1'").arg(usernameLineEditForRegister->text());
    QSqlQuery query(accountInformation->db);    //查询密码
    query.exec(sql);
    if(query.next()){
        usernameTips->setText("✖已存在");
        usernameTips->setStyleSheet("color:red;");
    }
    else{
        nameValid = true;
        usernameTips->setText("✔");
        usernameTips->setStyleSheet("color:green;");
    }


    if(!usernameLineEditForRegister->text().isEmpty() &&
           !passwordLineEditForRegister->text().isEmpty() &&
            !passwordConfirmLineEdit->text().isEmpty())
    {
        if(passwordLineEditForRegister->text() == passwordConfirmLineEdit->text())
        {
            passwordValid = true;
            passwordTips->setText("✔");
            passwordTips->setStyleSheet("color:green;");
            passwordConfirmTips->setText("✔");
            passwordConfirmTips->setStyleSheet("color:green;");
        }
        else
        {
            passwordTips->setText("✖");
            passwordTips->setStyleSheet("color:red;");
            passwordConfirmTips->setText("✖");
            passwordConfirmTips->setStyleSheet("color:red;");
        }

    }
    else    //有空的
    {
        if(usernameLineEditForRegister->text().isEmpty()){
            usernameTips->setText("✖");
            usernameTips->setStyleSheet("color:red;");
        }
        if(passwordTips->text().isEmpty()){
            passwordTips->setText("✖");
            passwordTips->setStyleSheet("color:red;");
        }
        if(passwordConfirmTips->text().isEmpty()){
            passwordConfirmTips->setText("✖");
            passwordConfirmTips->setStyleSheet("color:red;");
        }

    }

    if(nameValid && passwordValid){
        registerButtonForRegister->setEnabled(true);
    }
    else{
        registerButtonForRegister->setEnabled(false);
    }
}
