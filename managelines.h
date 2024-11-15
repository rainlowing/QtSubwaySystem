#ifndef MANAGELINES_H
#define MANAGELINES_H

#include <QDialog>

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

protected:
    Ui::ManageLines *ui;

private:

friend class MainWindow;

};

#endif // MANAGELINES_H
