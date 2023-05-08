#ifndef CONNECTION_H
#define CONNECTION_H
#include <QtSql>
#include <QFileInfo>
#include "datastructure.h"

void createTable(QSqlDatabase db)    //建表
{
    QSqlQuery query(db);
    query.exec("create table account(user_id INTEGER primary key AUTOINCREMENT, "
               "username vchar, password vchar)");
    query.exec("create table creation(user_id int, project_id int)");
    query.exec("create table project(project_id INTEGER primary key AUTOINCREMENT, "
               "project_name vchar, project_intro vchar,"
               "subjects_number int, subjects_namer vchar, "
               "alternative_number int, alternative_name vchar, "
               "criteria_number int, criteria_name vchar,"
               "data vchar)");
}

bool createConnection(AccountInformation* ai)
{
    bool isSuccess;
    bool isExist;

    QFileInfo dir(QDir::currentPath() + "/MCDMDatabase");    //检测数据库是否存在，用于第一次连接时初始化数据库
    if(dir.isFile()){
        isExist = true;
    }
    else {
        isExist = false;
    }

    ai->db = QSqlDatabase::addDatabase("QSQLITE");    //创建SQLITE数据库

    ai->db.setHostName("localhost");            //主机名
    ai->db.setDatabaseName("MCDMDatabase");     //数据库名字
    ai->db.setUserName("root");                 //数据库登录账号
    ai->db.setPassword("password");             //数据库登录密码
    isSuccess = ai->db.open();

    if(isSuccess && !isExist){    //第一次登录，建表
        createTable(ai->db);
    }

    return isSuccess;
}

#endif // CONNECTION_H
