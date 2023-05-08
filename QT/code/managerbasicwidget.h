#ifndef MANAGERBASICWIDGET_H
#define MANAGERBASICWIDGET_H
#include <QWidget>
#include <QtSql>
#include <datastructure.h>
#include <QPushButton>
#include <QTabWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QLabel>


class DataTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    explicit DataTableWidget(QWidget *parent = nullptr, QSqlDatabase* db = nullptr, AccountInformation* ai = nullptr);
private:
    /********接口*********/
    QSqlDatabase* database;
    AccountInformation* accountInformation;
public slots:
    void slotsUpdateData(int);    //更新数据
};



class BasicTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit BasicTabWidget(QWidget *parent = nullptr, QSqlDatabase* db = nullptr, AccountInformation* ai = nullptr);
private:
    /**********数据接口**********/
    QSqlDatabase* database;    //数据库接口
    AccountInformation* accountInformation;    //当前用户信息
    /**********变量**********/
    int currentTable;

    /**********函数**********/
    void initWidget();    //初始化控件
    void connectSignalsAndSlots();    //连接信号和槽

public:
    /**********展示数据的表格**********/
    DataTableWidget* accountTableWidget;
    DataTableWidget* studentTableWidget;
    DataTableWidget* classTableWidget;
    DataTableWidget* departmentTableWidget;
    DataTableWidget* teacherTableWidget;
    DataTableWidget* courseTableWidget;
private slots:
    void slotCurrentChaged(int);
};


class ManagerBasicWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ManagerBasicWidget(QWidget *parent = nullptr, QSqlDatabase* db = nullptr, AccountInformation* ai = nullptr);
private:
    /**********数据接口**********/
    QSqlDatabase* database;    //数据库接口
    AccountInformation* accountInformation;    //当前用户信息
    int currentTable;

    /**********控件**********/
    QLabel* titleLabel;    //顶部控件
    QPushButton* backButton;

    BasicTabWidget* basicTabWidget;    //数据展示窗口

    QLineEdit* dataLineEdit1;    //底部第一行控件
    QLineEdit* dataLineEdit2;
    QLineEdit* dataLineEdit3;
    QLineEdit* dataLineEdit4;
    QLineEdit* dataLineEdit5;
    QLineEdit* dataLineEdit6;
    QLineEdit* dataLineEdit7;
    QPushButton* addButton;    //底部第二行控件
    QPushButton* updateButton;
    QPushButton* deleteButton;
    QPushButton* clearButton;
    QPushButton* bulkImportButton;
    QPushButton* bulkExportButton;

    QLabel* sqlHintLabel;    //底部第三行控件
    QLineEdit* sqlLineEdit;
    QPushButton* executeSqlButton;


    /**********函数**********/
    void initWidget();    //初始化控件
    void connectSignalsAndSlots();    //连接信号和槽
    void updataTableWidget();    //更新数据显示
signals:
    void switchTheWidget(int);    //发给上层的信号，用于QStackWidget页面切换

private slots:
    void slotBackButtonClicked();    //按钮响应函数
    void slotAddButtonClicked();
    void slotUpdateButtonClicked();
    void slotDeleteButtonClicked();
    void slotBulkImportButtonClicked();
    void slotBulkExportButtonClicked();
    void slotExecuteSqlButtonClicked();
    void slotClearButtonClicked();

    void slotAccountTableWidgetClicked(int);     //table点击响应函数
    void slotStudentTableWidgetClicked(int);
    void slotClassTableWidgetClicked(int);
    void slotDepartmentTableWidgetClicked(int);
    void slotTeacherTableWidgetClicked(int);
    void slotCourseTableWidgetClicked(int);

    void slotTableChaged(int);


};




#endif // MANAGERBASICWIDGET_H
