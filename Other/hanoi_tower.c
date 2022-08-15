#include <stdio.h>

/**
 * @brief  总共移动的次数
 * @note 一个盘只需1次---1
 * 两个盘上面两个要2次, 底部1次---2+1=3
 * 三个盘上面两个要移两次两个盘的次数, 底部1次---2*(3)+1=7
 * 四个盘上面三个要移两次两个盘的次数, 底部1次---2*(7)+1=15
 *
 * @param  plate_sum: 盘子总数
 * @retval
 */
int get_move_steps(int plate_sum)
{
    if (plate_sum == 1)
        return 1;
    else
        return 2 * get_move_steps(plate_sum - 1) + 1;
}

/**
 * @brief  盘子移动操作
 * @note
 * @param  column_start: 盘子原柱子位
 * @param  column_dist: 盘子目标柱子位
 * @param  plate: 盘子序号
 * @retval None
 */
void move_plate(char *column_start, char *column_dist, int plate)
{
    printf("盘子<%d>从<%s>移动至<%s>\n", plate, column_start, column_dist);
}

/**
 * @brief  解决汉诺塔问题
 * @note   
 * @param  *column_start: 最初放盘子的柱子
 * @param  *column_tmp: 中间过渡的柱子
 * @param  *column_dist: 最后目标堆放的柱子
 * @param  plate_sum: 盘子总数, 等效的盘子序号为1~总数
 * @retval None
 */
void solve_hanoi_tower(char *column_start, char *column_tmp, char *column_dist, int plate_sum)
{
    int plate_need_to_move = plate_sum - 1; //压在上面要移动的盘子总数
    int plate_bottom = plate_sum;           //最底部那块一次移到位的盘子序号

    if (plate_sum == 1) //剩一个直接移到位后结束
    {
        move_plate(column_start, column_dist, plate_bottom);
        return;
    }

    //柱子1上压着的盘子, 递归全移走到2, 让出1和3
    solve_hanoi_tower(column_start, column_dist, column_tmp, plate_need_to_move);

    //柱子1空出的最底部的盘子直接移到位, 1->3
    move_plate(column_start, column_dist, plate_bottom);

    //柱子2上剩下的继续移到位, 2->3
    solve_hanoi_tower(column_tmp, column_start, column_dist, plate_need_to_move);
}

int main(void)
{
    //三根柱子
    char column1[] = "柱1";
    char column2[] = "柱2";
    char column3[] = "柱3";
    //盘子数量, 全在柱子1, 需移到柱子3
    int plate_sum = 3;

    printf("盘子数: %d, 共需移: %d步\n", plate_sum, get_move_steps(plate_sum));

    solve_hanoi_tower(column1, column2, column3, plate_sum);
}