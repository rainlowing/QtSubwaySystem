//
// Created by rainl on 2024/11/14.
//

#include "line.h"

Line::Line() {

}

Line::Line(QString lineName, QColor lineColor) : name(lineName), color(lineColor) {

}

// 函数：获取所有站点名
QVector<QString> Line::getAllStationNames() {
    return stationNames;
}
