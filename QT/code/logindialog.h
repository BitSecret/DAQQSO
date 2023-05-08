#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include <QtSql>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDialog>
#include <QGridLayout>
#include <QMessageBox>
#include <QDebug>
#include <QStackedWidget>
#include "datastructure.h"


class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr, AccountInformation* ai = nullptr);
    QWidget* loginLayoutWidget;     //登录窗口
    QLabel* welcomeLabel;
    QLabel* usernameLabel;
    QLineEdit* usernameLineEdit;
    QLabel* passwordLabel;
    QLineEdit* passwordLineEdit;
    QPushButton* okButton;
    QPushButton* registerButton;
    QPushButton* cancelButton;
    AccountInformation* accountInformation;

    QStackedWidget* stackedWidget;    //栈窗口

    QWidget* registerLayoutWidget;     //注册窗口
    QLabel* welcomeLabelForRegister;
    QLabel* usernameLabelForRegister;
    QLineEdit* usernameLineEditForRegister;
    QLabel* usernameTips;
    QLabel* passwordLabelForRegister;
    QLineEdit* passwordLineEditForRegister;
    QLabel* passwordTips;
    QLabel* passwordConfirmLabel;
    QLineEdit* passwordConfirmLineEdit;
    QLabel* passwordConfirmTips;
    QPushButton* registerButtonForRegister;
    QPushButton* backButtonForRegister;
    QPushButton* cancelButtonForRegister;
    AccountInformation* accountInformationForRegister;

private slots:
    void slotOkButton();
    void slotRegisterButton();
    void slotCancelButton();

    void slotRegisterButtonForRegister();
    void slotBackButtonForRegister();

    void slotPasswordLineEditTextChange();
};

#endif // LOGINDIALOG_H













































