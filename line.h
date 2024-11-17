#ifndef LINE_H
#define LINE_H

#include <QPair>
#include <QString>
#include <QColor>
#include <QVector>
#include <QSet>

// 线路类
class Line {
public:
    Line();
    // 拷贝构造函数
    Line& operator=(const Line& other);

    int index;                              // 索引
    QString id;                             // ID
    QString name;                           // 名称
    QColor color;                           // 颜色
    QString start;                          // 起始站
    QString end;                            // 终点站
    int total;                              // 站点数
    QVector<QString> stationNames;          // 数列，从起始站到终点站存储所有站点名
};


#endif //WUHANSUBWAY_LINE_H
