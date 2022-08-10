// #include "DFFT.h"

/**
 * @brief  两个复数的蝶形计算, 逆变换
 * @note   A   A+BW   dist1
 * @note   B   A-BW   dist2
 * @param  *c1: 第一个值
 * @param  *c2: 第二个值
 * @param  P: 增量为K时的旋转指数P
 * @param  N: 旋转总份数
 * @retval None
 */
void IDFFT_butterfly_computation(st_complex *c1, st_complex *c2, int P, int N);

/**
 * @brief  离散快速傅里叶逆变换
 *
 * @note   N-总数据个数, M-个数对应的2次幂, L-计算级数, N=2^M, L=1,2,3,...,M, 例N=16,M=4,L=1,2,3,4
 * @note   核心处理准则:
 * @note   1.每个蝶形运算中的两个数据间隔B=2^(L-1)
 * @note   2.有2^(L-1)个旋转因子
 * @note   3.旋转因子W增量K=2^(M-L)
 * @note   4.同一个W的间隔为2^L=2xB
 * @note   5.蝶形运算的种类数目=间隔B=2^(L-1)
 * @note   6.同种蝶形运算次数=K=2^(M-L)
 * @note   实际使用需要考虑采样定理, 会出现一对镜像峰值
 *
 * N=2^M, 即1/N=(1/2)^M, 蝶形共有M级把(1/2)^M分到每级中
 * x(n)=(1/N)*∑(k=0->N-1)X(k)W-N^-nk=((1/2)^M)*∑X(k)(W-N^nk)*  n=1,2...N-1
 * 和原有DFFT差不多, 相差在旋转因子从W-N^nk改为W-N^-nk即取共轭
 * 蝶形计算中的Tr Ti的加减相反, 计算结果全部*1/2
 *
 * @param  *array:
 * @param  array_size:
 * @retval None
 */
void IDFFT(st_complex *array, uint32_t array_size);
