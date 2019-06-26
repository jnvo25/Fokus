#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "breakdialog.h"
#include <iostream>
#include <QTimer>
#include <QTime>
#include <string>
#include <QMessageBox>
#include <QElapsedTimer>

const int BREAKTIME = 300000; // 5 minutes
const int STUDYTIME = 1500000; // 25 minutes

static QTimer *timer;
static int breaks, lastMilestone;

static QElapsedTimer stopwatch;
static qint64 lastStop;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start();

    breaks = 0;
    lastMilestone = 0;

    stopwatch = QElapsedTimer();

    ui->stop_button->hide();
}

void MainWindow::showTime()
{
    // Update label with elapsed time
    QTime timeElapsed(0,0,0,0);
    if(stopwatch.isValid()) {
        timeElapsed = timeElapsed.addMSecs(static_cast<int>(stopwatch.elapsed()+lastStop));
    } else if (lastStop) {
        timeElapsed = timeElapsed.addMSecs(static_cast<int>(lastStop));
    }
    ui->time_label->setText(timeElapsed.toString(Qt::TextDate));

    // Check if break is awarded
    if(stopwatch.isValid()) {
        int totalTime = static_cast<int>(stopwatch.elapsed() + lastStop);
        if(totalTime % STUDYTIME == 0) {
            if(lastMilestone != totalTime) {
                breaks++;
                lastMilestone = totalTime;
            }
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
    if(stopwatch.isValid()) {
        if(!lastStop) {
            lastStop = stopwatch.elapsed();
        } else {
            lastStop += stopwatch.elapsed();
        }
        stopwatch.invalidate();
        ui->start_button->setText("Resume");
        ui->start_button->show();
        ui->stop_button->hide();
    }
}

void MainWindow::on_start_button_clicked()
{
    if(!stopwatch.isValid()) {
        stopwatch.restart();
        ui->start_button->hide();
        ui->stop_button->show();
    }
}

void MainWindow::on_break_button_clicked()
{
    if(breaks > 0) {
        // If stopwatch is in session, stop it
        if(stopwatch.isValid()) {
            if(!lastStop) {
                lastStop = stopwatch.elapsed();
            } else {
                lastStop += stopwatch.elapsed();
            }
            stopwatch.invalidate();
        }

        // Create new window
        int * breakPtr = &breaks;
        BreakDialog breakWindow(nullptr, breakPtr, BREAKTIME);
        breakWindow.setModal(true);
        breakWindow.exec();

        ui->start_button->show();
        ui->stop_button->hide();
    } else {
        QMessageBox::critical(this, tr("Fokus"), tr("There are no breaks to redeem!"));
    }
}
