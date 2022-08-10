#include "DFFT.c" //TODO
#include <windows.h>

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
    float sin_A = 10.0f;  //正弦波幅值
    float f = 10.0f;      //频率Hz
    float t = 0;          //时间累计ms
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
