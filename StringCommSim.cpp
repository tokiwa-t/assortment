#include <stdio.h>
#include <iostream>
#include <random>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
using namespace std;

#define PI 3.141592653589
#define CODESIZE_BEFORE 4
#define CODESIZE_AFTER 7
#define REPEAT_L 1
#define BINARY_BIT 128
#define N_MAX 10000000
#define NUM_ARRAY_SIZE 2<<14

unsigned long long random_num(
    unsigned long long start, 
    unsigned long long end)
{ // startからendの範囲で乱数
    unsigned long long i;
    random_device rnd_root_sub;
    mt19937 mt_root_sub(rnd_root_sub());
    uniform_int_distribution<> rand_root_sub(start, end);
    i = rand_root_sub(mt_root_sub); 
    return i;
}

void mpAdd(int *ret, int *a, int *b)
{
    int  i;
    int  lr, la, lb;
    int  *rr;
    int  x;

    la = *a;
    lb = *b;
    lr = (la >= lb) ? la : lb;

    rr = ret;
    x = 0;
    for (i = 1; i <= lr; i++) {
        if (i <= la) x += *++a;
        if (i <= lb) x += *++b;
        if (x < N_MAX) {
            *++rr = x;
            x = 0;
        } else {
            *++rr = x - N_MAX;
            x = 1;
        }
    }
    *++rr = x;
    *ret = lr + x;
}

void mpSub(int *ret, int *a, int *b)
{
    int  i;
    int  la, lb;
    int  *rr;
    int  x;

    la = *a;
    lb = *b;

    rr = ret;
    x = 0;
    for (i = 1; i <= la; i++) {
        x += *++a;
        if (i <= lb) x -= *++b;
        if (x >= 0) {
            *++rr = x;
            x = 0;
        } else {
            *++rr = x + N_MAX;
            x = -1;
        }
    }
    while (--i > 0)
        if (*rr-- != 0) break;
    *ret = i;
}

void mpMul(int *ret, int *a, int *b)
{
    int  i, j;
    int  la, lb;
    int  *aa;
    int  ca;
    long x;

    la = *a;
    lb = *b;

    if (la == 0 || lb == 0) {
        *ret = 0;
        return;
    }

    for (i = la + lb; i > 0; i--) *(ret + i) = 0;

    for (j = 1; j <= lb; j++) {
        ca = 0;
        b++;
        for (i = 1, aa = a; i <= la; i++) {
            x = *++aa;
            x = x * *b + *(ret + i + j - 1) + ca;
            *(ret + i + j - 1) = x % N_MAX;
            ca = x / N_MAX;
        }
        *(ret + i + j - 1) = ca;
    }
    
    *ret = (ca != 0) ? la + lb : la + lb - 1;
}

int mpDiv(int *q, int *r, int *za, int *zb)
{
    int  i;
    int  m, n;
    int  *aa, *bb, *qq, *rr, *t;
    int  *a, *b;
    int  ca;
    unsigned long long x;
    int  k, Q;

    *q = 0, *r = 0;
    if (*zb == 0) return -2;
    if (*za == 0) return 0;

    if (*za < *zb) {
        for (aa = za, rr = r, i = *za; i >= 0; i--) *rr++ = *aa++; 
        return 0;
    }
    
    if (*zb == 1) {
        *q = *za;
        zb++;
        for (ca = 0, aa = za + *za, qq = q + *q, i = *za; i > 0; i--) {
            x = (unsigned long long)N_MAX * ca + *aa--;
            *qq-- = x / *zb, ca = x % *zb;
        }
        if (*(q + *q) == 0) (*q)--;
        if (ca > 0) {
            *r++ = 1;
            *r = ca;
        } else *r = 0;
        return 0;
    }

    /*
    if ((a = (int *)malloc((sizeof(*za)+1) * sizeof(int))) == NULL) return -1;
    if ((b = (int *)malloc((sizeof(*zb)+2) * sizeof(int))) == NULL) {
        free(a);
        return -1;
    }
    */

    a = new int[ (sizeof(*za)+1) * sizeof(int) * (sizeof(*za) * 2) ];
    b = new int[ (sizeof(*zb)+2) * sizeof(int) * (sizeof(*zb) * 2) ];

    for (aa = a, i = *za; i >= 0; i--) *aa++ = *za++; 
    for (bb = b, i = *zb; i >= 0; i--) *bb++ = *zb++; 

    if ((k = (N_MAX/2-1) / *(b + *b) + 1) > 1) {
        for (ca = 0, aa = a, qq = q, i = 0; i < *a; i++) {
            x = (unsigned long long)k * *++aa + ca;      /* a = a * k */
            *++qq = x % N_MAX, ca = x / N_MAX;
        }
        *++qq = ca;
        *q = i + (ca > 0);
        for (qq = q, aa = a, i = *q; i >= 0; i--) *aa++ = *qq++;

        for (ca = 0, bb = b, qq = q, i = 0; i < *b; i++) {
            x = (unsigned long long)k * *++bb + ca;      /* b = b * k */
            *++qq = x % N_MAX, ca = x / N_MAX;
        }
        *++qq = ca;
        *q = i + (ca > 0);
        for (qq = q, bb = b, i = *q; i >= 0; i--) *bb++ = *qq++;
    }

    *q = *a - *b + 1;
    for (qq = q, i = *q; i > 0; i--) *++qq = 0;
    n = *b;
    while ((m = *a) >= n) {
        if (*(a + *a) >= *(b + *b)) {
            for (aa = a + *a, bb = b + *b; bb != b; aa--, bb--) 
                if (*aa != *bb) break;
            if (bb == b) {
                *a -= *b;
                *(q + m - n + 1) = 1;
                continue;
            } else if (*aa > *bb) {
                for (t = bb, ca = 0, aa = a + m - n, bb = b; bb < t; ) {
                    *++aa -= *++bb + ca;
                    ca = 0;
                    if (*aa < 0) *aa += N_MAX, ca = 1;
                }
                while (*aa == 0) aa--;
                *a = aa - a;
                *(q + m - n + 1) = 1;
                continue;
            }
            Q = N_MAX - 1;
        } else Q = ((unsigned long long)N_MAX * *(a + *a) + *(a + *a - 1)) / *(b + *b);
        if (m == n) break;
        
        for ( ; ; ) {
            if (Q == 1) {
                *(b + *b + 1) = 0;
                for (ca=0, aa=a+(*a - *b-1), bb=b, i= *b; i >= 0; i--) {
                    *++aa -= *++bb + ca;
                    ca = 0;
                    if (*aa < 0) *aa += N_MAX, ca = 1;
                }
                while (*aa == 0) aa--;
                *a = aa - a;
                break;
            }
            for (ca = 0, rr = r, bb = b, i = 0; i < *b; i++) {
                x = (unsigned long long)Q * *++bb + ca;
                *++rr = x % N_MAX, ca = x / N_MAX;
            }
            *++rr = ca;
            *r = i + 1;
            for (aa = a + *a, rr = r + *r; rr != r; aa--, rr--)
                if (*aa != *rr) break;
            if (rr == r) {
                *a -= *r;
                break;
            } else if (*aa > *rr) {
                for (t = rr, ca = 0, aa = a + (*a - *r), rr = r; rr < t; ) {
                    *++aa -= *++rr + ca;
                    ca = 0;
                    if (*aa < 0) *aa += N_MAX, ca = 1;
                }
                while (*aa == 0) aa--;
                *a = aa - a;
                break;
            } else Q--;
        }
        *(q + m - n) = Q;
    }
    if (*(q + *q) == 0) (*q)--;
    
    if (k > 1) {
        for (ca = 0, aa = a + *a, rr = r + *a, i = *a; i > 0; i--) {
            x = (unsigned long long)N_MAX * ca + *aa--;
            *rr-- = x / k, ca = x % k;
        }
        *r = *a - (*(r + *a) == 0);
    } else for (aa = a, rr = r, i = *a; i >= 0; i--) *rr++ = *aa++; 

    /*
    // free(b);
    free(a);
    */

    delete[] b;
    delete[] a;
    return 0;
}

