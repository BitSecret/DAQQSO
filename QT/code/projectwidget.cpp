#include "projectwidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTabBar>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QtMath>

ProjectWidget::ProjectWidget(QWidget *parent, AccountInformation* ai) : QWidget(parent)
{
    accountInformation = ai;
    computeInfo = nullptr;
    initWidget();     //初始化控件
    connectSignalsAndSlots();    //连接信号和槽
}


/****初始化相关的函数部分****/
void ProjectWidget::initWidget()    //初始化控件
{
    QFont font;    //字体

    titleLabel = new QLabel("项目信息");
    titleLabel->setFixedHeight(30);
    titleLabel->setFrameStyle(QLabel::StyledPanel);
    titleLabel->setAlignment(Qt::AlignCenter);
    font.setPointSize(15);
    titleLabel->setFont(font);

    projectNameLabel = new QLabel("项目名称:");
    projectNameLineEdit = new QLineEdit;

    projectIntroLabel = new QLabel("项目介绍:");
    projectIntroLineEdit = new QLineEdit;

    subjectsCountLabel = new QLabel("被试数量:");
    subjectsCountLabel->setFixedWidth(64);
    subjectsCountLineEdit = new QLineEdit;
    subjectsCountLineEdit->setFixedWidth(64);
    subjectsNameLabel = new QLabel("被试名称:");
    subjectsNameLabel->setAlignment(Qt::AlignCenter);
    subjectsNameLabel->setFixedWidth(64);
    subjectsNameLineEdit = new QLineEdit;

    alternativeCountLabel = new QLabel("方案数量:");
    alternativeCountLineEdit = new QLineEdit;
    alternativeCountLineEdit->setFixedWidth(64);
    alternativeNameLabel = new QLabel("方案名称:");
    alternativeNameLabel->setAlignment(Qt::AlignCenter);
    alternativeNameLineEdit = new QLineEdit;

    criteriaCountLabel = new QLabel("指标数量:");
    criteriaCountLineEdit = new QLineEdit;
    criteriaCountLineEdit->setFixedWidth(64);
    criteriaNameLabel = new QLabel("指标名称:");
    criteriaNameLabel->setAlignment(Qt::AlignCenter);
    criteriaNameLineEdit = new QLineEdit;

    dataButton = new QPushButton("点击生成下方数据矩阵(若您的项目是导入的，请勿点击此按钮)");

    dataTableWidget = new QTabWidget;    //数据
    dataTableWidget->tabBar()->setStyleSheet("QTabBar::tab{min-height: 30px; min-width: 110px; font:15px;}");

    generateLingoButton = new QPushButton("生成权重");
    weightLineEdit = new QLineEdit;
    importWeightButton = new QPushButton("导入权重");

    saveButton = new QPushButton("保存项目");   //保存项目
    executeButton = new QPushButton("生成决策");
    exportButton = new QPushButton("导出项目");    //导出项目成txt文件

    executeResultTextEdit = new QTextEdit;
    executeResultTextEdit->setFixedHeight(200);


    QGridLayout* sonLayout = new QGridLayout(this);
    sonLayout->setMargin(2);
    sonLayout->addWidget(titleLabel, 1, 1, 1, 10);

    sonLayout->addWidget(projectNameLabel, 2, 1, 1, 1);
    sonLayout->addWidget(projectNameLineEdit, 2, 2, 1, 9);

    sonLayout->addWidget(projectIntroLabel, 3, 1, 1, 1);
    sonLayout->addWidget(projectIntroLineEdit, 3, 2, 1, 9);

    sonLayout->addWidget(subjectsCountLabel, 4, 1, 1, 1);
    sonLayout->addWidget(subjectsCountLineEdit, 4, 2, 1, 1);
    sonLayout->addWidget(subjectsNameLabel, 4, 3, 1, 1);
    sonLayout->addWidget(subjectsNameLineEdit, 4, 4, 1, 7);


    sonLayout->addWidget(alternativeCountLabel, 5, 1, 1, 1);
    sonLayout->addWidget(alternativeCountLineEdit, 5, 2, 1, 1);
    sonLayout->addWidget(alternativeNameLabel, 5, 3, 1, 1);
    sonLayout->addWidget(alternativeNameLineEdit, 5, 4, 1, 7);


    sonLayout->addWidget(criteriaCountLabel, 6, 1, 1, 1);
    sonLayout->addWidget(criteriaCountLineEdit, 6, 2, 1, 1);
    sonLayout->addWidget(criteriaNameLabel, 6, 3, 1, 1);
    sonLayout->addWidget(criteriaNameLineEdit, 6, 4, 1, 7);

    sonLayout->addWidget(dataButton, 7, 1, 1, 10);

    sonLayout->addWidget(dataTableWidget, 8, 1, 1, 10);

    sonLayout->addWidget(generateLingoButton, 9, 1, 1, 1);
    sonLayout->addWidget(weightLineEdit, 9, 2, 1, 8);
    sonLayout->addWidget(importWeightButton, 9, 10, 1, 1);


    sonLayout->addWidget(saveButton, 10, 1, 1, 3);
    sonLayout->addWidget(executeButton, 10, 4, 1, 4);
    sonLayout->addWidget(exportButton, 10, 8, 1, 3);

    sonLayout->addWidget(executeResultTextEdit, 11, 1, 1, 10);
}

