#ifndef SUBWAYGRAPH_H
#define SUBWAYGRAPH_H

#include "line.h"
#include "station.h"

#include <QPair>
#include <QString>
#include <QHash>
#include <QVector>
#include <QSet>
#include <QPointF>

class Line;
class Station;

class SubwayGraph
{
public:
    // 构造函数
    SubwayGraph();
    // 函数：读取文件数据
    bool readFileData(QString fileName);
    // 函数：获取所有线路名
    QVector<QString> getAllLineNames();
    // 函数：获取两个站点的公共所属线路
    QVector<QString> getCommonLineIDs(QString s1, QString s2);
    // 函数：获取所有线路
    QVector<Line> getAllLines();
    // 函数：根据线路名称获得线路ID
    QString getLineID(QString lineName);
    // 函数：根据线路ID获得线路
    Line getLine(QString lineId);
    // 函数：获取所有站点
    QVector<Station> getAllStations();
    // 函数：获取所有连接
    QVector<Edge> getAllEdges();
    // 函数：根据站点ID获取站点的经纬度
    QPointF getStationCoord(QString stationID);
    // 函数：根据站点获取站点的经纬度
    QPointF getStationCoord(Station& station);
    // 函数：获取最小经纬度
    QPointF getMinCoord();
    // 函数：获取最大经纬度
    QPointF getmaxCoord();
    // 函数：根据站点ID获取站点名
    QString getStationName(QString stationID);
    // 函数：根据线路ID获取线路颜色
    QColor getLineColor(QString lineID);
    // 函数：根据站点获取站点名
    QString getStationName(Station& station);
    // 函数：根据站点获取站点所属线路ID
    QVector<QString> getStationLineID(Station station);
    // 函数：根据站点获取站点所属线路，并转换为字符串
    QString getStationLineInfo(Station& station);
    // 函数：根据线路ID数组获取站点所属线路，转换为字符串
    QString getStationLineInfo(QVector<QString> lineIDs);
    // 函数：根据名称查找线路是否已存在
    bool isLineExist(QString lineName);
    // 函数：添加一条线路
    void addLine(QString lineName, QColor lineColor);

private:
    QVector<QString> lineNames;                                 // 数组，索引 -> 线路名称
    QHash<QString, QString> lineIDs;                            // 哈希表，线路名称 -> 线路ID
    QHash<QString, Line> lines;                                 // 哈希表，线路ID -> 线路

    QHash<QString, Station> stations;                           // 哈希表，站点ID -> 站点
    QSet<Edge> edges;                                           // 集合，存储所有站点连接（边）

};

#endif // SUBWAYGRAPH_H
