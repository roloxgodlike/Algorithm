#include <stdio.h>
#include <math.h>
#include <string.h>
#include <windows.h>

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
void _print_bit(const uint32_t num)
{
    int size = sizeof(num) * 8 - 1;
    for (int i = size; i >= 0; i--)
    {
        if ((i != size) && (i + 1) % 8 == 0)
            printf(" ");
        uint32_t tmp = num;
        printf("%d", (tmp >> i) & 1);
    }
    printf("\n");
    printf("%u\n", num);
}

/**
 * @brief  打印float型数组
 * @note   保留5位小数
 * @param  *array:
 * @param  array_size:
 * @retval None
 */
void _print_float_array(const float *array, const uint32_t array_size)
{
    for (int i = 0; i < array_size; i++)
        printf("%.5f ", array[i]);
    printf("\n");
}

/**
 * @brief  打印复数型
 * @note   各参数保留5位小数
 * @param  *cpx:
 * @retval None
 */
void _print_complex(const st_complex *cpx)
{
    printf("R:%.3f,I:%.3f,M:%.3f,S:%.3f,P:%.3frad\n",
           cpx->R, cpx->I, cpx->M, cpx->S, cpx->P);
}

/**
 * @brief  打印复数型数组
 * @note   各参数保留5位小数
 * @param  *array:
 * @param  array_size:
 * @retval None
 */
void _print_complex_array(const st_complex *array, const uint32_t array_size)
{
    for (int i = 0; i < array_size; i++)
        _print_complex(&array[i]);
}

/**
 * @brief  判断并获取输入值是2的N次方, 2-0010 4-0100 8-1000 1024-100 0000 0000
 * @note   原理首先是偶数, 并且一定只有一个bit存在1, 且一定在最高位
 * @param  num:
 * @retval -1: 非法, >=0: 对应2的N次方
 */
int get_pow_of_two(const uint32_t num)
{
    if (num & 1) //排除奇数
        return -1;

    uint32_t _tmp = num;
    int _cnt_bit1 = 0, _pow = -1;
    while (_tmp)
    {
        if (_tmp & 1)
            _cnt_bit1++;
        if (_cnt_bit1 > 1) //排除存在多个bit1
            return -1;
        _tmp >>= 1;
        _pow++; //累积平方数
    }
    return _pow;
}

/**
 * @brief  从右往左按位翻转
 * @note
 * @param  input:
 * @param  bit_size: 数组内数据的最大bit长度
 * @retval
 */
uint32_t reverse_bit_0(const uint32_t input, const uint32_t bit_size)
{
#if DBG_LOG
    _print_bit(input);
#endif
    // uint32_t bit_size = sizeof(input) * 8; //按入参定长
    uint32_t result = 0, _input = input;
    for (int i = 0; i < bit_size; i++)
    {
        int last_bit = _input & 1;
        result <<= 1; //先左移, 保留高位让出新的低位
        result |= last_bit;
        _input >>= 1;
    }
#if DBG_LOG
    _print_bit(result);
#endif
    return result;
}

/**
 * @brief  从右往左直接翻转到指定位置
 * @note
 * @param  input:
 * @param  bit_size: 数组内数据的最大bit长度
 * @retval
 */
uint32_t reverse_bit_1(const uint32_t input, const uint32_t bit_size)
{
#if DBG_LOG
    _print_bit(input);
#endif
    // uint32_t bit_size = sizeof(input) * 8; //按入参定长
    uint32_t result = 0, _input = input;
    for (int i = 0; i < bit_size; i++)
    {
        int last_bit = _input & 1;
        uint32_t tmp = last_bit << (bit_size - i - 1); //最后一位直接移到翻转位置
        result |= tmp;
        _input >>= 1;
    }
#if DBG_LOG
    _print_bit(result);
#endif
    return result;
}

/**
 * @brief  跳过高位0, 尽快结束(高位0越多, 性能比上两个越好)
 * @note
 * @param  input:
 * @param  bit_size: 数组内数据的最大bit长度
 * @retval
 */
uint32_t reverse_bit_2(const uint32_t input, const uint32_t bit_size)
{
    // uint32_t bit_size = sizeof(input) * 8; //按入参定长
    int pos = bit_size - 1;
    uint32_t result = 0, _input = input;
    int cnt = 0;
#if DBG_LOG
    _print_bit(input);
    printf("pos:%d, mov:%d\n", pos, cnt);
#endif
    while (_input)
    {
        int last_bit = _input & 1;
        result |= (last_bit << pos);
        _input >>= 1;
        pos--;
        cnt++;
    }
#if DBG_LOG
    printf("pos:%d, mov:%d\n", pos, cnt);
    _print_bit(result);
#endif
    return result;
}

