#include "widget.h"
#include "ui_widget.h"
#include "func.h"
#include "loginform.h"

QTcpSocket *tcpClient;  //定义全局tcp
QChart *chart_lh, *chart_csh;  //定义全局图表
QLineSeries *series_lh[5], *series_csh[5];  //定义全局曲线序列
int curX_lh, curX_csh;   //定义全局当前X坐标
QTimer *timer_lh[2], *timer_csh[13], *timer_hb, *timer_led[2], *timer_point_lh, *timer_point_csh, *timer_reconn;   //定义全局计时器
bool isOnline_lh[2], isOnline_csh[13];  //定义是否上线标志
int datahead_len;   //数据头长度
QByteArray datahead_lh[2], datahead_csh[13];   //定义字节数组格式的数据包头
int led_stat[2];   //呼吸灯状态
QString reg_msg;

struct data_pkg cur_data_lh[CHART_MAX_X], cur_data_csh[CHART_MAX_X];

//构造函数
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("长寿区水源地物联网数据平台 V2.3.1.0");
    //setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    //setFixedSize(this->width(),this->height()); //禁止拖动窗口大小
    ui->pushButton_cut->setEnabled(false);  //断开按钮禁用
    //if(chara == 1)  //根据登录角色设置默认显示页面
    //{
    //    ui->tabWidget->setCurrentIndex(0);
    //}
    //else if(chara == 2)
    //{
    //    ui->tabWidget->setCurrentIndex(1);
    //}
    //else
    //{
    //    ui->tabWidget->setCurrentIndex(0);
    //}
    showMaximized();    //最大化

    //读取ini
    QSettings settingsread("Option.ini", QSettings::IniFormat);
    reg_msg = "{\"" + settingsread.value("Option/user").toString() + "\":\"" + settingsread.value("Option/pwd").toString() + "\"}";

    ui->label_s1->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s1_csh->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_1_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_2_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_3_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_4_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_5_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_6_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_7_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_8_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_9_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_10_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_11_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_12_s->setPixmap(QPixmap(":/pic/offline.jpg"));

    timer_lh[0] = new QTimer(this);   //创建计时器
    timer_lh[1] = new QTimer(this);
    timer_hb = new QTimer(this);
    timer_led[0] = new QTimer(this);
    timer_led[1] = new QTimer(this);
    timer_point_lh = new QTimer(this);
    timer_csh[0] = new QTimer(this);
    timer_csh[1] = new QTimer(this);
    timer_csh[2] = new QTimer(this);
    timer_csh[3] = new QTimer(this);
    timer_csh[4] = new QTimer(this);
    timer_csh[5] = new QTimer(this);
    timer_csh[6] = new QTimer(this);
    timer_csh[7] = new QTimer(this);
    timer_csh[8] = new QTimer(this);
    timer_csh[9] = new QTimer(this);
    timer_csh[10] = new QTimer(this);
    timer_csh[11] = new QTimer(this);
    timer_csh[12] = new QTimer(this);
    timer_point_csh = new QTimer(this);
    timer_reconn = new QTimer(this);
    connect(timer_lh[0], SIGNAL(timeout()), this, SLOT(TimeOut_S1()));   //连接超时处理槽函数
    connect(timer_lh[1], SIGNAL(timeout()), this, SLOT(TimeOut_S2()));
    connect(timer_hb, SIGNAL(timeout()), this, SLOT(TimeOut_hb()));
    connect(timer_led[0], SIGNAL(timeout()), this, SLOT(TimeOut_led1()));
    connect(timer_led[1], SIGNAL(timeout()), this, SLOT(TimeOut_led2()));
    connect(timer_point_lh, SIGNAL(timeout()), this, SLOT(TimeOut_point()));
    connect(timer_csh[0], SIGNAL(timeout()), this, SLOT(TimeOut_S1_csh()));
    connect(timer_csh[1], SIGNAL(timeout()), this, SLOT(TimeOut_S2_1_csh()));
    connect(timer_csh[2], SIGNAL(timeout()), this, SLOT(TimeOut_S2_2_csh()));
    connect(timer_csh[3], SIGNAL(timeout()), this, SLOT(TimeOut_S2_3_csh()));
    connect(timer_csh[4], SIGNAL(timeout()), this, SLOT(TimeOut_S2_4_csh()));
    connect(timer_csh[5], SIGNAL(timeout()), this, SLOT(TimeOut_S2_5_csh()));
    connect(timer_csh[6], SIGNAL(timeout()), this, SLOT(TimeOut_S2_6_csh()));
    connect(timer_csh[7], SIGNAL(timeout()), this, SLOT(TimeOut_S2_7_csh()));
    connect(timer_csh[8], SIGNAL(timeout()), this, SLOT(TimeOut_S2_8_csh()));
    connect(timer_csh[9], SIGNAL(timeout()), this, SLOT(TimeOut_S2_9_csh()));
    connect(timer_csh[10], SIGNAL(timeout()), this, SLOT(TimeOut_S2_10_csh()));
    connect(timer_csh[11], SIGNAL(timeout()), this, SLOT(TimeOut_S2_11_csh()));
    connect(timer_csh[12], SIGNAL(timeout()), this, SLOT(TimeOut_S2_12_csh()));
    connect(timer_point_csh, SIGNAL(timeout()), this, SLOT(TimeOut_point_csh()));
    connect(timer_reconn, SIGNAL(timeout()), this, SLOT(TimeOut_reconn()));

    this->CreatChart(); //创建图表
    curX_lh = 0;   //当前X轴坐标置零
    curX_csh = 0;
    isOnline_lh[0] = false;
    isOnline_lh[1] = false;
    isOnline_csh[0] = false;
    isOnline_csh[1] = false;
    isOnline_csh[2] = false;
    isOnline_csh[3] = false;
    isOnline_csh[4] = false;
    isOnline_csh[5] = false;
    isOnline_csh[6] = false;
    isOnline_csh[7] = false;
    isOnline_csh[8] = false;
    isOnline_csh[9] = false;
    isOnline_csh[10] = false;
    isOnline_csh[11] = false;
    isOnline_csh[12] = false;
    led_stat[0] = 0;  //呼吸灯状态
    led_stat[1] = 0;

    datahead_lh[0] = QString(HEAD_S1_LH).toUtf8();  //数据包头初始化
    datahead_lh[1] = QString(HEAD_S2_LH).toUtf8();
    datahead_csh[0] = QString(HEAD_S1_CSH).toUtf8();
    datahead_csh[1] = QString(HEAD_S2_1_CSH).toUtf8();
    datahead_csh[2] = QString(HEAD_S2_2_CSH).toUtf8();
    datahead_csh[3] = QString(HEAD_S2_3_CSH).toUtf8();
    datahead_csh[4] = QString(HEAD_S2_4_CSH).toUtf8();
    datahead_csh[5] = QString(HEAD_S2_5_CSH).toUtf8();
    datahead_csh[6] = QString(HEAD_S2_6_CSH).toUtf8();
    datahead_csh[7] = QString(HEAD_S2_7_CSH).toUtf8();
    datahead_csh[8] = QString(HEAD_S2_8_CSH).toUtf8();
    datahead_csh[9] = QString(HEAD_S2_9_CSH).toUtf8();
    datahead_csh[10] = QString(HEAD_S2_10_CSH).toUtf8();
    datahead_csh[11] = QString(HEAD_S2_11_CSH).toUtf8();
    datahead_csh[12] = QString(HEAD_S2_12_CSH).toUtf8();
    datahead_len = HEAD_LEN;    //数据包头长度
}