void ProjectWidget::connectSignalsAndSlots()    //连接信号和槽
{
    /*按钮的响应函数*/
    connect(dataButton, SIGNAL(clicked(bool)), this, SLOT(slotDataButton()));
    connect(generateLingoButton, SIGNAL(clicked(bool)), this, SLOT(slotGenerateLingoButton()));
    connect(importWeightButton, SIGNAL(clicked(bool)), this, SLOT(slotImportWeightButton()));
    connect(saveButton, SIGNAL(clicked(bool)), this, SLOT(slotSaveButton()));
    connect(exportButton, SIGNAL(clicked(bool)), this, SLOT(slotExportButton()));
    connect(executeButton, SIGNAL(clicked(bool)), this, SLOT(slotExecuteButton()));

    /*项目信息修改的响应函数*/
    connect(projectNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotTextChange()));
    connect(projectIntroLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotTextChange()));
    connect(subjectsCountLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotTextChange()));
    connect(subjectsNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotTextChange()));
    connect(alternativeCountLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotTextChange()));
    connect(alternativeNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotTextChange()));
    connect(criteriaCountLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotTextChange()));
    connect(criteriaNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotTextChange()));
}


/****函数部分****/
ProjectInformation* ProjectWidget::getDataByFile(QString filePath)    //通过文件导入数据
{
    ProjectInformation* projectInfo = new ProjectInformation;

    QFile file(filePath);           //读文件
    file.open(QIODevice::ReadOnly);   //只读
    QTextStream in(&file);    //输入流
    in.setCodec("UTF-8");    //防止中文乱码
    QString temp;

    projectInfo->projectId = -1;
    in >> projectInfo->projectName;    //项目名
    in >> projectInfo->projectIntro;   //项目简介

    in >> projectInfo->subjectsCount;  //被试数量
    projectInfo->subjectsName = new QString[projectInfo->subjectsCount];
    for(int i = 0; i < projectInfo->subjectsCount; i++){
        in >> projectInfo->subjectsName[i];
    }

    in >> projectInfo->alternativeCount;  //方案数量
    projectInfo->alternativeName = new QString[projectInfo->alternativeCount];
    for(int i = 0; i < projectInfo->alternativeCount; i++){
        in >> projectInfo->alternativeName[i];
    }

    in >> projectInfo->criteriaCount;  //指标数量
    projectInfo->criteriaName = new QString[projectInfo->criteriaCount];
    for(int i = 0; i < projectInfo->criteriaCount; i++){
        in >> projectInfo->criteriaName[i];
    }

    projectInfo->data = new double[projectInfo->alternativeCount * projectInfo->subjectsCount * projectInfo->criteriaCount];
    for(int i = 0; i < projectInfo->alternativeCount * projectInfo->subjectsCount * projectInfo->criteriaCount; i++){    //方案是第一个维度
        in >> projectInfo->data[i];
    }

    file.close();
    return projectInfo;
}

ProjectInformation* ProjectWidget::getDataByDB(int projectId)     //通过数据库导入数据
{
    QSqlQuery query(accountInformation->db);    //数据库连接
    QStringList temp;

    ProjectInformation* projectInfo = new ProjectInformation;
    projectInfo->projectId = projectId;

    QString sql = QString("select * from project where project_id=%1").arg(projectId);
    if(query.exec(sql)){
        query.next();
        projectInfo->projectName = query.value(1).toString();    //项目名
        projectInfo->projectIntro = query.value(2).toString();   //项目简介

        projectInfo->subjectsCount = query.value(3).toInt();  //被试数量

        temp = query.value(4).toString().split(" ");
        projectInfo->subjectsName = new QString[projectInfo->subjectsCount];
        for(int i = 0; i < projectInfo->subjectsCount; i++){
            projectInfo->subjectsName[i] = temp[i];
        }

        projectInfo->alternativeCount = query.value(5).toInt();;  //方案数量
        temp = query.value(6).toString().split(" ");
        projectInfo->alternativeName = new QString[projectInfo->alternativeCount];
        for(int i = 0; i < projectInfo->alternativeCount; i++){
            projectInfo->alternativeName[i] = temp[i];
        }

        projectInfo->criteriaCount = query.value(7).toInt();  //指标数量
        temp = query.value(8).toString().split(" ");
        projectInfo->criteriaName = new QString[projectInfo->criteriaCount];
        for(int i = 0; i < projectInfo->criteriaCount; i++){
            projectInfo->criteriaName[i] = temp[i];
        }

        projectInfo->data = new double[projectInfo->alternativeCount * projectInfo->subjectsCount * projectInfo->criteriaCount];
        temp = query.value(9).toString().split(" ");
        for(int i = 0; i < projectInfo->alternativeCount * projectInfo->subjectsCount * projectInfo->criteriaCount; i++){    //方案是第一个维度
            projectInfo->data[i] = temp[i].toDouble();
        }
    }
    return projectInfo;
}

