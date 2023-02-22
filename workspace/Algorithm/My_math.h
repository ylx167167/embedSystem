#include <iostream>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define TINY 1.0e-20
#define FREE_ARG char *
#define PI 3.1415926

struct complex
/*���帴���ṹ��*/
{
  double real;
  double imag;
};

double Cabs(complex a)
/*�����ľ���ֵ*/
{
  double c;
  c = sqrt(a.real * a.real + a.imag * a.imag);
  return c;
}
complex Cadd(complex a, complex b)
/* �����ӷ�*/
{
  complex c;
  c.real = a.real + b.real;
  c.imag = a.imag + b.imag;
  return c;
}
complex Cminus(complex a, complex b)
/* ��������*/
{
  complex c;
  c.real = a.real - b.real;
  c.imag = a.imag - b.imag;
  return c;
}
complex Cmultiply(complex a, complex b)
/*�����˷�*/
{
  complex c;
  c.real = a.real * b.real - a.imag * b.imag;
  c.imag = a.imag * b.real + a.real * b.imag;
  return c;
}
complex Cdiv(complex a, complex b)
/*��������*/
{
  complex c;
  double r, den;
  if (fabs(b.real) >= fabs(b.imag)) {
    r = b.imag / b.real;
    den = b.real + r * b.imag;
    c.real = (a.real + r * a.imag) / den;
    c.imag = (a.imag - r * a.real) / den;
  } else {
    r = b.real / b.imag;
    den = b.imag + r * b.real;
    c.real = (a.real * r + a.imag) / den;
    c.imag = (a.imag * r - a.real) / den;
  }
  return c;
}
void CMatrix_print(complex **a, long nl, long nh)
/*
������������
*/
{
  int i, j;
  for (i = 1; i <= nl; i++) {
    for (j = 1; j <= nh; j++) {
      if (a[i][j].imag > 0)
        std::cout << a[i][j].real << " +" << a[i][j].imag << "i         ";
      else if (a[i][j].imag < 0)
        std::cout << a[i][j].real << " " << a[i][j].imag << "i        ";
      else
        std::cout << a[i][j].real << "            ";
    }
    std::cout << std::endl;
  }
}
void nrerror(char error_text[])
/*
�������������
*/
{
  std::cout << stderr << "Numerical Recipes run-time error.." << std::endl;
  std::cout << stderr << error_text << std::endl;
  std::cout << stderr << "..now exiting to system.." << std::endl;
  exit(1);
}

double *vector(long nl, long nh)
/*
����һ��double������v[nl..nh]���ڴ�
*/
{
  double *v;
  v = (double *)malloc((size_t)(nh - nl + 1) * sizeof(double));
  if (!v) nrerror("allocation double failure!");
  return v - nl;
}
complex *Cvector(long nl, long nh)
/*
����һ��complex������v[nl..nh]���ڴ�
*/
{
  complex *v;
  v = (complex *)malloc((size_t)(nh - nl + 1) * sizeof(complex));
  if (!v) nrerror("allocation double failure!");
  return v - nl;
}
void free_vector(double *v, long nl, long nh) { free((FREE_ARG)(v + nl)); }
void free_Cvector(complex *v, long nl, long nh) { free((FREE_ARG)(v + nl)); }

int *ivector(long nl, long nh)
/*
����һ��int ������v[nl..nh]���ڴ�
*/
{
  int *v;
  v = (int *)malloc((size_t)(nh - nl + 1) * sizeof(int));
  if (!v) nrerror("allocation double failure!");
  return v - nl;
}
void free_ivector(int *v, long nl, long nh) { free((FREE_ARG)(v + nl)); }


