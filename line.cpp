#include "line.h"

Line::Line() {

}

// 赋值运算符重载
Line& Line::operator=(const Line& other) {
    if (this != &other) {
        index = other.index;
        name = other.name;
        color = other.color;
        start = other.start;
        end = other.end;
        total = other.total;
        stationNames = other.stationNames;
    }
    return *this;
}
