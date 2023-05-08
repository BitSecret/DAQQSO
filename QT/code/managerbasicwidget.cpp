#include "managerbasicwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabBar>
#include <QHeaderView>
#include <QSqlQuery>
#include <QMessageBox>
#include <QFileDialog>

ManagerBasicWidget::ManagerBasicWidget(QWidget *parent, QSqlDatabase* db, AccountInformation* ai) : QWidget(parent)
{
    database = db;    //初始化接口
    accountInformation = ai;

    initWidget();    //初始化控件
    connectSignalsAndSlots();    //连接信号和槽
}

void ManagerBasicWidget::initWidget()    //初始化控件
{
   titleLabel = new QLabel("基本表");
   titleLabel->setFixedHeight(30);
   titleLabel->setFrameStyle(QLabel::StyledPanel);
   titleLabel->setAlignment(Qt::AlignCenter);
   QFont font;
   font.setPointSize(16);
   titleLabel->setFont(font);

   backButton = new QPushButton("返回主页面>");
   backButton->setFixedSize(80, 30);

   basicTabWidget = new BasicTabWidget(this, database, accountInformation);    //数据展示窗口

   dataLineEdit1 = new QLineEdit;    //底部第一行控件
   dataLineEdit2 = new QLineEdit;
   dataLineEdit3 = new QLineEdit;
   dataLineEdit4 = new QLineEdit;
   dataLineEdit5 = new QLineEdit;
   dataLineEdit6 = new QLineEdit;
   dataLineEdit7 = new QLineEdit;
   dataLineEdit1->setFixedHeight(30);
   dataLineEdit2->setFixedHeight(30);
   dataLineEdit3->setFixedHeight(30);
   dataLineEdit4->setFixedHeight(30);
   dataLineEdit5->setFixedHeight(30);
   dataLineEdit6->setFixedHeight(30);
   dataLineEdit7->setFixedHeight(30);

   QLabel* layoutLabel1 = new QLabel;    //布局用，用于对齐表格
   layoutLabel1->setFixedWidth(28);
   QLabel* layoutLabel2 = new QLabel;    //布局用，用于对齐表格
   layoutLabel2->setFixedWidth(19);
   addButton = new QPushButton("添加");    //底部第二行控件
   addButton->setFixedSize(120, 40);
   updateButton = new QPushButton("修改");
   updateButton->setFixedSize(120, 40);
   deleteButton = new QPushButton("删除");
   deleteButton->setFixedSize(120, 40);
   clearButton = new QPushButton("清空输入");
   clearButton->setFixedSize(120, 40);
   bulkImportButton = new QPushButton("批量导入");
   bulkImportButton->setFixedSize(120, 40);
   bulkExportButton = new QPushButton("批量导出");
   bulkExportButton->setFixedSize(120, 40);

   sqlHintLabel = new QLabel("SQL语句:");    //底部第三行控件
   sqlHintLabel->setFixedHeight(30);
   sqlLineEdit = new QLineEdit;
   sqlLineEdit->setFixedHeight(30);
   executeSqlButton = new QPushButton("执行");
   executeSqlButton->setFixedHeight(30);

   currentTable = 0;   //初始化当前显示的表
   slotTableChaged(0);

   /**********布局部分**********/
   QHBoxLayout* titleLayout = new QHBoxLayout;
   titleLayout->addWidget(titleLabel);
   titleLayout->addWidget(backButton);

   QHBoxLayout* dataLineEditLayout = new QHBoxLayout;
   dataLineEditLayout->setSpacing(3);
   dataLineEditLayout->addWidget(layoutLabel1);
   dataLineEditLayout->addWidget(dataLineEdit1);
   dataLineEditLayout->addWidget(dataLineEdit2);
   dataLineEditLayout->addWidget(dataLineEdit3);
   dataLineEditLayout->addWidget(dataLineEdit4);
   dataLineEditLayout->addWidget(dataLineEdit5);
   dataLineEditLayout->addWidget(dataLineEdit6);
   dataLineEditLayout->addWidget(dataLineEdit7);
   dataLineEditLayout->addWidget(layoutLabel2);

   QHBoxLayout* buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget(addButton);
   buttonLayout->addWidget(updateButton);
   buttonLayout->addWidget(deleteButton);
   buttonLayout->addWidget(clearButton);
   buttonLayout->addStretch(0);
   buttonLayout->addWidget(bulkImportButton);
   buttonLayout->addWidget(bulkExportButton);

   QHBoxLayout* sqlLayout = new QHBoxLayout;
   sqlLayout->addWidget(sqlHintLabel);
   sqlLayout->addWidget(sqlLineEdit);
   sqlLayout->addWidget(executeSqlButton);

   QVBoxLayout* mainLayout = new QVBoxLayout(this);
   mainLayout->setMargin(0);    //设置页边距
   mainLayout->setSpacing(4);    //设置控件间距
   mainLayout->addLayout(titleLayout);
   mainLayout->addWidget(basicTabWidget);
   mainLayout->addLayout(dataLineEditLayout);
   mainLayout->addLayout(buttonLayout);
   mainLayout->addLayout(sqlLayout);
}