double **Matrix(long nrl, long nrh, long ncl, long nch)
/*
����һ������ m[nrl..nrh][ncl..nch]
*/
{
  long i, nrow = nrh - nrl + 1, ncol = nch - ncl + 1;
  double **m;

  m = (double **)malloc((size_t)((nrow) * sizeof(double *)));  //����row����ָ��
  if (!m) nrerror("allocation matrix row failure!");
  m -= nrl;

  m[nrl] = (double *)malloc(
      (size_t)((nrow * ncol) * sizeof(double)));  //����row*col���ڴ�ռ�
  if (!m[nrl]) nrerror("allocation matrix col failure!");
  m[nrl] -= ncl;

  for (i = nrl + 1; i <= nrh; i++) m[i] = m[i - 1] + ncol;
  return m;
}
complex **CMatrix(long nrl, long nrh, long ncl, long nch)
/*
����һ������double�ľ��� m[nrl..nrh][ncl..nch]
*/
{
  long i, nrow = nrh - nrl + 1, ncol = nch - ncl + 1;
  complex **m;

  m = (complex **)malloc(
      (size_t)((nrow) * sizeof(complex *)));  //����row����ָ��
  if (!m) nrerror("allocation matrix row failure!");
  m -= nrl;

  m[nrl] = (complex *)malloc(
      (size_t)((nrow * ncol) * sizeof(complex)));  //����row*col���ڴ�ռ�
  if (!m[nrl]) nrerror("allocation matrix col failure!");
  m[nrl] -= ncl;

  for (i = nrl + 1; i <= nrh; i++) m[i] = m[i - 1] + ncol;
  return m;
}
void free_Matrix(double **m, long nrl, long nrh, long ncl, long nch) {
  free((FREE_ARG)(m[nrl] + ncl));
  free((FREE_ARG)(m + nrl));
}
void free_CMatrix(complex **m, long nrl, long nrh, long ncl, long nch) {
  free((FREE_ARG)(m[nrl] + ncl));
  free((FREE_ARG)(m + nrl));
}

void Matrix_memcpy(double **a, long nrl, long nrh, long ncl, long nch,
                   double **b)
/*
ʵ�־�����ڴ濽������bָ��ľ��󿽱���aָ��ľ���
*/
{
  int nLen;
  long nrow = nrh - nrl + 1, ncol = nch - ncl + 1;

  nLen = (size_t)((nrow * ncol) * sizeof(double));
  memcpy(a[nrl] + ncl, b[nrl] + ncl, nLen);
}
void CMatrix_memcpy(complex **a, long nrl, long nrh, long ncl, long nch,
                    complex **b)
/*
ʵ�ָ���������ڴ濽������bָ��ľ��󿽱���aָ��ľ���
*/
{
  int nLen;
  long nrow = nrh - nrl + 1, ncol = nch - ncl + 1;

  nLen = (size_t)((nrow * ncol) * sizeof(complex));
  memcpy(a[nrl] + ncl, b[nrl] + ncl, nLen);
}

void ludcmp(double **a, int n, int *index, double *d)
/*
  ʵ�ֶ�a�������û���LU�ֽ�
*/
{
  int i, imax, j, k;
  double big, dum, sum, temp;
  double *vv;

  vv = vector(1, n);
  *d = 1.0;
  for (i = 1; i <= n; i++) {
    big = 0.0;
    for (j = 1; j <= n; j++) {
      temp = fabs(a[i][j]);
      if (temp > big) big = temp;
    }
    if (big == 0)
      nrerror("Singular matrix in routine ludcmp");
    else
      vv[i] = 1.0 / big;
  }
  for (j = 1; j <= n; j++) {
    for (i = 1; i <= j; i++) {
      sum = a[i][j];
      for (k = 1; k < i; k++) sum -= a[i][k] * a[k][j];
      a[i][j] = sum;
    }
    big = a[j][j];
    imax = j;
    for (i = j + 1; i <= n; i++) {
      sum = a[i][j];
      for (k = 1; k < j; k++) sum -= a[i][k] * a[k][j];
      a[i][j] = sum;
      dum = vv[i] * fabs(sum);
      if (dum >= big) {
        big = dum;
        imax = i;
      }
    }
    if (j != imax) {
      for (k = 1; k <= n; k++) {
        dum = a[imax][k];
        a[imax][k] = a[j][k];
        a[j][k] = dum;
      }
      *d = -(*d);
      dum = vv[imax];
      vv[imax] = vv[j];
      vv[j] = dum;
    }
    index[j] = imax;
    if (a[j][j] == 0.0) a[j][j] = TINY;
    if (j != n) {
      dum = 1.0 / (a[j][j]);
      for (i = j + 1; i <= n; i++) a[i][j] *= dum;
    }
  }
  free_vector(vv, 1, n);
}
void Cludcmp(complex **a, int n, int *index, double *d)
/*
  ʵ�ֶԸ���double����a �����û���LU�ֽ�
*/
{
  int i, imax, j, k;
  complex big, dum, sum, temp;
  complex *vv, cc;

  vv = Cvector(1, n);
  *d = 1.0;
  cc.real = 1; /* cc=1+0*i */
  cc.imag = 0;
  for (i = 1; i <= n; i++) {
    big.real = 0.0;
    big.imag = 0.0;
    for (j = 1; j <= n; j++) {
      temp = a[i][j];
      if (Cabs(temp) > Cabs(big)) big = temp;
    }
    if (big.real == 0 && big.imag == 0.0)
      nrerror("Singular matrix in routine ludcmp");
    else
      vv[i] = Cdiv(cc, big);
    /*vv[i]=1.0/big;*/
  }
  for (j = 1; j <= n; j++) {
    for (i = 1; i <= j; i++) {
      sum.real = a[i][j].real;
      sum.imag = a[i][j].imag;
      for (k = 1; k < i; k++) {
        temp = Cmultiply(a[i][k], a[k][j]);
        sum = Cminus(sum, temp);
      }
      a[i][j].real = sum.real;
      a[i][j].imag = sum.imag;
    }
    big.real = a[j][j].real;
    big.imag = a[j][j].imag;
    imax = j;
    for (i = j + 1; i <= n; i++) {
      sum.real = a[i][j].real;
      sum.imag = a[i][j].imag;
      for (k = 1; k < j; k++) {
        temp = Cmultiply(a[i][k], a[k][j]);
        sum = Cminus(sum, temp);
      }
      a[i][j].real = sum.real;
      a[i][j].imag = sum.imag;
      /*dum=vv[i]*fabs(sum);*/
      dum = Cmultiply(vv[i], sum);
      if (Cabs(dum) >= Cabs(big)) {
        big.real = dum.real;
        big.imag = dum.imag;
        imax = i;
      }
    }
    if (j != imax) {
      for (k = 1; k <= n; k++) {
        dum.real = a[imax][k].real;
        dum.imag = a[imax][k].imag;
        a[imax][k].real = a[j][k].real;
        a[imax][k].imag = a[j][k].imag;
        a[j][k].real = dum.real;
        a[j][k].imag = dum.imag;
      }
      *d = -(*d);
      dum.real = vv[imax].real;
      dum.imag = vv[imax].imag;
      vv[imax].real = vv[j].real;
      vv[imax].imag = vv[j].imag;
      vv[j].real = dum.real;
      vv[j].imag = dum.imag;
    }
    index[j] = imax;
    if (a[j][j].real == 0.0 && a[j][j].imag == 0.0) {
      a[j][j].real = TINY;
      a[j][j].imag = TINY;
    }
    if (j != n) {
      dum = Cdiv(cc, a[j][j]);
      for (i = j + 1; i <= n; i++) a[i][j] = Cmultiply(a[i][j], dum);
    }
  }
  free_Cvector(vv, 1, n);
}

