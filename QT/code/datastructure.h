#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H
#include <QString>
#include <QtSql>

struct ProjectInformation
{
    int projectId;
    QString projectName;
    QString projectIntro;

    int subjectsCount;
    QString* subjectsName;

    int alternativeCount;
    QString* alternativeName;

    int criteriaCount;
    QString* criteriaName;

    double* data;
};

struct AccountInformation    //账号信息
{
    /*数据库信息*/
    QSqlDatabase db;    //数据库连接

    /*用户信息*/
    int user_id;    //用户id
    QString username;

    /*用户操作信息*/
    bool isProjectSaved;    //项目是否保存
    bool isNewProject;    //是否是新项目，这决定了保存的时候，是update还是insert
    int projectId;    //项目信息
};

struct ComputeInfomation
{
    int raw;
    int column;
    double* weight;
    double** decisionMatrix;
};

#endif // DATASTRUCTURE_H
