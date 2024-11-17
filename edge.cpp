#include "edge.h"

Edge::Edge() {}

Edge::Edge(const Station& s1, const Station& s2, const Line& l) : station1(s1), station2(s2), line(l) {
    dist = s1.distance(s2);
}
