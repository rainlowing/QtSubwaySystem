#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphicsviewzoom.h"
#include "subwaygraph.h"

#include <QMainWindow>
#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// 主窗口类
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // 槽函数：ComboBoxStartLine 索引改变
    void transferStartLineChanged(const QString&);
    // 槽函数：ComboBoxEndLine 索引改变
    void transferEndLineChanged(const QString&);
    // 槽函数：更新状态栏时间
    void updateTime();
    // 槽函数：pushButtonSearchCity 按下
    void searchCity();
    // 槽函数：comboBoxSearchStaiton 被编辑
    void searchStation();
    // 槽函数：pushButtonSearchStation 按下
    void mapToStation();
    // 槽函数：pushButtonReset 按下
    void reset();
    // 槽函数：pushButtonTransfer 按下
    void transfer();
    // 槽函数：action_linemap 绘制并显示地铁图所有线路
    void on_action_linemap_triggered();
    // 槽函数：action_enlarge 放大视图
    void on_action_enlarge_triggered();
    // 槽函数：action_shrink 缩小视图
    void on_action_shrink_triggered();
    // 槽函数：action_toolbar 隐藏工具栏
    void on_action_toolbar_triggered();
    // 槽函数：action_statusbar 隐藏状态栏
    void on_action_statusbar_triggered();
    // 槽函数：aciton_wheel 启用滚轮放大缩小
    void on_action_wheel_triggered();
    // 槽函数：action_close 退出程序
    void on_action_close_triggered();
    // 槽函数：action_qt 关于Qt
    void on_action_qt_triggered();
    // 槽函数：action_author 制作者信息
    void on_action_author_triggered();

private:
    Ui::MainWindow *ui;                         // 主窗口 UI
    GraphicsViewZoom* view;                     // 自定义图形视图，用于地图缩放
    QGraphicsScene* scene;                      // 场景，用于渲染地铁图
    SubwayGraph* subwayGraph;                   // 后端管理类

    // 场景遮罩
    QGraphicsRectItem* mask;                    // 场景遮罩

    // 状态栏内标签
    QLabel* labelInfo;
    QLabel* labelTime;
    QLabel* labelHint;

    // 计时器
    QTimer* timer{};

    // 函数：连接槽函数
    void initConnect();
    // 函数：断开槽函数
    void destroyConnect();
    // 函数：初始化状态栏 StatusBar
    void initStatus();
    // 函数：更新换乘信息
    void updateTransferQueryInfo();
    // 函数：根据连接数组绘制地铁图的连接
    void drawEdge(const QVector<Edge>&);
    // 函数：根据站点数组绘制地铁图的站点
    void drawStation(const QVector<Station>&);
    // 函数：根据查询结果绘制地铁图
    void drawByQueryResult(const QVector<Edge>&, const QVector<Station>&);
    // 函数：根据查询结果输出文本
    void updateResultText(const QVector<Edge>&, const QVector<Station>&, int);
    // 函数：根据线路ID数组计算混合颜色
    QColor getLinesColor(const QVector<QString>& lineIDs);
    // 函数：根据站点的地理坐标计算站点的场景坐标
    static QPointF getStationScenePos(QPointF coord);
    // 函数：根据站点计算混合颜色
    QColor getStationColor(const Station& station);
    // 函数：重绘地铁图
    void reDraw();
    // 事件过滤器
    bool eventFilter(QObject* object, QEvent* event) override;


#define MARGIN 30                                       // 场景对于视口的边距
#define GRAPH_WIDTH 2000                                // 地铁图的宽度
#define GRAPH_HEIGHT 2000                               // 地铁图的高度
#define SCENE_WIDTH 2060                                // 场景宽度
#define SCENE_HEIGHT 2060                               // 场景高度
#define EDGE_PEN_WIDTH 2                                // 线条 Item 宽度
#define NODE_HALF_WIDTH 3                               // 站点大小

#define SPEED 40
};
#endif // MAINWINDOW_H
