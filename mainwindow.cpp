#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "timer.h"
#include "breakdialog.h"
#include <iostream>
#include <QTimer>
#include <string>
#include <QMessageBox>

static Timer sessionTimer;
static QTimer *timer;
static int breaks, lastMilestone;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    std::cout << "Initializing" << std::endl;
    sessionTimer = Timer();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start();

    breaks = 0;
    lastMilestone = 0;
}

void MainWindow::showTime()
{
    if (sessionTimer.hasStarted) {
        ui->time_label->setText(QString::number(sessionTimer.getRunningTime()));
    } else {
        ui->time_label->setText(QString::number(sessionTimer.getTotalTime()));
    }
    if(sessionTimer.getRunningTime() % 5 == 0) {
        if(lastMilestone != sessionTimer.getRunningTime() && sessionTimer.getRunningTime() != -1) {
            std::cout << sessionTimer.getRunningTime() << std::endl;
            breaks += 1;
            lastMilestone = sessionTimer.getRunningTime();
        }
    }
    ui->breaks_label->setText("Redeemable: " + QString::number(breaks) + " breaks");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
}

void MainWindow::on_stop_button_clicked()
{
    std::cout << "Stopping timer" << std::endl;
    std::cout << sessionTimer.getRunningTime() << std::endl;
    sessionTimer.stop();
}

void MainWindow::on_start_button_clicked()
{
    std::cout << "Starting timer" << std::endl;
    sessionTimer.start();
}

void MainWindow::on_break_button_clicked()
{
    if(breaks > 0) {
    sessionTimer.stop();
    int * breakPtr = &breaks;
    BreakDialog breakWindow(nullptr, breakPtr);
    breakWindow.setModal(true);
    breakWindow.exec();
    } else {
        QMessageBox::critical(this, tr("Fokus"), tr("There are no breaks to redeem!"));
    }
}
