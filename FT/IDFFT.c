#include "DFFT.c" //TODO
#include "IDFFT.h"

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
void IDFFT_butterfly_computation(st_complex *c1, st_complex *c2, int P, int N)
{
    float W_COS = cos(2.0f * PI * P / N); //旋转因子中的cos分量
    float W_SIN = sin(2.0f * PI * P / N); //旋转因子中的sin分量
    float X1_R = c1->R;                   //蝶形第1计算值的实部
    float X1_I = c1->I;                   //蝶形第1计算值的虚部, 离散时为0
    float X2_R = c2->R;                   //蝶形第2计算值的实部
    float X2_I = c2->I;                   //蝶形第2计算值的虚部, 离散时为0

    //和DFFT相比, 旋转因子的加减相反, 即旋转因子被取共轭处理
    float T_R = X2_R * W_COS - X2_I * W_SIN; // X2和旋转因子计算后的实部
    float T_I = X2_I * W_COS + X2_R * W_SIN; // X2和旋转因子计算后的虚部

    c1->R = (1.0f / 2.0f) * (X1_R + T_R); //蝶形上+下
    c1->I = (1.0f / 2.0f) * (X1_I + T_I);

    c2->R = (1.0f / 2.0f) * (X1_R - T_R); //蝶形上-下
    c2->I = (1.0f / 2.0f) * (X1_I - T_I);

    // TODO 可以继续把复数的幅值和相角一起算了, 增加了计算量, 如果用不到可以不处理
    complex_get_magnitude(c1);
    complex_get_magnitude(c2);

    complex_get_phase(c1);
    complex_get_phase(c2);
}

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
void IDFFT(st_complex *array, uint32_t array_size)
{
    int L, M, N = array_size;
    int W_C, B, C_C, K, P;
    int i, j, idx, idx2;

    /***** 1.取得size的2次方并保证有效 *****/
    M = get_pow_of_two(array_size);
    if (M == -1)
    {
        printf("invalid size:%d\n", array_size);
        return;
    }

    /***** 2.翻转数组索引二进制位并排序适合蝶形计算顺序 *****/
    DFFT_sort_array(array, array_size);

    /***** 3.构建蝶形计算框架, L级数=1,2,3...M *****/
    for (L = 1; L <= M; L++)
    {

        /***** 3.1.计算每一级L *****/
        W_C = B = 1 << (L - 1); // W个数=计算数据间隔=2^L-1
        C_C = K = 1 << (M - L); //计算次数=W增量=2^M-L

        for (i = 0; i < W_C; i++)
        {

            /***** 3.2.计算同种蝶形 *****/
            P = i * K; //对应增量为K时的旋转指数P

            for (j = 0; j < C_C; j++)
            {

                idx = i + 2 * j * B; //确定蝶形计算的两组下标
                idx2 = idx + B;

                /***** 3.3.蝶形计算并卷积 *****/
                IDFFT_butterfly_computation(&array[idx], &array[idx2], P, N);
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    // TODO 先伪造DFFT变换后的值再测试IDDF逆变换
    int pow = 3;                          // 2的次幂, 3->8, 10->1024, 兼顾采样准确性和性能推荐1024或2048
    int array_size = 1 << pow;            //保证大小是2的次幂
    int sin_A = 0xFF;                     //正弦波幅值
    st_complex adc_val_array[array_size]; //离散采样值对应的复数域只有纯实部

    for (int i = 0; i < array_size; i++)
    {
        //只叠加一个固定频率正弦波
        float PI_ratio = (float)i / (float)array_size * 2.0f;
        float val = sin_A * sin(PI_ratio * PI);
        st_complex cpx = {val};
        adc_val_array[i] = cpx;
        printf("%dsin(%.3fπ)=%.5f\n", sin_A, PI_ratio, val);

        //叠加两个不同频率的正弦波
        // float PI_ratio1 = (float)i * 0.04f * 2.0f;
        // float PI_ratio2 = (float)i * 0.4f * 2.0f;
        // float val = sin_A * (sin(PI_ratio1 * PI) + sin(PI_ratio2 * PI));
        // printf("%dsin(%.3fπ)+%dsin(%.3fπ)=%.3f\n", sin_A, PI_ratio1, sin_A, PI_ratio2, val);
        // st_complex cpx = {val};
        // adc_val_array[i] = cpx;
    }

    DFFT(adc_val_array, array_size);
    printf("----------------DFFT Result---------------\n");
    _print_complex_array(adc_val_array, array_size);

    IDFFT(adc_val_array, array_size);
    printf("----------------IDFFT Result---------------\n");
    _print_complex_array(adc_val_array, array_size);
}
