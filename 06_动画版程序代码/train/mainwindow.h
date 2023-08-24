#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include "animatezone.h"
#include "statetable.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void timerEvent(QTimerEvent *e);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int timerId1;
    int timerId2;
    AnimateZone *az;
    StateTable *stateTable;
    QLineEdit *cmdInput;
    bool animateLock=false;
};
#endif // MAINWINDOW_H
