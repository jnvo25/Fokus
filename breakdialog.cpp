#include <iostream>
#include <Qt>
#include <QTimer>
#include <QTime>
#include <QMessageBox>

#include "timer.h"
#include "breakdialog.h"
#include "ui_breakdialog.h"

const int BREAKTIME = 2000;

static int* totalBreaks;

// Signals timeout every second
static QTimer* updateTimer;

// Timer for breaktime
static QTimer* countdownTimer;

BreakDialog::BreakDialog(QWidget *parent, int * breaks) :
    QDialog(parent),
    ui(new Ui::BreakDialog)
{
    ui->setupUi(this);

    // Get breaks from main timer window
    totalBreaks = breaks;

    // Create timer to update view every second
    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateView()));
    updateTimer->start();

    // Create countdown timer for break time
    countdownTimer = new QTimer(this);
    countdownTimer->setSingleShot(true);
    countdownTimer->setTimerType(Qt::PreciseTimer);
    countdownTimer->setInterval(BREAKTIME);

    // When break is over, show message popup
    connect(countdownTimer, SIGNAL(timeout()), this, SLOT(showDialog()));
}

void BreakDialog::showDialog()
{
    if(*totalBreaks > 0) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Fokus", "Do you want to use another break?", QMessageBox::Yes | QMessageBox::No);
        switch (reply) {
            case QMessageBox::Yes:
                countdownTimer->start();
                *totalBreaks -= 1;
                break;
            case QMessageBox::No:
                close();
                break;
            default: break;
        }
    } else {
        QMessageBox::warning(this, tr("Fokus"), tr("Break is over! No more breaks to redeem!"));
        close();
    }
}

void BreakDialog::updateView()
{
    ui->breaksLeft_label->setText("Breaks left: " + QString::number(*totalBreaks));

    if(countdownTimer->isActive()) {
        QTime timeLeft(0,0,0,0);
        timeLeft = timeLeft.addMSecs(countdownTimer->remainingTime());
        ui->time_label->setText(timeLeft.toString(Qt::TextDate));
    }
}

BreakDialog::~BreakDialog()
{
    delete countdownTimer;
    delete updateTimer;
    delete ui;
}

void BreakDialog::on_end_button_clicked()
{
    // If used more breaks than used, put back breaks
    if(countdownTimer->remainingTime() > BREAKTIME) {
        *totalBreaks += countdownTimer->remainingTime()/BREAKTIME;
    }

    close();
}

void BreakDialog::on_start_button_clicked()
{
    if(!countdownTimer->isActive()) {
        if(*totalBreaks > 0) {
            countdownTimer->start();
            *totalBreaks -= 1;
            ui->start_button->setText("Add");
        } else {
            QMessageBox::critical(this, tr("Fokus"), tr("There are no more breaks to redeem!"));
        }
    } else {
        if(*totalBreaks > 0) {
            countdownTimer->setInterval(countdownTimer->remainingTime() + BREAKTIME);
            countdownTimer->start();
            *totalBreaks -= 1;
        }
    }
}
