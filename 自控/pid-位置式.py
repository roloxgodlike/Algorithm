import matplotlib.pyplot as plt

class Pid():
    def __init__(self,exp,kp,ki,kd):
        self.exp=exp
        self.kp=kp
        self.ki=ki
        self.kd=kd
        self.error=0
        self.last_error=0
        self.real=0
        self.sum_error=0
        self.minu_error=0

    def calculate(self):
        self.last_error = self.error
        self.error = self.exp - self.real
#        self.minu_error=self.error-self.last_error
        self.sum_error += self.error
        self.real=self.kp*self.error + self.ki*self.sum_error + self.kd*(self.error-self.last_error)
        return self.real

    #@property
    def calcu_pi(self):
        self.error=self.exp-self.real
        self.sum_error +=self.error
        self.real=self.kp*self.error + self.ki*self.sum_error
        return self.real


    def calcu_pd(self):
        self.last_error=self.error
        self.error=self.exp-self.real
        self.real=self.kp*self.error + self.kd*(self.error-self.last_error)
        return self.real

jud='1'
while jud=='1':
    print('1.pid')
    print('2.pi')
    print('3.pd')
    print('4.exit')
    a=input('????')
    if a=='1':
        k = []
        trans_pid = Pid(5000, 0.1, 0.15, 0.05)
        for i in range(0, 100):
            k.append(trans_pid.calculate())
        plt.plot(k)
        plt.show()
    elif a=='2':
        k = []
        trans_pid = Pid(5000, 0.01, 0.15, 0.05)
        for i in range(0, 100):
            k.append(trans_pid.calcu_pi())
        plt.plot(k)
        plt.show()
    elif a=='3':
        k = []
        trans_pid = Pid(5000, 0.5, 0.05, 0.15)
        for i in range(0, 100):
            k.append(trans_pid.calcu_pd())
        plt.plot(k)
        plt.show()
    elif a=='4':
        exit()
    else:
        print('the right choose')
    jud=input('1;2?')