#ifndef LINE_H
#define LINE_H

#include <vector>
#include <set>

#include <QPair>
#include <QString>
#include <QColor>
#include <QVector>
#include <QSet>

class SubwayGraph;

// 连接类，存储两个相连站点的ID
typedef QPair<QString, QString> Edge;

// 线路类
class Line {
public:
    Line();
    Line(QString lineName, QColor lineColor);
    // 函数：根据线路名获取该线路上所有站点名
    const QVector<QString> &getAllStationNames() const;

protected:
    int index;                              // 索引
    QString id;                             // ID
    QString name;                           // 名称
    QColor color;                           // 颜色
    QString start;                          // 起始站
    QString end;                            // 终点站
    int total;                              // 站点数
    QVector<QString> stationNameVector;     // 数列，从起始站到终点站存储所有站点的名称
    QVector<QString> stationIDs;            // 数列，存储所有站点ID
    QSet<Edge> edgeSet;                     // 集合，存储所有站点连接关系

    // 友元类
    friend class SubwayGraph;
};


#endif //WUHANSUBWAY_LINE_H
