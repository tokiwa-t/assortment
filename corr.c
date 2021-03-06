#include <stdio.h>
#include <math.h>

int main(){
    int i, j, k;
    double data[12][6] = { // 6列のサンプルデータ
        {90, 0.267, 141, 651, 491, 3.16},
        {79, 0.258, 106, 615, 581, 3.62},
        {73, 0.257, 85, 561, 563, 3.69},
        {69, 0.263, 136, 631, 573, 3.69},
        {61, 0.249, 94, 519, 548, 3.59},
        {57, 0.241, 85, 463, 612, 3.82},
        {76, 0.257, 107, 574, 518, 3.31},
        {75, 0.243, 98, 489, 443, 2.78},
        {70, 0.247, 78, 465, 550, 3.47},
        {69, 0.246, 105, 506, 474, 2.92},
        {62, 0.253, 71, 473, 504, 3.19},
        {62, 0.249, 112, 508, 598, 3.80}
    };
    double ave[6] = {0}; // 各データ列の平均を格納
    double corr[6][6]; // 相関行列
    double xp, xq; // 各データ列の標準偏差
    double numer, denom; // 分子, 分母

    // 各データ列の平均を求める.
    for(i=0;i<6;i++){
        for(j=0;j<12;j++){
            ave[i] += data[j][i];
        }
        ave[i] /= 12; // データ点数が各列で12個存在するため.
    }

    // 相関行列を求める.
    for(i=0;i<6;i++){
        for(j=0;j<6;j++){
            // 総和計算の初期化
            numer = 0;
            xp = 0; xq = 0;
            

            for(k=0;k<12;k++){
                numer += (data[k][i] - ave[i]) * (data[k][j] - ave[j]); // 分散の積を求める
                xp += pow((data[k][i] - ave[i]), 2); // 標準偏差
                xq += pow((data[k][j] - ave[j]), 2); // 標準偏差
            }

            denom = sqrt(xp) * sqrt(xq); // 標準偏差の積
            corr[i][j] = numer / denom; // 相関行列に格納
        }
    }

    // 表示部
    for(i=0;i<6;i++){
        for(j=0;j<6;j++){
            if(corr >= 0) // 正の数であれば, 符号分を空けて表示
                printf(" %2.4f ", corr[i][j]);
            else
                printf(" %2.4f ", corr[i][j]);
        }
        printf("\n");
    }

    return 0;

}