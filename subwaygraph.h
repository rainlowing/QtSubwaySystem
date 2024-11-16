#ifndef SUBWAYGRAPH_H
#define SUBWAYGRAPH_H

#include "line.h"
#include "station.h"
#include "edge.h"

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
    // 函数：构建图
    void makeGraph();
    // 函数：获取所有线路名
    QVector<QString> getAllLineNames();
    // 函数：获取所有线路
    QVector<Line> getAllLines();
    // 函数：根据线路名获取该线路上所有线路名数组
    QVector<QString> getAllStationNamesByLineName(QString lineName);
    // 函数：获取所有站点
    QVector<Station> getAllStations();
    // 函数：获取所有连接
    QVector<Edge> getAllEdges();
    // 函数：获取最小经纬度
    QPointF getMinCoord();
    // 函数：获取最大经纬度
    QPointF getmaxCoord();
    // 函数：根据站点ID获取站点名
    QString getStationNameByID(QString stationID);
    // 函数：根据线路名获取线路颜色
    QColor getLineColor(QString lineName);
    // 函数：根据站点名获取站点
    Station* getStationByName(QString stationName);
    // 函数：将线路名数组转换为字符串
    QString toLineNamesToString(QVector<QString> lineNames);
    // 函数：根据名称查找线路是否已存在
    bool isLineExist(QString lineName);
    // 函数：根据名称查找站点是否已存在
    bool isStationExist(QString stationName);
    // 函数：添加一条线路
    void addLine(QString lineName, QColor lineColor);
    // 函数：计算途径站数最少路线
    bool leastStations();
    // 函数：计算搭乘时间最短路线
    bool leastTime(QString s1Name, QString s2Name, QVector<Station>& resS, QVector<Edge>& resE);

private:
    QVector<Line> lines;                                    // 数组，存储所有线路
    QHash<QString, Line*> lineHash;                         // 哈希表，线路名 -> 线路

    QVector<Station> stations;                              // 数组，存储所有站点
    QHash<QString, Station*> stationHash;                   // 哈希表，站点名 -> 站点

    QVector<Edge> edges;                                    // 数组，存储所有连接

//    QVector<QString> lineNames;                                 // 数组，索引 -> 线路名称
//    QHash<QString, QString> lineIDs;                            // 哈希表，线路名称 -> 线路ID
//    QHash<QString, Line> lines;                                 // 哈希表，线路ID -> 线路
//
//    QHash<QString, Station> stations;                           // 哈希表，站点ID -> 站点
//    QSet<Edge> edges;                                           // 集合，存储所有站点连接（边）


    // 图，键为当前站点名，值为键值对数组，其中键为邻接站点名，值为当前站点到邻接站点的距离
    QHash<QString, QVector<QPair<QString, double>>> graph;

};

#endif // SUBWAYGRAPH_H
