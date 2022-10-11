import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(0, 100, 200)  # 线性取值
print(type(x), x)

# 加入高斯噪音, loc中值, scale方差σ(越大正态分布越胖)
noise = np.random.normal(loc=0, scale=5, size=200)
print(type(noise), noise)
y = 3 * x + 10 + noise

# plt.scatter(x, y)
# plt.xlabel('x')
# plt.ylabel('y')
# plt.title('x-y')
# plt.show()

"""
线性函数为: y = w * x + b
损失函数(目标): L = Σ(i=1~n)(WXi + b - Yi)² 即点到直线的距离
目的是找出w, b使得L最小

   Σ(i=1~n)(Xi - X_bar)(Yi - Y_bar)    Σ(i=1~n)(Xi * Yi) - n * (X_bar - Y_bar)
w= -------------------------------- = -------------------------------------
        Σ(i=1~n)(Xi - X_bar)²               Σ(i=1~n)Xi² - n * X_bar²

b = Y_bar - w * X_bar
"""


def computer_L(w, b, x, y):
    """
    计算损失函数L
    """
    loss_total = 0
    for i in range(len(x)):
        loss_total += (w*x[i] + b - y[i])**2
    return loss_total
# print(computer_L(2, 3, x, y))


def average(data):
    """
    计算数据的平均数
    """
    cnt = 0
    for i in range(len(data)):
        cnt += data[i]
    return cnt/len(data)
# print(average([1, 2, 3]))


def fix(x, y):
    """
    通过x,y线性回归推测出w和b
    """
    up = 0  # 分子
    down = 0  # 分母
    x_bar = average(x)
    y_bar = average(y)

    for i in range(len(x)):
        up += (x[i] - x_bar) * (y[i] - y_bar)
        down += (x[i] - x_bar) ** 2

    w = up/down
    b = y_bar - w * x_bar
    return w, b


w, b = fix(x, y)
print('w', w, 'b', b)
plt.scatter(x, y)
plt.xlabel('x')
plt.ylabel('y')
plt.title('x-y')
y_pre = w * x + b
plt.plot(x, y_pre, c='r')
plt.show()
