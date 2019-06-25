#include <iostream>
#include <Qt>
#include <QTimer>
#include <QTime>
#include <QMessageBox>

#include "timer.h"
#include "breakdialog.h"
#include "ui_breakdialog.h"

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
    countdownTimer->setInterval(2000);

    // When break is over, show message popup
    connect(countdownTimer, SIGNAL(timeout()), this, SLOT(showDialog()));
}

void BreakDialog::showDialog()
{
    QMessageBox::warning(this, tr("Fokus"), tr("Break is over!"));
}

void BreakDialog::updateView()
{
    ui->breaksLeft_label->setText(QString::number(*totalBreaks));

    if(countdownTimer->isActive()) {
        QTime timeLeft(0,0,0,0);
        timeLeft = timeLeft.addMSecs(countdownTimer->remainingTime());
        std::cout << "OUTPUT: " << timeLeft.toString(Qt::TextDate).toStdString() << std::endl;
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
    close();
}

void BreakDialog::on_start_button_clicked()
{
    if(!countdownTimer->isActive()) {
        if(*totalBreaks > 0) {
            countdownTimer->start();
            *totalBreaks -= 1;
        } else {
            QMessageBox::critical(this, tr("Fokus"), tr("There are no more breaks to redeem!"));
        }
    }
}
