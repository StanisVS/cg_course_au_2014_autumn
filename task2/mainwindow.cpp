#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QColorDialog"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lightCPicker_clicked()
{
    QColor color;
    color = QColorDialog::getColor(Qt::white,this,"pick color");
    ui->widget->setLightColor(color);
}

void MainWindow::on_ambientCPicker_clicked()
{
    QColor color;
    color = QColorDialog::getColor(Qt::white,this,"pick color");
    ui->widget->setAmbient(color);
}

void MainWindow::on_specularCPicker_clicked()
{
    QColor color;
    color = QColorDialog::getColor(Qt::white,this,"pick color");
    ui->widget->setSpecularColor(color);
}
