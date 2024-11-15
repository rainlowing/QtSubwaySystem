#include "managelines.h"
#include "ui_managelines.h"

ManageLines::ManageLines(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ManageLines)
{
    ui->setupUi(this);
}

ManageLines::~ManageLines()
{
    delete ui;
}