void mpNum2Str(char *str, int *num)
{
    int  i, j;
    char *ss;
    int  x;

    if (*num == 0) {
        *str++ = '0';
        *str = '\0';
        return;
    }

    ss = str - 1;
    for (i = *num; i > 0; i--) {
        x = *++num;
        for (j = 1; j < N_MAX; j *= 10) {
            *++ss = x % 10 + '0';
            x /= 10;
        }
    }
    while (*ss == '0') ss--;

    *(ss + 1) = '\0';    
    while (str < ss) {
        x = *str;
        *str++ = *ss;
        *ss-- = x;
    }
}

void mpStr2Num(int *num, char *str)
{
    char *ss;
    int  *nn;
    int  k;
    int  x;

    while (*str == ' ' || *str == '\t') str++;
    while (*str == '0') str++;
    ss = str;
    while (*ss >= '0' && *ss <= '9') ss++;

    if (ss == str) {
        *num = 0;
        return;
    }

    x = 0;
    k = 1;
    nn = num;
    do {
        x += (*--ss - '0') * k;
        k *= 10;
        if (k == N_MAX || ss == str) {
            *++nn = x;
            x = 0;
            k = 1;
        }
    } while (ss != str);

    *num = nn - num;
}

int mpNum2Long(unsigned long long *ul, int *num)
{
    unsigned long long maxUL, maxULdivN;
    unsigned long long x;
    int      *nn;

    maxUL = ~0;
    maxULdivN = maxUL / N_MAX;

    x = 0;
    for (nn = num + *num; nn != num; ) {
        if (x > maxULdivN) return -1;
        x *= N_MAX;
        if (*nn > maxUL - x) return -1;
        x += *nn--;
    }
    *ul = x;

    return 0;
}

int mpNum2Int(int *ul, int *num)
{
    unsigned long long maxUL, maxULdivN;
    unsigned long long x;
    int      *nn;

    maxUL = ~0;
    maxULdivN = maxUL / N_MAX;

    x = 0;
    for (nn = num + *num; nn != num; ) {
        if (x > maxULdivN) return -1;
        x *= N_MAX;
        if (*nn > maxUL - x) return -1;
        x += *nn--;
    }
    *ul = x;

    return 0;
}

void mpLong2Num(int *num, unsigned long long ul)
{
    int  *nn = num;

    while (ul != 0) {
        *++nn = ul % N_MAX;
        ul /= N_MAX;
    }
    *num = nn - num;
}

int mpCmp(int *a, int *b)
{
    int  *aa;

    if (*a != *b) return *a - *b;

    aa = a;
    a += *aa;
    b += *aa;
    while (a != aa) {
        if (*a != *b) return *a - *b;
        a--;
        b--;
    }
    return 0;
}

void mpDec2Bin(int *count, int *bin, int *dec){
    int *tmp, *tmp2, *tmp3;
    tmp = new int[NUM_ARRAY_SIZE]; tmp2 = new int[NUM_ARRAY_SIZE]; tmp3 = new int[NUM_ARRAY_SIZE];
    int zero[2] = {0}; int two[2] = {1, 2};
    int i = 0;
    int tst;
    *count = 0;

    mpAdd(tmp, dec, zero);
    do{
        if( mpCmp(tmp, zero) > 0 ){
            mpDiv(tmp2, tmp3, tmp, two);
            mpNum2Int(&bin[i], tmp3);
            mpNum2Int(&tst, tmp3);
            mpAdd(tmp, tmp2, zero);
            *count += 1;
        }else{
            //bin[i] = 0;
            //*count += 1;
        }
        i++;
    }while( mpCmp(tmp, zero) > 0);

    // free(tmp); free(tmp2); free(tmp3);
    delete[] tmp;
    delete[] tmp2;
    delete[] tmp3;
}

void mpBin2Dec(int *dec, int *bin, int *count){
    int *tmp, *tmp2;
    tmp = new int[NUM_ARRAY_SIZE]; tmp2 = new int[NUM_ARRAY_SIZE];
    int zero[2] = {0}; int one[2] = {1, 1}; int two[2] = {1, 2};
    int i, j;
    for(i=0;i<*count;i++){
        if(bin[i] == 1){
            mpAdd(tmp, one, zero);
            for(j=0;j<i;j++){
                mpMul(tmp2, tmp, two);
                mpAdd(tmp, tmp2, zero);
            }
            if(i!=0) mpAdd(tmp2, dec, zero);
            mpAdd(dec, tmp, tmp2);
        }
    }
    delete[] tmp;
    delete[] tmp2;
    // free(tmp); free(tmp2);
}

class montgomery{

private:
    int *m_n;   // 法(m_n)
    int *m_r2;  // rを(m_n)より大きな２のべきとして、(m_r2)=(r)^2 mod n
    int *m_n2;  // (m_n)*(m_n2) ≡ -1 mod r を満たす(m_n2)
    int  m_nb;  // (m_n)の有効Bit数, 例えば(m_n)=5 のとき(m_nb)=3

    int i;
    int z;

public:
    
