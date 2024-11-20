#include "graphicsviewzoom.h"

#include <QMouseEvent>
#include <QPoint>
#include <QApplication>

GraphicsViewZoom::GraphicsViewZoom() {
    zoomEnabled = true;
}

GraphicsViewZoom::GraphicsViewZoom(QGraphicsView* view)
    : QObject(view), view(view)
{
    // 安装事件过滤器
    view->viewport()->installEventFilter(this);
    // 鼠标跟踪生效，即使不按下也会触发 MouseMove 事件
    view->setMouseTracking(true);
    // 快捷键设置为 Ctrl
    modifiers = Qt::ControlModifier;
    // 缩放因子基数
    zoomFactorBase = 1.0015;
    // 设置视图可鼠标拖拽移动
    view->setDragMode(QGraphicsView::ScrollHandDrag);
}

// 函数：设置 view
void GraphicsViewZoom::setView(QGraphicsView *view) {
    this->view = view;
    // 安装事件过滤器
    view->viewport()->installEventFilter(this);
    // 鼠标跟踪生效，即使不按下也会触发 MouseMove 事件
    view->setMouseTracking(true);
    // 快捷键设置为 Ctrl
    modifiers = Qt::ControlModifier;
    // 缩放因子基数
    zoomFactorBase = 1.0015;
    // 设置视图可鼠标拖拽移动
    view->setDragMode(QGraphicsView::ScrollHandDrag);
}

// 函数：设置滚轮缩放是否可用
void GraphicsViewZoom::setZoomEnabled(bool flag) {
    zoomEnabled = flag;
}

// 函数：返回缩放可用状态
bool GraphicsViewZoom::getZoomEnabled() {
    return zoomEnabled;
}

// 缩放方法
void GraphicsViewZoom::gentleZoom(double factor) {
    // 计算视口中心的场景位置，防止缩放后视口的焦点偏移
    QPointF sceneCenter = view->mapToScene(view->viewport()->rect().center());
    // 缩放视口，factor > 1 时视口缩小，场景放大；factor < 1 时视口放大，场景缩小
    view->scale(factor, factor);
    // 缩放后重新将视口集中在缩放中心位置
    QPointF newCenter = view->mapToScene(view->viewport()->rect().center());
    QPointF delta = sceneCenter - newCenter;
    // 移动视口，以确保目标位置不变
    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    view->translate(delta.x(), delta.y());
    emit zoomed();
}

// 事件过滤器
bool GraphicsViewZoom::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::Wheel && zoomEnabled) {
        // 鼠标滚轮事件，处理缩放
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
        if (QApplication::keyboardModifiers() == modifiers) {
            if (wheelEvent->angleDelta().y()) {
                double factor = qPow(zoomFactorBase, wheelEvent->angleDelta().y());
                gentleZoom(factor);
                return true;
            }
        }
    }
    return false;
}


void GraphicsViewZoom::setModifiers(Qt::KeyboardModifiers modifiers) {
    this->modifiers = modifiers;
}

void GraphicsViewZoom::setZoomFactorBase(double value) {
    zoomFactorBase = value;
}