void lubksb(double **a, int n, int *index, double b[])
/*
   ���nά���Է�����A*X=B��
*/
{
  int i, ii = 0, ip, j;
  double sum;

  for (i = 1; i <= n; i++) {
    ip = index[i];
    sum = b[ip];
    b[ip] = b[i];
    if (ii)
      for (j = ii; j <= i - 1; j++) sum -= a[i][j] * b[j];
    else if (sum)
      ii = i;
    b[i] = sum;
  }
  for (i = n; i >= 1; i--) {
    sum = b[i];
    for (j = i + 1; j <= n; j++) sum -= a[i][j] * b[j];
    b[i] = sum / a[i][i];
  }
}
void Clubksb(complex **a, int n, int *index, complex b[])
/*
   ��⸴��double�͵�nά���Է�����A*X=B��
*/
{
  int i, ii = 0, ip, j;
  complex sum, temp;

  for (i = 1; i <= n; i++) {
    ip = index[i];
    sum.real = b[ip].real;
    sum.imag = b[ip].imag;
    b[ip].real = b[i].real;
    b[ip].imag = b[i].imag;
    if (ii)
      for (j = ii; j <= i - 1; j++) {
        temp = Cmultiply(a[i][j], b[j]);
        sum = Cminus(sum, temp);
      }
    else if (Cabs(sum) > 0)
      ii = i;
    b[i].real = sum.real;
    b[i].imag = sum.imag;
  }
  for (i = n; i >= 1; i--) {
    sum.real = b[i].real;
    sum.imag = b[i].imag;
    for (j = i + 1; j <= n; j++) {
      temp = Cmultiply(a[i][j], b[j]);
      sum = Cminus(sum, temp);
    }
    b[i] = Cdiv(sum, a[i][i]);
  }
}
void Matrix_inv(double **a, int n)
/*
   �������溯��������Ϊ��ά����a ��ά��n ,���Ϊ��ά����a.
*/
{
  double **y, d, *col;
  int i, j, *index;

  y = Matrix(1, n, 1, n);
  col = vector(1, n);
  index = ivector(1, n);

  ludcmp(a, n, index, &d);  //��a����LU�ֽ�,������a�С�

  for (j = 1; j <= n; j++) {
    for (i = 1; i <= n; i++) col[i] = 0.0;
    col[j] = 1.0;
    lubksb(a, n, index, col);                   //�ⷽ����
    for (i = 1; i <= n; i++) y[i][j] = col[i];  // y����a��ת�þ���
  }

  Matrix_memcpy(a, 1, n, 1, n, y);  //��y����a

  free_Matrix(y, 1, n, 1, n);
  free_vector(col, 1, n);
  free_ivector(index, 1, n);
}
void CMatrix_inv(complex **a, int n, complex **y)
/*
   �����������溯��
   ����Ϊ��ά����a ��ά��n ,���Ϊ��ά����y.
*/
{
  double d;
  complex *col;
  int i, j, *index;

  col = Cvector(1, n);
  index = ivector(1, n);

  Cludcmp(a, n, index, &d);  //��a����LU�ֽ�,������a�С�

  for (j = 1; j <= n; j++) {
    for (i = 1; i <= n; i++) {
      col[i].real = 0.0;
      col[i].imag = 0.0;
    }
    col[j].real = 1.0;
    col[j].imag = 0;
    Clubksb(a, n, index, col);  //�ⷽ����
    for (i = 1; i <= n; i++) {
      y[i][j].real = col[i].real;  // y����a��ת�þ���
      y[i][j].imag = col[i].imag;
    }
  }

  free_Cvector(col, 1, n);
  free_ivector(index, 1, n);
}
void Matrix_transpose(double **a, int row, int col, double **b)
/*
  ʵ�־���aת��,���������b;
*/
{
  int i, j;
  for (i = 1; i <= col; i++)
    for (j = 1; j <= row; j++) {
      b[i][j] = a[j][i];
    }
}
void CMatrix_transpose(complex **a, int row, int col, complex **b)
/*
  ʵ�ָ�������a�Ĺ���ת��,���������b;
*/
{
  int i, j;
  for (i = 1; i <= col; i++)
    for (j = 1; j <= row; j++) {
      b[i][j].real = a[j][i].real;
      b[i][j].imag = -a[j][i].imag;
    }
}