    montgomery(int *n){
        int zero[2] = {0, 0};
        m_n = new int[NUM_ARRAY_SIZE]; m_r2 = new int[NUM_ARRAY_SIZE]; m_n2 = new int[NUM_ARRAY_SIZE];
        // m_n = new int[NUM_ARRAY_SIZE]; m_r2 = new int[NUM_ARRAY_SIZE]; m_n2 = new int[NUM_ARRAY_SIZE];
        mpAdd(m_n, n, zero);

        int *bin = new int[NUM_ARRAY_SIZE];
        mpDec2Bin(&m_nb, bin, m_n);
        // cout << "N bit = " << m_nb << endl;

        // r: nより大きな２のべき
        // unsigned long long r(1);
        int *r = new int[NUM_ARRAY_SIZE]; r[0] = 1; r[1] = 1;
        int *two = new int[2]; two[0] = 1; two[1] = 2;
        int *tmp_pow = new int[NUM_ARRAY_SIZE];

        // r = pow(2, m_nb);
        
        for(i=0;i<m_nb;i++){
            mpMul(tmp_pow, r, two);
            mpAdd(r, tmp_pow, zero);
        }

        // r2 = r^2 mod n
        // m_r2 = ( (r<<m_nb) % n) * ( (r<<m_nb) % n) % n;
        int *r2 = new int[(NUM_ARRAY_SIZE)*10]; r2[0] = 1; r2[1] = 1;
        int *tmp_div = new int[NUM_ARRAY_SIZE];
        tmp_div[0] = 1; tmp_div[1] = 0;
        mpDiv(r2, tmp_div, r, m_n);
        mpAdd(tmp_pow, tmp_div, zero);
        mpMul(r2, tmp_div, tmp_pow);

        mpAdd(tmp_div, r2, zero);
        mpDiv(tmp_pow, r2, tmp_div, m_n);
        mpAdd(m_r2, r2, zero);

        // n*n2 ≡ -1 mod r
        //mpMul(tmp_pow, zero, m_n);
        m_n2[0] = 1; m_n2[1] = 0; /* 求めるN' */
        int *t = new int[NUM_ARRAY_SIZE];
        t[0] = 1; t[1] = 0;
        int *vi = new int[NUM_ARRAY_SIZE]; vi[0] = 1; vi[1] = 1;
        int ni = m_nb;
        while (ni--){ /* Rのトップビットを除いたビット数分繰り返す */
            mpDec2Bin(&z, bin, t);
            if(bin[0] == 0){
                /* ゼロになっているビットがあったら、N'のその部分を1にする（NはRと互いに素なので必ず奇数）*/
                mpAdd(tmp_pow, t, m_n);
                mpAdd(t, tmp_pow, zero); //t += n; /* 掛け算だが、二進数一桁の掛け算なので実質は足し算 */
                mpAdd(tmp_pow, m_n2, vi);
                mpAdd(m_n2, tmp_pow, zero); //m_n2 += vi; /* N'のその部分を1にする */
            }
            mpDiv(tmp_div, tmp_pow, t, two);
            mpAdd(t, tmp_div, zero); //t >>= 1; /* 必ず端数が出るが切り捨てる */
            mpMul(tmp_div, vi, two);
            mpAdd(vi, tmp_div, zero); //vi <<= 1; /* Rは2の冪なので、絶対端数は出ない */
        }

        delete[] bin;
        delete[] r2;
        delete[] r;
        delete[] tmp_pow;
        delete[] tmp_div;
        delete[] t;
        delete[] vi;

        //char str1[1024]; mpNum2Str(str1,m_n); cout << "m_n(mp) = " << str1 << endl;
        //char str2[1024]; mpNum2Str(str2,r2); cout << "r2(mp) = " << str2 << endl;

    }

    ~montgomery(){}

    void reduction_big(int *ret, int *t1_rev, int *t2_rev){
        // t1, t2 のモンゴメリリダクション(桁あふれ対策)

        int *r = new int[NUM_ARRAY_SIZE]; r[0] = 1; r[1] = 1;
        int *tmp = new int[NUM_ARRAY_SIZE];
        int zero[2]; zero[0] = 0; zero[1] = 0;
        int two[2]; two[0] = 1; two[1] = 2;
        for(i=0;i<m_nb;i++){
            mpMul(tmp, r, two);
            mpAdd(r, tmp, zero);
        }

        int *t = new int[(NUM_ARRAY_SIZE)*2];
        int *t1 = new int[NUM_ARRAY_SIZE];
        int *t2 = new int[NUM_ARRAY_SIZE];
        mpAdd(t1, t1_rev, zero);
        mpAdd(t2, t2_rev, zero);
        mpMul(t, t1, t2);
        int *c = new int[(NUM_ARRAY_SIZE)*2];
        c[0] = 0; c[1] = 0;
        //int m_n2_num[16384]; mpLong2Num(m_n2_num, m_n2);
        //int m_n_num[16384]; mpLong2Num(m_n_num, m_n);
        mpMul(c, t, m_n2);

        mpDiv(t1, t2, c, r);
        mpMul(c, m_n, t2);
        mpAdd(t2, c, t); mpAdd(c, t2, zero);
        for(i=0;i<m_nb;i++){
            mpDiv(t2, t1, c, two);
            mpAdd(c, t2, zero);
        }
        if ( mpCmp(c, m_n) >= 0 ){
            mpSub(tmp, c, m_n);
            mpAdd(c, tmp, zero);
        }
        mpAdd(ret, c, zero);

        delete[] r;
        delete[] tmp;
        delete[] t;
        delete[] t1;
        delete[] t2;
        delete[] c;
    }

    // 冪剰余 a^b mod n, バイナリ法の下位桁から計算
    void exp(int *ret, int *a, int *b){
        int zero[2] = {0, 0}; int one[2] = {1, 1}; int two[2] = {1, 2};
        int *p = new int[NUM_ARRAY_SIZE];
        reduction_big(p, a, m_r2);  // reduction(a * m_r2);
        int *x = new int[NUM_ARRAY_SIZE];
        reduction_big(x, m_r2, one);
        int *y = new int[NUM_ARRAY_SIZE];
        mpAdd(y, b, zero);
        int *tmp = new int[NUM_ARRAY_SIZE];
        int *tmp2 = new int[NUM_ARRAY_SIZE];
        int *bin = new int[NUM_ARRAY_SIZE];

        while ( mpCmp(y, zero) > 0 ){
            mpDec2Bin(&z, bin, y);
            if(bin[0] != 0){
                reduction_big(tmp, x, p); // reduction(x * p); 
                mpAdd(x, tmp, zero);
            }
            reduction_big(tmp, p, p); // reduction(p * p);
            mpAdd(p, tmp, zero);
            mpDiv(tmp, tmp2, y, two);
            mpAdd(y, tmp, zero);
        }

        reduction_big(tmp, x, one);
        mpAdd(x, tmp, zero);
        mpAdd(ret, x, zero);

        delete[] p;
        delete[] x;
        delete[] y;
        delete[] tmp;
        delete[] tmp2;
        delete[] bin;
    }
};

