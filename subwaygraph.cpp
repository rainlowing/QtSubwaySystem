#include "subwaygraph.h"

#include <QFile>
#include <QTextStream>

#include <QDebug>

// 构造函数
SubwayGraph::SubwayGraph() {

}

// 函数：读取文件数据
bool SubwayGraph::readFileData(QString fileName) {
    // 打开数据文件
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: " << file.errorString();
        return false;
    }
    // 字节流绑定数据源
    QTextStream in(&file);
    // 读取文件内容
    while (!in.atEnd()) {
        int lineIndex = 0;
        Line line;
        // 数据文件中的字段名
        QString header, id, name, color, start, end, total;
        // 读取线路信息
        line.index = lineIndex;
        in >> header >> id;             line.id = id;                                       in.readLine();
        in >> header >> name;           line.name = name;                                   in.readLine();
        in >> header >> color;          line.color = QColor::fromString(color);      in.readLine();
        in >> header >> start;          line.start = start;                                 in.readLine();
        in >> header >> end;            line.end = end;                                     in.readLine();
        in >> header >> total;          line.total = total.toInt();                         in.readLine();

        // 上一个站点的ID，用于建立连接
        QString lastStationId;
        for (int i = 0; !in.atEnd() && i < line.total; ++i) {
            // 读取站点信息
            Station station;
            Station& ref = station;

            QString longitudeAndLatitude;
            in >> ref.id >> ref.name >> longitudeAndLatitude;
            // 如果该站点已经在其他线路中被添加过，则直接用哈希表中存储的值进行操作
            if (stations.contains(ref.id)) {
                Station& exStation = stations[station.id];
                // 站点和线路互相添加信息
                line.stationIDs.push_back(exStation.id);
                exStation.lineIDs.push_back(line.id);
                // 如果 i > 0，那么当前站点和前一个站点建立连接关系
                if (i > 0) {
                    line.edgeSet.insert(Edge(exStation.id, lastStationId));
                    line.edgeSet.insert(Edge(lastStationId, exStation.id));
                    edges.insert(Edge(exStation.id, lastStationId));
                    edges.insert(Edge(lastStationId, exStation.id));
                }
                lastStationId = exStation.id;
                // 站点名存储到线路中
                line.stationNameVector.push_back(exStation.name);
            } else {
                station.longitude = longitudeAndLatitude.split(',').first().toDouble();
                station.latitude = longitudeAndLatitude.split(',').last().toDouble();
                // 站点和线路互相添加信息
                line.stationIDs.push_back(station.id);
                station.lineIDs.push_back(line.id);
                // 如果 i > 0，那么当前站点和前一个站点建立连接关系
                if (i > 0) {
                    line.edgeSet.insert(Edge(station.id, lastStationId));
                    line.edgeSet.insert(Edge(lastStationId, station.id));
                    edges.insert(Edge(station.id, lastStationId));
                    edges.insert(Edge(lastStationId, station.id));
                }
                lastStationId = station.id;
                // 站点存储到哈希表中
                stations.insert(station.id, station);
                // 站点名存储到线路中
                line.stationNameVector.push_back(station.name);
                // 记录边界经纬度
                if (station.longitude > Station::maxLongitude) {
                    Station::maxLongitude = station.longitude;
                } else if (station.longitude < Station::minLongitude) {
                    Station::minLongitude = station.longitude;
                }
                if (station.latitude > Station::maxLatitude) {
                    Station::maxLatitude = station.latitude;
                } else if (station.latitude < Station::minLatitude) {
                    Station::minLatitude = station.latitude;
                }
            }
            in.readLine();
        }

        // 将线路存储到哈希表中
        if (!lines.contains(line.name)) {
            lineNames.push_back(line.name);
            lineIDs.insert(line.name, line.id);
            lines.insert(line.id, line);
        }
        in.readLine();
    }

    file.close();
    return true;
}

// 函数：获取所有线路名
QVector<QString> SubwayGraph::getAllLineNames() {
    return lineNames;
}

// 函数：根据站点ID获取两个站点的公共所属线路ID
QVector<QString> SubwayGraph::getCommonLineIDs(QString s1, QString s2) {
    QVector<QString> lineCommonIDs;
    for (auto &lineCommonID : stations[s1].lineIDs) {
        if (stations[s2].lineIDs.contains(lineCommonID)) {
            lineCommonIDs.push_back(lineCommonID);
        }
    }
    return lineCommonIDs;
}

// 函数：根据线路名称获得线路ID
QString SubwayGraph::getLineID(QString lineName) {
    return lineIDs[lineName];
}

// 函数：根据线路ID获得线路
Line SubwayGraph::getLine(QString lineID) {
    return lines[lineID];
}

// 函数：获取所有线路
QVector<Line> SubwayGraph::getAllLines() {
    QVector<Line> allLines;
    for (int i = 0; i < lineNames.size(); ++i) {
        allLines.push_back(getLine(getLineID(lineNames[i])));
    }
    return allLines;
}

// 函数：获取所有站点
QVector<Station> SubwayGraph::getAllStations() {
    QVector<Station> allStations;
    for (auto& station : stations) {
        allStations.push_back(station);
    }
    return allStations;
}

// 函数：获取所有连接
QVector<Edge> SubwayGraph::getAllEdges() {
    QVector<Edge> allEdges;
    for (auto& edge : edges) {
        allEdges.push_back(edge);
    }
    return allEdges;
}

// 函数：根据站点ID获取站点的经纬度
QPointF SubwayGraph::getStationCoord(QString stationID) {
    return QPointF(stations[stationID].longitude, stations[stationID].latitude);
}

// 函数：根据站点获取站点的经纬度
QPointF SubwayGraph::getStationCoord(Station &station) {
    return QPointF(stations[station.id].longitude, stations[station.id].latitude);
}

// 函数：获取最小经纬度
QPointF SubwayGraph::getMinCoord() {
    return QPointF(Station::minLongitude, Station::minLatitude);
}

// 函数：获取最大经纬度
QPointF SubwayGraph::getmaxCoord() {
    return QPointF(Station::maxLongitude, Station::maxLatitude);
}

// 函数：根据站点ID获取站点名
QString SubwayGraph::getStationName(QString stationID) {
    return stations[stationID].name;
}

// 函数：根据线路ID获取线路颜色
QColor SubwayGraph::getLineColor(QString lineID) {
    return lines[lineID].color;
}

// 函数：根据站点获取站点名
QString SubwayGraph::getStationName(Station& station) {
    return stations[station.id].name;
}

// 函数：根据站点获取站点所属线路ID
QVector<QString> SubwayGraph::getStationLineID(Station station) {
    return stations[station.id].lineIDs;
}

// 函数：根据站点获取站点所属线路，并转换为字符串
QString SubwayGraph::getStationLineInfo(Station &station) {
    QString lineInfo;
    for (auto& lineID : stations[station.id].lineIDs) {
        lineInfo += " ";
        lineInfo += lines[lineID].name;
    }
    lineInfo.remove(0, 1);
    return lineInfo;
}

// 函数：很具线路ID数组获取站点所属线路，转换为字符串
QString SubwayGraph::getStationLineInfo(QVector<QString> lineIDs) {
    QString lineInfo;
    for (auto& lineID : lineIDs) {
        lineInfo += " ";
        lineInfo += lines[lineID].name;
    }
    lineInfo.remove(0, 1);
    return lineInfo;
}