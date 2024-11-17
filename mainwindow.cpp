#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./ui_managelines.h"

#include <QDateTime>
#include <QTimer>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QGraphicsLineItem>
#include <QWheelEvent>
#include <QtSvgWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , labelInfo(new QLabel)
    , labelTime(new QLabel)
    , labelHint(new QLabel)
    , timer(new QTimer)
    , scene(new QGraphicsScene)
    , view(new GraphicsViewZoom)
    , manageLines(new ManageLines(this))
    , subwayGraph(new SubwayGraph)
    , appHelp(new AppHelp)
    , mask(new QGraphicsRectItem)
{
    this->installEventFilter(this);

    ui->setupUi(this);
    view->setView(ui->graphicsView);

    ui->graphicsView->setScene(scene);
    // 图形渲染：抗锯齿
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    // 设置场景大小 2060 * 2060
    scene->setSceneRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    // 设置初始视口大小 50 * 50，场景不缩放变形
    ui->graphicsView->fitInView(QRect(0, 0, 50, 50), Qt::KeepAspectRatio);
    // 使视口中心正对场景中心
    ui->graphicsView->centerOn(scene->sceneRect().center());

    // 设置遮罩
    mask->setRect(scene->sceneRect());
    mask->setBrush(QBrush(QColor(255, 255, 255, 180)));
    mask->setVisible(false);
    mask->setZValue(4);
    scene->addItem(mask);

    // 初始化状态栏
    initStatus();

    // 读取内置数据
    bool flag = subwayGraph->readFileData(":/data/data/subway_wuhan.txt");
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
    connect(ui->comboBoxStartLine, SIGNAL(currentTextChanged(QString)), this, SLOT(transferStartLineChanged(QString)));
    connect(ui->comboBoxEndLine, SIGNAL(currentTextChanged(QString)), this, SLOT(transferEndLineChanged(QString)));
    connect(ui->pushButtonTransfer, SIGNAL(clicked(bool)), this, SLOT(transfer()));

    connect(manageLines->ui->pushButtonAddLine, SIGNAL(clicked(bool)), this, SLOT(addLine()));
    timer->start(1000);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
}

// 槽函数：更新状态栏时间
void MainWindow::updateTime() {
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
    labelTime->setText(str);
}

// 槽函数：pushButtonTransfer 按下
void MainWindow::transfer() {
    QString startStationName = ui->comboBoxStartStation->currentText();
    QString endStationName = ui->comboBoxEndStation->currentText();
    // 换乘方式：1 站数最少  2 时间最短
    int way = ui->radioButtonMinStation->isChecked() ? 1 : 2;
    // 返回结果
    QVector<Station> tranStations;
    QVector<Edge> tranEdges;
    bool success;

    switch (way) {
        case 1:
            labelHint->setText(tr("正在查询从 ") + startStationName + tr("站 到 ") + endStationName + tr("站 的途径站数最少路线"));
            success = subwayGraph->leastStations(startStationName, endStationName, tranStations, tranEdges);
            break;
        case 2:
            labelHint->setText(tr("正在查询从 ") + startStationName + tr("站 到 ") + endStationName + tr("站 的搭乘时间最短路线"));
            success = subwayGraph->leastTime(startStationName, endStationName, tranStations, tranEdges);
            break;
        default:
            return;
    }

    if (!success) {
        QMessageBox::information(this, tr("提示"), tr("没有从 ") + startStationName + tr("站 到") + endStationName + tr("站 的换乘路线"));
    } else {
        labelHint->setText(tr("换乘查询成功"));
        drawByQueryResult(tranEdges, tranStations);
        updateResultText(tranEdges, tranStations, way);
    }

}

// 槽函数：pushButtonAddLine 按下
void MainWindow::addLine() {
    QString newLineName = manageLines->ui->lineEditLineName->text();
    if (newLineName.isEmpty()) {
        QMessageBox::warning(this, tr("添加线路"), tr("线路名称不能为空"), QMessageBox::Ok);
    } else if (subwayGraph->isLineExist(newLineName)) {
        QMessageBox::warning(this, tr("添加线路"), tr("线路已存在"), QMessageBox::Ok);
    } else if (!manageLines->isLineColorValid()) {
        QMessageBox::warning(this, tr("添加线路"), tr("未选择颜色"), QMessageBox::Ok);
    } else {
        QMessageBox::information(this, tr("添加线路"), tr("线路：") + newLineName + tr(" 添加成功"), QMessageBox::Ok);
//        subwayGraph->addLine(newLineName);
    }
}

// 槽函数：pushButtonAddStation 按下
void MainWindow::addStation() {
    QString newStationName = manageLines->ui->lineEditStationName->text();
    if (newStationName.isEmpty()) {
        QMessageBox::warning(this, tr("添加站点"), tr("站点名称不能为空"), QMessageBox::Ok);
    }
}

