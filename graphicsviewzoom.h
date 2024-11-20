#ifndef GRAPHICSVIEWZOOM_H
#define GRAPHICSVIEWZOOM_H

#include <QObject>
#include <QEvent>
#include <QGraphicsView>

class GraphicsViewZoom : public QObject
{
    Q_OBJECT
public:
    explicit GraphicsViewZoom();
    explicit GraphicsViewZoom(QGraphicsView* view);

    // 函数：设置 view
    void setView(QGraphicsView*);
    // 函数：设置滚轮缩放是否可用
    void setZoomEnabled(bool flag);
    // 函数：返回缩放可用状态
    bool getZoomEnabled() const;

private:
    QGraphicsView* view;
    Qt::KeyboardModifiers modifiers;
    double zoomFactorBase;                      // 缩放因子基数，缩放因子 = 基数 ^ 滚轮旋转量
    bool zoomEnabled;

    // 函数：实现缩放
    void gentleZoom(double factor);
    bool eventFilter(QObject* object, QEvent* event);

signals:
    void zoomed();                               // 信号，表示缩放事件发生
};

#endif // GRAPHICSVIEWZOOM_H