//析构函数
Widget::~Widget()
{
    if(tcpClient != NULL)
    {
        tcpClient->disconnectFromHost();
        delete tcpClient;
    }
    delete ui;
}

//收到TCP数据
void Widget::RecvData()
{
    QByteArray buffer = tcpClient->readAll();
    if(!buffer.isEmpty())
    {
        qDebug() << "收到数据->";
        qDebug() << "长度->" << buffer.length();
        qDebug() << buffer;

        int pos_lh[2], pos_csh[13];

        timer_led[1]->start(100);
        timer_reconn->stop();
        timer_reconn->start(RECONN_TIMEOUT);

        pos_lh[0] = FindDataHead(datahead_lh[0], buffer); //对比查找数据头
        pos_lh[1] = FindDataHead(datahead_lh[1], buffer);
        pos_csh[0] = FindDataHead(datahead_csh[0], buffer);
        pos_csh[1] = FindDataHead(datahead_csh[1], buffer);
        pos_csh[2] = FindDataHead(datahead_csh[2], buffer);
        pos_csh[3] = FindDataHead(datahead_csh[3], buffer);
        pos_csh[4] = FindDataHead(datahead_csh[4], buffer);
        pos_csh[5] = FindDataHead(datahead_csh[5], buffer);
        pos_csh[6] = FindDataHead(datahead_csh[6], buffer);
        pos_csh[7] = FindDataHead(datahead_csh[7], buffer);
        pos_csh[8] = FindDataHead(datahead_csh[8], buffer);
        pos_csh[9] = FindDataHead(datahead_csh[9], buffer);
        pos_csh[10] = FindDataHead(datahead_csh[10], buffer);
        pos_csh[11] = FindDataHead(datahead_csh[11], buffer);
        pos_csh[12] = FindDataHead(datahead_csh[12], buffer);

        //收到1号站仁和水厂的心跳注册包
        if(FindDataHead(REG_S1_LH, buffer) != -1)
        {
            RECVHEART(isOnline_lh[0], timer_lh[0], label_s1, textEdit_stat, "->仁和水厂上线！");
        }

        //收到2号站金明水厂的心跳注册包
        if(FindDataHead(REG_S2_LH, buffer) != -1)
        {
            RECVHEART(isOnline_lh[1], timer_lh[1], label_s2, textEdit_stat, "->金明水厂上线！");
        }

        //收到1号站石岭村供水站水源地的心跳注册包
        if(FindDataHead(REG_S1_CSH, buffer) != -1)
        {
            RECVHEART(isOnline_csh[0], timer_csh[0], label_s1_csh, textEdit_stat_csh, "->石岭村供水站水源地上线！");
        }

        //收到2号回龙01的心跳注册包
        if(FindDataHead(REG_S2_1_CSH, buffer) != -1)
        {
            RECVHEART(isOnline_csh[1], timer_csh[1], label_s2_1_s, textEdit_stat_csh, "->长寿湖回龙取水点01上线！");
        }

        //收到2号回龙02的心跳注册包
        if(FindDataHead(REG_S2_2_CSH, buffer) != -1)
        {
            RECVHEART(isOnline_csh[2], timer_csh[2], label_s2_2_s, textEdit_stat_csh, "->长寿湖回龙取水点02上线！");
        }

        //收到2号回龙03的心跳注册包
        if(FindDataHead(REG_S2_3_CSH, buffer) != -1)
        {
            RECVHEART(isOnline_csh[3], timer_csh[3], label_s2_3_s, textEdit_stat_csh, "->长寿湖回龙取水点03上线！");
        }

        //收到2号回龙04的心跳注册包
        if(FindDataHead(REG_S2_4_CSH, buffer) != -1)
        {
            RECVHEART(isOnline_csh[4], timer_csh[4], label_s2_4_s, textEdit_stat_csh, "->长寿湖回龙取水点04上线！");
        }

        //收到2号回龙05的心跳注册包
        if(FindDataHead(REG_S2_5_CSH, buffer) != -1)
        {
            RECVHEART(isOnline_csh[5], timer_csh[5], label_s2_5_s, textEdit_stat_csh, "->长寿湖回龙取水点05上线！");
        }

        //收到2号梁家冲01的心跳注册包
        if(FindDataHead(REG_S2_6_CSH, buffer) != -1)
        {
            RECVHEART(isOnline_csh[6], timer_csh[6], label_s2_6_s, textEdit_stat_csh, "->长寿湖梁家冲水库01上线！");
        }

        //收到2号梁家冲02的心跳注册包
        if(FindDataHead(REG_S2_7_CSH, buffer) != -1)
        {
            RECVHEART(isOnline_csh[7], timer_csh[7], label_s2_7_s, textEdit_stat_csh, "->长寿湖梁家冲水库02上线！");
        }

        //收到2号马达凼01的心跳注册包
        if(FindDataHead(REG_S2_8_CSH, buffer) != -1)
        {
            RECVHEART(isOnline_csh[8], timer_csh[8], label_s2_8_s, textEdit_stat_csh, "->长寿湖马达凼水库01上线！");
        }

        //收到2号马达凼02的心跳注册包
        if(FindDataHead(REG_S2_9_CSH, buffer) != -1)
        {
            RECVHEART(isOnline_csh[9], timer_csh[9], label_s2_9_s, textEdit_stat_csh, "->长寿湖马达凼水库02上线！");
        }

        //收到2号马达凼03的心跳注册包
        if(FindDataHead(REG_S2_10_CSH, buffer) != -1)
        {
            RECVHEART(isOnline_csh[10], timer_csh[10], label_s2_10_s, textEdit_stat_csh, "->长寿湖马达凼水库03上线！");
        }

        //收到2号大石村的心跳注册包
        if(FindDataHead(REG_S2_11_CSH, buffer) != -1)
        {
            RECVHEART(isOnline_csh[11], timer_csh[11], label_s2_11_s, textEdit_stat_csh, "->长寿湖大石村上线！");
        }

        //收到2号花山村的心跳注册包
        if(FindDataHead(REG_S2_12_CSH, buffer) != -1)
        {
            RECVHEART(isOnline_csh[12], timer_csh[12], label_s2_12_s, textEdit_stat_csh, "->长寿湖花山村上线！");
        }

        //风光互补站各项数据临时存储空间
        QString *battvol_S = new QString();
        QString *lightvol_S = new QString();
        QString *windvol_S = new QString();
        QString *lightcur_S = new QString();
        QString *windcur_S = new QString();

        //电压查询站各项数据临时存储空间
        QString *vol1 = new QString();
        QString *vol2 = new QString();

        //收到龙河镇1号站的消息
        if(pos_lh[0] != -1)
        {
            qDebug() << "&&& 收到龙河镇1号站数据 &&&&&&& " << pos_lh[0];
            ui->label_lastdata->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            DataProcessA(buffer, pos_lh[0], datahead_len, curX_lh, cur_data_lh, battvol_S, lightvol_S, windvol_S, lightcur_S, windcur_S);
            ui->lineEdit_battvol->setText(*battvol_S + " V");
            ui->lineEdit_lightvol->setText(*lightvol_S + " V");
            ui->lineEdit_windvol->setText(*windvol_S + " V");
            ui->lineEdit_lightcur->setText(*lightcur_S + " A");
            ui->lineEdit_windcur->setText(*windcur_S + " A");
        }

        //收到龙河镇2号站的消息
        if(pos_lh[1] != -1)
        {
            qDebug() << "&&& 收到龙河镇2号站数据 &&&&&&& " << pos_lh[1];
            ui->label_lastdata->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            DataProcessB(buffer, pos_lh[1], datahead_len, vol1, vol2);
            ui->lineEdit_vol1->setText(*vol1 + " V");
            ui->lineEdit_vol2->setText(*vol2 + " V");
        }

        //收到长寿湖1号站的消息
        if(pos_csh[0] != -1)
        {
            qDebug() << "&&& 收到长寿湖1号站数据 &&&&&&& " << pos_csh[0];
            ui->label_lastdata->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            DataProcessA(buffer, pos_csh[0], datahead_len, curX_csh, cur_data_csh, battvol_S, lightvol_S, windvol_S, lightcur_S, windcur_S);
            ui->lineEdit_battvol_csh->setText(*battvol_S + " V");
            ui->lineEdit_lightvol_csh->setText(*lightvol_S + " V");
            ui->lineEdit_windvol_csh->setText(*windvol_S + " V");
            ui->lineEdit_lightcur_csh->setText(*lightcur_S + " A");
            ui->lineEdit_windcur_csh->setText(*windcur_S + " A");

        }

        //收到长寿湖2-1号站的消息
        if(pos_csh[1] != -1)
        {
            qDebug() << "&&& 收到长寿湖2-1号站数据 &&&&&&& " << pos_csh[1];
            ui->label_lastdata->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            DataProcessB(buffer, pos_csh[1], datahead_len, vol1, vol2);
            ui->lineEdit_vol1_s1->setText(*vol1 + " V");
            ui->lineEdit_vol2_s1->setText(*vol2 + " V");
        }

        //收到长寿湖2-2号站的消息
        if(pos_csh[2] != -1)
        {
            qDebug() << "&&& 收到长寿湖2-2号站数据 &&&&&&& " << pos_csh[2];
            ui->label_lastdata->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            DataProcessB(buffer, pos_csh[2], datahead_len, vol1, vol2);
            ui->lineEdit_vol1_s2->setText(*vol1 + " V");
            ui->lineEdit_vol2_s2->setText(*vol2 + " V");
        }

        //收到长寿湖2-3号站的消息
        if(pos_csh[3] != -1)
        {
            qDebug() << "&&& 收到长寿湖2-3号站数据 &&&&&&& " << pos_csh[3];
            ui->label_lastdata->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            DataProcessB(buffer, pos_csh[3], datahead_len, vol1, vol2);
            ui->lineEdit_vol1_s3->setText(*vol1 + " V");
            ui->lineEdit_vol2_s3->setText(*vol2 + " V");
        }

        //收到长寿湖2-4号站的消息
        if(pos_csh[4] != -1)
        {
            qDebug() << "&&& 收到长寿湖2-4号站数据 &&&&&&& " << pos_csh[4];
            ui->label_lastdata->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            DataProcessB(buffer, pos_csh[4], datahead_len, vol1, vol2);
            ui->lineEdit_vol1_s4->setText(*vol1 + " V");
            ui->lineEdit_vol2_s4->setText(*vol2 + " V");
        }

        //收到长寿湖2-5号站的消息
        if(pos_csh[5] != -1)
        {
            qDebug() << "&&& 收到长寿湖2-5号站数据 &&&&&&& " << pos_csh[5];
            ui->label_lastdata->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            DataProcessB(buffer, pos_csh[5], datahead_len, vol1, vol2);
            ui->lineEdit_vol1_s5->setText(*vol1 + " V");
            ui->lineEdit_vol2_s5->setText(*vol2 + " V");
        }

        //收到长寿湖2-6号站的消息
        if(pos_csh[6] != -1)
        {
            qDebug() << "&&& 收到长寿湖2-6号站数据 &&&&&&& " << pos_csh[6];
            ui->label_lastdata->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            DataProcessB(buffer, pos_csh[6], datahead_len, vol1, vol2);
            ui->lineEdit_vol1_s6->setText(*vol1 + " V");
            ui->lineEdit_vol2_s6->setText(*vol2 + " V");
        }

        //收到长寿湖2-7号站的消息
        if(pos_csh[7] != -1)
        {
            qDebug() << "&&& 收到长寿湖2-7号站数据 &&&&&&& " << pos_csh[7];
            ui->label_lastdata->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            DataProcessB(buffer, pos_csh[7], datahead_len, vol1, vol2);
            ui->lineEdit_vol1_s7->setText(*vol1 + " V");
            ui->lineEdit_vol2_s7->setText(*vol2 + " V");
        }

        //收到长寿湖2-8号站的消息
        if(pos_csh[8] != -1)
        {
            qDebug() << "&&& 收到长寿湖2-8号站数据 &&&&&&& " << pos_csh[8];
            ui->label_lastdata->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            DataProcessB(buffer, pos_csh[8], datahead_len, vol1, vol2);
            ui->lineEdit_vol1_s8->setText(*vol1 + " V");
            ui->lineEdit_vol2_s8->setText(*vol2 + " V");
        }

        //收到长寿湖2-9号站的消息
        if(pos_csh[9] != -1)
        {
            qDebug() << "&&& 收到长寿湖2-9号站数据 &&&&&&& " << pos_csh[9];
            ui->label_lastdata->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            DataProcessB(buffer, pos_csh[9], datahead_len, vol1, vol2);
            ui->lineEdit_vol1_s9->setText(*vol1 + " V");
            ui->lineEdit_vol2_s9->setText(*vol2 + " V");
        }

        //收到长寿湖2-10号站的消息
        if(pos_csh[10] != -1)
        {
            qDebug() << "&&& 收到长寿湖2-10号站数据 &&&&&&& " << pos_csh[10];
            ui->label_lastdata->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            DataProcessB(buffer, pos_csh[10], datahead_len, vol1, vol2);
            ui->lineEdit_vol1_s10->setText(*vol1 + " V");
            ui->lineEdit_vol2_s10->setText(*vol2 + " V");
        }

        //收到长寿湖2-11号站的消息
        if(pos_csh[11] != -1)
        {
            qDebug() << "&&& 收到长寿湖2-11号站数据 &&&&&&& " << pos_csh[11];
            ui->label_lastdata->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            DataProcessB(buffer, pos_csh[11], datahead_len, vol1, vol2);
            ui->lineEdit_vol1_s11->setText(*vol1 + " V");
            ui->lineEdit_vol2_s11->setText(*vol2 + " V");
        }

        //收到长寿湖2-12号站的消息
        if(pos_csh[12] != -1)
        {
            qDebug() << "&&& 收到长寿湖2-12号站数据 &&&&&&& " << pos_csh[12];
            ui->label_lastdata->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            DataProcessB(buffer, pos_csh[12], datahead_len, vol1, vol2);
            ui->lineEdit_vol1_s12->setText(*vol1 + " V");
            ui->lineEdit_vol2_s12->setText(*vol2 + " V");
        }
    }
}

