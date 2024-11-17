#ifndef EDGE_H
#define EDGE_H

#include "station.h"
#include "line.h"

class Edge {
public:
    Edge();
    Edge(const Station& s1, const Station& s2, const Line& l);

    Station station1;           // 站点1，出发站点
    Station station2;           // 站点2，结束站点
    Line line;                  // 所属线路
    double dist;                // 站点距离
};

#endif // EDGE_H
