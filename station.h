#ifndef WUHANSUBWAY_STATION_H
#define WUHANSUBWAY_STATION_H

#include "subwaygraph.h"

#include <QString>
#include <QSet>
#include <QtGlobal>

// 地铁站类
class Station {
public:
    Station();
    Station(QString name, double longi, double lati, QList<int> linesList);

protected:
    QString id;                         // ID
    QString name;                       // 名称
    double longitude, latitude;         // 经纬度
    QVector<QString> lineIDs;           // 所属线路ID，同一站点可能属于多条线路

    static double minLongitude;         // 最小经度
    static double minLatitude;          // 最小纬度
    static double maxLongitude;         // 最大经度
    static double maxLatitude;          // 最大纬度

    friend class SubwayGraph;
};


#endif //WUHANSUBWAY_STATION_H
