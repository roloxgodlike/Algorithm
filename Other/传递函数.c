#include <stdio.h>

typedef void (*void_function_type)(char *, int);

void fun1(char *key, int val)
{
    printf("fun1-%s-%d\n", key, val);
}

void fun2(void_function_type callback)
{
    callback("fun2", 999);
}

int main()
{
    int *pFunInt = (int *)fun1;
    printf("0x%p, %d\n", pFunInt, pFunInt);
    ((void_function_type)pFunInt)("test1", 1);

    pFunInt = (int *)&fun1;
    printf("0x%p, %d\n", pFunInt, pFunInt);
    ((void_function_type)pFunInt)("test2", 2);

    void *pFun = fun1;
    printf("0x%p, %d\n", pFun, pFun);
    ((void_function_type)pFun)("test3", 3);

    ((void_function_type)0x00401460)("test4", 4);

    void_function_type fun;
    pFun = &fun1;
    *(void **)&fun = pFun;
    fun("test5", 5);

    fun2(fun);

    return 0;
}
