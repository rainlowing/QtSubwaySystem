#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphicsviewzoom.h"
#include "managelines.h"
#include "subwaygraph.h"
#include "apphelp.h"

#include <QMainWindow>
#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 槽函数：ComboBoxStartLine 索引改变
    void transferStartLineChanged(QString);
    // 槽函数：ComboBoxEndLine 索引改变
    void transferEndLineChanged(QString);
    // 槽函数：更新状态栏时间
    void updateTime();
    // 槽函数：pushButtonAddLine 按下
    void addLine();
    // 槽函数：action_addall 添加所有
    void on_action_addall_triggered();
    // 槽函数：aciton_addline 添加线路
    void on_action_addline_triggered();
    // 槽函数：aciton_addstation 添加站台
    void on_action_addstation_triggered();
    // 槽函数：aciton_addconnect 添加连接
    void on_action_addconnect_triggered();
    // 槽函数：aciton_addbytext 文本添加
    void on_action_addbytext_triggered();
    // 槽函数：action_linemap 绘制并显示地铁图所有线路
    void on_action_linemap_triggered();
    // 槽函数：action_enlarge 放大视图
    void on_action_enlarge_triggered();
    // 槽函数：action_shrink 缩小视图
    void on_action_shrink_triggered();

private:
    Ui::MainWindow *ui;                         // 主窗口 UI
    GraphicsViewZoom* view;                     // 自定义图形视图，用于地图缩放
    QGraphicsScene* scene;                      // 场景，用于渲染地铁图
    ManageLines* manageLines;                   // 添加功能前端管理类
    SubwayGraph* subwayGraph;                   // 后端管理类
    AppHelp* appHelp;                           // 帮助显示类

    // 状态栏内标签
    QLabel* labelInfo;
    QLabel* labelTime;
    QLabel* labelHint;

    // 函数：连接槽函数
    void initConnect();
    // 函数：初始化状态栏 StatusBar
    void initStatus();
    // 函数：更新换乘信息
    void updateTransferQueryInfo();
    // 函数：根据连接数组绘制地铁图的连接
    void drawEdge(QVector<Edge>& allEdges);
    // 函数：根据站点数组绘制地铁图的站点
    void drawStation(QVector<Station>& allStations);
    // 函数：根据线路ID数组计算混合颜色
    QColor getLinesColor(const QVector<QString>& lineIDs);
    // 函数：根据站点的地理坐标计算站点的场景坐标
    QPointF getStationScenePos(QPointF coord);
    // 函数：根据站点计算混合颜色
    QColor getStationColor(Station& station);
    // 事件过滤器
    bool eventFilter(QObject* object, QEvent* event) override;


#define MARGIN 30                                       // 场景对于视口的边距
#define NET_WIDTH 2000                                  // 场景宽度
#define NET_HEIGHT 2000                                 // 场景高度
#define EDGE_PEN_WIDTH 2                                // 线条 Item 宽度
#define NODE_HALF_WIDTH 3                               // 站点大小

#define SCENE_WIDTH MARGIN * 2 + NET_WIDTH              // 场景宽度
#define SCENE_HEIGHT MARGIN * 2 + NET_HEIGHT            // 场景高度
};
#endif // MAINWINDOW_H