void ManagerBasicWidget::connectSignalsAndSlots()    //连接信号和槽
{
    connect(basicTabWidget, &QTabWidget::currentChanged, this, &ManagerBasicWidget::slotTableChaged);

    connect(backButton, &QPushButton::clicked, this, &ManagerBasicWidget::slotBackButtonClicked);
    connect(addButton, &QPushButton::clicked, this, &ManagerBasicWidget::slotAddButtonClicked);
    connect(updateButton, &QPushButton::clicked, this, &ManagerBasicWidget::slotUpdateButtonClicked);
    connect(deleteButton, &QPushButton::clicked, this, &ManagerBasicWidget::slotDeleteButtonClicked);
    connect(clearButton, &QPushButton::clicked, this, &ManagerBasicWidget::slotClearButtonClicked);
    connect(bulkImportButton, &QPushButton::clicked, this, &ManagerBasicWidget::slotBulkImportButtonClicked);
    connect(bulkExportButton, &QPushButton::clicked, this, &ManagerBasicWidget::slotBulkExportButtonClicked);
    connect(executeSqlButton, &QPushButton::clicked, this, &ManagerBasicWidget::slotExecuteSqlButtonClicked);

    connect(basicTabWidget->accountTableWidget, &DataTableWidget::cellClicked, this, &ManagerBasicWidget::slotAccountTableWidgetClicked);
    connect(basicTabWidget->studentTableWidget, &DataTableWidget::cellClicked, this, &ManagerBasicWidget::slotStudentTableWidgetClicked);
    connect(basicTabWidget->classTableWidget, &DataTableWidget::cellClicked, this, &ManagerBasicWidget::slotClassTableWidgetClicked);
    connect(basicTabWidget->departmentTableWidget, &DataTableWidget::cellClicked, this, &ManagerBasicWidget::slotDepartmentTableWidgetClicked);
    connect(basicTabWidget->teacherTableWidget, &DataTableWidget::cellClicked, this, &ManagerBasicWidget::slotTeacherTableWidgetClicked);
    connect(basicTabWidget->courseTableWidget, &DataTableWidget::cellClicked, this, &ManagerBasicWidget::slotCourseTableWidgetClicked);
}

void ManagerBasicWidget::updataTableWidget()
{
    basicTabWidget->accountTableWidget->slotsUpdateData(0);
    basicTabWidget->studentTableWidget->slotsUpdateData(1);
    basicTabWidget->classTableWidget->slotsUpdateData(2);
    basicTabWidget->departmentTableWidget->slotsUpdateData(3);
    basicTabWidget->teacherTableWidget->slotsUpdateData(4);
    basicTabWidget->courseTableWidget->slotsUpdateData(5);
}

/***表切换的响应函数***/
void ManagerBasicWidget::slotTableChaged(int nextTable){
    currentTable = nextTable;

    slotClearButtonClicked();

    if(currentTable == 0){
        dataLineEdit1->setEnabled(true);
        dataLineEdit2->setEnabled(true);
        dataLineEdit3->setEnabled(true);
        dataLineEdit4->setEnabled(false);
        dataLineEdit5->setEnabled(false);
        dataLineEdit6->setEnabled(false);
        dataLineEdit7->setEnabled(false);
    }
    else if(currentTable == 1){
        dataLineEdit1->setEnabled(true);
        dataLineEdit2->setEnabled(true);
        dataLineEdit3->setEnabled(true);
        dataLineEdit4->setEnabled(true);
        dataLineEdit5->setEnabled(true);
        dataLineEdit6->setEnabled(true);
        dataLineEdit7->setEnabled(true);
    }
    else if(currentTable == 2){
        dataLineEdit1->setEnabled(true);
        dataLineEdit2->setEnabled(true);
        dataLineEdit3->setEnabled(true);
        dataLineEdit4->setEnabled(true);
        dataLineEdit5->setEnabled(false);
        dataLineEdit6->setEnabled(false);
        dataLineEdit7->setEnabled(false);
    }
    else if(currentTable == 3){
        dataLineEdit1->setEnabled(true);
        dataLineEdit2->setEnabled(true);
        dataLineEdit3->setEnabled(true);
        dataLineEdit4->setEnabled(false);
        dataLineEdit5->setEnabled(false);
        dataLineEdit6->setEnabled(false);
        dataLineEdit7->setEnabled(false);
    }
    else if(currentTable == 4){
        dataLineEdit1->setEnabled(true);
        dataLineEdit2->setEnabled(true);
        dataLineEdit3->setEnabled(true);
        dataLineEdit4->setEnabled(true);
        dataLineEdit5->setEnabled(true);
        dataLineEdit6->setEnabled(true);
        dataLineEdit7->setEnabled(false);
    }
    else{
        dataLineEdit1->setEnabled(true);
        dataLineEdit2->setEnabled(true);
        dataLineEdit3->setEnabled(true);
        dataLineEdit4->setEnabled(false);
        dataLineEdit5->setEnabled(false);
        dataLineEdit6->setEnabled(false);
        dataLineEdit7->setEnabled(false);
    }
}