//异常后断开
void Widget::ReadError(QAbstractSocket::SocketError error)
{
    qDebug() << "异常，超时后重连服务器！" ;
    qDebug() << error;
    //tcpClient->disconnectFromHost();
    timer_reconn->stop();
    timer_reconn->start(RECONN_TIMEOUT);
}

//发送TCP数据
void Widget::SendData(QString msg)
{
    if(tcpClient->isOpen())
    {
        QByteArray sendMessage = msg.toUtf8();
        tcpClient->write(sendMessage);
        tcpClient->flush();
    }
}

//计时器龙河1号站点超时
void Widget::TimeOut_S1()
{
    ui->textEdit_stat->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit_stat->append("->仁和水厂掉线！");
    isOnline_lh[0] = false;
    timer_lh[0]->stop();
    ui->label_s1->setPixmap(QPixmap(":/pic/offline.jpg"));
}

//计时器龙河2号站点超时
void Widget::TimeOut_S2()
{
    ui->textEdit_stat->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit_stat->append("->金明水厂掉线！");
    isOnline_lh[1] = false;
    timer_lh[1]->stop();
    ui->label_s2->setPixmap(QPixmap(":/pic/offline.jpg"));
}

//心跳计时器超时
void Widget::TimeOut_hb()
{
    SendData(reg_msg);  //发送注册消息至服务器
    timer_led[0]->start(100);
}

