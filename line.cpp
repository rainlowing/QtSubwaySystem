//
// Created by rainl on 2024/11/14.
//

#include "line.h"

Line::Line() {

}

Line::Line(QString lineName, QColor lineColor) : name(lineName), color(lineColor) {

}

// 函数：根据线路名获取该线路上所有站点名
const QVector<QString> &Line::getAllStationNames() const {
    return stationNameVector;
}
