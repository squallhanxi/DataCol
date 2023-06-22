#ifndef FUNC_H
#define FUNC_H

#include "widget.h"

//宏函数：收到心跳注册包的处理流程
#define RECVHEART(stat, timer, label, textEdit, strmsg) \
{\
    if(stat == false)\
    {\
        stat = true;\
        timer->start(DOWN_TIMEOUT);\
        ui->label->setPixmap(QPixmap(":/pic/online.jpg"));\
        ui->textEdit->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));\
        ui->textEdit->append(strmsg);\
    }\
    else\
    {\
        timer->stop();\
        timer->start(DOWN_TIMEOUT);\
    }\
}

//风光互补站数据处理函数声明
void DataProcessA(const QByteArray buffer,
                  const int pos,
                  const int dataheadlen,
                  const int curx,
                  struct data_pkg *cdata,
                  QString *battvol,
                  QString *lightvol,
                  QString *windvol,
                  QString *lightcur,
                  QString *windcur);

//电压检测站数据处理函数声明
void DataProcessB(const QByteArray buffer,
                  const int pos,
                  const int dataheadlen,
                  QString *vol1,
                  QString *vol2);

//查找数据头函数声明
int FindDataHead(QByteArray data,
                 QByteArray src);

#endif // FUNC_H
