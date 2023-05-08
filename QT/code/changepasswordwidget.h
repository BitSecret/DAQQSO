#ifndef CHANGEPASSWORDWIDGET_H
#define CHANGEPASSWORDWIDGET_H
#include <QWidget>
#include "datastructure.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class ChangePasswordWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChangePasswordWidget(QWidget *parent = nullptr, AccountInformation* ai = nullptr);
private:
    /********控件*********/
    QLabel* titleLabel;
    QLabel* changeLabel;
    QLabel* newPasswordLabel;
    QLineEdit* newPasswordLineEdit;
    QLabel* confirmPasswordLabel;
    QLineEdit* confirmPasswordLineEdit;
    QLabel* confirmHintLabel;
    QPushButton* okButton;

    /********接口*********/
    AccountInformation* accountInformation;
private slots:
    /********控件响应槽函数*********/
    void slotOkButton();    //确定按钮槽函数
    void slotChangeLineEditTextChange();    //lineEdit文字变化槽函数
};

#endif // CHANGEPASSWORDWIDGET_H
