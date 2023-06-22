#ifndef WIDGET_H
#define WIDGET_H

#define IPADDR "211.139.56.136" //服务器IP地址
//#define IPADDR "115.29.109.104"
#define PORT 6593   //服务器端口
#define CONN_TIMEOUT 3000   //连接超时（ms）
#define DOWN_TIMEOUT 40000  //掉线超时（ms）
#define POINT_TIMEOUT 30000 //画图打点超时（ms）
#define RECONN_TIMEOUT 60000   //接收数据超时（ms）
//#define REG_MSG "{\"se\":\"ht\"}"   //注册为控制台的消息
#define REG_S1_LH "{\"id\":\"100001\",\"hb\":\"online\"}"    //龙河镇1号站心跳注册包
#define REG_S2_LH "{\"id\":\"100002\",\"hb\":\"online\"}"    //龙河镇2号站心跳注册包
#define REG_S1_CSH "{\"id\":\"100101\",\"hb\":\"online\"}"    //长寿湖1号站心跳注册包
#define REG_S2_1_CSH "{\"id\":\"100102\",\"hb\":\"online\"}"    //长寿湖2-1号站心跳注册包
#define REG_S2_2_CSH "{\"id\":\"100103\",\"hb\":\"online\"}"    //长寿湖2-2号站心跳注册包
#define REG_S2_3_CSH "{\"id\":\"100104\",\"hb\":\"online\"}"    //长寿湖2-3号站心跳注册包
#define REG_S2_4_CSH "{\"id\":\"100105\",\"hb\":\"online\"}"    //长寿湖2-4号站心跳注册包
#define REG_S2_5_CSH "{\"id\":\"100106\",\"hb\":\"online\"}"    //长寿湖2-5号站心跳注册包
#define REG_S2_6_CSH "{\"id\":\"100107\",\"hb\":\"online\"}"    //长寿湖2-6号站心跳注册包
#define REG_S2_7_CSH "{\"id\":\"100108\",\"hb\":\"online\"}"    //长寿湖2-7号站心跳注册包
#define REG_S2_8_CSH "{\"id\":\"100109\",\"hb\":\"online\"}"    //长寿湖2-8号站心跳注册包
#define REG_S2_9_CSH "{\"id\":\"100110\",\"hb\":\"online\"}"    //长寿湖2-9号站心跳注册包
#define REG_S2_10_CSH "{\"id\":\"100111\",\"hb\":\"online\"}"    //长寿湖2-10号站心跳注册包
#define REG_S2_11_CSH "{\"id\":\"100112\",\"hb\":\"online\"}"    //长寿湖2-11号站心跳注册包
#define REG_S2_12_CSH "{\"id\":\"100113\",\"hb\":\"online\"}"    //长寿湖2-12号站心跳注册包
#define HEAD_S1_LH "{\"hd\":\"100001\"}"    //1号站数据包头
#define HEAD_S2_LH "{\"hd\":\"100002\"}"    //2号站数据包头
#define HEAD_S1_CSH "{\"hd\":\"100101\"}"    //长寿湖1号站数据包头
#define HEAD_S2_1_CSH "{\"hd\":\"100102\"}"    //长寿湖2-1号站数据包头
#define HEAD_S2_2_CSH "{\"hd\":\"100103\"}"    //长寿湖2-2号站数据包头
#define HEAD_S2_3_CSH "{\"hd\":\"100104\"}"    //长寿湖2-3号站数据包头
#define HEAD_S2_4_CSH "{\"hd\":\"100105\"}"    //长寿湖2-4号站数据包头
#define HEAD_S2_5_CSH "{\"hd\":\"100106\"}"    //长寿湖2-5号站数据包头
#define HEAD_S2_6_CSH "{\"hd\":\"100107\"}"    //长寿湖2-6号站数据包头
#define HEAD_S2_7_CSH "{\"hd\":\"100108\"}"    //长寿湖2-7号站数据包头
#define HEAD_S2_8_CSH "{\"hd\":\"100109\"}"    //长寿湖2-8号站数据包头
#define HEAD_S2_9_CSH "{\"hd\":\"100110\"}"    //长寿湖2-9号站数据包头
#define HEAD_S2_10_CSH "{\"hd\":\"100111\"}"    //长寿湖2-10号站数据包头
#define HEAD_S2_11_CSH "{\"hd\":\"100112\"}"    //长寿湖2-11号站数据包头
#define HEAD_S2_12_CSH "{\"hd\":\"100113\"}"    //长寿湖2-12号站数据包头
#define HEAD_LEN 15 //数据包头长度
#define CHART_MAX_X 120 //半分钟1次上报，120为1小时
#define CHART_MAX_Y 50  //电压电流共用

#include <QWidget>
#include <QDebug>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include <QTimer>
#include <QDateTime>

using namespace QtCharts;

struct data_pkg  //定义当前实时数据包结构
{
    int data_index;
    QDateTime now_time;
    float battvol;
    float lightvol;
    float windvol;
    float lightcur;
    float windcur;
};

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    //客户端槽函数
    void RecvData();
    void ReadError(QAbstractSocket::SocketError error);
    void SendData(QString msg);
    void TimeOut_S1();
    void TimeOut_S2();
    void TimeOut_hb();
    void TimeOut_led1();
    void TimeOut_led2();
    void TimeOut_point();
    void TimeOut_S1_csh();
    void TimeOut_S2_1_csh();
    void TimeOut_S2_2_csh();
    void TimeOut_S2_3_csh();
    void TimeOut_S2_4_csh();
    void TimeOut_S2_5_csh();
    void TimeOut_S2_6_csh();
    void TimeOut_S2_7_csh();
    void TimeOut_S2_8_csh();
    void TimeOut_S2_9_csh();
    void TimeOut_S2_10_csh();
    void TimeOut_S2_11_csh();
    void TimeOut_S2_12_csh();
    void TimeOut_point_csh();
    void TimeOut_reconn();

    void on_pushButton_conn_clicked();
    void on_pushButton_cut_clicked();

    virtual void resizeEvent(QResizeEvent *event) override;

private:
    Ui::Widget *ui;
    void CreatChart();
    //int FindDataHead(QByteArray data, QByteArray src);
};

#endif // WIDGET_H
