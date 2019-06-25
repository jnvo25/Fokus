#ifndef BREAKDIALOG_H
#define BREAKDIALOG_H

#include <QDialog>

namespace Ui {
class BreakDialog;
}

class BreakDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BreakDialog(QWidget *parent = nullptr, int * = nullptr);
    ~BreakDialog();

private slots:
    void showTime();

    void on_end_button_clicked();

    void on_start_button_clicked();

private:
    Ui::BreakDialog *ui;
};

#endif // BREAKDIALOG_H
