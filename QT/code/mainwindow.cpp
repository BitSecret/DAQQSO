#include "mainwindow.h"
#include "logindialog.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include "projectlistdialog.h"
#include "projectmanagedialog.h"

MainWindow::MainWindow(QWidget *parent, AccountInformation* ai) :
    QMainWindow(parent)
{
    accountInformation = ai;    //用户信息和数据库接口
    accountInformation->isProjectSaved = true;    //项目是否保存

    setWindowTitle(QString("DAQQ产品外观设计方案评价系统客户端-%1").arg(accountInformation->username));

    setFixedSize(1200, 675);
    initWidget();    //初始化页面
    creatActionAndMenu();    //创建菜单栏
    //connectSwitchSignalsAndSlots();    //连接页面切换信号和槽
}

void MainWindow::creatActionAndMenu()
{

    projectAction = new QAction("当前项目", this);    //创建Action
    projectAction->setShortcut(tr("Ctrl+P"));
    projectAction->setEnabled(false);    //一开始不可点
    openAction = new QAction("打开项目", this);
    openAction->setShortcut(tr("Ctrl+O"));
    createAction = new QAction("新建项目", this);
    createAction->setShortcut(tr("Ctrl+N"));
    downloadAction = new QAction("下载模板", this);
    downloadAction->setShortcut(tr("Ctrl+D"));
    importAction = new QAction("导入数据", this);
    importAction->setShortcut(tr("Ctrl+U"));
    manageAction = new QAction("项目管理", this);
    manageAction->setShortcut(tr("Ctrl+M"));

    changepasswordAction = new QAction("修改密码", this);
    changepasswordAction->setShortcut(tr("Ctrl+C"));
    changeUserAction = new QAction("切换用户", this);
    changeUserAction->setShortcut(tr("Ctrl+U"));
    quitAction = new QAction("退出系统", this);
    quitAction->setShortcut(tr("Ctrl+Q"));

    helpAction = new QAction("帮助", this);
    helpAction->setShortcut(tr("Ctrl+H"));
    aboutAction = new QAction("关于", this);
    aboutAction->setShortcut(tr("Ctrl+A"));
    copyrightAction = new QAction("版权", this);
    copyrightAction->setShortcut(tr("Ctrl+R"));

    connect(projectAction, SIGNAL(triggered(bool)), this, SLOT(on_projectAction_clicked()));    //关联Action和Slot
    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(on_openAction_clicked()));
    connect(createAction, SIGNAL(triggered(bool)), this, SLOT(on_createAction_clicked()));
    connect(downloadAction, SIGNAL(triggered(bool)), this, SLOT(on_downloadAction_clicked()));
    connect(importAction, SIGNAL(triggered(bool)), this, SLOT(on_importAction_clicked()));
    connect(changepasswordAction, SIGNAL(triggered(bool)), this, SLOT(on_changepasswordAction_clicked()));
    connect(changeUserAction, SIGNAL(triggered(bool)), this, SLOT(on_changeUserAction_clicked()));
    connect(quitAction, SIGNAL(triggered(bool)), this, SLOT(on_quitAction_clicked()));
    connect(helpAction, SIGNAL(triggered(bool)), this, SLOT(on_helpAction_clicked()));
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(on_aboutAction_clicked()));
    connect(copyrightAction, SIGNAL(triggered(bool)), this, SLOT(on_copyrightAction_clicked()));
    connect(manageAction, SIGNAL(triggered(bool)), this, SLOT(on_manageAction_clicked()));

    projectMenu = menuBar()->addMenu("操作");    //创建Menu
    projectMenu->addAction(projectAction);
    projectMenu->addAction(manageAction);
    projectMenu->addAction(openAction);
    projectMenu->addAction(createAction);
    projectMenu->addAction(downloadAction);
    projectMenu->addAction(importAction);

    userMenu = menuBar()->addMenu("权限");
    userMenu->addAction(changepasswordAction);
    userMenu->addAction(changeUserAction);
    userMenu->addAction(quitAction);

    aboutMenu = menuBar()->addMenu("关于");
    aboutMenu->addAction(helpAction);
    aboutMenu->addAction(aboutAction);
    aboutMenu->addAction(copyrightAction);
}

void MainWindow::initWidget()
{
    /**********页面控件**********/
    stackedWidget = new QStackedWidget;    //栈窗口，控制页面切换

    projectWidget = new ProjectWidget(this, accountInformation);    //几个页面
    helpWidget = new HelpWidget(this, accountInformation);
    changePasswordWidget = new ChangePasswordWidget(this, accountInformation);

    stackedWidget->addWidget(projectWidget);    //添加页面到stack
    stackedWidget->addWidget(helpWidget);
    stackedWidget->addWidget(changePasswordWidget);

    stackedWidget->setCurrentIndex(1);    //设置显示的首页面

    /**********布局部分**********/
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(stackedWidget);

    layoutWidget = new QWidget;
    layoutWidget->setLayout(mainLayout);
    setCentralWidget(layoutWidget);
}

