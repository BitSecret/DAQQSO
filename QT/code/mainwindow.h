#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QStackedWidget>
#include <QtSql>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QWidget>
#include <QLabel>
#include "datastructure.h"
#include "projectwidget.h"
#include "helpwidget.h"
#include "changepasswordwidget.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, AccountInformation* ai = nullptr);

    /**********用户信息**********/
    AccountInformation* accountInformation;    //登录人和数据库接口

    /**********菜单部分**********/
    QMenu* projectMenu;    //项目菜单
    QAction* projectAction;    //当前项目
    QAction* openAction;    //打开项目
    QAction* createAction;    //新建项目
    QAction* downloadAction;    //下载模板
    QAction* importAction;    //导入项目
    QAction* manageAction;    //项目管理

    QMenu* userMenu;    //用户菜单
    QAction* changepasswordAction;    //修改密码
    QAction* changeUserAction;    //切换用户
    QAction* quitAction;    //退出系统

    QMenu* aboutMenu;      //关于菜单
    QAction* helpAction;   //帮助页
    QAction* aboutAction;   //关于
    QAction* copyrightAction;   //版权信息

    /**********页面控件**********/
    ProjectWidget* projectWidget;
    HelpWidget* helpWidget;
    ChangePasswordWidget* changePasswordWidget;

    /*******布局和窗口切换*******/
    QStackedWidget* stackedWidget;    //栈窗口，控制页面切换
    QWidget* layoutWidget;    //MainWidnwo中已有布局，不能再创建，所以通过添加QWidget来布局
private slots:
    void on_projectAction_clicked();
    void on_openAction_clicked();
    void on_createAction_clicked();
    void on_downloadAction_clicked();
    void on_importAction_clicked();
    void on_changepasswordAction_clicked();
    void on_changeUserAction_clicked();
    void on_quitAction_clicked();
    void on_helpAction_clicked();
    void on_aboutAction_clicked();
    void on_copyrightAction_clicked();
    void on_manageAction_clicked();
private:
    void creatActionAndMenu();    //创建菜单栏
    void initWidget();    //创建页面
    //void connectSwitchSignalsAndSlots();    //连接信号和槽

};

#endif // MAINWINDOW_H