void ProjectWidget::showData(ProjectInformation* projectInfo)    //加载数据到显示页面
{
    QString temp = "";
    projectNameLineEdit->setText(projectInfo->projectName);    //项目名称
    projectIntroLineEdit->setText(projectInfo->projectIntro);    //项目简介
    subjectsCountLineEdit->setText(QString("%1").arg(projectInfo->subjectsCount));    //被试数量
    for(int i = 0; i < projectInfo->subjectsCount; i++){    //被试名称
        temp = temp + projectInfo->subjectsName[i] + ";";
    }
    subjectsNameLineEdit->setText(temp);
    temp = "";


    alternativeCountLineEdit->setText(QString("%1").arg(projectInfo->alternativeCount));    //方案数量
    for(int i = 0; i < projectInfo->alternativeCount; i++){    //被试名称
        temp = temp + projectInfo->alternativeName[i] + ";";
    }
    alternativeNameLineEdit->setText(temp);
    temp = "";

    criteriaCountLineEdit->setText(QString("%1").arg(projectInfo->criteriaCount));    //指标数量
    for(int i = 0; i < projectInfo->criteriaCount; i++){    //被试名称
        temp = temp + projectInfo->criteriaName[i] + ";";
    }
    criteriaNameLineEdit->setText(temp);
    temp = "";


    QStringList header;    //表头
    for(int j = 0; j < projectInfo->criteriaCount; j++){
        header << projectInfo->criteriaName[j];
    }
    dataTable = new QTableWidget[projectInfo->alternativeCount];
    for(int i = 0; i < projectInfo->alternativeCount; i++){    //生成各个表
        dataTable[i].setRowCount(projectInfo->subjectsCount);     //设置行数
        dataTable[i].setColumnCount(projectInfo->criteriaCount);     //设置列数
        dataTable[i].horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //均分各列
        dataTable[i].setHorizontalHeaderLabels(header);    //设置表头
        dataTable[i].horizontalHeader()->setDefaultSectionSize(161);
        dataTable[i].setShowGrid(true);     //设置显示格子线

        for(int j = 0; j < projectInfo->subjectsCount; j++){    //被试是第二个维度
            for(int k = 0; k < projectInfo->criteriaCount; k++){    //指标是第三个维度
                QTableWidgetItem* item = new QTableWidgetItem(QString("%1").arg(projectInfo->data[i * projectInfo->subjectsCount * projectInfo->criteriaCount + j * projectInfo->criteriaCount + k]));    //一个单元格
                item->setTextAlignment(Qt::AlignCenter);
                dataTable[i].setItem(j, k, item);  //插入到表格
                ;
            }
        }

        dataTableWidget->addTab(&dataTable[i], projectInfo->alternativeName[i]);
    }
}
ProjectInformation* ProjectWidget::getDataFromWidget()    //将面板的数据整理到ProjectInformation数据结构
{
    QStringList temp;    //临时存放数据

    ProjectInformation* projectInfo = new ProjectInformation;

    projectInfo->projectId = accountInformation->projectId;
    projectInfo->projectName = projectNameLineEdit->text();
    projectInfo->projectIntro = projectIntroLineEdit->text();

    projectInfo->subjectsCount = subjectsCountLineEdit->text().toInt();
    temp = subjectsNameLineEdit->text().split(";");
    projectInfo->subjectsName = new QString[projectInfo->subjectsCount];
    for(int j = 0; j < projectInfo->subjectsCount; j++){
        if((j + 1) <= (temp.count() - 1)){
            projectInfo->subjectsName[j] = temp.value(j);
        }
        else{
            projectInfo->subjectsName[j] = QString("s%1").arg(j + 1);
        }
    }


    projectInfo->alternativeCount = alternativeCountLineEdit->text().toInt();
    temp = alternativeNameLineEdit->text().split(";");
    projectInfo->alternativeName = new QString[projectInfo->alternativeCount];
    for(int j = 0; j < projectInfo->alternativeCount; j++){
        if((j + 1) <= (temp.count() - 1)){
            projectInfo->alternativeName[j] = temp.value(j);
        }
        else{
            projectInfo->alternativeName[j] = QString("c%1").arg(j + 1);
        }
    }


    projectInfo->criteriaCount = criteriaCountLineEdit->text().toInt();
    temp = criteriaNameLineEdit->text().split(";");
    projectInfo->criteriaName = new QString[projectInfo->criteriaCount];
    for(int j = 0; j < projectInfo->criteriaCount; j++){
        if((j + 1) <= (temp.count() - 1)){
            projectInfo->criteriaName[j] = temp.value(j);
        }
        else{
            projectInfo->criteriaName[j] = QString("c%1").arg(j + 1);
        }
    }

    projectInfo->data = new double[projectInfo->alternativeCount * projectInfo->subjectsCount * projectInfo->criteriaCount];
    int dataCount = 0;
    for(int i = 0; i < projectInfo->alternativeCount; i++){
        for(int j = 0; j < projectInfo->subjectsCount; j++){
            for(int k = 0; k < projectInfo->criteriaCount; k++){
                projectInfo->data[dataCount++] = dataTable[i].item(j, k)->text().toDouble();
            }
        }
    }

    return projectInfo;
}

