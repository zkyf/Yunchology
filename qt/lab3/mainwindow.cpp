#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>
#include <QTableWidget>
#include <QMessageBox>
#include <QStringList>
#include <QFileDialog>
#include <QTextStream>

int isInteger(int n, double x[]){
    for (int i = 1; i <= n; i++){
        if (x[i] - (int)x[i] != 0){
            return 0;
        }
    }
    return 1;
}

double Simplex(int n, int m, double a[][MAXN], double b[], double c[], double x[]){
    //初始化单纯型表
    double d[MAXN][MAXN];
    d[0][0] = 0;
    for (int j = 1; j <= n; j++)
        d[0][j] = -c[j];
    for (int j = 1; j <= m; j++)
        d[0][n + j] = 0;


    int t = n + m;
    for (int i = 1; i <= m; i++){
        if (b[i] >= 0){
            for (int j = 1; j <= n; j++)
                d[i][j] = a[i][j];
            for (int j = 1; j <= m; j++)
                d[i][n + j] = (i == j) ? 1 : 0;
            d[i][0] = b[i];
        }
        else{
            for (int j = 1; j <= n; j++)
                d[i][j] = a[i][j] * -1;
            for (int j = 1; j <= m; j++)
                d[i][n + j] = (i == j) ? -1 : 0;
            t++;
            for (int j = 1; j <= m; j++)
                d[j][t] = (j == i) ? 1 : 0;
            d[0][t] = LARGEM;
            d[i][0] = -b[i];
            for (int j = 0; j <= t; j++)
                d[0][j] -= LARGEM * d[i][j];
        }
    }
    n = t;

    int flag;
    while (1){
        for (int i = 0; i <= m; i++){
            for (int j = 0; j <= n; j++)
                printf("%10.2f", d[i][j]);
            printf("\n");
        }
        printf("---------------------------------\n");

        //判断是否找到解
        flag = 1;
        for (int i = 1; i <= n; i++){
            if (d[0][i] < 0){
                flag = 0;
                break;
            }
        }
        if (flag) break;

        //获取主元列
        int pivot = 1;
        for (int i = 2; i <= n; i++){
            if (d[0][i] < d[0][pivot])
                pivot = i;
        }

        //若主元列全部元素均为非正数，无解
        int depart;
        for (depart = 1; (d[depart][pivot] * d[depart][0] <= 0) && (depart <= m); depart++);
        if (depart == m + 1) {
            flag = 2;
            break;
        }

        //获取主元行
        for (int i = 2; i <= m; i++){
            if ((d[i][pivot] * d[i][0] > 0) && (d[i][0] / d[i][pivot] < d[depart][0] / d[depart][pivot]))
                depart = i;
        }


        //建立新的单纯形表
        double temp = d[depart][pivot];
        for (int i = 0; i <= n; i++)
            d[depart][i] /= temp;

        for (int i = 0; i <= m; i++)
        if (i != depart){
            double temp = d[i][pivot] ;
            for (int j = 0; j <= n; j++)
                d[i][j] -= temp * d[depart][j];
        }
    }

    if (flag == 2) return -1;

    for (int i = 1; i <= m; i++){
        if (d[i][0] < 0) {
            flag = 3;
            break;
        }
        for (int j = 1; j <= n - m; j++){
            if (d[i][j] == 1)
                x[j] = d[i][0];
        }
    }
    if (flag == 3) return -1;

    return d[0][0];
}

class IP {
public:
    int n, m;
    double a[MAXN][MAXN];
    double b[MAXN];
    double c[MAXN];

    void set(int tn, int tm, double ta[][MAXN], double tb[], double tc[]){
        n = tn;
        m = tm;

        for (int j = 1; j <= n; j++)
            c[j] = tc[j];

        for (int i = 1; i <= m; i++){
            b[i] = tb[i];
            for (int j = 1; j <= n; j++)
                a[i][j] = ta[i][j];
        }
    }

