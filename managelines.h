#ifndef MANAGELINES_H
#define MANAGELINES_H

#include <QDialog>
#include <QVector>

class MainWindow;

namespace Ui {
class ManageLines;
}

class ManageLines : public QDialog
{
    Q_OBJECT

public:
    explicit ManageLines(QWidget *parent = nullptr);
    ~ManageLines();


public slots:
    // 槽函数：pushButtonChooseColor 按下
    QColor chooseColor();

protected:
    Ui::ManageLines *ui;
    bool colorFlag;

    // 函数：显示所有页面
    void setAllVisible();
    // 函数：显示指定页面
    void setTabVisible(int index);
    // 函数：连接槽函数
    void initConnect();
    // 函数：是否设置有效线路颜色
    bool isLineColorValid();



private:

friend class MainWindow;

};

#endif // MANAGELINES_H
