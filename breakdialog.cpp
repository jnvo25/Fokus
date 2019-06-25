#include <iostream>
#include <QTimer>
#include <QMessageBox>

#include "timer.h"
#include "breakdialog.h"
#include "ui_breakdialog.h"

static int* totalBreaks;
static int lastMilestone;
static Timer sessionTimer;
static QTimer* timer;

BreakDialog::BreakDialog(QWidget *parent, int * breaks) :
    QDialog(parent),
    ui(new Ui::BreakDialog)
{
    ui->setupUi(this);
    totalBreaks = breaks;
    ui->breaksLeft_label->setText(QString::number(*totalBreaks));

    sessionTimer = Timer();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start();

    lastMilestone = 0;
}

void BreakDialog::showTime()
{
    if (sessionTimer.hasStarted) {
        ui->time_label->setText(QString::number(25- sessionTimer.getRunningTime()));
    } else {
        ui->time_label->setText("25");
    }

    if(sessionTimer.getRunningTime() != 0 && sessionTimer.getRunningTime() % 10 == 0) {
        std::cout << "Stopping" << std::endl;
        sessionTimer.stop();
        QMessageBox::critical(this, tr("Fokus"), tr("Break is over!"));
    }
}

BreakDialog::~BreakDialog()
{
    delete ui;
}

void BreakDialog::on_end_button_clicked()
{
    close();
}

void BreakDialog::on_start_button_clicked()
{
    if(!sessionTimer.hasStarted) {
        if(*totalBreaks > 0) {
            std::cout << "Starting" << std::endl;
            *totalBreaks -= 1;
            ui->breaksLeft_label->setText(QString::number(*totalBreaks));
            sessionTimer.start();
        } else {
            QMessageBox::critical(this, tr("Fokus"), tr("There are no more breaks to redeem!"));
        }
    } else {
        QMessageBox::critical(this, tr("Fokus"), tr("There is already a session in progress!"));
    }
}
