#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDateTime>
#include <QTimer>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QGraphicsLineItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    view = new GraphicsViewZoom(ui->graphicsView);
    scene = new QGraphicsScene;
    manageLines = new ManageLines(this);
    subwayGraph = new SubwayGraph;
    appHelp = new AppHelp;

    ui->graphicsView->setScene(scene);
    // 图形渲染：抗锯齿
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    // 设置场景大小 2060 * 2060
    scene->setSceneRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    // 设置初始视口大小 50 * 50，场景不缩放变形
    ui->graphicsView->fitInView(QRect(0, 0, 50, 50), Qt::KeepAspectRatio);
    // 使视口中心正对场景中心
    ui->graphicsView->centerOn(scene->sceneRect().center());

    // 初始化状态栏
    initStatus();

    // 读取内置数据
    bool flag = subwayGraph->readFileData(":/data/data/subway.txt");
    if (!flag) {
        QMessageBox::warning(this, tr("读取数据错误"), tr("\n将无法展示内置线路！"), QMessageBox::Ok);
    }

    // 根据读取得到的线路数据更新 ui
    updateTransferQueryInfo();
    // 连接槽函数
    initConnect();
    // 绘制地铁图
    on_action_linemap_triggered();


}

// 函数：连接槽函数
void MainWindow::initConnect() {
    connect(ui->comboBoxStartLine, SIGNAL(currentTextChanged(const QString &)), this, SLOT(transferStartLineChanged(QString)));
    connect(ui->comboBoxEndLine, SIGNAL(currentTextChanged(const QString &)), this, SLOT(transferEndLineChanged(QString)));
    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
}

// 槽函数：更新状态栏时间
void MainWindow::updateTime() {
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
    labelTime->setText(str);
}

// 函数：初始化状态栏
void MainWindow::initStatus() {
    labelInfo = new QLabel;
    labelTime = new QLabel;
    labelHint = new QLabel;

    labelInfo->setMinimumSize(200, 15);
    labelTime->setMinimumSize(200, 15);
    labelHint->setMinimumSize(250, 15);

    labelInfo->setText(tr("原作者：BaiJiazm@github"));
    labelHint->setText(tr("欢迎使用武汉地铁换乘系统"));

    ui->statusbar->addWidget(labelInfo);
    ui->statusbar->addWidget(labelTime);
    ui->statusbar->addWidget(labelHint);

}

// 函数：更新换乘信息
void MainWindow::updateTransferQueryInfo() {
    labelHint->setText(tr("欢迎使用武汉地铁换乘系统，已更新数据"));
    ui->comboBoxStartLine->clear();
    ui->comboBoxStartStation->clear();
    ui->comboBoxEndLine->clear();
    ui->comboBoxEndStation->clear();
    QVector<QString> linesName = subwayGraph->getAllLineNames();
    for (auto &lineName : linesName) {
        ui->comboBoxStartLine->addItem(lineName);
        ui->comboBoxEndLine->addItem(lineName);
    }
    transferStartLineChanged(linesName[0]);
    transferEndLineChanged(linesName[0]);
}

// 槽函数：ComboBoxStartLine 索引改变
void MainWindow::transferStartLineChanged(QString lineName) {
    ui->comboBoxStartStation->clear();
    QVector<QString> stationNames = subwayGraph->getLine(subwayGraph->getLineID(lineName)).getAllStationNames();
    for (auto& stationName : stationNames) {
        ui->comboBoxStartStation->addItem(stationName);
    }
}

// 槽函数：ComboBoxEndLine 索引改变
void MainWindow::transferEndLineChanged(QString lineName) {
    ui->comboBoxEndStation->clear();
    QVector<QString> stationNames = subwayGraph->getLine(subwayGraph->getLineID(lineName)).getAllStationNames();
    for (auto& stationName : stationNames) {
        ui->comboBoxEndStation->addItem(stationName);
    }
}

// 函数：根据线路ID数组计算混合颜色
QColor MainWindow::getLinesColor(const QVector<QString> &lineIDs) {
    QColor color1 = QColor(255, 255, 255);
    QColor color2;
    for (int i = 0; i < lineIDs.size(); ++i) {
        color2 = subwayGraph->getLineColor(lineIDs[i]);
        color1.setRed(color1.red() * color2.red() / 255);
        color1.setGreen(color1.green() * color2.green() / 255);
        color1.setBlue(color1.blue() * color2.blue() / 255);
    }
    return color1;
}

// 函数：根据站点计算混合颜色
QColor MainWindow::getStationColor(Station& station) {
    return getLinesColor(subwayGraph->getStationLineID(station));
}