//心跳呼吸灯计时器
void Widget::TimeOut_led1()
{
    QPalette palette;
    QColor cl;
    cl.setRgb(10*(led_stat[0]),255,10*(led_stat[0]));
    led_stat[0]++;
    palette.setColor(QPalette::Base, cl);
    ui->lineEdit_led1->setPalette(palette);
    if(led_stat[0] > 25)
    {
        led_stat[0] = 0;
        timer_led[0]->stop();
    }
}

//收数据呼吸灯计时器
void Widget::TimeOut_led2()
{
    QPalette palette;
    QColor cl;
    cl.setRgb(10*(led_stat[1]),255,10*(led_stat[1]));
    led_stat[1]++;
    palette.setColor(QPalette::Base, cl);
    ui->lineEdit_led2->setPalette(palette);
    if(led_stat[1] > 25)
    {
        led_stat[1] = 0;
        timer_led[1]->stop();
    }
}

//龙河1号站数据刷新打点
void Widget::TimeOut_point()
{
    //画图
    series_lh[0]->append(curX_lh, cur_data_lh[curX_lh].battvol);
    series_lh[1]->append(curX_lh, cur_data_lh[curX_lh].lightvol);
    series_lh[2]->append(curX_lh, cur_data_lh[curX_lh].windvol);
    series_lh[3]->append(curX_lh, cur_data_lh[curX_lh].lightcur);
    series_lh[4]->append(curX_lh, cur_data_lh[curX_lh].windcur);

    if(curX_lh == CHART_MAX_X-1)  //超出后清零
    {
        series_lh[0]->clear();
        series_lh[1]->clear();
        series_lh[2]->clear();
        series_lh[3]->clear();
        series_lh[4]->clear();
        curX_lh = 0;
        ui->label_starttime->setText("T1:[" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm") + "]");
        ui->label_endtime->setText("T2:[" + QDateTime::currentDateTime().addSecs(3600).toString("yyyy-MM-dd hh:mm") + "]");
    }
    else
    {
        curX_lh++;
    }

    //清零
    cur_data_lh[curX_lh].battvol = 0;
    cur_data_lh[curX_lh].lightvol = 0;
    cur_data_lh[curX_lh].windvol = 0;
    cur_data_lh[curX_lh].lightcur = 0;
    cur_data_lh[curX_lh].windcur = 0;
}