/***按钮响应函数部分***/
void ManagerBasicWidget::slotBackButtonClicked()
{
    emit switchTheWidget(0);
}
void ManagerBasicWidget::slotAddButtonClicked()
{
    QString data1 = dataLineEdit1->text();
    QString data2 = dataLineEdit2->text();
    QString data3 = dataLineEdit3->text();
    QString data4 = dataLineEdit4->text();
    QString data5 = dataLineEdit5->text();
    QString data6 = dataLineEdit6->text();
    QString data7 = dataLineEdit7->text();
    QString sql;
    if(currentTable == 0){
        if(!data1.isEmpty()&&!data2.isEmpty()&&!data3.isEmpty()){
            sql = QString("insert into account values('%1', '%2', %3)").arg(data1).arg(data2).arg(data3);
        }
        else {
            sql = "";
        }
    }
    else if(currentTable == 1){
        if(!data1.isEmpty()&&!data2.isEmpty()&&!data3.isEmpty()&&!data4.isEmpty()&&!data5.isEmpty()&&!data6.isEmpty()&&!data7.isEmpty()){
            sql = QString("insert into student values('%1', '%2', '%3', '%4', '%5', '%6', '%7')").arg(data1).arg(data2).arg(data3).arg(data4).arg(data5).arg(data6).arg(data7);
        }
        else {
            sql = "";
        }
    }
    else if(currentTable == 2){
        if(!data1.isEmpty()&&!data2.isEmpty()&&!data3.isEmpty()&&!data4.isEmpty()){
            sql = QString("insert into class values('%1', '%2', '%3', '%4')").arg(data1).arg(data2).arg(data3).arg(data4);
        }
        else {
            sql = "";
        }
    }
    else if(currentTable == 3){
        if(!data1.isEmpty()&&!data2.isEmpty()&&!data3.isEmpty()){
            sql = QString("insert into department values('%1', '%2', '%3')").arg(data1).arg(data2).arg(data3);
        }
        else {
            sql = "";
        }
    }
    else if(currentTable == 4){
        if(!data1.isEmpty()&&!data2.isEmpty()&&!data3.isEmpty()&&!data4.isEmpty()&&!data5.isEmpty()&&!data6.isEmpty()){
            sql = QString("insert into teacher values('%1', '%2', '%3', '%4', '%5', '%6')").arg(data1).arg(data2).arg(data3).arg(data4).arg(data5).arg(data6);
        }
        else {
            sql = "";
        }
    }
    else{
        if(!data1.isEmpty()&&!data2.isEmpty()&&!data3.isEmpty()){
            sql = QString("insert into course values('%1', '%2', '%3')").arg(data1).arg(data2).arg(data3);
        }
        else {
            sql = "";
        }
    }
    if(!sql.isEmpty()){
        QSqlQuery query(*database);
        if(query.exec(sql)){
            QMessageBox::information(this, "提示", "添加成功！", QMessageBox::Ok);
            updataTableWidget();
        }
        else {
            QMessageBox::warning(this, "添加失败", "添加信息失败，可能原因：与数据库连接已断开或记录已存在！", QMessageBox::Ok);
        }
    }
    else{
        QMessageBox::warning(this, "添加失败", "添加信息失败，请输确保所有信息全部输入！", QMessageBox::Ok);
    }

}
void ManagerBasicWidget::slotUpdateButtonClicked(){
    QString data1= dataLineEdit1->text();
    QString sql;

    if(!data1.isEmpty()){
        if(currentTable == 0){
            sql = QString("delete from account where account_number='%1'").arg(data1);
        }
        else if(currentTable == 1){
            sql = QString("delete from student where student_number='%1'").arg(data1);
        }
        else if(currentTable == 2){
            sql = QString("delete from class where class_number='%1'").arg(data1);
        }
        else if(currentTable == 3){
            sql = QString("delete from department where department_number='%1'").arg(data1);
        }
        else if(currentTable == 4){
            sql = QString("delete from teacher where teacher_number='%1'").arg(data1);
        }
        else{
            sql = QString("delete from course where course_number='%1'").arg(data1);
        }
        QSqlQuery query(*database);
        query.exec(sql);

        QString data2 = dataLineEdit2->text();
        QString data3 = dataLineEdit3->text();
        QString data4 = dataLineEdit4->text();
        QString data5 = dataLineEdit5->text();
        QString data6 = dataLineEdit6->text();
        QString data7 = dataLineEdit7->text();
        QString sql;
        if(currentTable == 0){
            if(!data1.isEmpty()&&!data2.isEmpty()&&!data3.isEmpty()){
                sql = QString("insert into account values('%1', '%2', %3)").arg(data1).arg(data2).arg(data3);
            }
            else {
                sql = "";
            }
        }
        else if(currentTable == 1){
            if(!data1.isEmpty()&&!data2.isEmpty()&&!data3.isEmpty()&&!data4.isEmpty()&&!data5.isEmpty()&&!data6.isEmpty()&&!data7.isEmpty()){
                sql = QString("insert into student values('%1', '%2', '%3', '%4', '%5', '%6', '%7')").arg(data1).arg(data2).arg(data3).arg(data4).arg(data5).arg(data6).arg(data7);
            }
            else {
                sql = "";
            }
        }
        else if(currentTable == 2){
            if(!data1.isEmpty()&&!data2.isEmpty()&&!data3.isEmpty()&&!data4.isEmpty()){
                sql = QString("insert into class values('%1', '%2', '%3', '%4')").arg(data1).arg(data2).arg(data3).arg(data4);
            }
            else {
                sql = "";
            }
        }
        else if(currentTable == 3){
            if(!data1.isEmpty()&&!data2.isEmpty()&&!data3.isEmpty()){
                sql = QString("insert into department values('%1', '%2', '%3')").arg(data1).arg(data2).arg(data3);
            }
            else {
                sql = "";
            }
        }
        else if(currentTable == 4){
            if(!data1.isEmpty()&&!data2.isEmpty()&&!data3.isEmpty()&&!data4.isEmpty()&&!data5.isEmpty()&&!data6.isEmpty()){
                sql = QString("insert into teacher values('%1', '%2', '%3', '%4', '%5', '%6')").arg(data1).arg(data2).arg(data3).arg(data4).arg(data5).arg(data6);
            }
            else {
                sql = "";
            }
        }
        else{
            if(!data1.isEmpty()&&!data2.isEmpty()&&!data3.isEmpty()){
                sql = QString("insert into course values('%1', '%2', '%3')").arg(data1).arg(data2).arg(data3);
            }
            else {
                sql = "";
            }
        }
        if(!sql.isEmpty()){
            QSqlQuery query(*database);
            if(query.exec(sql)){
                QMessageBox::information(this, "提示", "更新成功！", QMessageBox::Ok);
                updataTableWidget();
            }
            else {
                QMessageBox::warning(this, "更新失败", "更新信息失败，可能原因：与数据库连接已断开！", QMessageBox::Ok);
            }
        }
        else{
            QMessageBox::warning(this, "更新失败", "更新信息失败，请输确保所有信息全部输入！", QMessageBox::Ok);
        }
    }
    else{
        QMessageBox::warning(this, "更新失败", "请输入键值!", QMessageBox::Ok);
    }
}
void ManagerBasicWidget::slotDeleteButtonClicked(){
    QString dataKey= dataLineEdit1->text();
    QString sql;

    if(!dataKey.isEmpty()){
        if(currentTable == 0){
            sql = QString("delete from account where account_number='%1'").arg(dataKey);
        }
        else if(currentTable == 1){
            sql = QString("delete from student where student_number='%1'").arg(dataKey);
        }
        else if(currentTable == 2){
            sql = QString("delete from class where class_number='%1'").arg(dataKey);
        }
        else if(currentTable == 3){
            sql = QString("delete from department where department_number='%1'").arg(dataKey);
        }
        else if(currentTable == 4){
            sql = QString("delete from teacher where teacher_number='%1'").arg(dataKey);
        }
        else{
            sql = QString("delete from course where course_number='%1'").arg(dataKey);
        }
        QSqlQuery query(*database);
        if(query.exec(sql)){
            QMessageBox::information(this, "提示", "成功删除！", QMessageBox::Ok);
            updataTableWidget();
        }
        else {
            QMessageBox::warning(this, "删除失败", "请确认键值输入是否正确！", QMessageBox::Ok);
        }
    }
    else{
        QMessageBox::warning(this, "删除失败", "请输入键值!", QMessageBox::Ok);
    }
}
void ManagerBasicWidget::slotClearButtonClicked(){
    dataLineEdit1->clear();
    dataLineEdit2->clear();
    dataLineEdit3->clear();
    dataLineEdit4->clear();
    dataLineEdit5->clear();
    dataLineEdit6->clear();
    dataLineEdit7->clear();
}
void ManagerBasicWidget::slotBulkImportButtonClicked(){
    if(currentTable == 0){        //account
        int successCount = 0;
        int failCount = 0;
        QString dataFilePath = QFileDialog::getOpenFileName(this, "打开", "C:/Users/Admin/Desktop", "files (*.TSData)");
        if(!dataFilePath.isEmpty())    //判断用户是否选择了文件
        {
            QFile file(dataFilePath);
            file.open(QIODevice::ReadOnly);   //只读
            QTextStream in(&file);
            in.setCodec("UTF-8");    //防止中文乱码

            QString countString;    //获得记录个数
            in >> countString;
            in.setCodec("UTF-8");    //防止中文乱码
            int countInt = countString.toInt();

            QSqlQuery query(*database);    //查询接口
            QString sql;

            for(int i = 0; i < countInt; i++)
            {
                QString data1;    //数据
                QString data2;
                QString data3;
                in >> data1;
                in >> data2;
                in >> data3;
                sql = QString("insert into account values('%1', '%2', %3)").arg(data1).arg(data2).arg(data3);
                if(query.exec(sql)){
                    successCount++;
                }
                else {
                    failCount++;
                }
            }
            file.close();

            QMessageBox::information(this, "提示", QString("导入数据%1条，成功%2条，失败%3条").arg(countInt).arg(successCount).arg(failCount), QMessageBox::Ok);
            updataTableWidget();
        }
    }
    else if(currentTable == 1){
        int successCount = 0;
        int failCount = 0;
        QString dataFilePath = QFileDialog::getOpenFileName(this, "打开", "C:/Users/Admin/Desktop", "files (*.TSData)");
        if(!dataFilePath.isEmpty())    //判断用户是否选择了文件
        {
            QFile file(dataFilePath);
            file.open(QIODevice::ReadOnly);   //只读
            QTextStream in(&file);
            in.setCodec("UTF-8");    //防止中文乱码

            QString countString;    //获得记录个数
            in >> countString;
            int countInt = countString.toInt();

            QSqlQuery query(*database);    //查询接口
            QString sql;

            for(int i = 0; i < countInt; i++)
            {
                QString data1;    //数据
                QString data2;
                QString data3;
                QString data4;
                QString data5;
                QString data6;
                QString data7;
                in >> data1;
                in >> data2;
                in >> data3;
                in >> data4;
                in >> data5;
                in >> data6;
                in >> data7;
                sql = QString("insert into student values('%1', '%2', '%3', '%4', '%5', '%6', '%7')").arg(data1).arg(data2).arg(data3).arg(data4).arg(data5).arg(data6).arg(data7);
                if(query.exec(sql)){
                    successCount++;
                }
                else {
                    failCount++;
                }
            }
            file.close();

            QMessageBox::information(this, "提示", QString("导入数据%1条，成功%2条，失败%3条").arg(countInt).arg(successCount).arg(failCount), QMessageBox::Ok);
            updataTableWidget();
        }
    }
    else if(currentTable == 2){
        int successCount = 0;
        int failCount = 0;
        QString dataFilePath = QFileDialog::getOpenFileName(this, "打开", "C:/Users/Admin/Desktop", "files (*.TSData)");
        if(!dataFilePath.isEmpty())    //判断用户是否选择了文件
        {
            QFile file(dataFilePath);
            file.open(QIODevice::ReadOnly);   //只读
            QTextStream in(&file);

            QString countString;    //获得记录个数
            in >> countString;
            in.setCodec("UTF-8");    //防止中文乱码
            int countInt = countString.toInt();

            QSqlQuery query(*database);    //查询接口
            QString sql;

            for(int i = 0; i < countInt; i++)
            {
                QString data1;    //数据
                QString data2;
                QString data3;
                QString data4;
                in >> data1;
                in >> data2;
                in >> data3;
                in >> data4;
                sql = QString("insert into class values('%1', '%2', '%3', '%4')").arg(data1).arg(data2).arg(data3).arg(data4);
                if(query.exec(sql)){
                    successCount++;
                }
                else {
                    failCount++;
                }
            }
            file.close();

            QMessageBox::information(this, "提示", QString("导入数据%1条，成功%2条，失败%3条").arg(countInt).arg(successCount).arg(failCount), QMessageBox::Ok);
            updataTableWidget();
        }
    }
    else if(currentTable == 3){
        int successCount = 0;
        int failCount = 0;
        QString dataFilePath = QFileDialog::getOpenFileName(this, "打开", "C:/Users/Admin/Desktop", "files (*.TSData)");
        if(!dataFilePath.isEmpty())    //判断用户是否选择了文件
        {
            QFile file(dataFilePath);
            file.open(QIODevice::ReadOnly);   //只读
            QTextStream in(&file);

            QString countString;    //获得记录个数
            in >> countString;
            in.setCodec("UTF-8");    //防止中文乱码
            int countInt = countString.toInt();

            QSqlQuery query(*database);    //查询接口
            QString sql;

            for(int i = 0; i < countInt; i++)
            {
                QString data1;    //数据
                QString data2;
                QString data3;
                in >> data1;
                in >> data2;
                in >> data3;
                sql = QString("insert into department values('%1', '%2', '%3')").arg(data1).arg(data2).arg(data3);
                if(query.exec(sql)){
                    successCount++;
                }
                else {
                    failCount++;
                }
            }
            file.close();

            QMessageBox::information(this, "提示", QString("导入数据%1条，成功%2条，失败%3条").arg(countInt).arg(successCount).arg(failCount), QMessageBox::Ok);
            updataTableWidget();
        }
    }
    else if(currentTable == 4){
        int successCount = 0;
        int failCount = 0;
        QString dataFilePath = QFileDialog::getOpenFileName(this, "打开", "C:/Users/Admin/Desktop", "files (*.TSData)");
        if(!dataFilePath.isEmpty())    //判断用户是否选择了文件
        {
            QFile file(dataFilePath);
            file.open(QIODevice::ReadOnly);   //只读
            QTextStream in(&file);

            QString countString;    //获得记录个数
            in >> countString;
            in.setCodec("UTF-8");    //防止中文乱码
            int countInt = countString.toInt();

            QSqlQuery query(*database);    //查询接口
            QString sql;

            for(int i = 0; i < countInt; i++)
            {
                QString data1;    //数据
                QString data2;
                QString data3;
                QString data4;
                QString data5;
                QString data6;
                in >> data1;
                in >> data2;
                in >> data3;
                in >> data4;
                in >> data5;
                in >> data6;
                sql = QString("insert into teacher values('%1', '%2', '%3', '%4', '%5', '%6')").arg(data1).arg(data2).arg(data3).arg(data4).arg(data5).arg(data6);
                if(query.exec(sql)){
                    successCount++;
                }
                else {
                    failCount++;
                }
            }
            file.close();

            QMessageBox::information(this, "提示", QString("导入数据%1条，成功%2条，失败%3条").arg(countInt).arg(successCount).arg(failCount), QMessageBox::Ok);
            updataTableWidget();
        }
    }
    else{
        int successCount = 0;
        int failCount = 0;
        QString dataFilePath = QFileDialog::getOpenFileName(this, "打开", "C:/Users/Admin/Desktop", "files (*.TSData)");
        if(!dataFilePath.isEmpty())    //判断用户是否选择了文件
        {
            QFile file(dataFilePath);
            file.open(QIODevice::ReadOnly);   //只读
            QTextStream in(&file);
            in.setCodec("UTF-8");    //防止中文乱码

            QString countString;    //获得记录个数
            in >> countString;
            int countInt = countString.toInt();

            QSqlQuery query(*database);    //查询接口
            QString sql;

            for(int i = 0; i < countInt; i++)
            {
                QString data1;    //数据
                QString data2;
                QString data3;
                QString data4;
                QString data5;
                QString data6;
                QString data7;
                in >> data1;
                in >> data2;
                in >> data3;
                sql = QString("insert into course values('%1', '%2', '%3')").arg(data1).arg(data2).arg(data3);
                if(query.exec(sql)){
                    successCount++;
                }
                else {
                    failCount++;
                }
            }
            file.close();

            QMessageBox::information(this, "提示", QString("导入数据%1条，成功%2条，失败%3条").arg(countInt).arg(successCount).arg(failCount), QMessageBox::Ok);
            updataTableWidget();
        }
    }

}
void ManagerBasicWidget::slotBulkExportButtonClicked(){
    if(currentTable == 0){        //account
        QString dataFilePath = QFileDialog::getSaveFileName(this, "打开", "C:/Users/Admin/Desktop", "files (*.TSData)");
        if(!dataFilePath.isEmpty())    //判断用户是否选择了文件
        {
            QFile file(dataFilePath);
            file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);   //只读、覆盖原数据、写入文本
            QTextStream out(&file);
            out.setCodec("UTF-8");    //防止中文乱码

            QSqlQuery query(*database);
            QString sql = "select count(*) from account";
            query.exec(sql);

            if(query.next()){
                out << query.value(0).toString() << "\n";
                sql = "select * from account";
                query.exec(sql);
                while(query.next()){
                    out << query.value(0).toString() << " ";
                    out << query.value(1).toString() << " ";
                    out << query.value(2).toString() << "\n";
                }
            }
            file.close();

            QMessageBox::information(this, "提示", "导出成功!", QMessageBox::Ok);
        }
    }
    else if(currentTable == 1){
        QString dataFilePath = QFileDialog::getSaveFileName(this, "打开", "C:/Users/Admin/Desktop", "files (*.TSData)");
        if(!dataFilePath.isEmpty())    //判断用户是否选择了文件
        {
            QFile file(dataFilePath);
            file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);   //只读、覆盖原数据、写入文本
            QTextStream out(&file);
            out.setCodec("UTF-8");    //防止中文乱码

            QSqlQuery query(*database);
            QString sql = "select count(*) from student";
            query.exec(sql);

            if(query.next()){
                out << query.value(0).toString() << "\n";
                sql = "select * from student";
                query.exec(sql);
                while(query.next()){
                    out << query.value(0).toString() << " ";
                    out << query.value(1).toString() << " ";
                    out << query.value(2).toString() << " ";
                    out << query.value(3).toString() << " ";
                    out << query.value(4).toString() << " ";
                    out << query.value(5).toString() << " ";
                    out << query.value(6).toString() << "\n";
                }
            }
            file.close();

            QMessageBox::information(this, "提示", "导出成功!", QMessageBox::Ok);
        }
    }
    else if(currentTable == 2){
        QString dataFilePath = QFileDialog::getSaveFileName(this, "打开", "C:/Users/Admin/Desktop", "files (*.TSData)");
        if(!dataFilePath.isEmpty())    //判断用户是否选择了文件
        {
            QFile file(dataFilePath);
            file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);   //只读、覆盖原数据、写入文本
            QTextStream out(&file);
            out.setCodec("UTF-8");    //防止中文乱码

            QSqlQuery query(*database);
            QString sql = "select count(*) from class";
            query.exec(sql);

            if(query.next()){
                out << query.value(0).toString() << "\n";
                sql = "select * from class";
                query.exec(sql);
                while(query.next()){
                    out << query.value(0).toString() << " ";
                    out << query.value(1).toString() << " ";
                    out << query.value(2).toString() << " ";
                    out << query.value(3).toString() << "\n";
                }
            }
            file.close();

            QMessageBox::information(this, "提示", "导出成功!", QMessageBox::Ok);
         }
    }
    else if(currentTable == 3){
        QString dataFilePath = QFileDialog::getSaveFileName(this, "打开", "C:/Users/Admin/Desktop", "files (*.TSData)");
        if(!dataFilePath.isEmpty()){    //判断用户是否选择了文件
            QFile file(dataFilePath);
            file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);   //只读、覆盖原数据、写入文本
            QTextStream out(&file);
            out.setCodec("UTF-8");    //防止中文乱码

            QSqlQuery query(*database);
            QString sql = "select count(*) from department";
            query.exec(sql);

            if(query.next()){
                out << query.value(0).toString() << "\n";
                sql = "select * from department";
                query.exec(sql);
                while(query.next()){
                    out << query.value(0).toString() << " ";
                    out << query.value(1).toString() << " ";
                    out << query.value(2).toString() << "\n";
                }
            }
            file.close();

            QMessageBox::information(this, "提示", "导出成功!", QMessageBox::Ok);
        }
    }
    else if(currentTable == 4){
        QString dataFilePath = QFileDialog::getSaveFileName(this, "打开", "C:/Users/Admin/Desktop", "files (*.TSData)");
        if(!dataFilePath.isEmpty()){    //判断用户是否选择了文件
            QFile file(dataFilePath);
            file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);   //只读、覆盖原数据、写入文本
            QTextStream out(&file);
            out.setCodec("UTF-8");    //防止中文乱码

            QSqlQuery query(*database);
            QString sql = "select count(*) from teacher";
            query.exec(sql);

            if(query.next()){
                out << query.value(0).toString() << "\n";
                sql = "select * from teacher";
                query.exec(sql);
                while(query.next()){
                    out << query.value(0).toString() << " ";
                    out << query.value(1).toString() << " ";
                    out << query.value(2).toString() << " ";
                    out << query.value(3).toString() << " ";
                    out << query.value(4).toString() << " ";
                    out << query.value(5).toString() << "\n";
                }
            }
            file.close();

            QMessageBox::information(this, "提示", "导出成功!", QMessageBox::Ok);
        }
    }
    else{
        QString dataFilePath = QFileDialog::getSaveFileName(this, "打开", "C:/Users/Admin/Desktop", "files (*.TSData)");
        if(!dataFilePath.isEmpty()){    //判断用户是否选择了文件
            QFile file(dataFilePath);
            file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);   //只读、覆盖原数据、写入文本
            QTextStream out(&file);
            out.setCodec("UTF-8");    //防止中文乱码

            QSqlQuery query(*database);
            QString sql = "select count(*) from course";
            query.exec(sql);

            if(query.next()){
                out << query.value(0).toString() << "\n";
                sql = "select * from course";
                query.exec(sql);
                while(query.next()){
                    out << query.value(0).toString() << " ";
                    out << query.value(1).toString() << " ";
                    out << query.value(2).toString() << "\n";
                }
            }
            file.close();

            QMessageBox::information(this, "提示", "导出成功!", QMessageBox::Ok);
        }
    }
}
void ManagerBasicWidget::slotExecuteSqlButtonClicked(){
    QString sql = sqlLineEdit->text();
    if(!sql.isEmpty()){
        QSqlQuery query(*database);
        if(query.exec(sql)){
            QMessageBox::information(this, "提示", "成功执行！", QMessageBox::Ok);
            updataTableWidget();
        }
        else {
            QMessageBox::warning(this, "执行失败", "请正确输入sql语句！", QMessageBox::Ok);
        }
    }
    else{
        QMessageBox::warning(this, "执行失败", "请输入sql语句！", QMessageBox::Ok);
    }
}


