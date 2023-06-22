#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QSettings>
#include "widget.h"

#define LOGIN_TIMEOUT 2000  //登录超时时间

class LoginForm : public QDialog
{
    Q_OBJECT //使用信号与槽需要的宏
public:
    explicit LoginForm(QDialog *parent = 0);  //explicit 防止歧义

signals:

public slots:
    void login();   //点击登录按钮是执行的槽函数
    void RecvLogin();   //收到登录消息的槽函数
    void SendLogin(QString msg);    //发送消息函数
    void UpdeteINI(QString ur, QString pw); //更新配置文件
    void TimeOut_tout();    //登录超时处理函数

private:
    QLabel *userNameLbl;         //"用户名"标签
    QLabel *pwdLbl;              //"密码"标签
    QLineEdit *userNameLEd;      //用户名编辑行
    QLineEdit *pwdLEd;           //密码编辑行
    QPushButton *loginBtn;       //登录按钮
    QPushButton *exitBtn;        //退出按钮
};

#endif // LOGINFORM_H
