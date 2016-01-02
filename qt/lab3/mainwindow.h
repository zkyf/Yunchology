#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#define MAXN 20
#define LARGEM 100000

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setTableSize();
    void checkTableSize();
    ~MainWindow();

private slots:
    void on_tw_A_cellChanged(int row, int column);

    void on_tw_C_cellChanged(int row, int column);

    void on_tw_b_cellChanged(int row, int column);

    void on_actionReset_triggered();

    void on_actionCalaulate_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionHelp_triggered();

private:
    Ui::MainWindow *ui;

    int m;
    int n;
    int ignore_tw_A_cellChanged;
    int ignore_tw_C_cellChanged;
    int ignore_tw_b_cellChanged;
    double A[MAXN][MAXN];
    bool _A[MAXN][MAXN];
    double b[MAXN];
    bool _b[MAXN];
    double c[MAXN];
    bool _c[MAXN];
    double x[MAXN];
};

#endif // MAINWINDOW_H
