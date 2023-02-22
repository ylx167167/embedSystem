#include "stdio.h"
#include "My_capon.h"

int main() {
  double G_capon[360];
  int G[360];
  double nMax = -100000.0, nMin = 100000.0;
  int lableMax, lableMin;
  int i, j;
  int M = 8;  //阵元数


  //计算capon波束形成器的各角度幅值,计算结果有G_capon返回
  Compute_capon(60, 30, 0, M, G_capon);

  for (i = 0; i < 360; i++) {
    if (G_capon[i] > nMax) {
      nMax = G_capon[i];  //找到最大值
      lableMax = i;
    }
    if (G_capon[i] < nMin) {
      nMin = G_capon[i];  //找到最小值
      lableMin = i;
    }
  }
  for (i = 0; i < 360; i++) {
    G_capon[i] += abs(nMin);  //通过整体平移，把负值都变成正值
  }
  nMax = nMax + abs(nMin);
  for (i = 0; i < 360; i++)  //量化成高为250,宽为360的数组
  {
    G[i] = int((double)G_capon[i] / nMax * 250);
    if (G[i] <= 0) G[i] = 0;
  }
  printf("%d %d ", lableMax, lableMin);
  float test = 2.f;
  printf("%f ", test);
}