#include "func.h"

//解析风光互补站数据
void DataProcessA(const QByteArray buffer,
                  const int pos,
                  const int dataheadlen,
                  const int curx,
                  struct data_pkg *cdata,
                  QString *battvol,
                  QString *lightvol,
                  QString *windvol,
                  QString *lightcur,
                  QString *windcur)
{
    cdata[curx].data_index = curx;
    cdata[curx].now_time = QDateTime::currentDateTime();
    cdata[curx].battvol = ((uchar)buffer[pos+dataheadlen+3] * 256 + (uchar)buffer[pos+dataheadlen+4]) * 0.1;
    cdata[curx].lightvol = ((uchar)buffer[pos+dataheadlen+5] * 256 + (uchar)buffer[pos+dataheadlen+6]) * 0.1;
    cdata[curx].windvol = ((uchar)buffer[pos+dataheadlen+7] * 256 + (uchar)buffer[pos+dataheadlen+8]) * 0.1;
    cdata[curx].lightcur = ((uchar)buffer[pos+dataheadlen+9] * 256 + (uchar)buffer[pos+dataheadlen+10]) * 0.1;
    cdata[curx].windcur = ((uchar)buffer[pos+dataheadlen+11] * 256 + (uchar)buffer[pos+dataheadlen+12]) * 0.1;
    *battvol = QString::number(cdata[curx].battvol);
    *lightvol = QString::number(cdata[curx].lightvol);
    *windvol = QString::number(cdata[curx].windvol);
    *lightcur = QString::number(cdata[curx].lightcur);
    *windcur = QString::number(cdata[curx].windcur);
}

//解析电压查询站数据
void DataProcessB(const QByteArray buffer,
                  const int pos,
                  const int dataheadlen,
                  QString *vol1,
                  QString *vol2)
{
    uchar vol1_4b[4] = {0};
    uchar vol2_4b[4] = {0};
    float vol1_f, vol2_f;
    vol1_4b[0] = buffer[pos+dataheadlen+6];
    vol1_4b[1] = buffer[pos+dataheadlen+5];
    vol1_4b[2] = buffer[pos+dataheadlen+4];
    vol1_4b[3] = buffer[pos+dataheadlen+3];
    vol2_4b[0] = buffer[pos+dataheadlen+10];
    vol2_4b[1] = buffer[pos+dataheadlen+9];
    vol2_4b[2] = buffer[pos+dataheadlen+8];
    vol2_4b[3] = buffer[pos+dataheadlen+7];
    memcpy_s(&vol1_f, sizeof(float), vol1_4b, 4);
    memcpy_s(&vol2_f, sizeof(float), vol2_4b, 4);
    *vol1 = QString::number(vol1_f, 'f', 2);
    *vol2 = QString::number(vol2_f, 'f', 2);
}

//查找数据
int FindDataHead(QByteArray data, QByteArray src)
{
    int j;
    int data_len = data.length();
    for(int i=0; i<=src.length()-data_len; i++)
    {
        j = 0;
        while(j < data_len && data[j] == src[i+j])
        {
            j++;
        }
        if(j == data_len)
        {
            return i;   //找到返回位置
        }
    }
    return -1;  //未找到返回-1
}

