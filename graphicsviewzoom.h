#ifndef GRAPHICSVIEWZOOM_H
#define GRAPHICSVIEWZOOM_H

#include <QObject>
#include <QEvent>
#include <QGraphicsView>

class GraphicsViewZoom : public QObject
{
    Q_OBJECT
public:
    explicit GraphicsViewZoom(QGraphicsView* view);

private:
    QGraphicsView* view;
    Qt::KeyboardModifiers modifiers;            // 修饰键
    double zoomFactorBase;                      // 缩放因子基数，缩放因子 = 基数 ^ 滚轮旋转量

    void gentleZoom(double factor);                         // 缩放方法
    bool eventFilter(QObject* object, QEvent* event);       // 事件过滤器
    void setModifiers(Qt::KeyboardModifiers modifiers);     // 设置修饰键
    void setZoomFactorBase(double value);                   // 设置缩放因子基数

signals:
    void zoomed();                                      // 信号，表示缩放事件发生
};

#endif // GRAPHICSVIEWZOOM_H
