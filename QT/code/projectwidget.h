#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H
#include <QWidget>
#include "datastructure.h"
#include <QLabel>
#include <QTableWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>

class ProjectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProjectWidget(QWidget *parent = nullptr, AccountInformation* ai = nullptr);
    ProjectInformation* getDataByFile(QString);    //从文件导入数据
    ProjectInformation* getDataByDB(int);    //从数据库导入数据
    void showData(ProjectInformation*);    //显示数据
    ProjectInformation* getDataFromWidget();    //将面板的数据整理到ProjectInformation数据结构
    void getWeightFromWidget();    //将面板数据整理到Weight数据结构
    void clearData();    //清空数据
    void save();    //保存数据到数据库
    void computeDecisionMatrix();
private:
    /**********数据接口**********/
    ComputeInfomation* computeInfo;
    AccountInformation* accountInformation;    //当前用户信息

    /**********控件**********/
    QLabel* titleLabel;    //顶部控件

    QLabel* projectNameLabel;
    QLineEdit* projectNameLineEdit;

    QLabel* projectIntroLabel;
    QLineEdit* projectIntroLineEdit;

    QLabel* subjectsCountLabel;
    QLineEdit* subjectsCountLineEdit;
    QLabel* subjectsNameLabel;
    QLineEdit* subjectsNameLineEdit;

    QLabel* alternativeCountLabel;
    QLineEdit* alternativeCountLineEdit;
    QLabel* alternativeNameLabel;
    QLineEdit* alternativeNameLineEdit;

    QLabel* criteriaCountLabel;
    QLineEdit* criteriaCountLineEdit;
    QLabel* criteriaNameLabel;
    QLineEdit* criteriaNameLineEdit;

    QPushButton* dataButton;

    QTabWidget* dataTableWidget;
    QTableWidget* dataTable;

    QPushButton* generateLingoButton;
    QLineEdit* weightLineEdit;
    QPushButton* importWeightButton;
    QPushButton* saveButton;   //保存项目
    QPushButton* exportButton;    //导出项目成txt文件

    QPushButton* executeButton;
    QTextEdit* executeResultTextEdit;


    /**********函数**********/
    void initWidget();    //初始化控件
    void connectSignalsAndSlots();    //连接信号和槽
signals:
    void switchTheWidget(int);    //发给上层的信号，用于QStackWidget页面切换
private slots:
    /**按钮响应函数**/
    void slotDataButton();    //生成数据矩阵
    void slotGenerateLingoButton();    //生成lingo语句
    void slotImportWeightButton();    //导入权重
    void slotSaveButton();    //保存项目
    void slotExportButton();    //导出项目
    void slotExecuteButton();    //执行项目生成结果

    void slotTextChange();    //文本变化响应函数

};

#endif // PROJECTWIDGET_H
