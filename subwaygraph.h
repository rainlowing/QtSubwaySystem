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
    // 析构函数
    ~SubwayGraph();

    // 函数：读取文件数据
    bool readFileData(const QString& fileName);
    // 函数：构建图
    void makeGraph();
    // 函数：获取所有线路名
    QVector<QString> getAllLineNames();
    // 函数：获取所有线路
    QVector<Line> getAllLines();
    // 函数：获取所有站点
    QVector<Station> getAllStations();
    // 函数：获取所有连接
    QVector<Edge> getAllEdges();
    // 函数：获取最小经纬度
    static QPointF getMinCoord();
    // 函数：获取最大经纬度
    static QPointF getMaxCoord();
    // 函数：根据线路名获取线路
    Line getLineByName(const QString& lineName);
    // 函数：根据线路名获取线路颜色
    QColor getLineColor(const QString& lineName);
    // 函数：根据站点名获取站点的指针
    Station* getStationPtrByName(const QString& stationName);
    // 函数：根据站点名获取站点
    Station getStationByName(const QString& stationName);
    // 函数：将线路名数组转换为字符串
    static QString toLineNamesToString(const QVector<QString>& lineNames);
    // 函数：根据名称查找线路是否已存在
    bool isLineExist(const QString& lineName);
    // 函数：根据名称查找站点是否已存在
    bool isStationExist(const QString& stationName);
    // 函数：根据两个站点的站点名查找连接
    Edge getEdgeByTwoStationNames(const QString&, const QString&);
    // 函数：添加一条线路
    void addLine(QString lineName, QColor lineColor);
    // 函数：计算途径站数最少路线
    bool leastStations(const QString&, const QString&, QVector<Station>&, QVector<Edge>&);
    // 函数：计算搭乘时间最短路线
    bool leastTime(const QString&, const QString&, QVector<Station>&, QVector<Edge>&);
    // 函数：搜索站点名
    QVector<QString> searchStationName(const QString &);


private:
    QVector<QString> lineNames;                             // 数组，便于线路名有序输出
    QVector<Line*> lines;                                   // 数组，存储所有线路
    QHash<QString, Line*> lineHash;                         // 哈希表，线路名 -> 线路

    QVector<Station*> stations;                             // 数组，存储所有站点
    QHash<QString, Station*> stationHash;                   // 哈希表，站点名 -> 站点

    QVector<Edge*> edges;                                   // 数组，存储所有连接
    QHash<QPair<QString, QString>, Edge*> edgeHash;         // 哈希表，站点名元素对 -> 连接

    // 图，键为当前站点名，值为键值对数组，其中键为邻接站点名，值为当前站点到邻接站点的距离
    QHash<QString, QVector<QPair<QString, double>>> graph;

};

#endif // SUBWAYGRAPH_H