/***单元格点击事件响应函数***/
void ManagerBasicWidget::slotAccountTableWidgetClicked(int row)
{
    if(basicTabWidget->accountTableWidget->item(row, 0) != nullptr){
        QString data1 = basicTabWidget->accountTableWidget->item(row, 0)->text();    //获得单元格数据
        QString data2 = basicTabWidget->accountTableWidget->item(row, 1)->text();    //获得单元格数据
        QString data3 = basicTabWidget->accountTableWidget->item(row, 2)->text();    //获得单元格数据

        dataLineEdit1->setText(data1);
        dataLineEdit2->setText(data2);
        dataLineEdit3->setText(data3);
    }
}
void ManagerBasicWidget::slotStudentTableWidgetClicked(int row)
{
    if(basicTabWidget->studentTableWidget->item(row, 0) != nullptr){
        QString data1 = basicTabWidget->studentTableWidget->item(row, 0)->text();    //获得单元格数据
        QString data2 = basicTabWidget->studentTableWidget->item(row, 1)->text();    //获得单元格数据
        QString data3 = basicTabWidget->studentTableWidget->item(row, 2)->text();    //获得单元格数据
        QString data4 = basicTabWidget->studentTableWidget->item(row, 3)->text();    //获得单元格数据
        QString data5 = basicTabWidget->studentTableWidget->item(row, 4)->text();    //获得单元格数据
        QString data6 = basicTabWidget->studentTableWidget->item(row, 5)->text();    //获得单元格数据
        QString data7 = basicTabWidget->studentTableWidget->item(row, 6)->text();    //获得单元格数据

        dataLineEdit1->setText(data1);
        dataLineEdit2->setText(data2);
        dataLineEdit3->setText(data3);
        dataLineEdit4->setText(data4);
        dataLineEdit5->setText(data5);
        dataLineEdit6->setText(data6);
        dataLineEdit7->setText(data7);
    }
}
void ManagerBasicWidget::slotClassTableWidgetClicked(int row)
{
    if(basicTabWidget->classTableWidget->item(row, 0) != nullptr){
        QString data1 = basicTabWidget->classTableWidget->item(row, 0)->text();    //获得单元格数据
        QString data2 = basicTabWidget->classTableWidget->item(row, 1)->text();    //获得单元格数据
        QString data3 = basicTabWidget->classTableWidget->item(row, 2)->text();    //获得单元格数据
        QString data4 = basicTabWidget->classTableWidget->item(row, 3)->text();    //获得单元格数据

        dataLineEdit1->setText(data1);
        dataLineEdit2->setText(data2);
        dataLineEdit3->setText(data3);
        dataLineEdit4->setText(data4);
    }
}
void ManagerBasicWidget::slotDepartmentTableWidgetClicked(int row)
{
    if(basicTabWidget->departmentTableWidget->item(row, 0) != nullptr){
        QString data1 = basicTabWidget->departmentTableWidget->item(row, 0)->text();    //获得单元格数据
        QString data2 = basicTabWidget->departmentTableWidget->item(row, 1)->text();    //获得单元格数据
        QString data3 = basicTabWidget->departmentTableWidget->item(row, 2)->text();    //获得单元格数据

        dataLineEdit1->setText(data1);
        dataLineEdit2->setText(data2);
        dataLineEdit3->setText(data3);
    }
}
void ManagerBasicWidget::slotTeacherTableWidgetClicked(int row)
{
    if(basicTabWidget->teacherTableWidget->item(row, 0) != nullptr){
        QString data1 = basicTabWidget->teacherTableWidget->item(row, 0)->text();    //获得单元格数据
        QString data2 = basicTabWidget->teacherTableWidget->item(row, 1)->text();    //获得单元格数据
        QString data3 = basicTabWidget->teacherTableWidget->item(row, 2)->text();    //获得单元格数据
        QString data4 = basicTabWidget->teacherTableWidget->item(row, 3)->text();    //获得单元格数据
        QString data5 = basicTabWidget->teacherTableWidget->item(row, 4)->text();    //获得单元格数据
        QString data6 = basicTabWidget->teacherTableWidget->item(row, 5)->text();    //获得单元格数据

        dataLineEdit1->setText(data1);
        dataLineEdit2->setText(data2);
        dataLineEdit3->setText(data3);
        dataLineEdit4->setText(data4);
        dataLineEdit5->setText(data5);
        dataLineEdit6->setText(data6);
    }

}
void ManagerBasicWidget::slotCourseTableWidgetClicked(int row)
{
    if(basicTabWidget->courseTableWidget->item(row, 0) != nullptr){
        QString data1 = basicTabWidget->courseTableWidget->item(row, 0)->text();    //获得单元格数据
        QString data2 = basicTabWidget->courseTableWidget->item(row, 1)->text();    //获得单元格数据
        QString data3 = basicTabWidget->courseTableWidget->item(row, 2)->text();    //获得单元格数据

        dataLineEdit1->setText(data1);
        dataLineEdit2->setText(data2);
        dataLineEdit3->setText(data3);
    }
}