void sqrt_big(int *ret, int *n){
    int *tmp = new int[NUM_ARRAY_SIZE];
    int *tmp2 = new int[NUM_ARRAY_SIZE];
    int *tmp3 = new int[NUM_ARRAY_SIZE];
    int *tmp4 = new int[NUM_ARRAY_SIZE];
    int *x = new int[NUM_ARRAY_SIZE];
    int *x2 = new int[NUM_ARRAY_SIZE];
    int *num2 = new int[NUM_ARRAY_SIZE];
    int zero[2] = {1, 0}; int two[2] = {1, 2};
    int *num = new int[NUM_ARRAY_SIZE];

    mpAdd(num2, n, zero);
    mpMul(num, n, two);
    mpAdd(x, num, zero);
    do{
        mpAdd(tmp3, x, zero);
        mpMul(tmp2, x, tmp3);
        mpSub(tmp, tmp2, num2);
        mpMul(tmp3, x, two);
        mpDiv(tmp4, tmp2, tmp, tmp3);
        mpSub(tmp, x, tmp4);
        mpAdd(x2, tmp, zero);

        mpSub(tmp2, x2, x);
        if(mpCmp(tmp2, zero) < 0) break;

        mpAdd(x, x2, zero);
    }while(true);

    mpAdd(ret, x, zero);

    delete[] tmp; delete[] tmp2; delete[] tmp3; delete[] tmp4;
    delete[] x; delete[] x2;
}

bool prime_chk(int *p){
	// 整数pを3からその平方根まで試し割る
    bool sw = true;
    if(p[1] % 2 == 0){
        return false; // 偶数はカット
    }

    int *sqrtNum = new int[NUM_ARRAY_SIZE];
    sqrt_big(sqrtNum, p);

    char str_p[1024], str_sqrt[1024];
    mpNum2Str(str_p, p); mpNum2Str(str_sqrt, sqrtNum);
    cout << "元の数 = " << str_p << endl;
    cout << "平方根 = " << str_sqrt << endl;

    int *i = new int[NUM_ARRAY_SIZE];
    int three[2] = {1, 3};
    int *tmp = new int[NUM_ARRAY_SIZE];
    int *tmp2 = new int[NUM_ARRAY_SIZE];
    int zero[2] = {1, 0}; int two[2] = {1, 2};
    mpAdd(i, three, zero);
    while(mpCmp(sqrtNum, i) !=0){
        mpDiv(tmp2, tmp, p, i);
        if(mpCmp(tmp, zero) == 0){
            sw = false;
            break;
        }
        mpAdd(tmp, i, two);
        mpAdd(i, tmp, zero);
    }

    delete[] sqrtNum;
    delete[] tmp;
    delete[] tmp2;

    return sw;
}

bool mrrbin(int *n, int &k){
	// ミラーラビン法による素数判定
    int zero[2] = {1, 0}; int one[2] = {1, 1}; int two[2] = {1, 2};
    int *tmp = new int[NUM_ARRAY_SIZE]; int *tmp2 = new int[NUM_ARRAY_SIZE];
    int i;
    int *j = new int[NUM_ARRAY_SIZE]; mpAdd(j, zero, zero);

    if(mpCmp(n, two) == 0){
        return true;
    }
    mpDiv(tmp2, tmp, n, two);
    if(mpCmp(n, one)==0 || n[1] % 2 ==0){
        delete[] tmp; delete[] tmp2;
        delete[] j;

        return false;
    }

    // n-1を2のべき乗で割って2^s*dの形式に変形
    int *d = new int[NUM_ARRAY_SIZE]; mpSub(d, n, one);
    int *s = new int[NUM_ARRAY_SIZE]; mpAdd(s, zero, zero);
    while(d[1] % 2 ==0){
        mpDiv(tmp2, tmp, d, two);
        mpAdd(d, tmp2, zero);
        mpAdd(tmp2, s, one); mpAdd(s, tmp2, zero);
    }

    // 判定サイクル
    bool judge;
    int *a = new int[NUM_ARRAY_SIZE];
    int *r = new int[NUM_ARRAY_SIZE];
    int *n_min = new int[NUM_ARRAY_SIZE];
    mpSub(n_min, n, one);
    montgomery mon(n);
    for(i=0;i<k;i++){
        judge = false;

        // 1からn-1の範囲から整数aをランダムに選ぶ
        a[0] = 1; a[1] = 2;

        // a^d mod n が1もしくはn-1なら、nは素数
        mon.exp(r, a, d);
        if(mpCmp(r, one) == 0 || mpCmp(r, n_min) == 0){
            judge = true;
        }

        // r^2 mod n が-1なら、nは素数
        do{
            mon.exp(tmp, r, two);
            mpAdd(r, tmp, zero);
            if(mpCmp(r, n_min)==0){
                judge = true;
            }
            mpAdd(tmp, j, one);
            mpAdd(j, tmp, zero);
        }while(mpCmp(j, s) == 0);

        // 上記のどちらにも当てはまらない場合、nは素数でない
        if(!judge){

            delete[] tmp; delete[] tmp2;
            delete[] j;
            delete[] d; delete[] s;
            delete[] a; delete[] r; delete[] n_min;

            return false;
        }
    }

    delete[] tmp; delete[] tmp2;
    delete[] j;
    delete[] d; delete[] s;
    delete[] a; delete[] r; delete[] n_min;

    return true;
}


bool prim_root2(int *p, int *q, int *g){ 
	// p-1の素因数を2, qとし, gがpの原始元であるか調べる
	bool flag = true;
    int *tmp = new int[NUM_ARRAY_SIZE]; int *tmp2 = new int[NUM_ARRAY_SIZE];
    int *tmp3 = new int[NUM_ARRAY_SIZE];
    int zero[2] = {1, 0}; int one[2] = {1, 1}; int two[2] = {1, 2};
	montgomery mon(p);
	
	//if(mod_calc(g, ( (p-1)/q ), p) == 1){
    mpSub(tmp, p, one);
    mpDiv(tmp2, tmp3, tmp, q);
    mon.exp(tmp3, g, tmp2);
	if(mpCmp(tmp3, one) == 0){ // if(mon.exp(g, ((p-1)/q) )
		flag = false;

        delete[] tmp; delete[] tmp2; delete[] tmp3;
		return flag;
	}
	//if(mod_calc(g, ( (p-1)/2 ), p) == 1){
    mpDiv(tmp2, tmp3, tmp, two);
    mon.exp(tmp3, g, tmp2);
	if(mpCmp(tmp3, one) == 0){ // if(mon.exp(g, ( (p-1)/ 2))
		flag = false;

        delete[] tmp; delete[] tmp2; delete[] tmp3;
		return flag;
	}
	
    delete[] tmp; delete[] tmp2; delete[] tmp3;

	return flag;
}

