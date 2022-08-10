#include <stdio.h>
#include <string.h>
#include <math.h>

#define DBG_LOG 0
#define PI (3.14159265359f)

typedef unsigned int uint32_t;
typedef struct
{
    float R; //实部
    float I; //虚部
    float M; //幅值, 频率谱
    float S; //功率谱
    float P; //相位
} st_complex;

/**
 * @brief  打印数值对应的二进制bit
 * @note   每隔8bit增加空格
 * @param  num:
 * @retval None
 */
void _print_bit(const uint32_t num);

/**
 * @brief  打印float型数组
 * @note   保留5位小数
 * @param  *array:
 * @param  array_size:
 * @retval None
 */
void _print_float_array(const float *array, const uint32_t array_size);

/**
 * @brief  打印复数型
 * @note   各参数保留5位小数
 * @param  *cpx:
 * @retval None
 */
void _print_complex(const st_complex *cpx);

/**
 * @brief  打印复数型数组
 * @note   各参数保留5位小数
 * @param  *array:
 * @param  array_size:
 * @retval None
 */
void _print_complex_array(const st_complex *array, const uint32_t array_size);

/**
 * @brief  判断并获取输入值是2的N次方, 2-0010 4-0100 8-1000 1024-100 0000 0000
 * @note   原理首先是偶数, 并且一定只有一个bit存在1, 且一定在最高位
 * @param  num:
 * @retval -1: 非法, >=0: 对应2的N次方
 */
int get_pow_of_two(const uint32_t num);

/**
 * @brief  从右往左按位翻转
 * @note
 * @param  input:
 * @param  bit_size: 数组内数据的最大bit长度
 * @retval
 */
uint32_t reverse_bit_0(const uint32_t input, const uint32_t bit_size);

/**
 * @brief  从右往左直接翻转到指定位置
 * @note
 * @param  input:
 * @param  bit_size: 数组内数据的最大bit长度
 * @retval
 */
uint32_t reverse_bit_1(const uint32_t input, const uint32_t bit_size);

/**
 * @brief  跳过高位0, 尽快结束(高位0越多, 性能比上两个越好)
 * @note
 * @param  input:
 * @param  bit_size: 数组内数据的最大bit长度
 * @retval
 */
uint32_t reverse_bit_2(const uint32_t input, const uint32_t bit_size);

/**
 * @brief  分组治理, 先16bit一起, 再8->4->2->1, 时间复杂度最低但是不灵活
 * @note   只处理32bit型
 * @param  input:
 * @retval
 */
uint32_t reverse_bit_3(const uint32_t input);

/**
 * @brief  根据偶奇抽取排序数组
 * @note   0,1,2,3,4,5,6,7-->0,2,4,6|1,3,5,7-->0,4|2,6|1,5|3,7
 * @param  *array: 输入数组
 * @param  array_size: 数组大小, 必须保证是2的次方
 * @retval None
 */
void DFFT_sort_array(st_complex *array, uint32_t array_size);

/**
 * @brief  弧度转角度
 * @note
 * @param  rad:
 * @retval
 */
float rad_to_degree(float rad);

/**
 * @brief  角度转弧度
 * @note
 * @param  degree:
 * @retval
 */
float degree_to_rad(float degree);

/**
 * @brief  复数加/减/乘/除
 * @note
 * @param  *c1:
 * @param  *c2:
 * @param  calc_type: 0-加, 1-减, 2-乘, 3-除
 * @retval None
 */
void complext_calculate(st_complex *dist, st_complex *c1, st_complex *c2, char calc_type);

/**
 * @brief  复数获取功率谱
 * @note
 * @param  *complex:
 * @retval None
 */
void complex_get_power_spectral(st_complex *complex);

/**
 * @brief  复数获取幅值
 * @note
 * @param  *complex:
 * @retval None
 */
void complex_get_magnitude(st_complex *complex);

/**
 * @brief  复数获取相位
 * @note
 * @param  *complex:
 * @retval None
 */
void complex_get_phase(st_complex *complex);

/**
 * @brief  两个复数的蝶形计算
 * @note   A   A+BW   dist1
 * @note   B   A-BW   dist2
 * @param  *c1: 第一个值
 * @param  *c2: 第二个值
 * @param  P: 增量为K时的旋转指数P
 * @param  N: 旋转总份数
 * @retval None
 */
void DFFT_butterfly_computation(st_complex *c1, st_complex *c2, int P, int N);

/**
 * @brief  离散快速傅里叶变换
 * @note   N-总数据个数, M-个数对应的2次幂, L-计算级数, N=2^M, L=1,2,3,...,M, 例N=16,M=4,L=1,2,3,4
 * @note   核心处理准则:
 * @note   1.每个蝶形运算中的两个数据间隔B=2^(L-1)
 * @note   2.有2^(L-1)个旋转因子
 * @note   3.旋转因子W增量K=2^(M-L)
 * @note   4.同一个W的间隔为2^L=2xB
 * @note   5.蝶形运算的种类数目=间隔B=2^(L-1)
 * @note   6.同种蝶形运算次数=K=2^(M-L)
 * @note   实际使用需要考虑采样定理, 会出现一对镜像峰值
 * @param  *array:
 * @param  array_size:
 * @retval None
 */
void DFFT(st_complex *array, uint32_t array_size);