void ProjectWidget::getWeightFromWidget()    //将面板的数据整理到Weight数据结构
{
    computeInfo->weight = new double[computeInfo->column];
    QStringList weightString = weightLineEdit->text().split(";");
    for(int i = 0; i < computeInfo->column; i++){
        if(i + 1 <= weightString.count() - 1){
            computeInfo->weight[i] = weightString[i].toDouble();
        }
        else{
            computeInfo->weight[i] = 0;
        }
    }
}

void ProjectWidget::computeDecisionMatrix()    //计算decisionMatrix
{
    computeInfo = new ComputeInfomation;
    ProjectInformation* proInfo = getDataFromWidget();
    int alternativeCount = proInfo->alternativeCount;    //方案数量
    int raw = proInfo->subjectsCount;     //被试数量
    int column = proInfo->criteriaCount;    //指标数量
    int classification = 3;
    double k = 0.5;

    double*** expertDecisionMatrix = new double**[alternativeCount];    //原始数据矩阵
    int **classificationMatrix = new int*[raw];             //分类的辅助信息
    computeInfo->decisionMatrix = new double*[alternativeCount];               //决策矩阵
    computeInfo->raw = alternativeCount;
    computeInfo->column = column;

    for(int i = 0; i < alternativeCount; i++){
        expertDecisionMatrix[i] = new double*[raw];
        for(int j = 0; j < raw; j++){
            expertDecisionMatrix[i][j] = new double[column];
        }
    }
    for(int i = 0; i < raw; i++){
        classificationMatrix[i] = new int[column];
    }
    for(int i = 0; i < alternativeCount; i++){
        computeInfo->decisionMatrix[i] = new double[column];
    }

    for(int i = 0; i < alternativeCount; i++){    //给专家矩阵赋值
        for(int j = 0; j < raw; j++){
            for(int c = 0; c < column; c++){
                expertDecisionMatrix[i][j][c] = proInfo->data[i * raw * column + j * column + c];
            }
        }
    }

    double* min = new double[column];    //最大值
    double* max = new double[column];    //最小值
    for (int j = 0; j < column; j++){    //计算最小和最大值(所有方案20*4)
        min[j] = expertDecisionMatrix[0][0][j];
        max[j] = expertDecisionMatrix[0][0][j];
    }

    for (int a = 0; a < alternativeCount; a++){
        for (int j = 0; j < column; j++){
            for (int i = 0; i < raw; i++){
                if (expertDecisionMatrix[a][i][j] < min[j]){
                    min[j] = expertDecisionMatrix[a][i][j];
                }
                if (expertDecisionMatrix[a][i][j] > max[j]){
                    max[j] = expertDecisionMatrix[a][i][j];
                }
            }
        }
    }

    for (int a = 0; a < alternativeCount; a++){    //聚合
        for (int i = 0; i < raw; i++){
            for (int j = 0; j < column; j++){
                int rank = 1;
                double part = (max[j] - min[j]) / classification;
                while (true){
                    if (expertDecisionMatrix[a][i][j] >= (min[j] + part * (rank - 1)) &&
                            expertDecisionMatrix[a][i][j] <= (min[j] + part * (rank))){
                        classificationMatrix[i][j] = rank;
                        break;
                    }
                    else{
                        rank++;
                    }
                }
            }
        }
        for (int j = 0; j < column; j++){    //规范化
            for (int i = 0; i < raw; i++){
                expertDecisionMatrix[a][i][j] = expertDecisionMatrix[a][i][j] / max[j];
            }
        }


        double sum = 0;
        for (int j = 0; j < column; j++){
            sum = 0;
            for (int i = 0; i < raw; i++){
                double current = qSqrt(
                        k * qPow(expertDecisionMatrix[a][i][j], 2)
                                + (1 - k) * qPow(classificationMatrix[i][j] / classification, 2)
                );
                sum += current;
            }
            sum = sum / raw;
            computeInfo->decisionMatrix[a][j] = sum;
        }

        sum = 0;
        for (int i = 0; i < raw; i++){
            sum += expertDecisionMatrix[a][i][0];
        }
        computeInfo->decisionMatrix[a][0] = sum / raw;
    }
}