// 函数：根据连接数组绘制地铁图的连接
void MainWindow::drawEdge(QVector<Edge>& allEdges) {
    for (int i = 0; i < allEdges.size(); ++i) {
        QString station1 = allEdges[i].first;
        QString station2 = allEdges[i].second;

        // 获取两个站点间所有线路，渲染边的颜色
        QVector<QString> commonLineIDs = subwayGraph->getCommonLineIDs(station1, station2);
        QColor color = getLinesColor(commonLineIDs);
        QString tip = "途径：" + subwayGraph->getStationName(station1) + " -- " + subwayGraph->getStationName(station2) + "\n"
                + subwayGraph->getStationLineInfo(commonLineIDs);

        // 获取站点的场景坐标
        QPointF station1ScenePos = getStationScenePos(subwayGraph->getStationCoord(station1));
        QPointF station2ScenePos = getStationScenePos(subwayGraph->getStationCoord(station2));

        // 直线 Item
        QGraphicsLineItem* edgeItem = new QGraphicsLineItem;
        // 设置线条颜色和宽度
        edgeItem->setPen(QPen(color, EDGE_PEN_WIDTH));
        // 设置鼠标悬停样式：手指
        edgeItem->setCursor(Qt::WhatsThisCursor);
        // 设置鼠标悬停提示框内容
        edgeItem->setToolTip(tip);
        // 设置 Item 场景坐标
        edgeItem->setPos(station1ScenePos);
        // 设置 Item 线条的起点和终点：起点为 Item 的场景坐标（即第一个站），终点为第二个站相对 Item 的坐标
        edgeItem->setLine(0, 0, station2ScenePos.x() - station1ScenePos.x(), station2ScenePos.y() - station1ScenePos.y());
        scene->addItem(edgeItem);
    }
}

// 函数：根据站点数组绘制地铁图的站点
void MainWindow::drawStation(QVector<Station> &allStations) {
    for (int i = 0; i < allStations.size(); ++i) {
        QString name = subwayGraph->getStationName(allStations[i]);
        QPointF coord = subwayGraph->getStationCoord(allStations[i]);
        QString tip = "站名：" + name + "\n"
                + "经度：" + QString::number(coord.x(), 'f', 7) + "\n"
                + "纬度：" + QString::number(coord.y(), 'f', 7) + "\n"
                + "经过线路：" + subwayGraph->getStationLineInfo(allStations[i]);
        QPointF stationScenePos = getStationScenePos(coord);

        // 椭圆或圆形 Item
        QGraphicsEllipseItem* stationItem = new QGraphicsEllipseItem;
        // 设置椭圆的矩形区域，矩形左上角坐标 (x, y) ，宽，高
        stationItem->setRect(-NODE_HALF_WIDTH, -NODE_HALF_WIDTH, NODE_HALF_WIDTH * 2, NODE_HALF_WIDTH * 2);
        // 设置椭圆的中心位置
        stationItem->setPos(stationScenePos);
        // 设置线条颜色和宽度
        stationItem->setPen(getStationColor(allStations[i]));
        stationItem->setBrush(QColor(QRgb(0xffffff)));
        // 设置鼠标悬停样式：手指
        stationItem->setCursor(Qt::WhatsThisCursor);
        // 设置鼠标悬停提示框内容
        stationItem->setToolTip(tip);
        scene->addItem(stationItem);

        // 文本 Item
        QGraphicsTextItem* textItem = new QGraphicsTextItem;
        textItem->setPlainText(name);
        textItem->setFont(QFont("微软雅黑", 4, 1));
        textItem->setPos(stationScenePos.x(), stationScenePos.y() - NODE_HALF_WIDTH * 2);
        scene->addItem(textItem);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete view;
    delete scene;
    delete manageLines;
    delete subwayGraph;
    delete appHelp;
}



void MainWindow::on_action_addall_triggered()
{
    scene->clear();

}


void MainWindow::on_action_linemap_triggered()
{
    scene->clear();
    QVector<Edge> allEdges = subwayGraph->getAllEdges();
    QVector<Station> allStations = subwayGraph->getAllStations();
    drawEdge(allEdges);
    drawStation(allStations);
}

QPointF MainWindow::getStationScenePos(QPointF coord) {
    QPointF minCoord = subwayGraph->getMinCoord();
    QPointF maxCoord = subwayGraph->getmaxCoord();
    double x = (coord.x() - minCoord.x()) / (maxCoord.x() - minCoord.x()) * NET_WIDTH + MARGIN;
    double y = (maxCoord.y() - coord.y()) / (maxCoord.y() - minCoord.y()) * NET_HEIGHT + MARGIN;
    return QPointF(x, y);
}