// 函数：初始化状态栏
void MainWindow::initStatus() {
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
void MainWindow::transferStartLineChanged(const QString& lineName) {
    ui->comboBoxStartStation->clear();
    QVector<QString> stationNames = subwayGraph->getLineByName(lineName).stationNames;
    for (auto& stationName : stationNames) {
        ui->comboBoxStartStation->addItem(stationName);
    }
}

// 槽函数：ComboBoxEndLine 索引改变
void MainWindow::transferEndLineChanged(const QString& lineName) {
    ui->comboBoxEndStation->clear();
    QVector<QString> stationNames = subwayGraph->getLineByName(lineName).stationNames;
    for (auto& stationName : stationNames) {
        ui->comboBoxEndStation->addItem(stationName);
    }
}

// 函数：根据线路名数组计算混合颜色
QColor MainWindow::getLinesColor(const QVector<QString> &lineNames) {
    QColor color1 = QColor(255, 255, 255);
    QColor color2;
    for (const auto & lineName : lineNames) {
        color2 = subwayGraph->getLineColor(lineName);
        color1.setRed(color1.red() * color2.red() / 255);
        color1.setGreen(color1.green() * color2.green() / 255);
        color1.setBlue(color1.blue() * color2.blue() / 255);
    }
    return color1;
}

// 函数：根据站点计算混合颜色
QColor MainWindow::getStationColor(const Station& station) {
    return getLinesColor(station.lineNames);
}

// 函数：根据连接数组绘制地铁图的连接
void MainWindow::drawEdge(const QVector<Edge>& edges) {
    for (auto & edge : edges) {
        Station station1 = edge.station1;
        Station station2 = edge.station2;

        // 获取两个站点间所有线路，渲染边的颜色
        QVector<QString> commonLineNames = station1.getCommonLineNames(station2);
        QColor color = getLinesColor(commonLineNames);
        QString tip = "途径：" + station1.name + " -- " + station2.name + "\n"
                      + SubwayGraph::toLineNamesToString(commonLineNames);

        // 获取站点的场景坐标
        QPointF station1ScenePos = getStationScenePos(station1.getCoord());
        QPointF station2ScenePos = getStationScenePos(station2.getCoord());

        // 直线 Item
        auto* lineItem = new QGraphicsLineItem;
        // 设置线条颜色和宽度
        lineItem->setPen(QPen(color, EDGE_PEN_WIDTH));
        // 设置鼠标悬停样式：手指
        lineItem->setCursor(Qt::WhatsThisCursor);
        // 设置鼠标悬停提示框内容
        lineItem->setToolTip(tip);
        // 设置 Item 场景坐标
        lineItem->setPos(station1ScenePos);
        // 设置 Item 线条的起点和终点：起点为 Item 的场景坐标（即第一个站），终点为第二个站相对 Item 的坐标
        lineItem->setLine(0, 0, station2ScenePos.x() - station1ScenePos.x(), station2ScenePos.y() - station1ScenePos.y());
        if (mask->isVisible()) {
            lineItem->setZValue(4);
        }
        scene->addItem(lineItem);
    }
}

// 函数：根据站点数组绘制地铁图的站点
void MainWindow::drawStation(const QVector<Station>& stations) {
    for (auto & station : stations) {
        QString name = station.name;
        QPointF coord = station.getCoord();
        QString tip = "站名：" + name + "\n"
                + "经度：" + QString::number(coord.x(), 'f', 7) + "\n"
                + "纬度：" + QString::number(coord.y(), 'f', 7) + "\n"
                + "经过线路：" + station.getLineInfo();
        QPointF stationScenePos = getStationScenePos(coord);

        // 椭圆或圆形 Item
        auto* ellipseItem = new QGraphicsEllipseItem;
        // 设置椭圆的矩形区域，矩形左上角坐标 (x, y) ，宽，高
        ellipseItem->setRect(-NODE_HALF_WIDTH, -NODE_HALF_WIDTH, NODE_HALF_WIDTH * 2, NODE_HALF_WIDTH * 2);
        // 设置椭圆的中心位置
        ellipseItem->setPos(stationScenePos);
        // 设置鼠标悬停样式
        ellipseItem->setCursor(Qt::WhatsThisCursor);
        // 设置鼠标悬停提示框内容
        ellipseItem->setToolTip(tip);
        if (mask->isVisible()) {
            ellipseItem->setZValue(5);
        }
        if (station.lineNames.size() < 2) {
            // 设置线条颜色和宽度
            ellipseItem->setPen(getStationColor(station));
            ellipseItem->setBrush(QColor(QRgb(0xffffff)));
        } else {
            ellipseItem->setPen(QColor(0xffffff));
            ellipseItem->setBrush(QColor(0xffffff));
            auto* svgItem = new QGraphicsSvgItem(":/icon/icon/repost.svg");
            if (mask->isVisible()) {
                svgItem->setZValue(6);
            } else {
                svgItem->setZValue(2);
            }
            svgItem->setPos(stationScenePos.x() - (NODE_HALF_WIDTH + 1.33), stationScenePos.y() - (NODE_HALF_WIDTH + 1.33));
            svgItem->setScale((NODE_HALF_WIDTH + 1.33) * 2 / svgItem->boundingRect().width());
            scene->addItem(svgItem);
        }
        scene->addItem(ellipseItem);

        // 文本 Item
        auto* textItem = new QGraphicsTextItem;
        textItem->setPlainText(name);
        textItem->setFont(QFont("微软雅黑", 4, 1));
        textItem->setPos(stationScenePos.x(), stationScenePos.y() - NODE_HALF_WIDTH * 2);
        if (mask->isVisible()) {
            textItem->setZValue(7);
        } else {
            textItem->setZValue(3);
        }
        scene->addItem(textItem);
    }
}

// 函数：根据查询结果绘制地铁图
void MainWindow::drawByQueryResult(const QVector<Edge> & edges, const QVector<Station> & stations) {
    // 删除之前的查询结果
    QList<QGraphicsItem*> items =  scene->items();
    for (auto item : items) {
        if (item->zValue() >= mask->zValue()) {
            if (item != mask) delete item;
        }
    }
    // 场景遮罩
    if (!mask->isVisible()) mask->setVisible(true);
    drawStation(stations);
    drawEdge(edges);
}

// 函数：根据查询结果输出文本
void MainWindow::updateResultText(const QVector<Edge> &edges, const QVector<Station> &stations, int way) {
    ui->textBrowser->clear();
    QString text;
    switch (way) {
        case 1:
            text = tr("以下路线换乘次数最少，共换乘") + QString::number(stations.size()) + tr("个站点");
            break;
        case 2:
            double time;
            for (const auto& edge : edges) {
                time += edge.dist / SPEED;
            }
            time *= 60;
            text = tr("以下路线时间最短，大约需要") + QString::number(time, 'f',0) + tr("分钟\n（在时速40km/h下）");
            break;
        defalut:
            return;
    }
    text += "\n";
    for (int i = 0; i < stations.size(); ++i) {
        text += stations[i].name + "\t    " + stations[i].getLineInfo();
        if (i != stations.size() - 1) {
            text += "\n  ↓\n";
        }
    }

    ui->textBrowser->setText(text);
}

// 函数：根据站点的地理坐标计算站点的场景坐标
QPointF MainWindow::getStationScenePos(QPointF coord) {
    QPointF minCoord = SubwayGraph::getMinCoord();
    QPointF maxCoord = SubwayGraph::getMaxCoord();
    double x = (coord.x() - minCoord.x()) / (maxCoord.x() - minCoord.x()) * NET_WIDTH + MARGIN;
    double y = (maxCoord.y() - coord.y()) / (maxCoord.y() - minCoord.y()) * NET_HEIGHT + MARGIN;
    return {x, y};
}

MainWindow::~MainWindow()
{
    delete ui;
    delete view;
    delete scene;
    delete manageLines;
    delete subwayGraph;
    delete appHelp;

    delete labelInfo;
    delete labelTime;
    delete labelHint;

    delete timer;
}

// 槽函数：action_addall，添加所有
void MainWindow::on_action_addall_triggered()
{
    manageLines->setAllVisible();
}

// 槽函数：aciton_addline 添加线路
void MainWindow::on_action_addline_triggered() {
    manageLines->setTabVisible(0);
}

// 槽函数：aciton_addstation 添加站台
void MainWindow::on_action_addstation_triggered() {
    manageLines->setTabVisible(1);
}

// 槽函数：aciton_addconnect 添加连接
void MainWindow::on_action_addconnect_triggered() {
    manageLines->setTabVisible(2);
}

// 槽函数：aciton_addbytext 文本添加
void MainWindow::on_action_addbytext_triggered() {
    manageLines->setTabVisible(3);
}

// 槽函数：action_enlarge，放大视图
void MainWindow::on_action_enlarge_triggered() {
    labelHint->setText(tr("已放大"));
    ui->graphicsView->scale(1.5, 1.5);
}

// 槽函数：action_shrink，缩小视图
void MainWindow::on_action_shrink_triggered() {
    labelHint->setText(tr("已缩小"));
    ui->graphicsView->scale(0.677, 0.677);
}

// 槽函数：action_linemap 绘制并显示地铁图所有线路
void MainWindow::on_action_linemap_triggered()
{
    drawEdge(subwayGraph->getAllEdges());
    drawStation(subwayGraph->getAllStations());
}

// 事件过滤器
bool MainWindow::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::Wheel) {
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
        if (wheelEvent->angleDelta().y()) {
            labelHint->setText(tr("提示：按下 Ctrl 键滚动鼠标滑轮可缩放视图"));
            return true;
        }
    }
    return false;
}