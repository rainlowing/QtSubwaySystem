#include "managelines.h"
#include "ui_managelines.h"

#include <QMessageBox>
#include <QColorDialog>

ManageLines::ManageLines(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ManageLines)
{
    ui->setupUi(this);

    initConnect();
}

// 函数：连接槽函数
void ManageLines::initConnect() {
    connect(ui->pushButtonChooseColor, SIGNAL(clicked(bool)), this, SLOT(chooseColor()));
}

// 函数：显示所有可见
void ManageLines::setAllVisible() {
    ui->tabWidget->tabBar()->show();
    ui->tabWidget->setCurrentIndex(0);
    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        ui->tabWidget->setTabVisible(i, true);
    }
    show();
    this->setWindowTitle(tr("添加所有"));
}

// 函数：显示指定页面
void ManageLines::setTabVisible(int index) {
    ui->tabWidget->tabBar()->hide();
    ui->tabWidget->setCurrentIndex(index);
    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        if (i != index) {
            ui->tabWidget->setTabVisible(i, false);
        }
    }
    switch (index) {
        case 0:
            this->setWindowTitle(tr("添加线路"));
//            this->setWindowIcon(QIcon(QPixmap(":icon/icon/line.png")));
            break;
        case 1:
            this->setWindowTitle(tr("添加站点"));
            break;
        case 2:
            this->setWindowTitle(tr("添加连接"));
            break;
        case 3:
            this->setWindowTitle(tr("以文本方式添加"));
            break;
        default:
            this->setWindowTitle(tr("添加设置"));
    }
    show();
}

// 函数：是否设置有效线路颜色
bool ManageLines::isLineColorValid() {
    return colorFlag;
}

ManageLines::~ManageLines()
{
    delete ui;
}

// 槽函数：pushButtonChooseColor 按下
QColor ManageLines::chooseColor() {
    QColorDialog colorDialog = QColorDialog(this);
    colorDialog.setWindowTitle(tr("为线路选择颜色"));
    colorDialog.exec();

    QColor color = colorDialog.selectedColor();
    if (color.isValid()) {
        colorFlag = true;
        ui->colorWidget->setStyleSheet(QString("background-color : %1;").arg(color.name()));
    }
    return color;
}