/**************BasicTabWidget部分**************/
BasicTabWidget::BasicTabWidget(QWidget *parent, QSqlDatabase* db, AccountInformation* ai):QTabWidget(parent)
{
    database = db;    //初始化接口
    accountInformation = ai;

    initWidget();     //初始化控件
    connectSignalsAndSlots();    //连接信号和槽
}

/**********函数**********/
void BasicTabWidget::initWidget()    //初始化控件
{
    accountTableWidget = new DataTableWidget(this, database, accountInformation);
    studentTableWidget = new DataTableWidget(this, database, accountInformation);
    classTableWidget = new DataTableWidget(this, database, accountInformation);
    departmentTableWidget = new DataTableWidget(this, database, accountInformation);
    teacherTableWidget = new DataTableWidget(this, database, accountInformation);
    courseTableWidget = new DataTableWidget(this, database, accountInformation);
    this->tabBar()->setStyleSheet("QTabBar::tab{min-height: 30px; min-width: 110px; font:15px;}");
    this->addTab(accountTableWidget, "account");
    this->addTab(studentTableWidget, "student");
    this->addTab(classTableWidget, "class");
    this->addTab(departmentTableWidget, "department");
    this->addTab(teacherTableWidget, "teacher");
    this->addTab(courseTableWidget, "course");
    currentTable = 0;
    slotCurrentChaged(currentTable);
}

