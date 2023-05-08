#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QtSql>
#include <QMessageBox>
#include "logindialog.h"
#include "connection.h"
#include "datastructure.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    AccountInformation* ai = new AccountInformation;    //登录账户信息


    if(!createConnection(ai)){    //连接数据库
        QMessageBox::information(nullptr, QObject::tr("无法建立连接"), "无法建立连接，请检查网络设置！ ", QMessageBox::Ok);
        return 0;
    };

    LoginDialog loginDialog(nullptr, ai);    //登陆界面
    if(loginDialog.exec() == QDialog::Accepted){    //验证身份成功，登录
        MainWindow* mainWindow = new MainWindow(nullptr, ai);
        mainWindow->show();
        return app.exec();
    }

    return 0;    //退出
}
