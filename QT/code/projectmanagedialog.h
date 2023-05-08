#ifndef PROJECTMANAGEDIALOG_H
#define PROJECTMANAGEDIALOG_H
#include <QtSql>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDialog>
#include <QGridLayout>
#include <QMessageBox>
#include <QDebug>
#include <QTableWidget>
#include "datastructure.h"

class ProjectManageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectManageDialog(QWidget *parent = nullptr, AccountInformation* ai = nullptr);

private:
    int selectedProjectId;
    AccountInformation* accountInformation;

    QTableWidget* projectListTable;
    QPushButton* selectButton;
    QPushButton* deleteButton;
    QPushButton* cancleButton;

    void initWidget();
    void showData();
private slots:
    void slotselectButton();
    void slotdeleteButton();
    void slotcancleButton();
    void slotTableClicked(int, int);
};

#endif // PROJECTMANAGEDIALOG_H
