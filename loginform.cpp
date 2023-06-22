#include "loginform.h"

QTcpSocket *tcpClient_login;  //定义登录tcp
QTimer *timer_tout; //定义登录超时计时器

LoginForm::LoginForm(QDialog *parent) :
    QDialog(parent)
{
    this->setWindowTitle(tr("登录界面"));   //设置窗体标题
    timer_tout = new QTimer(this);   //创建计时器
    connect(timer_tout, SIGNAL(timeout()), this, SLOT(TimeOut_tout()));   //连接超时处理槽函数

    //用户名Label
    userNameLbl = new QLabel(this);   //new一个标签对象
    userNameLbl->move(70, 80);  //移动到(70,80)位置(Label左上角坐标，相对于父窗体)
    userNameLbl->setText("用户名:");  //设置标签文本

    //用户名输入框
    userNameLEd = new QLineEdit(this);
    userNameLEd->move(120, 80);
    userNameLEd->setPlaceholderText(tr("请输入用户名!"));//占位符

    //密码Label
    pwdLbl = new QLabel(this);
    pwdLbl->move(80,130);
    pwdLbl->setText("密码:");

    //密码输入框
    pwdLEd = new QLineEdit(this);
    pwdLEd->move(120, 130);
    pwdLEd->setPlaceholderText("请输入密码!");
    pwdLEd->setEchoMode(QLineEdit::Password);//输入的密码以圆点显示

    //登录按钮
    loginBtn = new QPushButton(this);
    loginBtn->move(80, 200);
    loginBtn->setText("登录");

    //退出按钮
    exitBtn = new QPushButton(this);
    exitBtn->move(170, 200);
    exitBtn->setText("退出");

    //单击登录按钮时 执行 LoginForm::login 槽函数(自定义)；单击退出按钮时 执行 LoginForm::close 槽函数(窗体的关闭函数，不用自己写)
    connect(loginBtn, &QPushButton::clicked, this, &LoginForm::login);
    connect(exitBtn, &QPushButton::clicked, this, &LoginForm::close);

    //读取ini
    QSettings settingsread("Option.ini", QSettings::IniFormat);
    userNameLEd->setText(settingsread.value("Option/user").toString());
    pwdLEd->setText(settingsread.value("Option/pwd").toString());
}

void LoginForm::login()
{
    tcpClient_login = new QTcpSocket(this);   //实例化
    tcpClient_login->abort(); //取消原有连接
    tcpClient_login->connectToHost(IPADDR, PORT); //IP以及端口号
    connect(tcpClient_login, SIGNAL(readyRead()), this, SLOT(RecvLogin()));    //连接收数据的槽函数

    if (tcpClient_login->waitForConnected(CONN_TIMEOUT))  // 连接成功
    {
        SendLogin("{\"" + userNameLEd->text().trimmed() + "\":\"" + pwdLEd->text() + "\"}");  //发送注册消息至服务器
        qDebug() << "注册消息已发送到服务器！";
        timer_tout->start(LOGIN_TIMEOUT);
        UpdeteINI(userNameLEd->text().trimmed(), pwdLEd->text());
    }
    else
    {
        UpdeteINI(userNameLEd->text().trimmed(), pwdLEd->text());
        qDebug() << "无法连接注册服务器！";
    }

    //===============trimmed()去掉前后空格
    //===============tr()函数，防止设置中文时乱码
}

//发送TCP数据
void LoginForm::SendLogin(QString msg)
{
    if(tcpClient_login->isOpen())
    {
        QByteArray sendMessage = msg.toUtf8();
        tcpClient_login->write(sendMessage);
        tcpClient_login->flush();
    }
}

//更新ini文件
void LoginForm::UpdeteINI(QString ur, QString pw)
{
    QSettings settings("Option.ini", QSettings::IniFormat);
    settings.beginGroup("Option");
    settings.setValue("user", ur);
    settings.setValue("pwd", pw);
    settings.endGroup();
}

//收到TCP数据
void LoginForm::RecvLogin()
{
    QByteArray buffer = tcpClient_login->readAll();
    if(!buffer.isEmpty())
    {
        qDebug() << "收到登录消息->";
        qDebug() << "长度->" << buffer.length();
        qDebug() << buffer;

        if(QString::fromUtf8(buffer) == "{\"" + userNameLEd->text().trimmed() + "\":\"" + pwdLEd->text() + "\"}")
        {
            tcpClient_login->abort(); //取消连接
            tcpClient_login->close();
            timer_tout->stop();
            accept();   //关闭窗体，并设置返回值为Accepted
            qDebug() << "登录成功";
        }
        else
        {
            QMessageBox::warning(this, tr("警告！"), tr("用户名或密码错误！"), QMessageBox::Yes);
            //userNameLEd->clear();   //清空输入框内容
            //pwdLEd->clear();
            //userNameLEd->setFocus();    //光标定位
            qDebug() << "登录失败";
        }
    }
}

//登录超时处理函数
void LoginForm::TimeOut_tout()
{
    QMessageBox::warning(this, tr("警告！"), tr("用户名或密码错误！"), QMessageBox::Yes);
    //userNameLEd->clear();   //清空输入框内容
    //pwdLEd->clear();
    //userNameLEd->setFocus();    //光标定位
    timer_tout->stop();
    qDebug() << "登录超时";
}