void ProjectWidget::clearData()    //清除数据
{
    projectNameLineEdit->clear();
    projectIntroLineEdit->clear();
    subjectsCountLineEdit->clear();
    subjectsNameLineEdit->clear();
    alternativeCountLineEdit->clear();
    alternativeNameLineEdit->clear();
    criteriaCountLineEdit->clear();
    criteriaNameLineEdit->clear();
    dataTableWidget->clear();
    weightLineEdit->clear();

    executeResultTextEdit->clear();
}

void ProjectWidget::save()    //保存数据到数据库
{
    ProjectInformation* projectInfo = getDataFromWidget();
    QSqlQuery query(accountInformation->db);    //数据库连接

    QString subjectsName = "";
    for(int i = 0; i < projectInfo->subjectsCount; i++){
        if(i < projectInfo->subjectsCount - 1){
            subjectsName = subjectsName + projectInfo->subjectsName[i] + " ";
        }
        else{
            subjectsName = subjectsName + projectInfo->subjectsName[i];
        }
    }
    QString alternativeName = "";
    for(int i = 0; i < projectInfo->alternativeCount; i++){
        if(i < projectInfo->alternativeCount - 1){
            alternativeName = alternativeName + projectInfo->alternativeName[i] + " ";
        }
        else{
            alternativeName = alternativeName + projectInfo->alternativeName[i];
        }
    }
    QString criteriaName = "";
    for(int i = 0; i < projectInfo->criteriaCount; i++){
        if(i < projectInfo->criteriaCount - 1){
            criteriaName = criteriaName + projectInfo->criteriaName[i] + " ";
        }
        else{
            criteriaName = criteriaName + projectInfo->criteriaName[i];
        }
    }
    QString data = "";
    int dataCount = projectInfo->subjectsCount * projectInfo->alternativeCount * projectInfo->criteriaCount;

    for(int i = 0; i < dataCount; i++){
        if(i < dataCount - 1){
            data = data + QString("%1").arg(projectInfo->data[i]) + " ";
        }
        else{
            data = data + QString("%1").arg(projectInfo->data[i]);
        }
    }


    if(accountInformation->isNewProject){
        QString sqlProject = QString("insert into project values(null, '%1', '%2', %3, '%4', %5, '%6', %7, '%8', '%9')")
                .arg(projectInfo->projectName).arg(projectInfo->projectIntro)
                .arg(projectInfo->subjectsCount).arg(subjectsName)
                .arg(projectInfo->alternativeCount).arg(alternativeName)
                .arg(projectInfo->criteriaCount).arg(criteriaName)
                .arg(data);

        if(query.exec(sqlProject)){
            accountInformation->isNewProject = false;
            QString sqlProjectId = QString("select project_id from project where project_name='%1' and project_intro='%2'")
                                           .arg(projectInfo->projectName).arg(projectInfo->projectIntro);
            if(query.exec(sqlProjectId)){
                query.next();
                accountInformation->projectId = query.value(0).toInt();
            }

            QString sqlCreation = QString("insert into creation values(%1, %2)").arg(accountInformation->user_id).arg(accountInformation->projectId);
            query.exec(sqlCreation);

            QMessageBox::information(this, "提示", "保存成功", QMessageBox::Ok);
            accountInformation->isProjectSaved = true;
        }
        else{
            QMessageBox::information(this, "提示", "保存失败", QMessageBox::Ok);
        }
    }
    else{
        QString sql = QString("update project set project_name='%1', project_intro='%2', "
                              "subjects_number=%3, subjects_namer='%4', "
                              "alternative_number=%5, alternative_name='%6', "
                              "criteria_number=%7, criteria_name='%8', "
                              "data='%9' where project_id=%10")
                              .arg(projectInfo->projectName).arg(projectInfo->projectIntro)
                              .arg(projectInfo->subjectsCount).arg(subjectsName)
                              .arg(projectInfo->alternativeCount).arg(alternativeName)
                              .arg(projectInfo->criteriaCount).arg(criteriaName)
                              .arg(data).arg(accountInformation->projectId);
        if(query.exec(sql)){
            QMessageBox::information(this, "提示", "保存成功", QMessageBox::Ok);
            accountInformation->isProjectSaved = true;
        }
    }
}


