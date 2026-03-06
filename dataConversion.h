#ifndef DATACONVERSION_H
#define DATACONVERSION_H


#include <QtCore>
#include <math.h>
//数据转换类
class DataConversion
{
public:
    DataConversion();

    void double2bytes(double data,unsigned char bytes[]);
    void int2bytes(int data, unsigned char bytes[]);
    bool equal(double a, double b);
    void LonLat2UTM(double longitude, double latitude, double& UTME, double& UTMN);

};

#endif // DATACONVERSION_H