void BasicTabWidget::connectSignalsAndSlots()    //连接信号和槽
{
    connect(this, &QTabWidget::currentChanged, this, &BasicTabWidget::slotCurrentChaged);
}

/**********槽函数部分**********/
void BasicTabWidget::slotCurrentChaged(int tableIndex)    //表切换响应函数
{
    currentTable = tableIndex;
    switch(currentTable){    //更新数据
        case 0:{
            accountTableWidget->slotsUpdateData(currentTable);
            break;
        }
        case 1:{
            studentTableWidget->slotsUpdateData(currentTable);
            break;
        }
        case 2:{
            classTableWidget->slotsUpdateData(currentTable);
            break;
        }
        case 3:{
            departmentTableWidget->slotsUpdateData(currentTable);
            break;
        }
        case 4:{
            teacherTableWidget->slotsUpdateData(currentTable);
            break;
        }
        default:{
            courseTableWidget->slotsUpdateData(currentTable);
        }
    }
}




/**************DataTableWidget部分**************/
DataTableWidget::DataTableWidget(QWidget *parent, QSqlDatabase* db, AccountInformation* ai):QTableWidget(parent)
{
    database = db;    //初始化接口
    accountInformation = ai;
    this->setRowCount(300);     //设置行数
    this->setColumnCount(7);     //设置列数
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);    //设置内容不可更改
    this->setSelectionBehavior(QAbstractItemView::SelectRows);    //设置整行选中
    this->horizontalHeader()->setDefaultSectionSize(161);
    this->setShowGrid(true);     //设置显示格子线
}

