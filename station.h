#ifndef WUHANSUBWAY_STATION_H
#define WUHANSUBWAY_STATION_H

#include <QString>
#include <QSet>
#include <QtGlobal>
#include <QPointF>

// 地铁站类
class Station {
public:
    Station();
    // 赋值运算符重载
    Station& operator=(const Station& other);

    // 函数：将角度转换为弧度
    static double toRadians(double degree);
    // 函数：计算站点间地理直线距离
    double distance(const Station& other) const;
    // 函数：获取站点的地理点
    QPointF getCoord() const;
    // 函数：获取站点所属线路名数组，并转换为字符串
    QString getLineInfo() const;
    // 函数：获取两个站点的共同所属线路名数组
    QVector<QString> getCommonLineNames(Station& other);

    QString id;                         // ID
    QString name;                       // 名称
    double longitude{}, latitude{};     // 经纬度
    QVector<QString> lineNames;         // 所属线路名，同一站点可能属于多条线路

    static double minLongitude;         // 最小经度
    static double minLatitude;          // 最小纬度
    static double maxLongitude;         // 最大经度
    static double maxLatitude;          // 最大纬度

    const double R = 6371.0;            // 地球半径

};


#endif //WUHANSUBWAY_STATION_H