void MainWindow::on_projectAction_clicked()
{
    stackedWidget->setCurrentIndex(0);    //设置当前页为项目页
}
void MainWindow::on_openAction_clicked()
{
    if(!accountInformation->isProjectSaved && (QMessageBox::information(this, "提示", "当前项目未保存，是否保存？", QMessageBox::Ok, QMessageBox::No) == QMessageBox::Ok)){
        projectWidget->save();    //保存项目
        accountInformation->isProjectSaved = true;
    }

    ProjectListDialog projectListDialog(this, accountInformation);    //选择界面
    if(projectListDialog.exec() == QDialog::Accepted)    //用户选择了项目
    {
        projectWidget->clearData();    //先把之前的数据清空

        projectWidget->showData(projectWidget->getDataByDB(accountInformation->projectId));    //从数据库导入数据
        stackedWidget->setCurrentIndex(0);    //设置当前页为项目页

        accountInformation->isNewProject = false;
        accountInformation->isProjectSaved = true;
        projectAction->setEnabled(true);
    }
}
void MainWindow::on_createAction_clicked()
{
    if(!accountInformation->isProjectSaved && (QMessageBox::information(this, "提示", "当前项目未保存，是否保存？", QMessageBox::Ok, QMessageBox::No) == QMessageBox::Ok)){
        projectWidget->save();    //保存项目
        accountInformation->isProjectSaved = true;
    }

    projectWidget->clearData();    //先把之前的数据清空
    accountInformation->projectId = -1;    //表示新项目
    stackedWidget->setCurrentIndex(0);    //设置当前页为项目页

    accountInformation->isNewProject = true;
    accountInformation->isProjectSaved = false;
    projectAction->setEnabled(true);
}
void MainWindow::on_downloadAction_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "打开", "C:/Users/Admin/Desktop");
    if(!dir.isEmpty())    //如果用户选择了路径，才运行
    {
        QString filePath = dir + "\\" + "mode.MCDMData";
        QFile file(filePath);           //读文件
        file.open(QIODevice::WriteOnly);   //只读
        QTextStream out(&file);    //输入流
        out.setCodec("utf-8");    //防止中文乱码

        out << "project name" << "\t//project name    tips:you need to delete the content // in the end of every line" << endl;
        out << "project introduction" << "\t//project intro" << endl;
        out << 20 << "\t//subjects number" << endl;
        out << "s1	s2	s3	s4	s5	s6	s7	s8	s9	s10	s11	s12	s13	s14	s15	s16	s17	s18	s19	s20" << "\t//subjects name" << endl;
        out << 4 << "\t//alternative count" << endl;
        out << "a1	a2	a3	a4" << "\t//alternative name" << endl;
        out << 13 << "\t//criteria count" << endl;
        out << "c1	c2	c3	c4	c5	c6	c7	c8	c9	c10	c11	c12	c13" << "\t//criteria name" << endl;
        out << "1.2468 	14.7920 	50.1692 	1.2001 	2.3940..." << "\t//please input data, count:subjects*alternative*criteria" << endl;
        file.close();
        QMessageBox::information(this, "提示", "保存成功。");
    }
}
void MainWindow::on_importAction_clicked()
{
    if(!accountInformation->isProjectSaved && (QMessageBox::information(this, "提示", "当前项目未保存，是否保存？", QMessageBox::Ok, QMessageBox::No) == QMessageBox::Ok)){
        projectWidget->save();    //保存项目
        accountInformation->isProjectSaved = true;
    }


    QString dataFilePath = QFileDialog::getOpenFileName(this, "打开", "C:/Users/Admin/Desktop", "files (*.MCDMData)");
    if(!dataFilePath.isEmpty())    //如果用户选择了文件，才运行
    {
        projectWidget->clearData();    //先把之前的数据清空
        accountInformation->projectId = -1;    //表示新项目

        projectWidget->showData(projectWidget->getDataByFile(dataFilePath));    //从文件导入数据数据
        stackedWidget->setCurrentIndex(0);    //设置当前页为项目页

        accountInformation->isNewProject = true;
        accountInformation->isProjectSaved = false;
        projectAction->setEnabled(true);
    }
}
void MainWindow::on_changepasswordAction_clicked()
{
    stackedWidget->setCurrentIndex(2);    //设置当前页为修改密码页
}
void MainWindow::on_changeUserAction_clicked()
{
    this->hide();    //先隐藏
    LoginDialog loginDialog(nullptr, accountInformation);    //登陆界面
    if(loginDialog.exec() == QDialog::Accepted){    //验证身份成功
        MainWindow* mainWindow = new MainWindow(nullptr, accountInformation);
        mainWindow->show();
    }
    this->close();
}
void MainWindow::on_quitAction_clicked()
{
    this->close();
}
void MainWindow::on_helpAction_clicked()
{
    stackedWidget->setCurrentIndex(1);    //设置当前页为帮助页
}
void MainWindow::on_aboutAction_clicked()
{
    QMessageBox::information(this, "联系我们", "作品问题发送至邮箱13583322920@163.com\n");
}
void MainWindow::on_copyrightAction_clicked()
{
    QMessageBox::information(this, "作品声明", "1.本系统由重庆工商大学2017级计算机二班张效凯所开发。\n"
                                           "2.未经作者允许禁止用于任何途径。\n"
                                           "3.本程序最终解释权归作者所有。");
}
void MainWindow::on_manageAction_clicked()
{
    if(!accountInformation->isProjectSaved && (QMessageBox::information(this, "提示", "当前项目未保存，是否保存？", QMessageBox::Ok, QMessageBox::No) == QMessageBox::Ok)){
        projectWidget->save();    //保存项目
        accountInformation->isProjectSaved = true;
    }

    ProjectManageDialog projectManageDialog(this, accountInformation);    //选择界面
    if(projectManageDialog.exec() == QDialog::Accepted)    //用户选择了项目
    {
        projectWidget->clearData();    //先把之前的数据清空

        projectWidget->showData(projectWidget->getDataByDB(accountInformation->projectId));    //从数据库导入数据
        stackedWidget->setCurrentIndex(0);    //设置当前页为项目页

        accountInformation->isNewProject = false;
        accountInformation->isProjectSaved = true;
        projectAction->setEnabled(true);
    }
}