    double Simplex(double x[]){
        //初始化单纯型表
        double d[MAXN][MAXN];
        d[0][0] = 0;
        for (int j = 1; j <= n; j++)
            d[0][j] = -c[j];
        for (int j = 1; j <= m; j++)
            d[0][n + j] = 0;


        int t = n + m;
        for (int i = 1; i <= m; i++){
            if (b[i] >= 0){
                for (int j = 1; j <= n; j++)
                    d[i][j] = a[i][j];
                for (int j = 1; j <= m; j++)
                    d[i][n + j] = (i == j) ? 1 : 0;
                d[i][0] = b[i];
            }
            else{
                for (int j = 1; j <= n; j++)
                    d[i][j] = a[i][j] * -1;
                for (int j = 1; j <= m; j++)
                    d[i][n + j] = (i == j) ? -1 : 0;
                t++;
                for (int j = 1; j <= m; j++)
                    d[j][t] = (j == i) ? 1 : 0;
                d[0][t] = LARGEM;
                d[i][0] = -b[i];
                for (int j = 0; j <= t; j++)
                    d[0][j] -= LARGEM * d[i][j];
            }
        }
        n = t;

        int flag;
        while (1){
            //for (int i = 0; i <= m; i++){
            //	for (int j = 0; j <= n; j++)
            //		printf("%10.2f", d[i][j]);
            //	printf("\n");
            //}
            //printf("---------------------------------\n");

            //判断是否找到解
            flag = 1;
            for (int i = 1; i <= n; i++){
                if (d[0][i] < 0){
                    flag = 0;
                    break;
                }
            }
            if (flag) break;

            //获取主元列
            int pivot = 1;
            for (int i = 2; i <= n; i++){
                if (d[0][i] < d[0][pivot])
                    pivot = i;
            }

            //若主元列全部元素均为非正数，无解
            int depart;
            for (depart = 1; (d[depart][pivot] <= 0) && (depart <= m); depart++);
            if (depart == m + 1) {
                flag = 2;
                break;
            }

            //获取主元行
            for (int i = 2; i <= m; i++){
                if ((d[i][pivot] > 0) && (d[i][0] / d[i][pivot] < d[depart][0] / d[depart][pivot]))
                    depart = i;
            }


            //建立新的单纯形表
            double temp = d[depart][pivot];
            for (int i = 0; i <= n; i++)
                d[depart][i] /= temp;

            for (int i = 0; i <= m; i++)
            if (i != depart){
                double temp = d[i][pivot];
                for (int j = 0; j <= n; j++)
                    d[i][j] -= temp * d[depart][j];
            }
        }

        if (flag == 2) return -1;

        n = n - m;
        for (int i = 1; i <= m; i++){
            if (d[i][0] < 0) {
                flag = 3;
                break;
            }
            for (int j = 1; j <= n; j++){
                if (d[i][j] == 1)
                    x[j] = d[i][0];
            }
        }
        if (flag == 3) return -1;

        return d[0][0];
    }
};

