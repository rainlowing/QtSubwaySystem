#include "edge.h"

Edge::Edge() {}

Edge::Edge(Station s1, Station s2, Line l) : station1(s1), station2(s2), line(l) {
    dist = s1.distance(s2);
}
