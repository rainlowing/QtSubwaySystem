#include "subwaygraph.h"
#include "edge.h"

#include <queue>

#include <QFile>
#include <QTextStream>

// 构造函数
SubwayGraph::SubwayGraph() {

}

// 函数：读取文件数据
bool SubwayGraph::readFileData(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
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
        line.index = lineIndex;
        in >> header >> id;             line.id = id;                                       in.readLine();
        in >> header >> name;           line.name = name;                                   in.readLine();
        in >> header >> color;          line.color = QColor::fromString(color);      in.readLine();
        in >> header >> start;          line.start = start;                                 in.readLine();
        in >> header >> end;            line.end = end;                                     in.readLine();
        in >> header >> total;          line.total = total.toInt();                         in.readLine();

        Station lastStation;
        for (int i = 0; !in.atEnd() && i < line.total; ++i) {
            Station station;

            QString longitudeAndLatitude;
            in >> station.id >> station.name >> longitudeAndLatitude;
            // 如果该站点已经在其他线路中被添加过，则直接用哈希表中存储的值进行操作
            if (isStationExist(station.name)) {
                Station* exStation = getStationPtrByName(station.name);
                // 站点和线路互相添加信息
                line.stationNames.push_back(exStation->name);
                exStation->lineNames.push_back(line.name);
                // 如果 i > 0，那么当前站点和前一个站点建立连接关系
                if (i > 0) {
                    edges.push_back(new Edge(*exStation, lastStation, line));
                    edges.push_back(new Edge(lastStation, *exStation, line));
                }
                lastStation = *exStation;
            } else {
                station.longitude = longitudeAndLatitude.split(',').first().toDouble();
                station.latitude = longitudeAndLatitude.split(',').last().toDouble();
                // 站点和线路互相添加信息
                line.stationNames.push_back(station.name);
                station.lineNames.push_back(line.name);
                // 如果 i > 0，那么当前站点和前一个站点建立连接关系
                if (i > 0) {
                    edges.push_back(new Edge(station, lastStation, line));
                    edges.push_back(new Edge(lastStation, station, line));
                }
                lastStation = station;
                // 存储站点
                stations.push_back(new Station(station));
                stationHash[station.name] = stations.last();
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

        // 存储线路
        if (!isLineExist(line.name)) {
            lines.push_back(new Line(line));
            lineHash[line.name] = lines.last();
        }
        in.readLine();
    }

    file.close();
    // 构建图
    makeGraph();
    return true;
}

// 函数：构建图
void SubwayGraph::makeGraph() {
    graph.clear();
    for (const auto& edge : edges) {
        graph[edge->station1.name].push_back({edge->station2.name, edge->dist});
    }
}

// 函数：获取所有线路名
QVector<QString> SubwayGraph::getAllLineNames() {
    return lineHash.keys();
}

// 函数：获取所有线路
QVector<Line> SubwayGraph::getAllLines() {
    QVector<Line> lines;
    for (const Line* line : this->lines) {
        if (line != nullptr) {
            lines.push_back(*line);
        }
    }
    return lines;
}

// 函数：获取所有站点
QVector<Station> SubwayGraph::getAllStations() {
    QVector<Station> stations;
    for (const Station* station : this->stations) {
        if (station != nullptr) {
            stations.push_back(*station);
        }
    }
    return stations;
}

// 函数：获取所有连接
QVector<Edge> SubwayGraph::getAllEdges() {
    QVector<Edge> edges;
    for (const Edge* edge : this->edges) {
        if (edge != nullptr) {
            edges.push_back(*edge);
        }
    }
    return edges;
}

// 函数：获取最小经纬度
QPointF SubwayGraph::getMinCoord() {
    return {Station::minLongitude, Station::minLatitude};
}

// 函数：获取最大经纬度
QPointF SubwayGraph::getMaxCoord() {
    return {Station::maxLongitude, Station::maxLatitude};
}

// 函数：根据线路名获取线路
Line SubwayGraph::getLineByName(const QString& lineName) {
    return *lineHash.value(lineName);
}

// 函数：根据线路名获取线路颜色
QColor SubwayGraph::getLineColor(const QString& lineName) {
    return lineHash[lineName]->color;
}


// 函数：将线路名数组转换为字符串
QString SubwayGraph::toLineNamesToString(const QVector<QString>& lineNames) {
    QString lineInfo;
    for (auto& lineName : lineNames) {
        lineInfo += " ";
        lineInfo += lineName;
    }
    lineInfo.remove(0, 1);
    return lineInfo;
}

// 函数：根据站点名获取站点的指针
Station *SubwayGraph::getStationPtrByName(const QString &stationName) {
    return stationHash.value(stationName);
}

// 函数：根据站点名获取站点
Station SubwayGraph::getStationByName(const QString& stationName) {
    return *stationHash.value(stationName);
}

// 函数：根据名称查找线路是否已存在
bool SubwayGraph::isLineExist(const QString& lineName) {
    return lineHash.contains(lineName);
}

// 函数：根据名称查找站点是否已存在
bool SubwayGraph::isStationExist(const QString& stationName) {
    return stationHash.contains(stationName);
}

// 函数：添加一条线路
void SubwayGraph::addLine(QString lineName, QColor lineColor) {

}

// 函数：计算搭乘时间最短路线
bool SubwayGraph::leastTime(const QString& s1Name, const QString& s2Name, QVector<Station>& resS, QVector<Edge>& resE) {
    resS.clear();
    resE.clear();

    const double INF = 1e9;             // 定义无法到达的距离

    QHash<QString, double> dist;        // 哈希，键为目标站点名，值为起始站到目标站点的最短距离
    QHash<QString, QString> prev;       // 哈希，键为要到达目标站点名的前驱站点名，值为目标站点名

    QVector<QString> keys = graph.keys();
    for (const QString& node : keys) {
        dist[node] = INF;
    }
    dist[s1Name] = 0;

    std::priority_queue<QPair<double, QString>, QVector<QPair<double, QString>>, std::greater<>> pq;
    pq.emplace(0, s1Name);

    while (!pq.empty()) {
        auto [curDist, curNode] = pq.top();
        pq.pop();

        if (curDist > dist[curNode]) continue;  // 去除堆中过时的信息

        for (const auto& [neighbor, weight] : graph[curNode]) {
            double newDist = dist[curNode] + curDist;
            if (dist[neighbor] > newDist) {
                dist[neighbor] = newDist;
                prev[neighbor] = curNode;
                pq.emplace(newDist, neighbor);
            }
        }
    }

    // 还原路径
    if (dist[s2Name] == INF) {
        return false;
    } else {
        for (QString at = s2Name; at != ""; at = prev[at]) {
//            resS.push_back(stations);
        return true;
        }
    }
}

SubwayGraph::~SubwayGraph() {
    for (Line* line : lines) {
        delete line;
    }
    for (Station* station : stations) {
        delete station;
    }
    for (Edge* edge : edges) {
        delete edge;
    }
}