void Matrix_multiply(double **a, int row1, int col1, double **b, int row2,
                     int col2, double **c)
/*
 ʵ������ĳ˷�
 c=a x b
*/
{
  int i, j, k;

  if (col1 != row2)
    nrerror("��һ����������������ڵڶ���������������������!");
  for (i = 1; i <= row1; i++)
    for (j = 1; j <= col2; j++) {
      c[i][j] = 0;
      for (k = 1; k <= col1; k++) c[i][j] += a[i][k] * b[k][j];
    }
}
void CMatrix_multiply(complex **a, int row1, int col1, complex **b, int row2,
                      int col2, complex **c)
/*
��������˷�
 c=a x b
*/
{
  int i, j, k;

  if (col1 != row2)
    nrerror("��һ����������������ڵڶ���������������������!");
  for (i = 1; i <= row1; i++)
    for (j = 1; j <= col2; j++) {
      c[i][j].real = 0;
      c[i][j].imag = 0;
      for (k = 1; k <= col1; k++) {
        c[i][j] = Cadd(c[i][j], Cmultiply(a[i][k], b[k][j]));
      }
    }
}
void Matrix_plus(double **a, double **b, int row, int col)
/*
 ʵ�־���ӷ���a[1..row][1..col]��a[1..row][1..col]+b[1..row][1..col],����ֵa.
*/
{
  int i, j;
  for (i = 1; i <= row; i++)
    for (j = 1; j <= col; j++) a[i][j] += b[i][j];
}
void CMatrix_plus(complex **a, complex **b, int row, int col, complex **c)
/*
 ��������ļӷ���
 c��a + b
*/
{
  int i, j;
  for (i = 1; i <= row; i++)
    for (j = 1; j <= col; j++) c[i][j] = Cadd(a[i][j], b[i][j]);
}
void Matrix_minus(double **a, double **b, int row, int col)
/*
 ʵ�־��������a[1..row][1..col]��a[1..row][1..col]-b[1..row][1..col],����ֵa.
*/
{
  int i, j;
  for (i = 1; i <= row; i++)
    for (j = 1; j <= col; j++) a[i][j] -= b[i][j];
}
void CMatrix_minus(complex **a, complex **b, int row, int col, complex **c)
/*
 �������������
 c = a - b
*/
{
  int i, j;
  for (i = 1; i <= row; i++)
    for (j = 1; j <= col; j++) c[i][j] = Cminus(a[i][j], b[i][j]);
}