/**
 * @brief  分组治理, 先16bit一起, 再8->4->2->1, 时间复杂度最低但是不灵活
 * @note   只处理32bit型
 * @param  input:
 * @retval
 */
uint32_t reverse_bit_3(const uint32_t input)
{
    uint32_t result = input;
    result = ((result & 0x0000FFFF) << 16) | ((result & 0xFFFF0000) >> 16);
#if DBG_LOG
    printf("16bit:");
    _print_bit(result);
#endif
    result = ((result & 0x00FF00FF) << 8) | ((result & 0xFF00FF00) >> 8);
#if DBG_LOG
    printf("08bit:");
    _print_bit(result);
#endif
    result = ((result & 0x0F0F0F0F) << 4) | ((result & 0xF0F0F0F0) >> 4);
#if DBG_LOG
    printf("04bit:");
    _print_bit(result);
#endif
    result = ((result & 0x33333333) << 2) | ((result & 0xCCCCCCCC) >> 2);
#if DBG_LOG
    printf("02bit:");
    _print_bit(result);
#endif
    result = ((result & 0x55555555) << 1) | ((result & 0xAAAAAAAA) >> 1);
#if DBG_LOG
    printf("01bit:");
    _print_bit(result);
#endif
    return result;
}

/**
 * @brief  根据偶奇抽取排序数组
 * @note   0,1,2,3,4,5,6,7-->0,2,4,6|1,3,5,7-->0,4|2,6|1,5|3,7
 * @param  *array: 输入数组
 * @param  array_size: 数组大小, 必须保证是2的次方
 * @retval None
 */
void DFFT_sort_array(st_complex *array, uint32_t array_size)
{
    st_complex val_tmp;
    uint32_t size_pow, array_idx, reverse_idx;
    // uint32_t bit_offset, lo_mask, hi_mask, lo_bit, hi_bit;

    //取得size的2次方并保证有效
    size_pow = get_pow_of_two(array_size);
    if (size_pow == -1)
    {
        printf("invalid size:%d\n", array_size);
        return;
    }
#if DBG_LOG
    printf("valid size:%d\n", array_size);
#endif

    //定位中间索引, 保证奇数时不丢失需向上取整
    //     uint32_t mid_idx = ceil((float)array_size / 2.0f);
    // #if DBG_LOG
    //     printf("mid_idx:%d\n", mid_idx);
    // #endif

    for (array_idx = 0; array_idx < array_size; array_idx++) //尝试翻转所有元素, 以防后半部分有丢失
    {
        //方法1:
        reverse_idx = reverse_bit_2(array_idx, size_pow); //翻转的目标索引

        //方法2:
        // reverse_idx = 0; //翻转的目标索引
        // for (bit_offset = 0; bit_offset < mid_idx; bit_offset++) //只需处理一半的索引
        // {
        //     lo_mask = 1; //低位掩码
        //     hi_mask = 1 << (size_pow - 1); //高位掩码, 头尾两个无需翻转所以左移-1
        //     lo_mask <<= bit_offset; //低位掩码根据offset左移
        //     hi_mask >>= bit_offset; //高位掩码根据offset右移
        //     lo_bit = array_idx & lo_mask; //用低位掩码依次判断低位bit是否存在
        //     hi_bit = array_idx & hi_mask; //用高位掩码依次判断高位bit是否存在
        //     if (lo_bit)
        //         reverse_idx |= hi_mask; //原始低位翻转到对称的高位
        //     if (hi_bit)
        //         reverse_idx |= lo_mask; //原始高位翻转到对称的低位
        // }

        //如果需要翻转则原始索引一定小于目标索引, 翻转对应位置的值
        if (array_idx < reverse_idx)
        {
            val_tmp = array[array_idx];
            array[array_idx] = array[reverse_idx];
            array[reverse_idx] = val_tmp;
        }
    }
}

/**
 * @brief  弧度转角度
 * @note
 * @param  rad:
 * @retval
 */
float rad_to_degree(float rad)
{
    return rad * 180.0f / PI;
}

/**
 * @brief  角度转弧度
 * @note
 * @param  degree:
 * @retval
 */
float degree_to_rad(float degree)
{
    return degree * PI / 180.0f;
}

/**
 * @brief  复数加/减/乘/除
 * @note
 * @param  *c1:
 * @param  *c2:
 * @param  calc_type: 0-加, 1-减, 2-乘, 3-除
 * @retval None
 */
