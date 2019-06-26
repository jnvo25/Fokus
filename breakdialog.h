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
    explicit BreakDialog(QWidget *parent = nullptr, int * = nullptr, int = 0);
    ~BreakDialog();

private slots:
    void updateView();

    void on_end_button_clicked();

    void on_start_button_clicked();

    void showDialog();
private:
    Ui::BreakDialog *ui;
};

#endif // BREAKDIALOG_H