//计时器长寿湖1号站超时
void Widget::TimeOut_S1_csh()
{
    ui->textEdit_stat_csh->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit_stat_csh->append("->石岭村供水站水源地掉线！");
    isOnline_csh[0] = false;
    timer_csh[0]->stop();
    ui->label_s1_csh->setPixmap(QPixmap(":/pic/offline.jpg"));
}

//计时器长寿湖2号站超时
void Widget::TimeOut_S2_1_csh()
{
    ui->textEdit_stat_csh->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit_stat_csh->append("->回龙取水点01掉线！");
    isOnline_csh[1] = false;
    timer_csh[1]->stop();
    ui->label_s2_1_s->setPixmap(QPixmap(":/pic/offline.jpg"));
}

//计时器长寿湖3号站超时
void Widget::TimeOut_S2_2_csh()
{
    ui->textEdit_stat_csh->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit_stat_csh->append("->回龙取水点02掉线！");
    isOnline_csh[2] = false;
    timer_csh[2]->stop();
    ui->label_s2_2_s->setPixmap(QPixmap(":/pic/offline.jpg"));
}

//计时器长寿湖4号站超时
void Widget::TimeOut_S2_3_csh()
{
    ui->textEdit_stat_csh->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit_stat_csh->append("->回龙取水点03掉线！");
    isOnline_csh[3] = false;
    timer_csh[3]->stop();
    ui->label_s2_3_s->setPixmap(QPixmap(":/pic/offline.jpg"));
}

//计时器长寿湖5号站超时
void Widget::TimeOut_S2_4_csh()
{
    ui->textEdit_stat_csh->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit_stat_csh->append("->回龙取水点04掉线！");
    isOnline_csh[4] = false;
    timer_csh[4]->stop();
    ui->label_s2_4_s->setPixmap(QPixmap(":/pic/offline.jpg"));
}

//计时器长寿湖6号站超时
void Widget::TimeOut_S2_5_csh()
{
    ui->textEdit_stat_csh->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit_stat_csh->append("->回龙取水点05掉线！");
    isOnline_csh[5] = false;
    timer_csh[5]->stop();
    ui->label_s2_5_s->setPixmap(QPixmap(":/pic/offline.jpg"));
}

//计时器长寿湖7号站超时
void Widget::TimeOut_S2_6_csh()
{
    ui->textEdit_stat_csh->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit_stat_csh->append("->梁家冲水库01掉线！");
    isOnline_csh[6] = false;
    timer_csh[6]->stop();
    ui->label_s2_6_s->setPixmap(QPixmap(":/pic/offline.jpg"));
}

//计时器长寿湖8号站超时
void Widget::TimeOut_S2_7_csh()
{
    ui->textEdit_stat_csh->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit_stat_csh->append("->梁家冲水库02掉线！");
    isOnline_csh[7] = false;
    timer_csh[7]->stop();
    ui->label_s2_7_s->setPixmap(QPixmap(":/pic/offline.jpg"));
}

//计时器长寿湖9号站超时
void Widget::TimeOut_S2_8_csh()
{
    ui->textEdit_stat_csh->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit_stat_csh->append("->马达凼水库01掉线！");
    isOnline_csh[8] = false;
    timer_csh[8]->stop();
    ui->label_s2_8_s->setPixmap(QPixmap(":/pic/offline.jpg"));
}

//计时器长寿湖10号站超时
void Widget::TimeOut_S2_9_csh()
{
    ui->textEdit_stat_csh->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit_stat_csh->append("->马达凼水库02掉线！");
    isOnline_csh[9] = false;
    timer_csh[9]->stop();
    ui->label_s2_9_s->setPixmap(QPixmap(":/pic/offline.jpg"));
}

//计时器长寿湖11号站超时
void Widget::TimeOut_S2_10_csh()
{
    ui->textEdit_stat_csh->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit_stat_csh->append("->马达凼水库03掉线！");
    isOnline_csh[10] = false;
    timer_csh[10]->stop();
    ui->label_s2_10_s->setPixmap(QPixmap(":/pic/offline.jpg"));
}

//计时器长寿湖12号站超时
void Widget::TimeOut_S2_11_csh()
{
    ui->textEdit_stat_csh->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit_stat_csh->append("->大石村掉线！");
    isOnline_csh[11] = false;
    timer_csh[11]->stop();
    ui->label_s2_11_s->setPixmap(QPixmap(":/pic/offline.jpg"));
}

