#ifndef EDGE_H
#define EDGE_H

#include "station.h"
#include "line.h"

class Edge {
public:
    Edge();
    Edge(Station s1, Station s2, Line l);

    Station station1;           // 站点1
    Station station2;           // 站点2
    Line line;                  // 所属线路
    double dist;                 // 站点距离
};

#endif // EDGE_H