/****槽函数部分****/
void ProjectWidget::slotDataButton()    //生成数据矩阵
{
    dataTableWidget->clear();    //先把数据清空

    int subjectsCount = subjectsCountLineEdit->text().toInt();    //读取用户输入的信息

    int alternativeCount = alternativeCountLineEdit->text().toInt();
    QStringList alternativeName = alternativeNameLineEdit->text().split(";");

    int criteriaCount = criteriaCountLineEdit->text().toInt();
    QStringList criteriaName = criteriaNameLineEdit->text().split(";");

    QStringList header;    //表头
    for(int j = 0; j < criteriaCount; j++){
        if((j + 1) <= (criteriaName.count() - 1)){
            header << criteriaName.value(j);
        }
        else{
            header << QString("c%1").arg(j + 1);
        }
    }

    dataTable = new QTableWidget[alternativeCount];
    for(int i = 0; i < alternativeCount; i++){      //生成各个表

        dataTable[i].setRowCount(subjectsCount);     //设置行数
        dataTable[i].setColumnCount(criteriaCount);     //设置列数
        dataTable[i].horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //均分各列
        dataTable[i].setHorizontalHeaderLabels(header);    //设置表头
        dataTable[i].horizontalHeader()->setDefaultSectionSize(161);
        dataTable[i].setShowGrid(true);     //设置显示格子线

        if((i + 1) <= (alternativeName.count() - 1)){
            dataTableWidget->addTab(&dataTable[i], alternativeName.value(i));
        }
        else{
            dataTableWidget->addTab(&dataTable[i], QString("a%1").arg(i + 1));
        }
    }


}