//计时器长寿湖13号站超时
void Widget::TimeOut_S2_12_csh()
{
    ui->textEdit_stat_csh->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit_stat_csh->append("->花山村掉线！");
    isOnline_csh[12] = false;
    timer_csh[12]->stop();
    ui->label_s2_12_s->setPixmap(QPixmap(":/pic/offline.jpg"));
}

//长寿湖1号站数据刷新打点
void Widget::TimeOut_point_csh()
{
    //画图
    series_csh[0]->append(curX_csh, cur_data_csh[curX_csh].battvol);
    series_csh[1]->append(curX_csh, cur_data_csh[curX_csh].lightvol);
    series_csh[2]->append(curX_csh, cur_data_csh[curX_csh].windvol);
    series_csh[3]->append(curX_csh, cur_data_csh[curX_csh].lightcur);
    series_csh[4]->append(curX_csh, cur_data_csh[curX_csh].windcur);

    if(curX_csh == CHART_MAX_X-1)  //超出后清零
    {
        series_csh[0]->clear();
        series_csh[1]->clear();
        series_csh[2]->clear();
        series_csh[3]->clear();
        series_csh[4]->clear();
        curX_csh = 0;
        ui->label_starttime_csh->setText("T1:[" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm") + "]");
        ui->label_endtime_csh->setText("T2:[" + QDateTime::currentDateTime().addSecs(3600).toString("yyyy-MM-dd hh:mm") + "]");
    }
    else
    {
        curX_csh++;
    }

    //清零
    cur_data_csh[curX_csh].battvol = 0;
    cur_data_csh[curX_csh].lightvol = 0;
    cur_data_csh[curX_csh].windvol = 0;
    cur_data_csh[curX_csh].lightcur = 0;
    cur_data_csh[curX_csh].windcur = 0;
}

//收数据超时
void Widget::TimeOut_reconn()
{
    qDebug() << "超时后断开与服务器的连接！";
    ui->pushButton_conn->setEnabled(false);
    ui->pushButton_cut->setEnabled(false);
    tcpClient->disconnectFromHost();
    timer_lh[0]->stop();
    timer_lh[1]->stop();
    timer_hb->stop();
    timer_csh[0]->stop();
    timer_csh[1]->stop();
    timer_csh[2]->stop();
    timer_csh[3]->stop();
    timer_csh[4]->stop();
    timer_csh[5]->stop();
    timer_csh[6]->stop();
    timer_csh[7]->stop();
    timer_csh[8]->stop();
    timer_csh[9]->stop();
    timer_csh[10]->stop();
    timer_csh[11]->stop();
    timer_csh[12]->stop();
    isOnline_lh[0] = false;
    isOnline_lh[1] = false;
    isOnline_csh[0] = false;
    isOnline_csh[1] = false;
    isOnline_csh[2] = false;
    isOnline_csh[3] = false;
    isOnline_csh[4] = false;
    isOnline_csh[5] = false;
    isOnline_csh[6] = false;
    isOnline_csh[7] = false;
    isOnline_csh[8] = false;
    isOnline_csh[9] = false;
    isOnline_csh[10] = false;
    isOnline_csh[11] = false;
    isOnline_csh[12] = false;

    tcpClient = new QTcpSocket(this);   //实例化
    tcpClient->abort(); //取消原有连接
    tcpClient->connectToHost(IPADDR, PORT); //IP以及端口号
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(RecvData()));    //连接收数据的槽函数
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),this,
            SLOT(ReadError(QAbstractSocket::SocketError))); //连接错误处理的槽函数
    if (tcpClient->waitForConnected(CONN_TIMEOUT))  // 连接成功
    {
        ui->pushButton_conn->setEnabled(false);
        ui->pushButton_cut->setEnabled(true);
        SendData(reg_msg);  //发送注册消息至服务器
        timer_hb->start(DOWN_TIMEOUT);
        timer_point_lh->start(POINT_TIMEOUT);
        timer_point_csh->start(POINT_TIMEOUT);
        timer_reconn->start(RECONN_TIMEOUT);
        ui->label_starttime->setText("T1:[" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm") + "]");
        ui->label_endtime->setText("T2:[" + QDateTime::currentDateTime().addSecs(3600).toString("yyyy-MM-dd hh:mm") + "]");
        ui->label_starttime_csh->setText("T1:[" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm") + "]");
        ui->label_endtime_csh->setText("T2:[" + QDateTime::currentDateTime().addSecs(3600).toString("yyyy-MM-dd hh:mm") + "]");
        qDebug() << "已连接到服务器！" ;
    }
    else
    {
        //ui->pushButton_conn->setEnabled(true);
        //ui->pushButton_cut->setEnabled(false);
        timer_reconn->stop();
        timer_reconn->start(RECONN_TIMEOUT);
        qDebug() << "无法连接服务器,重试！" ;
    }
}

