#include "My_math.h"

void Compute_capon(double theta1, double theta2, double SNR, int M,
                   double G_capon[360])
/*
实现capon波束形成中，角度搜索的峰值
输入参数：
                theta1:期望信号角度
                theta2:干扰信号角度
                SNR:信噪比
                M: 阵元数
输出参数：
                G:各个不同角度时的capon峰值,-90:90 采样间隔为0.5度
*/
{
  double theta;
  complex **s1, **s2, **s1_tran, **s2_tran, **Ryy, **Ryy_inv, **w, **w_tran;
  complex **temp1_MM, **temp2_MM, **temp3_MM, **noise_MM, **temp_1M, **temp_M1,
      **temp_11;
  double temp_G;
  int i, j, k;
  double pp;

  s1 = CMatrix(1, M, 1, 1);  //信号s1, 一个 M 维列向量
  s2 = CMatrix(1, M, 1, 1);  //信号s2, 一个 M 维列向量
  for (i = 0; i < M;
       i++)  //产生复指数信号s1,s2;s1=exp(-j.*pi.*sin(theta1*pi/180).*ind).';
  {
    k = i + 1;
    s1[k][1].real = cos(-PI * sin(theta1 * PI / 180) * i);
    s1[k][1].imag = sin(-PI * sin(theta1 * PI / 180) * i);
    s2[k][1].real = cos(-PI * sin(theta2 * PI / 180) * i);
    s2[k][1].imag = sin(-PI * sin(theta2 * PI / 180) * i);
  }


  //计算Ryy=s1*s1'+s2*s2'+10.^(-SNR/10)*eye()
  Ryy = CMatrix(1, M, 1, M);      //阵元输出信号的协方差矩阵
  Ryy_inv = CMatrix(1, M, 1, M);  // Ryy的逆矩阵
  s1_tran = CMatrix(1, 1, 1, M);  // s1的共轭转置 一个M维的行向量
  s2_tran = CMatrix(1, 1, 1, M);  // s2的共轭转置 一个M维的行向量
  temp1_MM = CMatrix(1, M, 1, M);
  temp2_MM = CMatrix(1, M, 1, M);
  temp3_MM = CMatrix(1, M, 1, M);
  noise_MM = CMatrix(1, M, 1, M);
  CMatrix_transpose(s1, M, 1, s1_tran);
  CMatrix_transpose(s2, M, 1, s2_tran);
  CMatrix_multiply(s1, M, 1, s1_tran, 1, M, temp1_MM);
  CMatrix_multiply(s2, M, 1, s2_tran, 1, M, temp2_MM);

  pp = pow(10, -SNR / 10);
  for (i = 1; i <= M; i++)
    for (j = 1; j <= M; j++) {
      if (j == i) {
        noise_MM[i][j].real = pp;
        noise_MM[i][j].imag = 0;
      } else {
        noise_MM[i][j].real = 0;
        noise_MM[i][j].imag = 0;
      }
    }

  CMatrix_plus(temp1_MM, temp2_MM, M, M, temp3_MM);
  CMatrix_plus(temp3_MM, noise_MM, M, M, Ryy);

  //	CMatrix_print(s1,M,1);
  //	CMatrix_print(Ryy,M,M);


  //计算w_MVDR=(inv(Ryy)*s1/(s1'*inv(Ryy)*s1))
  CMatrix_inv(Ryy, M, Ryy_inv);

  //	CMatrix_print(Ryy_inv,M,M);

  temp_1M = CMatrix(1, 1, 1, M);
  temp_M1 = CMatrix(1, M, 1, 1);
  temp_11 = CMatrix(1, 1, 1, 1);
  CMatrix_multiply(s1_tran, 1, M, Ryy_inv, M, M, temp_1M);
  // CMatrix_multiply(temp_1M,1,M,s1,M,1,temp_11); //temp_11=s1'*inv(Ryy)*s1)
  // 维数为 1x1 CMatrix_multiply(Ryy_inv,M,M,s1,M,1,temp_M1);
  // //temp_M1=inv(Ryy)*s1     维数为  M x 1
  CMatrix_multiply(temp_1M, 1, M, s2, M, 1,
                   temp_11);  // temp_11=s1'*inv(Ryy)*s1) 维数为 1x1
  CMatrix_multiply(Ryy_inv, M, M, s2, M, 1,
                   temp_M1);  // temp_M1=inv(Ryy)*s1     维数为  M x 1
  w = CMatrix(1, M, 1, 1);    //权向量 , M 维列向量
  for (i = 1; i <= M; i++) {
    w[i][1] = Cdiv(temp_M1[i][1], temp_11[1][1]);
  }

  //	CMatrix_print(w,M,1);


  //////计算不同输入角度的capon幅度/////////////////////////////////
  w_tran = CMatrix(1, 1, 1, M);  //权向量的共轭转置 , M 维行向量
  CMatrix_transpose(w, M, 1, w_tran);
  theta = -90;
  for (i = 0; i < 360; i++)  //角度搜索 （-90：0.5：89）
  {
    for (j = 0; j < M;
         j++)  //产生复指数信号s1=exp(-j.*pi.*sin(theta1*pi/180).*ind).';
    {
      k = j + 1;
      s1[k][1].real = cos(-PI * sin(theta * PI / 180) * j);
      s1[k][1].imag = sin(-PI * sin(theta * PI / 180) * j);
    }
    CMatrix_multiply(w_tran, 1, M, s1, M, 1, temp_11);
    temp_G = Cabs(temp_11[1][1]) *
             Cabs(temp_11[1][1]);               // G_MVDR(k)=abs((w_MVDR*s).^2);
    G_capon[i] = 10 * (log(temp_G) / log(10));  // G=10.*log10(G);
    theta = theta + 0.5;
    //	cout<<G_capon[i]<<"    " ;
  }


  free_CMatrix(s1, 1, M, 1, 1);
  free_CMatrix(s2, 1, M, 1, 1);
  free_CMatrix(s1_tran, 1, 1, 1, M);
  free_CMatrix(s2_tran, 1, 1, 1, M);
  free_CMatrix(Ryy, 1, M, 1, M);
  free_CMatrix(Ryy_inv, 1, M, 1, M);
  free_CMatrix(w, 1, M, 1, 1);
  free_CMatrix(w_tran, 1, 1, 1, M);
  free_CMatrix(temp1_MM, 1, M, 1, M);
  free_CMatrix(temp2_MM, 1, M, 1, M);
  free_CMatrix(temp3_MM, 1, M, 1, M);
  free_CMatrix(noise_MM, 1, M, 1, M);
  free_CMatrix(temp_1M, 1, 1, 1, M);
  free_CMatrix(temp_M1, 1, M, 1, 1);
  free_CMatrix(temp_11, 1, 1, 1, 1);
}