void DataTableWidget::slotsUpdateData(int index)    //更新数据
{
    this->clear();
    //更新表头
    QStringList header;
    if(index == 0){         //account
        header << tr("account_number(varchar)") << tr("account_password(varchar)") << tr("authority(int)") << tr("") << tr("") << tr("") << tr("");
    }
    else if(index == 1){    //student
        header << tr("student_number(varchar)") << tr("class_number(varchar)") << tr("department_number(varchar)") << tr("student_name(varchar)") << tr("sex(varchar)") << tr("grade(varchar)") << tr("phone(varchar)");
    }
    else if(index == 2){    //class
        header << tr("class_number(varchar)") << tr("department_number(varchar)") << tr("class_name(varchar)") << tr("grade(varchar)") << tr("") << tr("") << tr("");
    }
    else if(index == 3){    //department
        header << tr("department_number(varchar)") << tr("department_name(varchar)") << tr("department_type(varchar)") << tr("") << tr("") << tr("") << tr("");
    }
    else if(index == 4){    //teacher
        header << tr("teacher_number(varchar)") << tr("department_number(varchar)") << tr("teacher_name(varchar)") << tr("teacher_title(varchar)") << tr("sex(varchar)") << tr("phone(varchar)") << tr("");
    }
    else{                   //course
        header << tr("course_number(varchar)") << tr("course_name(varchar)") << tr("describe(varchar)") << tr("") << tr("") << tr("") << tr("");
    }
    this->setHorizontalHeaderLabels(header);

    //更新数据
    QSqlQuery query(*database);
    if(index == 0){         //account
        QString sql = "select * from account";
        if(query.exec(sql)){
            int i = 0;
            while(query.next()){
                for(int j = 0; j < 3; j++){
                    QTableWidgetItem* item = new QTableWidgetItem(query.value(j).toString());    //一个单元格
                    item->setTextAlignment(Qt::AlignCenter);
                    this->setItem(i, j, item);  //插入到表格
                }
                i++;
            }
        }
        else{
            QMessageBox::warning(this, "提示", "查询失败，可能原因：数据库连接已断开！", QMessageBox::Ok);
        }
    }
    else if(index == 1){    //student
        QString sql = "select * from student";
        if(query.exec(sql)){
            int i = 0;
            while(query.next()){
                for(int j = 0; j < 7; j++){
                    QTableWidgetItem* item = new QTableWidgetItem(query.value(j).toString());    //一个单元格
                    item->setTextAlignment(Qt::AlignCenter);
                    this->setItem(i, j, item);  //插入到表格
                }
                i++;
            }
        }
        else{
            QMessageBox::warning(this, "提示", "查询失败，可能原因：数据库连接已断开！", QMessageBox::Ok);
        }
    }
    else if(index == 2){    //class
        QString sql = "select * from class";
        if(query.exec(sql)){
            int i = 0;
            while(query.next()){
                for(int j = 0; j < 4; j++){
                    QTableWidgetItem* item = new QTableWidgetItem(query.value(j).toString());    //一个单元格
                    item->setTextAlignment(Qt::AlignCenter);
                    this->setItem(i, j, item);  //插入到表格
                }
                i++;
            }
        }
        else{
            QMessageBox::warning(this, "提示", "查询失败，可能原因：数据库连接已断开！", QMessageBox::Ok);
        }
    }
    else if(index == 3){    //department
        QString sql = "select * from department";
        if(query.exec(sql)){
            int i = 0;
            while(query.next()){
                for(int j = 0; j < 3; j++){
                    QTableWidgetItem* item = new QTableWidgetItem(query.value(j).toString());    //一个单元格
                    item->setTextAlignment(Qt::AlignCenter);
                    this->setItem(i, j, item);  //插入到表格
                }
                i++;
            }
        }
        else{
            QMessageBox::warning(this, "提示", "查询失败，可能原因：数据库连接已断开！", QMessageBox::Ok);
        }
    }
    else if(index == 4){    //teacher
        QString sql = "select * from teacher";
        if(query.exec(sql)){
            int i = 0;
            while(query.next()){
                for(int j = 0; j < 6; j++){
                    QTableWidgetItem* item = new QTableWidgetItem(query.value(j).toString());    //一个单元格
                    item->setTextAlignment(Qt::AlignCenter);
                    this->setItem(i, j, item);  //插入到表格
                }
                i++;
            }
        }
        else{
            QMessageBox::warning(this, "提示", "查询失败，可能原因：数据库连接已断开！", QMessageBox::Ok);
        }
    }
    else{                   //course
        QString sql = "select * from course";
        if(query.exec(sql)){
            int i = 0;
            while(query.next()){
                for(int j = 0; j < 3; j++){
                    QTableWidgetItem* item = new QTableWidgetItem(query.value(j).toString());    //一个单元格
                    item->setTextAlignment(Qt::AlignCenter);
                    this->setItem(i, j, item);  //插入到表格
                }
                i++;
            }
        }
        else{
            QMessageBox::warning(this, "提示", "查询失败，可能原因：数据库连接已断开！", QMessageBox::Ok);
        }
    }
}