double BranchBound(int n, int m, double a[][MAXN], double b[], double c[], double x[])
{
    double ans = 0;
    IP q[100];
    int h = -1, t = 0;
    q[t].set(n, m, a, b, c);

    while (h < t)
    {
        h++;
//        QMessageBox::information(0, "bb:h,t", QString::number(h)+","+QString::number(t), 0);
        double xt[MAXN];
        double LP = q[h].Simplex(xt);

        if (LP < ans)
        {
//            QMessageBox::information(0, "bb LP", QString::number(LP), 0);
            continue;
        }
        if (isInteger(q[h].n, xt))
        {
            if (LP > ans)
            {
                ans = LP;
                for (int i = 1; i <= n; i++) x[i] = xt[i];
            }
            continue;
        }

        int p;
        for (int i = 1; i <= n; i++){
            if (xt[i] - (int)xt[i] != 0){
                p = i;
                break;
            }
        }

        m = q[h].m;
        double  a1[MAXN][MAXN], b1[MAXN], a2[MAXN][MAXN], b2[MAXN];
        for (int i = 1; i <= m; i++){
            b1[i] = b2[i] = q[h].b[i];
            for (int j = 1; j <= n; j++)
                a1[i][j] = a2[i][j] = q[h].a[i][j];
        }
        for (int j = 1; j <= n; j++){
            a1[m + 1][j] = (j == p) ? 1 : 0;
            a2[m + 1][j] = (j == p) ? -1 : 0;
        }
        b1[m + 1] = (int)xt[p];
        b2[m + 1] = -(int)xt[p] - 1;

        q[++t].set(n, m + 1, a1, b1, c);
        q[++t].set(n, m + 1, a2, b2, c);
    }
    return ans;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m(1), n(1), ignore_tw_A_cellChanged(0),
    ignore_tw_C_cellChanged(0),
    ignore_tw_b_cellChanged(0)
{
    ui->setupUi(this);
    ui->tw_x->setEditTriggers(QAbstractItemView::NoEditTriggers);
    setTableSize();
    for(int i =0;i<MAXN;i++)
    {
        _b[i] = false;
        _c[i] = false;
        for(int j = 0;j<MAXN;j++)
        {
            _A[i][j] = false;
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setTableSize()
{
    if(n<MAXN) ui->tw_A->setColumnCount(n-1);
    if(m<MAXN) ui->tw_A->setRowCount(m-1);
    ui->tw_A->setColumnCount(n);
    ui->tw_A->setRowCount(m);

    ui->tw_b->setColumnCount(1);
    if(m<MAXN) ui->tw_b->setRowCount(m-1);
    ui->tw_b->setRowCount(m);

    ui->tw_C->setColumnCount(1);
    if(n<MAXN) ui->tw_C->setRowCount(n-1);
    ui->tw_C->setRowCount(n);

    ui->tw_x->setColumnCount(1);
    ui->tw_x->setRowCount(n-1);
}

void MainWindow::checkTableSize()
{
    int maxm = 0;
    int maxn = 0;
    for(int i =0;i<MAXN;i++)
    {
        for (int j = 0;j<MAXN;j++)
        {
            if(_A[i][j] && maxm<i)
            {
                maxm = i;
//                QMessageBox::information(0, "A: maxm: "+QString::number(i), "", QMessageBox::Ok);
            }
        }
    }
    for(int i = 0; i<MAXN; i++)
    {
        if(_b[i] && maxm<i)
        {
            maxm = i;
        }
    }

    for(int i =0;i<MAXN;i++)
    {
        for (int j = 0;j<MAXN;j++)
        {
            if(_A[i][j] && maxn<j) maxn = j;
        }
    }
    for(int i = 0; i<MAXN; i++)
    {
        if(_c[i] && maxn<i)
        {
            maxn = i;
        }
    }

    maxm++; maxn++;
    m = (maxm+1>MAXN-1)?MAXN-1:maxm+1;
    n = (maxn+1>MAXN-1)?MAXN-1:maxn+1;

//    QString msg = "m=" + QString::number(m) + ", n=" + QString::number(n);
//    QMessageBox::information(0, msg, msg, QMessageBox::Ok);

    ignore_tw_A_cellChanged = maxm*maxn;

    for(int i = 0;i<maxm;i++)
    {
        for(int j = 0;j<maxn;j++)
        {
            ui->tw_A->setItem(i,j, new QTableWidgetItem(QString::number(A[i][j])));
        }
    }

    ignore_tw_C_cellChanged = maxn;
    for(int i = 0;i<maxn;i++)
    {
        ui->tw_C->setItem(i,0,new QTableWidgetItem(QString::number(c[i])));
    }

    ignore_tw_b_cellChanged = maxm;
    for(int i =0;i<maxm;i++)
    {
        ui->tw_b->setItem(i,0,new QTableWidgetItem(QString::number(b[i])));
    }

    for(int i = 0;i<maxn;i++)
    {
        ui->tw_x->setItem(i,0,new QTableWidgetItem(QString::number(x[i])));
    }

    setTableSize();
}

void MainWindow::on_tw_A_cellChanged(int row, int column)
{
    if(ignore_tw_A_cellChanged>0)
    {
        ignore_tw_A_cellChanged--;
        return;
    }
    QString com = ui->tw_A->item(row, column)->text();
    if(com == "")
    {
        _A[row][column] = false;
        A[row][column] = 0;
    }
    else
    {
        bool flag = false;
        double num = com.toDouble(&flag);
        if(!flag)
        {
            _A[row][column] = false;
            A[row][column] = 0;
        }
        else
        {
            A[row][column]=num;
            _A[row][column] = true;
        }
    }
    checkTableSize();
}

void MainWindow::on_tw_C_cellChanged(int row, int column)
{
    if(ignore_tw_C_cellChanged>0)
    {
        ignore_tw_C_cellChanged--;
        return;
    }
    QString com = ui->tw_C->item(row, column)->text();
    if(com == "")
    {
        _c[row] = false;
        c[row] = 0;
    }
    else
    {
        bool flag = false;
        double num = com.toDouble(&flag);
        if(!flag)
        {
            _c[row] = false;
            c[row] = 0;
        }
        else
        {
            c[row]=num;
            _c[row] = true;
        }
    }
    checkTableSize();
}

void MainWindow::on_tw_b_cellChanged(int row, int column)
{
    if(ignore_tw_b_cellChanged>0)
    {
        ignore_tw_b_cellChanged--;
        return;
    }
    QString com = ui->tw_b->item(row, column)->text();
    if(com == "")
    {
        _b[row] = false;
        b[row] = 0;
    }
    else
    {
        bool flag = false;
        double num = com.toDouble(&flag);
        if(!flag)
        {
            _b[row] = false;
            b[row] = 0;
        }
        else
        {
            b[row]=num;
            _b[row] = true;
        }
    }
    checkTableSize();
}

void MainWindow::on_actionReset_triggered()
{
    for(int i =0;i<MAXN;i++)
    {
        for(int j = 0;j<MAXN;j++)
        {
            A[i][j] = 0;
            _A[i][j] = false;
        }
        b[i] = 0;
        _b[i] = false;
        c[i] = 0;
        _c[i] = false;
    }
    m = 1;
    n = 1;
    setTableSize();
}

void MainWindow::on_actionCalaulate_triggered()
{
    double __A[MAXN][MAXN];
    double __b[MAXN];
    double __c[MAXN];

    for(int i = 0;i<m-1;i++)
    {
        for(int j = 0;j<n-1;j++)
        {
            __A[i+1][j+1] = A[i][j];
        }
    }
    for(int j = 0;j<n-1;j++)
    {
        __c[j+1] = c[j];
    }
    for(int j = 0;j<m-1;j++)
    {
        __b[j+1] = b[j];
    }
    for(int j = 0;j<n-1;j++)
    {
        x[j] = 0;
    }
    double answer = BranchBound(n-1,m-1,__A,__b,__c,x);
    QMessageBox::information(0, "title", "Optimal Value: " + QString::number(answer), QMessageBox::Ok);
    checkTableSize();
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open a problem");
    QFile input(filename);
    if(!input.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(this, "Open Error", "Fail to open "+filename);
        return;
    }
    QTextStream file(&input);
    int _n,_m;
    file >> _n >>_m;
    if(_n<=0 || _n>=MAXN || _m<=0 || _m>=MAXN)
    {
        QMessageBox::information(this, "Open Error", "Illegal input");
        return;
    }
    for(int i =0;i<MAXN;i++)
    {
        for(int j = 0;j<MAXN;j++)
        {
            A[i][j] = 0;
            _A[i][j] = false;
        }
        b[i] = 0;
        _b[i] = false;
        c[i] = 0;
        _c[i] = false;
    }
    n=_n+1; m=_m+1;
    for(int i =0;i<_m;i++)
    {
        for(int j =0;j<_n;j++)
        {
            file>>A[i][j];
            _A[i][j] = true;
        }
    }
    for(int i =0;i<_n;i++)
    {
        file>>c[i];
        _c[i] = true;
    }
    for(int j = 0;j<_m;j++)
    {
        file>>b[j];
    }
    checkTableSize();
    checkTableSize();
}

void MainWindow::on_actionSave_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save a proble");
    QFile output(filename);
    if(!output.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::information(this, "Save Error", "Fail to save "+filename);
        return;
    }
    QTextStream file(&output);
    file << n-1 << " "<<m-1<<endl;
    for(int i=0;i<m-1;i++)
    {
        for(int j=0;j<n-1;j++)
        {
            file << A[i][j]<<" ";
        }
        file <<endl;
    }
    for(int i=0;i<n-1;i++)
    {
        file << c[i]<<" ";
    }
    file<<endl;
    for(int i =0;i<m-1;i++)
    {
        file<<b[i]<<" ";
    }
    file <<endl;
}

void MainWindow::on_actionHelp_triggered()
{
    QMessageBox::information(this, "Help", "This program solves problems in the form below\nMax cT*x S.T.\nAx<=b");
}
