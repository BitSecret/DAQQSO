#include "projectmanagedialog.h"
#include <QHeaderView>

ProjectManageDialog::ProjectManageDialog(QWidget *parent, AccountInformation* ai) : QDialog(parent)
{
    accountInformation = ai;
    selectedProjectId = -1;
    initWidget();
}

void ProjectManageDialog::initWidget(){
    setFixedSize(900, 350);
    setWindowTitle("已保存在数据库中的项目");
    setWindowFlags(windowFlags() &~ Qt::WindowContextHelpButtonHint);

    projectListTable = new QTableWidget;
    projectListTable->setFixedWidth(876);
    projectListTable->setColumnCount(3);     //设置列数
    QStringList header;    //表头
    header << "项目id" << "项目名称" << "简介";
    projectListTable->setHorizontalHeaderLabels(header);    //设置表头
    projectListTable->horizontalHeader()->setDefaultSectionSize(161);
    projectListTable->setShowGrid(true);     //设置显示格子线
    projectListTable->setEditTriggers(QAbstractItemView::NoEditTriggers);    //设置内容不可更改
    projectListTable->setSelectionBehavior(QAbstractItemView::SelectRows);    //设置整行选中

    projectListTable->setColumnWidth(0, 40);    //每列宽度
    projectListTable->setColumnWidth(1, 350);
    projectListTable->setColumnWidth(2, 476);
    showData();

    selectButton = new QPushButton("选择");
    deleteButton = new QPushButton("删除");
    cancleButton = new QPushButton("取消");

    QGridLayout* mainLayout = new QGridLayout(this);
    mainLayout->addWidget(projectListTable, 1, 1, 1, 6);
    mainLayout->addWidget(selectButton, 2, 1, 1, 2);
    mainLayout->addWidget(deleteButton, 2, 3, 1, 2);
    mainLayout->addWidget(cancleButton, 2, 5, 1, 2);

    connect(selectButton, SIGNAL(clicked(bool)), this, SLOT(slotselectButton()));
    connect(deleteButton, SIGNAL(clicked(bool)), this, SLOT(slotdeleteButton()));
    connect(cancleButton, SIGNAL(clicked(bool)), this, SLOT(slotcancleButton()));
    connect(projectListTable, SIGNAL(cellClicked(int, int)), this, SLOT(slotTableClicked(int, int)));
}

void ProjectManageDialog::showData(){
    QSqlQuery query(accountInformation->db);    //数据库连接
    QString sql;
    sql = QString("select project_id from creation where user_id=%1").arg(accountInformation->user_id);
    if(query.exec(sql)){
        int count = 0;
        while(query.next()){
            count++;
        }
        projectListTable->setRowCount(count);     //设置行数

        count = 0;
        query.exec(sql);
        while(query.next()){
            QTableWidgetItem* itemId = new QTableWidgetItem(query.value(0).toString());
            itemId->setTextAlignment(Qt::AlignCenter);
            projectListTable->setItem(count, 0, itemId);  //项目id

            QString introSql = QString("select project_name, project_intro from project where project_id=%1").arg(query.value(0).toInt());
            QSqlQuery newQuery(accountInformation->db);    //查名称和简介
            if(newQuery.exec(introSql)){
                newQuery.next();

                QTableWidgetItem* itemName = new QTableWidgetItem(newQuery.value(0).toString());
                itemName->setTextAlignment(Qt::AlignCenter);
                projectListTable->setItem(count, 1, itemName);  //项目名称

                QTableWidgetItem* itemIntro = new QTableWidgetItem(newQuery.value(1).toString());
                itemIntro->setTextAlignment(Qt::AlignCenter);
                projectListTable->setItem(count, 2, itemIntro);  //项目简介
            }
            count++;
        }
    };
}

void ProjectManageDialog::slotTableClicked(int raw, int column){
    selectedProjectId = projectListTable->item(raw, 0)->text().toInt();
}

void ProjectManageDialog::slotselectButton(){
    if(selectedProjectId == -1){
        QMessageBox::information(this, "提示", "请选择项目！");
    }
    else{
        accountInformation->projectId = selectedProjectId;
        QDialog::accept();
    }
}

void ProjectManageDialog::slotdeleteButton(){
    if(selectedProjectId == -1){
        QMessageBox::information(this, "提示", "请选择项目！");
    }
    else{
        QSqlQuery query(accountInformation->db);    //数据库连接
        QString sqlForProject = QString("delete from project where project_id=%1").arg(selectedProjectId);
        QString sqlForCreation =  QString("delete from creation where project_id=%1").arg(selectedProjectId);

        if(query.exec(sqlForCreation) && query.exec(sqlForProject)){
            QMessageBox::information(this, "提示", "删除成功！");
            selectedProjectId = -1;
            projectListTable->setRowCount(0);
            showData();
        }
        else{
            QMessageBox::information(this, "提示", "删除失败！");
        }
    }
}

void ProjectManageDialog::slotcancleButton(){
    QDialog::reject();
}
