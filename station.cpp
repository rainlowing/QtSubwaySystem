#include "station.h"

double Station::maxLongitude = 0.0;
double Station::maxLatitude = 0.0;
double Station::minLongitude = 180.0;
double Station::minLatitude = 90.0;

Station::Station() {

}

// 赋值运算符重载
Station& Station::operator=(const Station &other) {
    if (this != &other) {
        id = other.id;
        name = other.name;
        longitude = other.longitude;
        latitude = other.latitude;
        lineNames = other.lineNames;
    }
    return *this;
}

// 函数：将角度转换为弧度
double Station::toRadians(double degree) {
    return degree * M_PI / 180;
}

// 函数：计算站点间地理直线距离，使用 Vincenty 方法
double Station::distance(Station other) {
    // 角度转弧度
    double long1 = toRadians(longitude);
    double lat1 = toRadians(latitude);
    double long2 = toRadians(other.longitude);
    double lat2 = toRadians(other.latitude);

    // 经纬度差
    double dlong = long2 - long1;
    double dlat = lat2 - lat1;

    // Haversine 公式
    double a = sin(dlat / 2) * sin(dlat / 2) + cos(lat1) * cos(lat2) * sin(dlong / 2) * sin(dlong / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    // 计算距离
    return R * c;
}

// 函数：获取站点的地理点
QPointF Station::getCoord() {
    return QPointF(longitude, latitude);
}

// 函数：获取站点所属线路，并转换为字符串
QString Station::getLineInfo() {
    QString lineInfo;
    for (auto& lineName : lineNames) {
        lineInfo += " ";
        lineInfo += lineName;
    }
    lineInfo.remove(0, 1);
    return lineInfo;
}

// 函数：获取两个站点的共同所属线路名数组
QVector<QString> Station::getCommonLineNames(Station& other) {
    QVector<QString> commonLineNames;
    for (const auto& lineName : lineNames) {
        if (other.lineNames.contains(lineName)) {
            commonLineNames.push_back(lineName);
        }
    }
    return commonLineNames;
}