class BIT_ARRAY{
protected:
	//データメンバ
	vector<int> seq_bit;
	vector<int>::iterator itr;

public:
	BIT_ARRAY(bool sw, int len){
		// コンストラクタ. オブジェクト生成時に呼び出し.
		if(sw==true) seq_bit.resize(len); // 動的配列長を渡された値lenに指定
	}
	~BIT_ARRAY(void){

	}

	vector<int>::iterator ret_address(void){
		return seq_bit.begin();
	}

	void add_seq(int n){
		// 渡された値nを末尾に追加
		seq_bit.push_back(n);
	}

	void ins_seq(int i, int n){
		// 渡された位置iに値nを挿入
		seq_bit[i] = n;
	}

	int get_seq_len(void){
		// 現在の配列長を返す
		return seq_bit.size();
	}

	int get_seq_bin(int i){
		// 渡された位置iに格納されている値を返す
		return seq_bit[i];
	}
};

class SEQUENCE: public BIT_ARRAY{
public:
	//コンストラクタ。オブジェクト生成時に呼び出し
	SEQUENCE(bool sw, int len): BIT_ARRAY(sw, len){

	}
	//デストラクタ。オブジェクト消滅時に呼び出し
	~SEQUENCE(void){

	}
	
	void chg_seq(bool sw){
		// swがtrueならば系列の0→-1, falseならば系列の-1→0
		for(itr = seq_bit.begin(); itr != seq_bit.end(); ++itr){
        	// *itr でイテレータの指す要素を参照
			if(sw == true && *itr == 0) *itr = -1;
			else if(sw == false && *itr == -1) *itr = 0;
		}	
	}
};

class BUFFER: public BIT_ARRAY{
private:
	int count, err_count;
	int i;
public:
	BUFFER(void): BIT_ARRAY(true, CODESIZE_BEFORE){
		count = 0;
		err_count = 0;
	}
	~BUFFER(void){

	}

	void buf_reset(void){
		count = 0;
		err_count = 0;
	}

	void add_buf(vector<int>::iterator ptr, int len){
		for(i=0;i<CODESIZE_BEFORE;i++){
			seq_bit[i] = ptr[count%len];
			count++;
		}
	}

	int get_buf_count(void){
		return count;
	}

	void error_chk(vector<int>::iterator ptr){
		// エラーカウンタ(符号化あり)
    	for(i=0;i<CODESIZE_BEFORE;i++){
			if(ptr[i] != seq_bit[i]){
	  			err_count++;
			}
    	}
	}

	int get_err_count(void){
		return err_count;
	}

};

class HAMMING74: public SEQUENCE{
private:
	vector<int> pattern;
	vector<int> bit_tmp;
	vector<int>::iterator itr;
	int i;

public:
	HAMMING74(bool sw): SEQUENCE(true, CODESIZE_AFTER){
		// true: 符号化, false: 復号化
		if(sw == true){
			pattern.resize(CODESIZE_AFTER - CODESIZE_BEFORE); // チェックパターン
			bit_tmp.resize(CODESIZE_BEFORE);
		}else{
			pattern.resize(CODESIZE_AFTER - CODESIZE_BEFORE); // シンドローム
			bit_tmp.resize(CODESIZE_AFTER);
		}
	}
	~HAMMING74(void){

	}
	
	void encode_hamming(vector<int>::iterator ptr){
		//検査ビット生成のための計算(-1,1 to 0,1)
      	for(i=0;i<CODESIZE_BEFORE;i++){
			if(ptr[i] == 1){
	 	 		bit_tmp[i] = 1;
			}else{
	  			bit_tmp[i] = 0;
			}
      	}

      	// 送信ビットをハミング(7,4)で符号化
      	// チェックパターンの生成
      	pattern[0] = bit_tmp[0] ^ bit_tmp[1] ^ bit_tmp[3];
      	pattern[1] = bit_tmp[0] ^ bit_tmp[2] ^ bit_tmp[3];
      	pattern[2] = bit_tmp[1] ^ bit_tmp[2] ^ bit_tmp[3];

      	// 符号を配列に格納 	
      	seq_bit[0] = pattern[0];
      	seq_bit[1] = pattern[1];
      	seq_bit[2] = ptr[0];
      	seq_bit[3] = pattern[2];
      	seq_bit[4] = ptr[1];
      	seq_bit[5] = ptr[2];
      	seq_bit[6] = ptr[3];

      	chg_seq(true);
	}

	void decode_hamming(vector<int>::iterator ptr){
		// シンドローム計算のための計算(-1,1 to 0,1)
      	for(i=0;i<CODESIZE_AFTER;i++){
			if(seq_bit[i] == 1){
	  			bit_tmp[i] = 1;
			}else{
	  			bit_tmp[i] = 0;
			}
      	}
      
      	// ハミング(7,4)の復号化
      	// シンドローム計算
      	pattern[0] = (bit_tmp[3] ^ bit_tmp[4] ^ bit_tmp[5] ^ bit_tmp[6]);
      	pattern[1] = (bit_tmp[1] ^ bit_tmp[2] ^ bit_tmp[5] ^ bit_tmp[6]);
      	pattern[2] = (bit_tmp[0] ^ bit_tmp[2] ^ bit_tmp[4] ^ bit_tmp[6]);
      	
      	//誤り訂正
      	if(pattern[0] == 0 && pattern[1] == 0 && pattern[2] == 1){
			seq_bit[0] *= -1;
      	}else if(pattern[0] == 0 && pattern[1] == 1 && pattern[2] == 0){
			seq_bit[1] *= -1;
      	}else if(pattern[0] == 0 && pattern[1] == 1 && pattern[2] == 1){
			seq_bit[2] *= -1;
      	}else if(pattern[0] == 1 && pattern[1] == 0 && pattern[2] == 0){
			seq_bit[3] *= -1;
      	}else if(pattern[0] == 1 && pattern[1] == 0 && pattern[2] == 1){
			seq_bit[4] *= -1;
      	}else if(pattern[0] == 1 && pattern[1] == 1 && pattern[2] == 0){
			seq_bit[5] *= -1;
      	}else if(pattern[0] == 1 && pattern[1] == 1 && pattern[2] == 1){
			seq_bit[6] *= -1;
      	}


      	ptr[0] = seq_bit[2];
      	ptr[1] = seq_bit[4];
      	ptr[2] = seq_bit[5];
     	ptr[3] = seq_bit[6];
	}
};

