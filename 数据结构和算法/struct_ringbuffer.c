#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define RINGBUF_SIZE 8       // buf内存大小
#define RINGBUF_FORBIT_COVER // 禁止覆盖写入, 开启后不得覆盖未读的数据

typedef struct
{
    uint16_t w;                // 最新写指针
    uint16_t r;                // 最新读指针
    uint16_t len;              // 未读数据长度
    uint8_t buf[RINGBUF_SIZE]; // 缓冲区
} ringbuf_t;

/**
 * @brief  处理新指针
 * @note   如果达到buf最大位置要回到原点
 * @param  *p:
 * @retval None
 */
uint16_t ringbuf_get_next_point(uint16_t p)
{
    return (p + 1) % RINGBUF_SIZE;
}

/**
 * @brief  判断是否可写入, 如果写入位置即将和读位置重叠了, 说明写的快读的慢再写就会覆盖没读的数据
 * @note   如果实际写入快也允许直接覆盖可以不调用
 * @param  *r_buf:
 * @retval 1-可写, 0-不可写
 */
char ringbuf_is_writable(ringbuf_t *r_buf)
{
    return (ringbuf_get_next_point(r_buf->w) != r_buf->r);
}

/**
 * @brief  判断是否可读, 如果读取位置即将和写如位置重叠了, 说明已经读完有没剩余数据
 * @note
 * @param  *r_buf:
 * @retval 1-可读, 0-不可读
 */
char ringbuf_is_readable(ringbuf_t *r_buf)
{
    return (r_buf->r != r_buf->w);
}

void ringbuf_write_byte(ringbuf_t *r_buf, uint8_t data)
{
#ifdef RINGBUF_FORBIT_COVER
    if (!ringbuf_is_writable(r_buf))
        return;
#endif
    printf("W1-d-%d-w-%d-r-%d-len-%d\n", data, r_buf->w, r_buf->r, r_buf->len);
    r_buf->buf[r_buf->w] = data;
    r_buf->w = ringbuf_get_next_point(r_buf->w);
    r_buf->len++;
    printf("W2-d-%d-nw-%d-r-%d-nlen-%d\n", data, r_buf->w, r_buf->r, r_buf->len);
}

void ringbuf_read_byte(ringbuf_t *r_buf, uint8_t *dist)
{
    if (!ringbuf_is_readable(r_buf))
        return;
    *dist = r_buf->buf[r_buf->r];
    printf("R1-d-%d-w-%d-r-%d-len-%d\n", *dist, r_buf->w, r_buf->r, r_buf->len);
    r_buf->r = ringbuf_get_next_point(r_buf->r);
    r_buf->len--;
    printf("R2-d-%d-w-%d-nr-%d-nlen-%d\n", *dist, r_buf->w, r_buf->r, r_buf->len);
}

int main(int argc, char const *argv[])
{
    ringbuf_t _ringbuf = {0};
    uint8_t read;
    printf("ringbuff-size-%d\n", RINGBUF_SIZE);

    // for (int i = 0; i < 10; i++)
    // {
    //     ringbuf_write_byte(&_ringbuf, i + 31);
    //     printf("write-%d\n", i + 31);
    //     ringbuf_read_byte(&_ringbuf, &read);
    //     printf("read-%d\n", read);
    // }
    for (int i = 0; i < 10; i++)
    {
        ringbuf_write_byte(&_ringbuf, i + 30);
        printf("write-%d\n", i + 30);
    }
    for (int i = 0; i < 10; i++)
    {
        ringbuf_read_byte(&_ringbuf, &read);
        printf("read-%d\n", read);
    }

    return 0;
}