//创建波形图表
void Widget::CreatChart()
{
    chart_lh = new QChart();   //创建Chart
    chart_csh = new QChart();
    chart_lh->setTitle("电压电流可视化图");
    chart_csh->setTitle("电压电流可视化图");
    ui->widget_battvol->setChart(chart_lh); //Chart添加到ChartView
    ui->widget_battvol_csh->setChart(chart_csh);

    //创建曲线序列
    series_lh[0] = new QLineSeries();   //创建曲线
    series_lh[0]->setName("蓄电池电压");   //曲线名称
    series_lh[1] = new QLineSeries();
    series_lh[1]->setName("光伏电压");
    series_lh[2] = new QLineSeries();
    series_lh[2]->setName("风机电压");
    series_lh[3] = new QLineSeries();
    series_lh[3]->setName("光伏电流");
    series_lh[4] = new QLineSeries();
    series_lh[4]->setName("风机电流");
    chart_lh->addSeries(series_lh[0]);  //曲线添加到Chart
    chart_lh->addSeries(series_lh[1]);
    chart_lh->addSeries(series_lh[2]);
    chart_lh->addSeries(series_lh[3]);
    chart_lh->addSeries(series_lh[4]);
    series_csh[0] = new QLineSeries();   //创建曲线
    series_csh[0]->setName("蓄电池电压");   //曲线名称
    series_csh[1] = new QLineSeries();
    series_csh[1]->setName("光伏电压");
    series_csh[2] = new QLineSeries();
    series_csh[2]->setName("风机电压");
    series_csh[3] = new QLineSeries();
    series_csh[3]->setName("光伏电流");
    series_csh[4] = new QLineSeries();
    series_csh[4]->setName("风机电流");
    chart_csh->addSeries(series_csh[0]);  //曲线添加到Chart
    chart_csh->addSeries(series_csh[1]);
    chart_csh->addSeries(series_csh[2]);
    chart_csh->addSeries(series_csh[3]);
    chart_csh->addSeries(series_csh[4]);

    //创建坐标
    QValueAxis *axisX = new QValueAxis();   //X轴
    QValueAxis *axisX_csh = new QValueAxis();
    axisX->setRange(0, CHART_MAX_X);
    axisX->setTitleText("相对时间");
    axisX_csh->setRange(0, CHART_MAX_X);
    axisX_csh->setTitleText("相对时间");
    QValueAxis *axisY = new QValueAxis();   //Y轴
    QValueAxis *axisY_csh = new QValueAxis();
    axisY->setRange(0, CHART_MAX_Y);
    axisY->setTitleText("电压/电流");
    axisY_csh->setRange(0, CHART_MAX_Y);
    axisY_csh->setTitleText("电压/电流");

    chart_lh->setAxisX(axisX, series_lh[0]);
    chart_lh->setAxisY(axisY, series_lh[0]);
    chart_lh->setAxisX(axisX, series_lh[1]);
    chart_lh->setAxisY(axisY, series_lh[1]);
    chart_lh->setAxisX(axisX, series_lh[2]);
    chart_lh->setAxisY(axisY, series_lh[2]);
    chart_lh->setAxisX(axisX, series_lh[3]);
    chart_lh->setAxisY(axisY, series_lh[3]);
    chart_lh->setAxisX(axisX, series_lh[4]);
    chart_lh->setAxisY(axisY, series_lh[4]);
    chart_csh->setAxisX(axisX_csh, series_csh[0]);
    chart_csh->setAxisY(axisY_csh, series_csh[0]);
    chart_csh->setAxisX(axisX_csh, series_csh[1]);
    chart_csh->setAxisY(axisY_csh, series_csh[1]);
    chart_csh->setAxisX(axisX_csh, series_csh[2]);
    chart_csh->setAxisY(axisY_csh, series_csh[2]);
    chart_csh->setAxisX(axisX_csh, series_csh[3]);
    chart_csh->setAxisY(axisY_csh, series_csh[3]);
    chart_csh->setAxisX(axisX_csh, series_csh[4]);
    chart_csh->setAxisY(axisY_csh, series_csh[4]);
}

void Widget::on_pushButton_conn_clicked()
{
    tcpClient = new QTcpSocket(this);   //实例化
    tcpClient->abort(); //取消原有连接
    tcpClient->connectToHost(IPADDR, PORT); //IP以及端口号

    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(RecvData()));    //连接收数据的槽函数
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),this,
            SLOT(ReadError(QAbstractSocket::SocketError))); //连接错误处理的槽函数

    if (tcpClient->waitForConnected(CONN_TIMEOUT))  // 连接成功
    {
        ui->pushButton_conn->setEnabled(false);
        ui->pushButton_cut->setEnabled(true);
        SendData(reg_msg);  //发送注册消息至服务器
        timer_hb->start(DOWN_TIMEOUT);
        timer_point_lh->start(POINT_TIMEOUT);
        timer_point_csh->start(POINT_TIMEOUT);
        timer_reconn->start(RECONN_TIMEOUT);
        ui->label_starttime->setText("T1:[" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm") + "]");
        ui->label_endtime->setText("T2:[" + QDateTime::currentDateTime().addSecs(3600).toString("yyyy-MM-dd hh:mm") + "]");
        ui->label_starttime_csh->setText("T1:[" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm") + "]");
        ui->label_endtime_csh->setText("T2:[" + QDateTime::currentDateTime().addSecs(3600).toString("yyyy-MM-dd hh:mm") + "]");
        series_lh[0]->clear();
        series_lh[1]->clear();
        series_lh[2]->clear();
        series_lh[3]->clear();
        series_lh[4]->clear();
        curX_lh = 0;
        series_csh[0]->clear();
        series_csh[1]->clear();
        series_csh[2]->clear();
        series_csh[3]->clear();
        series_csh[4]->clear();
        curX_csh = 0;
        SendData(reg_msg);
        qDebug() << "已连接到服务器！" ;
    }
    else
    {
        ui->pushButton_conn->setEnabled(true);
        ui->pushButton_cut->setEnabled(false);
        qDebug() << "无法连接服务器！" ;
    }
}

void Widget::on_pushButton_cut_clicked()
{
    tcpClient->disconnectFromHost();
    ui->pushButton_conn->setEnabled(true);
    ui->pushButton_cut->setEnabled(false);
    ui->label_s1->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s1_csh->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_1_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_2_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_3_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_4_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_5_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_6_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_7_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_8_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_9_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_10_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_11_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    ui->label_s2_12_s->setPixmap(QPixmap(":/pic/offline.jpg"));
    timer_lh[0]->stop();
    timer_lh[1]->stop();
    timer_hb->stop();
    timer_csh[0]->stop();
    timer_csh[1]->stop();
    timer_csh[2]->stop();
    timer_csh[3]->stop();
    timer_csh[4]->stop();
    timer_csh[5]->stop();
    timer_csh[6]->stop();
    timer_csh[7]->stop();
    timer_csh[8]->stop();
    timer_csh[9]->stop();
    timer_csh[10]->stop();
    timer_csh[11]->stop();
    timer_csh[12]->stop();
    timer_reconn->stop();
    isOnline_lh[0] = false;
    isOnline_lh[1] = false;
    isOnline_csh[0] = false;
    isOnline_csh[1] = false;
    isOnline_csh[2] = false;
    isOnline_csh[3] = false;
    isOnline_csh[4] = false;
    isOnline_csh[5] = false;
    isOnline_csh[6] = false;
    isOnline_csh[7] = false;
    isOnline_csh[8] = false;
    isOnline_csh[9] = false;
    isOnline_csh[10] = false;
    isOnline_csh[11] = false;
    isOnline_csh[12] = false;
    qDebug() << "用户主动断开与服务器的连接！" ;
}