class REPEAT_CODE: public BIT_ARRAY{
private:
	int i, j, code_count;
public:
	REPEAT_CODE(void): BIT_ARRAY(true, CODESIZE_AFTER*REPEAT_L){

	}
	~REPEAT_CODE(void){

	}

	void encode_repeat(vector<int>::iterator ptr){
		for(i=0;i<CODESIZE_AFTER;i++){
      		for(j=(REPEAT_L * i);j<(REPEAT_L*i+REPEAT_L);j++){
      			seq_bit[j] = ptr[i];
      		}
      	}
	}

	void decode_repeat(vector<int>::iterator ptr){
		// 繰り返し符号の復号
      	for(i=0;i<CODESIZE_AFTER;i++){
      		code_count = 0;
      		for(j=REPEAT_L*i;j<REPEAT_L*i+REPEAT_L;j++){
      			if(seq_bit[j] == 1){
      				code_count++;
      			}else{
      				code_count--;
      			}    
      		}
      		if(code_count >= 0){
      			ptr[i] = 1;
      		}else{
      			ptr[i] = -1;
      		}
     	}
	}
};

class PSK{
private:
	vector<double> code_symbol_re;
	vector<double> code_symbol_im;
	int i;

	double snr, snrx;
  	double noise_re, noise_im, noise_var;
  	double ang, rad, xi, xq;
public:
	PSK(void){
		code_symbol_re.resize(CODESIZE_AFTER*REPEAT_L);
		code_symbol_im.resize(CODESIZE_AFTER*REPEAT_L);
	}
	~PSK(void){

	}

	vector<double>::iterator ret_address(void){
		return code_symbol_re.begin();
	}

	void encode_bpsk(vector<int>::iterator ptr){
		// BPSK変調
		for(i=0;i<CODESIZE_AFTER*REPEAT_L;i++){
		code_symbol_re[i] = (double)ptr[i];
		code_symbol_im[i] = 0.0; //BPSKなので
		// usleep(2000);
      }
	}

	void noise_add(int snr){
		for(i=0;i<CODESIZE_AFTER*REPEAT_L;i++){
			snrx = pow(10.0, snr/10.0); // dBから真値へ変換
			noise_var = 1.0 / snrx / 2.0;
	
			xi = 1.0 - rand()/(RAND_MAX+1.0);
			xq = 1.0 - rand()/(RAND_MAX+1.0);
	
			ang = 2.0 * PI * xq;
			rad = sqrt(-2.0*log(xi));
	
			noise_re = sqrt(noise_var)*(rad*cos(ang));
			noise_im = sqrt(noise_var)*(rad*sin(ang));
	
			// 受信信号へノイズの加算(符号化あり)
			code_symbol_re[i] += (double)noise_re;
			code_symbol_im[i] += (double)noise_im;
      	}
 	}

	void decode_bpsk(vector<double>::iterator input_ptr, vector<int>::iterator output_ptr){
      	//復調(符号化あり)
    	for(i=0;i<CODESIZE_AFTER*REPEAT_L;i++){
			code_symbol_re[i] = input_ptr[i];
			code_symbol_im[i] = 0;
			if(code_symbol_re[i] < 0.0){
	  			output_ptr[i] = -1;
			}else{
	 		 	output_ptr[i] = 1;
			}
    	}
	}

};