void ProjectWidget::slotGenerateLingoButton()    //生成lingo语句
{

    computeDecisionMatrix();    //计算decisionMatrix

    QString result = "";

    int raw = computeInfo->raw;
    int column = computeInfo->column;
    double** decisionMatrix = computeInfo->decisionMatrix;

    double* objectWeight = new double[column];
    double* subjectWeight = new double[column];

    result += "decisionMatrix:\n";
    for(int i = 0; i < raw; i++){
        for(int j = 0; j < column; j++){
             result += QString("%1\t").arg(decisionMatrix[i][j]);
        }
        result += "\n";
    }

    /****计算主观权重****/
    for(int i = 0; i < column; i++){
        subjectWeight[i] = 1.0 / column;
    }

    /****计算客观权重****/
    double* sum = new double[column];     //每一列的和
    for (int j = 0; j < column; j++){
        sum[j] = 0;
    }
    for (int i = 0; i < raw; i++){
        for (int j = 0; j < column; j++){
            sum[j] += decisionMatrix[i][j];
        }
    }

    double* avg = new double[column];    //每一列的均值
    for (int j = 0; j < column; j++){
        avg[j] = sum[j] / raw;
    }

    double** pearson = new double*[column];        //皮尔逊相关系数矩阵
    for(int i = 0; i < column; i++){
        pearson[i] = new double[column];
    }
    for (int i = 0; i < column; i++){
        for (int j = 0; j < column; j++){
            if (i == j){
                pearson[i][j] = 1;
            }
            else{
                double numerator = 0;
                double denominator_x = 0;
                double denominator_y = 0;
                for (int k = 0; k < raw; k++){
                    numerator += (decisionMatrix[k][i] - avg[i]) * (decisionMatrix[k][j] - avg[j]);
                    denominator_x += (decisionMatrix[k][i] - avg[i]) * (decisionMatrix[k][i] - avg[i]);
                    denominator_y += (decisionMatrix[k][j] - avg[j]) * (decisionMatrix[k][j] - avg[j]);
                }
                denominator_x = qSqrt(denominator_x);
                denominator_y = qSqrt(denominator_y);
                pearson[i][j] = numerator / (denominator_x * denominator_y);
            }
        }
    }

    double* G = new double[column];    //Gini系数
    result += "Gini:\n";        //计算Gini系数
    double giniSum;
    for (int j = 0; j < column; j++){
        giniSum = 0;
        for (int i = 0; i < raw; i++){
            giniSum += (decisionMatrix[i][j] / sum[j]) * (decisionMatrix[i][j] / sum[j]);
        }
        G[j] = 1.0 - giniSum;
        result += QString("%1\t").arg(G[j]);
    }
    result += "\n";

    result += "co-R:\n";    //计算R  相关性
    double* R = new double[column];
    for(int i = 0; i < column; i++){
        R[i] = 0;
    }
    for (int j = 0; j < column; j++){
        for (int i = 0; i < column; i++){
            R[j] += pearson[i][j];
        }
        result += QString("%1\t").arg(R[j]);
    }
    result += "\n";

    double weightSum = 0;          //计算权重
    for (int j = 0; j < column; j++){
        weightSum += G[j] * R[j] / (G[j] + R[j]);
    }
    for (int j = 0; j < column; j++){
        objectWeight[j] = G[j] * R[j] / (G[j] + R[j]) / weightSum;
    }


    result += "subjectWeight:\n";    //输出主客观权重
    for (int j = 0; j < column; j++){
        result += QString("%1\t").arg(subjectWeight[j]);
    }
    result += "\n";

    result += "objectWeight:\n";
    for (int j = 0; j < column; j++){
        result += QString("%1\t").arg(objectWeight[j]);
    }
    result += "\n";


    /*生成Lingo语句*/
    double** minAndMax = new double*[2];    //每个指标的最大最小值
    minAndMax[0] = new double[column];
    minAndMax[1] = new double[column];

    for (int j = 0; j < column; j++){
        minAndMax[0][j] = decisionMatrix[0][j];
        minAndMax[1][j] = decisionMatrix[0][j];
    }
    for (int j = 0; j < column; j++){
        for (int i = 1; i < raw; i++){
            if (decisionMatrix[i][j] < minAndMax[0][j]){
                minAndMax[0][j] = decisionMatrix[i][j];    //第一行储存最小值
            }
            if (decisionMatrix[i][j] > minAndMax[1][j]){
                minAndMax[1][j] = decisionMatrix[i][j];    //第二行储存最大值
            }
        }
    }

    for (int i = 0; i < raw; i++){    //规范化
        for (int j = 0; j < column; j++){
            decisionMatrix[i][j] = (decisionMatrix[i][j] - minAndMax[0][j]) / (minAndMax[1][j] - minAndMax[0][j]);
        }
    }

    result += "\nLingo Statement For Range Max Method:\ntitle: Range Max Method;\n";
    result += "!target function;\n";
    result += "max = ";    //最大优化
    for (int i = 0; i < raw; i++){
        result += "(";
        for (int j = 0; j < column; j++){
            result += QString("(%1)*w%2").arg(decisionMatrix[i][j] - avg[j]).arg(j + 1);
            if (j < column - 1){
                result += "+";
            }
        }

        if (i < raw - 1){
            result += ")^2\n+";
        }
        else{
            result += ")^2;\n";
        }
    }

    result += "!constrain;\n";    //输出约束
    for (int j = 0; j < column; j++){
        result += QString("w%1").arg(j + 1);
        if (j < column - 1){
            result += "+";
        }
        else{
            result += "=1;\n";
        }
    }
    for (int j = 0; j < column; j++){
        result += QString("w%1>=%2;w%3<=%4;\n").arg(j + 1).arg(qMin(subjectWeight[j], objectWeight[j]))
                .arg(j + 1).arg(qMax(subjectWeight[j], objectWeight[j]));
    }


    executeResultTextEdit->setText(result);
}