//窗体改变大小时执行,各控件根据大小刷新位置和尺寸
void Widget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    ui->groupBox_stat->setGeometry(ui->groupBox_stat->geometry().x(),
                                   ui->groupBox_stat->geometry().y(),
                                   ui->groupBox_stat->geometry().width(),
                                   this->geometry().height()-496);
    ui->groupBox_stat_csh->setGeometry(ui->groupBox_stat_csh->geometry().x(),
                                   ui->groupBox_stat_csh->geometry().y(),
                                   ui->groupBox_stat_csh->geometry().width(),
                                   this->geometry().height()-646);
    ui->textEdit_stat->setGeometry(ui->textEdit_stat->geometry().x(),
                                   ui->textEdit_stat->geometry().y(),
                                   ui->textEdit_stat->geometry().width(),
                                   ui->groupBox_stat->geometry().height()-30);
    ui->textEdit_stat_csh->setGeometry(ui->textEdit_stat_csh->geometry().x(),
                                   ui->textEdit_stat_csh->geometry().y(),
                                   ui->textEdit_stat_csh->geometry().width(),
                                   ui->groupBox_stat_csh->geometry().height()-30);
    ui->label_led->setGeometry(ui->label_led->geometry().x(),
                               this->geometry().height()-26,
                               ui->label_led->geometry().width(),
                               ui->label_led->geometry().height());
    ui->label_lastdata->setGeometry(ui->label_lastdata->geometry().x(),
                                    ui->label_led->geometry().y(),
                                    ui->label_lastdata->geometry().width(),
                                    ui->label_lastdata->geometry().height());
    ui->label_heartled->setGeometry(ui->label_heartled->geometry().x(),
                                    ui->label_led->geometry().y()-2,
                                    ui->label_heartled->geometry().width(),
                                    ui->label_heartled->geometry().height());
    ui->label_recvdataled->setGeometry(ui->label_recvdataled->geometry().x(),
                                    ui->label_led->geometry().y()-2,
                                    ui->label_recvdataled->geometry().width(),
                                    ui->label_recvdataled->geometry().height());
    ui->lineEdit_led1->setGeometry(ui->lineEdit_led1->geometry().x(),
                                   ui->label_led->geometry().y()-1,
                                   ui->lineEdit_led1->geometry().width(),
                                   ui->lineEdit_led1->geometry().height());
    ui->lineEdit_led2->setGeometry(ui->lineEdit_led2->geometry().x(),
                                   ui->label_led->geometry().y()-1,
                                   ui->lineEdit_led2->geometry().width(),
                                   ui->lineEdit_led2->geometry().height());
    ui->groupBox_battvol->setGeometry(ui->groupBox_battvol->geometry().x(),
                                      ui->groupBox_battvol->geometry().y(),
                                      this->geometry().width()-300,
                                      this->geometry().height()-166);
    ui->groupBox_battvol_csh->setGeometry(ui->groupBox_battvol->geometry().x(),
                                          ui->groupBox_battvol->geometry().y(),
                                          this->geometry().width()-300,
                                          this->geometry().height()-166);
    ui->widget_battvol->setGeometry(ui->widget_battvol->geometry().x(),
                                    ui->widget_battvol->geometry().y(),
                                    this->geometry().width()-320,
                                    this->geometry().height()-196);
    ui->widget_battvol_csh->setGeometry(ui->widget_battvol->geometry().x(),
                                    ui->widget_battvol->geometry().y(),
                                    this->geometry().width()-320,
                                    this->geometry().height()-196);
    ui->label_starttime->setGeometry(ui->label_starttime->geometry().x(),
                                     ui->widget_battvol->geometry().height()-41,
                                     ui->label_starttime->geometry().width(),
                                     ui->label_starttime->geometry().height());
    ui->label_endtime->setGeometry(ui->widget_battvol->geometry().width()-141,
                                     ui->label_starttime->geometry().y(),
                                     ui->label_starttime->geometry().width(),
                                     ui->label_starttime->geometry().height());
    ui->label_starttime_csh->setGeometry(ui->label_starttime->geometry().x(),
                                     ui->label_starttime->geometry().y(),
                                     ui->label_starttime->geometry().width(),
                                     ui->label_starttime->geometry().height());
    ui->label_endtime_csh->setGeometry(ui->label_endtime->geometry().x(),
                                     ui->label_endtime->geometry().y(),
                                     ui->label_endtime->geometry().width(),
                                     ui->label_endtime->geometry().height());
    ui->tabWidget->setGeometry(ui->tabWidget->geometry().x(),
                               ui->tabWidget->geometry().y(),
                               this->geometry().width()-19,
                               this->geometry().height()-126);
    ui->label_lhcsh1->setGeometry(ui->tabWidget->geometry().x()+ui->tabWidget->geometry().width()-251,
                                  ui->label_lhcsh1->geometry().y(),
                                  ui->label_lhcsh1->geometry().width(),
                                  ui->label_lhcsh1->geometry().height());
    ui->label_lhcsh2->setGeometry(ui->label_lhcsh1->geometry().x()+36,
                                  ui->label_lhcsh2->geometry().y(),
                                  ui->label_lhcsh2->geometry().width(),
                                  ui->label_lhcsh2->geometry().height());
    ui->label_lhcsh3->setGeometry(ui->label_lhcsh1->geometry().x()+36,
                                  ui->label_lhcsh3->geometry().y(),
                                  ui->label_lhcsh3->geometry().width(),
                                  ui->label_lhcsh3->geometry().height());
    ui->label_jszc->setGeometry(ui->label_lhcsh1->geometry().x(),
                             ui->label_led->geometry().y()-1,
                             ui->label_jszc->geometry().width(),
                             ui->label_jszc->geometry().height());
}
