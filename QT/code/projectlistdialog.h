#ifndef PROJECTLISTDIALOG_H
#define PROJECTLISTDIALOG_H
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

class ProjectListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectListDialog(QWidget *parent = nullptr, AccountInformation* ai = nullptr);

private:
    int selectedProjectId;
    AccountInformation* accountInformation;

    QTableWidget* projectListTable;
    QPushButton* selectButton;
    QPushButton* cancleButton;

    void initWidget();
private slots:
    void slotselectButton();
    void slotcancleButton();
    void slotTableClicked(int, int);
};

#endif // PROJECTLISTDIALOG_H