void complext_calculate(st_complex *dist, st_complex *c1, st_complex *c2, char calc_type)
{
    if (calc_type == 0) //加
    {
        dist->R = c1->R + c2->R;
        dist->I = c1->I + c2->I;
    }
    else if (calc_type == 1) //减
    {
        dist->R = c1->R - c2->R;
        dist->I = c1->I - c2->I;
    }
    else if (calc_type == 2) //乘, (a+bj)x(c+dj) = ac+adj+bcj+bdj^2 = (ac-bd)+(ad+bc)j
    {
        dist->R = c1->R * c2->R - c1->I * c2->I;
        dist->I = c1->R * c2->I + c1->I * c2->R;
    }
    else //除, 先取c2的共轭, a/b = (a*b共轭)/(b*b共轭)
    {
        st_complex conj = {c2->R, (0 - c2->I), 0, 0};
        st_complex tmp1 = {0};
        float tmp2;
        //分子做乘法
        tmp1.R = c1->R * conj.R - c1->I * conj.I;
        tmp1.I = c1->R * conj.I + c1->I * conj.R;
        //分母做乘法, 自身*自身共轭=实部平方+虚部平方, 分母一定只剩实部
        tmp2 = c2->R * c2->R + c2->I * c2->I;
        //做除法, 分子实部/分母, 分子虚部/分母
        dist->R = tmp1.R / tmp2;
        dist->I = tmp1.I / tmp2;
    }
}

/**
 * @brief  复数获取功率谱
 * @note
 * @param  *complex:
 * @retval None
 */
void complex_get_power_spectral(st_complex *complex)
{
    float S;
    S = complex->R * complex->R + complex->I * complex->I;
    complex->S = S;
}

/**
 * @brief  复数获取幅值
 * @note
 * @param  *complex:
 * @retval None
 */
void complex_get_magnitude(st_complex *complex)
{
    float M;
    complex_get_power_spectral(complex);
    M = sqrt(complex->S);
    complex->M = M;
}

/**
 * @brief  复数获取相位
 * @note
 * @param  *complex:
 * @retval None
 */
void complex_get_phase(st_complex *complex)
{
    float P;
    if (complex->I == 0.0f && complex->R == 0.0f) //复平面原点, 不存在
    {
        complex->P = 0;
        return;
    }
    if (complex->R == 0.0f) //实部为0, 虚部正数则为90度, 虚部负数则为-90度
    {
        if (complex->I > 0)
            complex->P = PI / 2.0f;
        else
            complex->P = -PI / 2.0f;
        return;
    }
    if (complex->I == 0.0f) //虚部为0, 实部正数则为0度, 实部负数则为180度
    {
        if (complex->R > 0)
            complex->P = 0.0f;
        else
            complex->P = 2.0f * PI;
        return;
    }
    P = atan(complex->I / complex->R);
    complex->P = P;
}

/**
 * @brief  连续的时间连续频率
 * @note
 * @param  *dist: 累计计算值
 * @param  *data: 当前时域函数值
 * @param  f: 当前频率
 * @param  t: 当前时间
 * @retval None
 */
void FT(st_complex *dist, st_complex *data, float f, float t)
{
    // data * (cos(2πft)-i*sin(2πft))
    st_complex fact = {0};
    fact.R = cos(2.0f * PI * f * t);
    fact.I = -sin(2.0f * PI * f * t);
    complext_calculate(dist, data, &fact, 2);
    complex_get_magnitude(dist);
    complex_get_phase(dist);
}

int main(int argc, char const *argv[])
{
    float sin_A = 10.0f;   //正弦波幅值
    float f = 10.0f;       //频率Hz
    float t = 0;           //时间累计ms
    float t_end = 200.0f; //采样结束时间ms

    float f_array[10] = {0}; //伪造频率组
    for (int i = 0; i < 10; i++)
        f_array[i] = f + i;

    st_complex dist[10] = {0}; //最终每组频率上的积分

    while (t < t_end) //循环产生连续时间离散频率的信号
    {
        for (int i = 0; i < 10; i++)
        {
            float _f = f_array[i];
            
            //只叠加一个固定频率正弦波 sin(wt)=sin(2πft)
            float wt = 2.0f * PI * _f * ((float)t / 1000); //按照每1ms时间递增
            float val = sin_A * sin(wt);

            st_complex cpx = {val}, tmp = {0};
            printf("f:%.3f, %.1fsin(%.3f)=%.3f\n", _f, sin_A, wt, val);

            FT(&tmp, &cpx, f, t);
            _print_complex(&tmp);

            complext_calculate(&dist[i], &dist[i], &tmp, 0); //对应频率做时域的积分
            complex_get_magnitude(&dist[i]);
            complex_get_phase(&dist[i]);
        }

        t++;
        Sleep(1);
    }
    printf("FT result:\n");
    _print_complex_array(dist, 10);
}
