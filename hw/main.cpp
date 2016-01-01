#include <iostream>

#define MAXN 20
#define LARGEM 10000

using namespace std;

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

double BranchBound(int n, int m, double a[][MAXN], double b[], double c[], double x[]){
	double ans = 0;
	IP q[100];
	int h = -1, t = 0;
	q[t].set(n, m, a, b, c);
	
	while (h < t){
		h++;
		double xt[MAXN];
		double LP = q[h].Simplex(xt);

		if (LP < ans) continue;
		if (isInteger(q[h].n, xt)){
			if (LP > ans){
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

int main(){
	int n, m;
	double a[MAXN][MAXN], b[MAXN], c[MAXN];
	cout << "单纯形法求解线性规划问题" << endl;
	cout << "By 周子孟 曲衡 刘嘉信" << endl;
	cout << "本程序可求解如下形式的线性规划问题：" << endl;
	cout << "Max CX" << endl;
	cout << "s.t. Ax<=b" << endl;
	cout << "     x>=0" << endl;
	cout << "输入C的维数n(n<20)：";
	cin >> n;
	while (n >= 20 || n <= 0)
	{
		cout << "超出允许范围！输入C的维数n(n<20)：";
		cin >> n;
	}
	cout << "输入b的维数m(m<20)：";
	cin >> m;
	while (m >= 20 || m < 0)
	{
		cout << "超出允许范围！输入b的维数m(m<20)：";
		cin >> m;
	}
	cout << "输入C：" << endl;
	for (int i = 1; i <= n; i++)
	{
		cout << "c : ";
		cin >> c[i];
	}
	cout << "输入A：" << endl;
	for (int i = 1; i <= m; i++)
	{
		cout << "A[" << i << "] : ";
		for (int j = 1; j <= n; j++)
		{
			cin >> a[i][j];
		}
	}
	cout << "输入b：" << endl;
	for (int i = 1; i <= m; i++)
	{
		cout << "b[" << i << "] : ";
		cin >> b[i];
	}

	double x[MAXN];
	cout << "最优值：" << endl;
	cout << BranchBound(n, m, a, b, c, x) << endl;
	cout << "解x：" << endl;
	for (int i = 1; i <= n; i++) cout << x[i] << " ";
	system("pause");
}