void ProjectWidget::slotExecuteButton()    //执行项目生成结果
{
    if(weightLineEdit->text().isNull()){
        QMessageBox::information(this, "提示", "请导入权重！");
    }
    else{
        computeDecisionMatrix();    //计算decisionMatrix
        getWeightFromWidget();     //计算权重

        QString result = "";

        int raw = computeInfo->raw;
        int column = computeInfo->column;
        double** decisionMatrix = computeInfo->decisionMatrix;
        double* weight = computeInfo->weight;
        int* indexType = new int[column];    //指标类型
        for(int i = 0; i < column; i++){
            indexType[i] = 1;
        }
        double* min = new double[column];
        double* max = new double[column];
        double* worst = new double[column];
        double* best = new double[column];

        result += "decisionMatrix:\n";
        for(int i = 0; i < raw; i++){
            for(int j = 0; j < column; j++){
                 result += QString("%1\t").arg(decisionMatrix[i][j]);
            }
            result += "\n";
        }

        result += "weight:\n";
        for(int j = 0; j < column; j++){
             result += QString("%1\t").arg(weight[j]);
        }
        result += "\n";

        result += "indexType:\n";
        for(int j = 0; j < column; j++){
             result += QString("%1\t").arg(indexType[j]);
        }
        result += "\n";

        for (int j = 0; j < column; j++){    //计算最小和最大值
            min[j] = decisionMatrix[0][j];
            max[j] = decisionMatrix[0][j];
        }
        for (int j = 0; j < column; j++){
            for (int i = 1; i < raw; i++){
                if (decisionMatrix[i][j] < min[j]){
                    min[j] = decisionMatrix[i][j];
                }
                if (decisionMatrix[i][j] > max[j]){
                    max[j] = decisionMatrix[i][j];
                }
            }
        }

        for (int j = 0; j < column; j++){    //计算AAI和AI
            if (indexType[j] == 1){    //benefit类型
                worst[j] = min[j];
                best[j] = max[j];
            }
            else{    //benefit类型
                worst[j] = max[j];
                best[j] = min[j];
            }
        }

        for (int i = 0; i < raw; i++){    //计算Nij
            for (int j = 0; j < column; j++){
                if (indexType[j] == 1){    //benefit类型
                    decisionMatrix[i][j] = decisionMatrix[i][j] / best[j];
                }
                else{    //cost类型
                    decisionMatrix[i][j] = best[j] / decisionMatrix[i][j];
                }
            }
        }

        double* S = new double[raw + 2];    //计算S
        for(int i = 0; i < raw + 2; i++){
            S[i] = 0;
        }
        for (int i = 0; i < raw; i++){
            for (int j = 0; j < column; j++){
                S[i] += decisionMatrix[i][j] * weight[j];
            }
        }
        for (int j = 0; j < column; j++){
            S[raw] += worst[j] * weight[j];
        }
        for (int j = 0; j < column; j++){
            S[raw + 1] += best[j] * weight[j];
        }

        result += "MRACOS决策结果：\nS\tK+\tK-\tK\n";
        for (int i = 0; i < raw; i++){
            double K_Positive = S[i] / S[raw];
            double K_Negative = S[i] / S[raw + 1];

            result += QString("%1\t%2\t%3\t%4\n").arg(S[i]).arg(K_Positive).arg(K_Negative).arg(K_Positive + K_Negative);
        }

        executeResultTextEdit->setText(result);
    }
}

void ProjectWidget::slotImportWeightButton()    //导入权重
{
    QString dataFilePath = QFileDialog::getOpenFileName(this, "打开", "C:/Users/Admin/Desktop", "files (*.MCDMWeight)");
    if(!dataFilePath.isEmpty())    //如果用户选择了文件，才运行
    {
        QFile file(dataFilePath);           //读文件
        file.open(QIODevice::ReadOnly);   //只读
        QTextStream in(&file);    //输入流
        in.setCodec("UTF-8");    //防止中文乱码
        QString temp;
        QString result = "";
        int count;

        in >> count;
        for(int i = 0; i < count; i++){
            in >> temp;
            result = result + temp + ";";
        }

        weightLineEdit->setText(result);
    }
}
void ProjectWidget::slotSaveButton()    //保存项目
{
    save();
}

void ProjectWidget::slotExportButton()    //导出项目
{
    QString dir = QFileDialog::getExistingDirectory(this, "打开", "C:/Users/Admin/Desktop");
    if(!dir.isEmpty())    //如果用户选择了路径，才运行
    {
        QString filePath = dir + "\\" + "export.MCDMData";
        QFile file(filePath);           //读文件
        file.open(QIODevice::WriteOnly);   //只读
        QTextStream out(&file);    //输入流
        out.setCodec("utf-8");    //防止中文乱码

        ProjectInformation* projectInfo = getDataFromWidget();

        out << projectInfo->projectName << endl;
        out << projectInfo->projectIntro << endl;
        out << projectInfo->subjectsCount << endl;
        for(int i = 0; i < projectInfo->subjectsCount; i++){
            out << projectInfo->subjectsName[i] << "\t";
        }
        out << endl;

        out << projectInfo->alternativeCount << endl;
        for(int i = 0; i < projectInfo->alternativeCount; i++){
            out << projectInfo->alternativeName[i] << "\t";
        }
        out << endl;

        out << projectInfo->criteriaCount << endl;
        for(int i = 0; i < projectInfo->criteriaCount; i++){
            out << projectInfo->criteriaName[i] << "\t";
        }
        out << endl;

        for(int i = 0; i < projectInfo->subjectsCount * projectInfo->alternativeCount * projectInfo->criteriaCount; i++){
            out << projectInfo->data[i] << "\t";
        }

        file.close();
        QMessageBox::information(this, "提示", "导出成功。");
    }
}

void ProjectWidget::slotTextChange(){
    accountInformation->isProjectSaved = false;
}