int main(){
    // cout << "unsigned long long max:" << ULLONG_MAX << endl;

  	int i,j,k = 0, itr, len, seq(0), seq2;
  	int *q = new int[NUM_ARRAY_SIZE];
    int *p = new int[NUM_ARRAY_SIZE];
    int *g = new int[NUM_ARRAY_SIZE];
    int *tmp = new int[NUM_ARRAY_SIZE];
    int *tmp2 = new int[NUM_ARRAY_SIZE];
    int *tmp3 = new int[NUM_ARRAY_SIZE];
    int *tmp4 = new int[NUM_ARRAY_SIZE];
    int zero[2] = {1, 0}; int one[2] = {1, 1}; int two[2] = {1, 2};
  	int n;

    char str_tmp[1024];


  	BUFFER tx_buf_bit1, tx_buf_bit2;
  	BUFFER rx_buf_bit1, rx_buf_bit2;

  	HAMMING74 tx_hamming_bit1(true), tx_hamming_bit2(true);
  	HAMMING74 rx_hamming_bit1(false), rx_hamming_bit2(false);

  	REPEAT_CODE tx_repeat_bit1, tx_repeat_bit2;
  	REPEAT_CODE rx_repeat_bit1, rx_repeat_bit2;

  	PSK tx_code, rx_code;
  
  	double snr;
  
  	int MAX_ITARATION;
  
  	char str[16384]; //平文
	cout << "平文を入力してください: ";
	// scanf("%s", str);
	cin.getline(str, sizeof(str)); // 空白を含んで受け取る
	len = strlen(str); //文字列長
	int m[len]; //平文数値
	bool rev[len]; // 平文数値が負であった時の反転;
	for(i=0;i<len;i++){
		rev[i] = false;
	}
	bool judge; // for文中の分岐判定用
	SEQUENCE tx_seq(false, (len+1)*BINARY_BIT);
  	SEQUENCE rx_seq(true, (len+1)*BINARY_BIT);
	
	cout << "平文 = " << str << endl;
	cout << "平文数値m = ";
	for(i=0;i<len;i++){
		m[i] = str[i];
		if(m[i]>0){
			cout << m[i] << " ";
		}else{
			m[i] *= (-1);
			rev[i] = true;
			cout << (-1) * m[i] << " ";
		}
	}
	cout << endl;
	
	// ランダムな2つの素数(p,q)の選択
    bool breaker = false;
    int k_mrr = 1; //ミラーラビンのチェック回数
    int l = 6; // 多売調整数配列数
    q[0] = l;
    p[0] = l;
	do{
        do{
            // cout << "現在のスレッド = " << omp_get_thread_num() + 1 << " ";
            for(i=1;i<l;i++){
                q[i] = random_num( (N_MAX/10)+1, (N_MAX-1) );
            }
            q[i] = random_num( (N_MAX/10000000+1)/2, (N_MAX/1000000)/2 );
            if(mrrbin(q, k_mrr)==true) breaker = true;
        }while(breaker!=true);
	
		//素数を生成する
        breaker = false;
		mpMul(tmp, q, two);
        mpAdd(p, tmp, one); // p = ( 2 * q ) + 1;
		mpNum2Str(str_tmp, p); cout << "p の候補: " << str_tmp << endl;
        if(mrrbin(p, k_mrr)==true) breaker = true;
	}while(breaker!=true);
	
	// p, qより原始元gをランダムに生成
    g[0] = l;

	do{
		for(i=1;i<l;i++){
        	g[i] = random_num( (N_MAX/10)+1, (N_MAX-1) );
        }
        g[i] = random_num( (N_MAX/100000000+1)/2, (N_MAX/10000000)/3 );
		// cout << "g の候補: " << g << endl;
	}while(prim_root2(p, q, g)!=true);
	
    mpNum2Str(str_tmp, p); cout << "大きな素数p = " << str_tmp << endl;
	mpNum2Str(str_tmp, q); cout << "対応する素数q = " << str_tmp << endl;
	mpNum2Str(str_tmp, g); cout << "原始元の1つg = " << str_tmp << endl;

	montgomery mon(p);

    //受信者による公開鍵の生成
	int *a = new int[NUM_ARRAY_SIZE];
    int *b = new int[NUM_ARRAY_SIZE];
	
    a[0] = 1; a[1] = 1; // a = 1;


	// b = random_num(1, p-2);
    b[0] = l;
    for(i=1;i<l;i++){
        b[i] = random_num( (N_MAX/10)+1, (N_MAX-1) );
    }
    b[i] = random_num( (N_MAX/100000000+1)/2, (N_MAX/10000000)/3 );
    mpNum2Str(str_tmp, b); cout << "秘密鍵b = " << str_tmp << endl;

	mon.exp(a, g, b);
	mpNum2Str(str_tmp, a); cout << "公開鍵a = " << str_tmp << endl;

	//暗号化・復号化
	//送信者による暗号文の生成
	
	int *c1 = new int[NUM_ARRAY_SIZE]; c1[0] = 1; c1[1]= 1; // c1 = 1;
    int **c2 = new int*[len];
	int *r = new int[NUM_ARRAY_SIZE];
	// // #pragma omp parallel for
	for(j=0;j<len;j++){ // c2配列の初期化
        c2[j] = new int[NUM_ARRAY_SIZE];
		mpAdd(&c2[j][0], zero, zero);
	}

    // 任意の乱数rの生成
    r[0] = l;
    for(i=1;i<l;i++){
        r[i] = random_num( (N_MAX/10)+1 , (N_MAX-1) );
    }
    r[i] = random_num( (N_MAX/100000000+1)/2, (N_MAX/10000000)/3 );
	mpNum2Str(str_tmp, r); cout << "任意の乱数r = " << str_tmp << endl;

	mon.exp(c1, g, r);
	mpNum2Str(str_tmp, c1); cout << "暗号文c1 = " << str_tmp << endl;

	mon.exp(tmp, a, r);

	//暗号文c2の生成
	cout << "暗号文c2 = ";
	// // #pragma omp parallel for
	for(i=0;i<len;i++){
        mpLong2Num(tmp2, m[i]);
        mpDiv(tmp3, tmp4, tmp2, p);
        mpMul(&c2[i][0], tmp, tmp4); // c2[i] = bigtmp * (m[i] % p);	
	}
	
	for(i=0;i<len;i++){
        mpNum2Str(str_tmp, &c2[i][0]); cout << str_tmp << " ";
	}
	cout << endl;
	
	//暗号文c1, c2の2進化
	int c1_bin[BINARY_BIT], c2_bin[len][BINARY_BIT];
    int count;
	mpDec2Bin(&count, c1_bin, c1); //c1の2進化
 	cout << "暗号文c1[binary] = ";
 	for(i=BINARY_BIT-1;i>=0;i--){
 	    tx_seq.add_seq(c1_bin[i]);
 		cout << tx_seq.get_seq_bin( tx_seq.get_seq_len() -1 );
 	}
 	cout << endl;
 
 	cout << "暗号文c2[binary] = ";
 	for(i=0;i<len;i++){
 		mpDec2Bin(&count, &c2_bin[i][0], &c2[i][0]);
 		if(rev[i] == true)
 			tx_seq.add_seq(1); // 平文数値が負の時, 先頭ビットを1にする
 		else 
 			tx_seq.add_seq(0); // 上でない時, 先頭ビットは0
 		cout << tx_seq.get_seq_bin( tx_seq.get_seq_len() - 1);
        int tmp_cnt = count-1;
 		for(j=BINARY_BIT-2;j>=0;j--){
            if(j>=count){
                tx_seq.add_seq(0);
                cout << tx_seq.get_seq_bin( tx_seq.get_seq_len() - 1);
            }else{
 		   	     tx_seq.add_seq(c2_bin[i][tmp_cnt--]);
 			    cout << tx_seq.get_seq_bin( tx_seq.get_seq_len() - 1);
            }
 		}
 		cout << " ";
 	}
 	cout << endl;
    
  	// 送信信号用に、送信ビット系列の0 → -1 へ変換
  	tx_seq.chg_seq(true);
  
	MAX_ITARATION = (len+1)*BINARY_BIT / 4;
	cout << "itr" << (len+1)*BINARY_BIT << endl;
	cout << "seq" << tx_seq.get_seq_len() << endl;
	cout << "MAX" << MAX_ITARATION << endl;
	for(snr=0;snr<16;snr++){
    	tx_buf_bit1.buf_reset(); rx_buf_bit1.buf_reset();
    	setbuf(stdout,NULL);
    	printf("0%%       50%%       100%%\n");
    	printf("+---------+---------+\n");
    	seq2 = 0;
    	for(itr=0;itr<MAX_ITARATION;itr++){

    	// バッファにbit系列から追加
    	tx_buf_bit1.add_buf( tx_seq.ret_address(), tx_seq.get_seq_len() );
      
     	// ハミング符号化
    	tx_hamming_bit1.encode_hamming( tx_buf_bit1.ret_address() );

    	// 繰り返し符号化
    	tx_repeat_bit1.encode_repeat( tx_hamming_bit1.ret_address() );
      
    	// BPSK変調
    	tx_code.encode_bpsk( tx_repeat_bit1.ret_address() );

    	// 送信信号にノイズを追加
      	tx_code.noise_add(snr);

    	// BPSK復調
    	rx_code.decode_bpsk( tx_code.ret_address(), rx_repeat_bit1.ret_address() );

    	// 繰り返し符号の復号化
 		rx_repeat_bit1.decode_repeat( rx_hamming_bit1.ret_address() );

    	// ハミング符号の復号化
    	rx_hamming_bit1.decode_hamming( rx_buf_bit1.ret_address() );
      
      	// バッファから受信ビット系列に追加
    	rx_seq.ins_seq( seq2%(tx_seq.get_seq_len()), (rx_buf_bit1.get_seq_bin(0)) ); seq2++;
      	rx_seq.ins_seq( seq2%(tx_seq.get_seq_len()), (rx_buf_bit1.get_seq_bin(1)) ); seq2++;
      	rx_seq.ins_seq( seq2%(tx_seq.get_seq_len()), (rx_buf_bit1.get_seq_bin(2)) ); seq2++;
      	rx_seq.ins_seq( seq2%(tx_seq.get_seq_len()), (rx_buf_bit1.get_seq_bin(3)) ); seq2++;
      
		rx_buf_bit1.error_chk( tx_buf_bit1.ret_address() );

    	if(itr%(MAX_ITARATION/20)==MAX_ITARATION/20-1){
			for(j=0;j<(itr+1)/(MAX_ITARATION/20)+1;j++)
	  			printf("#");
	  			printf("\r");
      	}
      	if(itr%(MAX_ITARATION/100)==MAX_ITARATION/100-1){
			printf("\n");
			printf("%3.1d %%\n",itr/(MAX_ITARATION/100)+1);
			printf("\x1b[2A");
	
      		// printf("送信信号: %d, 受信信号: %d", tx_bit1, rx_bit1);
      	}
    }
    printf("\x1b[2B");
    printf("シミュレーション終了\n");
    printf("SNR = %f[dB]", snr);
    printf("符号化あり: ");
    printf("送信ビット数 = %d \t 誤りビット数 = %d BER %1.9f\n", tx_buf_bit1.get_buf_count(), rx_buf_bit1.get_err_count(), (double)rx_buf_bit1.get_err_count()/tx_buf_bit1.get_buf_count());
    
    //受信者による復号化
    // 受信したバイナリを10進化
    bool rev_flag[len];
    int *c1_rx = new int[NUM_ARRAY_SIZE]; c1_rx[0] = 1; c1_rx[1] = 1; // c1_rx = 1;
    int **c2_rx = new int*[len];
    for(i=0;i<len;i++){
        c2_rx[i] = new int[NUM_ARRAY_SIZE];
        mpAdd(&c2_rx[i][0], zero, zero);
    }
    int m_dec[len];
    int *c1_dec = new int[NUM_ARRAY_SIZE];
    
    
    //強制的に送信系列を受信系列に代入(デバッグ用)
    /*
    for(i=0;i<seq;i++){
    	// pn_rx_seq[i] = pn_seq[i];
    	pn_rx_seq[i] = tx_seq.get_seq_bin( tx_seq.get_seq_len() - 1);
    }
    */
    
    mpAdd(tmp, zero, zero);
    for(i=0;i<BINARY_BIT;i++){
     if(rx_seq.get_seq_bin(i) == 1){
			//tmp += rx_seq.get_seq_bin(i) * pow(2, (BINARY_BIT-1)-i);
            
            mpAdd(tmp2, one, zero);
            for(j=0;j<(BINARY_BIT-1)-i;j++){
                mpMul(tmp3, tmp2, two);
                mpAdd(tmp2, tmp3, zero);
            }
            mpMul(tmp3, tmp2, one);
            mpAdd(tmp2, tmp3, tmp);
            mpAdd(tmp, tmp2, zero);
		}else{
			continue;
		}
	}
    mpAdd(c1_rx, tmp, zero); // c1_rx = tmp_bin;
    // mpNum2Str(str_tmp, c1_rx); cout << "c1_rx = " << str_tmp << endl;

    int z;
	for(i=0;i<len;i++){
	    mpAdd(tmp, zero, zero);
	    for(j=BINARY_BIT*(i+1) ; j<BINARY_BIT*(i+1)+BINARY_BIT ; j++){
	    	if(j%BINARY_BIT==0){
	    		if(rx_seq.get_seq_bin(j)==1) rev_flag[i] = true;
	    		else rev_flag[i] = false;
	    	}else if(rx_seq.get_seq_bin(j) == 1){
			    // tmp_bin += 1 * pow(2, (BINARY_BIT-1)-(j%BINARY_BIT));
                mpAdd(tmp2, one, zero);
                for(z=0;z<(BINARY_BIT-1)-(j%BINARY_BIT);z++){
                    mpMul(tmp3, tmp2, two);
                    mpAdd(tmp2, tmp3, zero);
                }
                mpMul(tmp3, tmp2, one);
                mpAdd(tmp2, tmp3, tmp);
                mpAdd(tmp, tmp2, zero);
                //mpNum2Str(str_tmp, tmp); cout << "tmp = " << str_tmp << endl;
		    }else{
			    continue;
		    }
		}
	    // c2_rx[i] = tmp_bin;
        mpAdd(&c2_rx[i][0], tmp, zero);
        // mpNum2Str(str_tmp, &c2_rx[i][0]); cout << "c2_rx = " << str_tmp << endl;
	}
	// c1_dec = mod_calc(c1_rx, b, p);
	mon.exp(c1_dec, c1_rx, b);
    // mpNum2Str(str_tmp, c1_dec); cout << "c1_dec = " << str_tmp << endl;
	
	for(i=0;i<len;i++){
        mpDiv(tmp2, tmp, &c2_rx[i][0], c1_dec);
        // mpNum2Str(str_tmp, tmp2); cout << "c2 / c1 = " << str_tmp << endl;
        // bigtmp = mon.exp(c2_rx[i], c1_dec);
		mpNum2Int(&m_dec[i], tmp2); // m_dec[i] = tmp;

		if(rev_flag[i]==true)
			m_dec[i] *= -1;
	}
	
	cout << "復号化数値: ";
	for(i=0;i<len;i++){
		cout << m_dec[i] << " ";
	}
	cout << endl;
	
	cout << "平文数値m = ";
	for(i=0;i<len;i++){
		m[i] = str[i];
		if(m[i]>0){
			cout << m[i] << " ";
		}else{
			m[i] *= (-1);
			rev[i] = true;
			cout << (-1) * m[i] << " ";
		}
	}
	cout << endl;
	
	cout << "復号化文: ";
	for(j=0;j<len;j++){
		putchar(m_dec[j]);
	}
	cout << endl;

	judge=0;
	for(j=0;j<len;j++){
		if( m_dec[j] == m[j] || m_dec[j] == (-1)*m[j] )
			continue;
		else
			judge = 1;
			break;
	}
	
	if(judge==0)
		cout << "復号成功" << endl;
	else
		cout << "復号失敗" << endl;
    
    if(((double)rx_buf_bit1.get_err_count()/tx_buf_bit1.get_buf_count())==0)
      break;
  	}

  	return 0;
}
