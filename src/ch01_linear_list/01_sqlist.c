/*
 * 01 - 线性表之顺序表（SqList: Se**q**uence List）
 *
 * 核心思想：用一段**连续**的内存存数据，靠下标直接定位 —— 随机访问 O(1)。
 * 代价：插入/删除要搬元素，平均 O(n)。
 *
 * 编译运行：
 *   Ctrl+Shift+B  ->  C: 编译并运行当前文件
 * 调试：
 *   在上面那个 launch 配置里打断点，按 F5
 */

#include <stdio.h>
#include <stdlib.h>
#include "common.h"

#define MAXSIZE 100 /* 顺序表最大容量，宏名字必须全大写 */

typedef struct {
    ElemType data[MAXSIZE]; /* 静态数组，容量编译期固定 */
    int length;             /* 当前元素个数，注意不是容量 */
} SqList;

/* ---------------- 基本操作 ---------------- */

/* 初始化：把 length 置 0 就是空表，不需要清数组内容 */
Status InitList(SqList *L)
{
    if (L == NULL) {
        return ERROR;
    }
    L->length = 0;
    return OK;
}

/* 判空 */
int ListEmpty(const SqList *L)
{
    return (L->length == 0) ? TRUE : FALSE;
}

/* 求表长 */
int ListLength(const SqList *L)
{
    return L->length;
}

/*
 * 插入：在第 i 个位置（1 <= i <= length+1）插入 e
 *
 *   i=3 时，先把 [3..length] 整体右移一格，再写入
 *   [a][b][c][d][ ]        i=3 插入 x
 *   [a][b][ ][c][d]        右移
 *   [a][b][x][c][d]        写入
 *
 * 时间：平均 O(n)，因为要搬元素
 */
Status ListInsert(SqList *L, int i, ElemType e)
{
    int j;

    /* 1. 位置合法性：i 必须是 [1, length+1]，length+1 表示追加到末尾 */
    if (i < 1 || i > L->length + 1) {
        return ERROR;
    }
    /* 2. 表满 */
    if (L->length >= MAXSIZE) {
        return OVERFLOW;
    }
    /* 3. 从后往前搬，不能从前往后，否则会覆盖数据 */
    for (j = L->length; j >= i; j--) {
        L->data[j] = L->data[j - 1];
    }
    L->data[i - 1] = e;
    L->length++;
    return OK;
}

/*
 * 删除：删掉第 i 个位置（1 <= i <= length），被删的值用 *e 带出去
 * 时间：平均 O(n)
 */
Status ListDelete(SqList *L, int i, ElemType *e)
{
    int j;

    if (i < 1 || i > L->length) {
        return ERROR;
    }
    if (e != NULL) {
        *e = L->data[i - 1];
    }
    /* 从前往后搬，后面的往前补位 */
    for (j = i; j < L->length; j++) {
        L->data[j - 1] = L->data[j];
    }
    L->length--;
    return OK;
}

/*
 * 按值查找：返回第一次出现的位置（1-based），找不到返回 0
 * 时间：O(n)，因为要逐个比
 */
int LocateElem(const SqList *L, ElemType e)
{
    int i;
    for (i = 0; i < L->length; i++) {
        if (L->data[i] == e) {
            return i + 1; /* 转成教材的 1-based 下标 */
        }
    }
    return 0;
}

/* 按位取值：第 i 个位置的值，通过 *e 带出。O(1) —— 顺序表的核心优势 */
Status GetElem(const SqList *L, int i, ElemType *e)
{
    if (i < 1 || i > L->length) {
        return ERROR;
    }
    *e = L->data[i - 1];
    return OK;
}

/* 遍历打印 */
void ListPrint(const SqList *L)
{
    int i;
    printf("SqList(len=%d): [", L->length);
    for (i = 0; i < L->length; i++) {
        printf("%d", L->data[i]);
        if (i != L->length - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

/*
 * 经典练习：合并两个**有序**顺序表，保持有序
 * 双指针各走一遍，时间 O(m+n)，空间要开新表
 */
Status MergeList(const SqList *La, const SqList *Lb, SqList *Lc)
{
    int i = 0, j = 0, k = 0;

    if (La->length + Lb->length > MAXSIZE) {
        return OVERFLOW;
    }
    while (i < La->length && j < Lb->length) {
        if (La->data[i] <= Lb->data[j]) {
            Lc->data[k++] = La->data[i++];
        } else {
            Lc->data[k++] = Lb->data[j++];
        }
    }
    while (i < La->length) {
        Lc->data[k++] = La->data[i++];
    }
    while (j < Lb->length) {
        Lc->data[k++] = Lb->data[j++];
    }
    Lc->length = k;
    return OK;
}

/* ---------------- 测试 ---------------- */

int main(void)
{
    SqList L, La, Lb, Lc;
    int i;
    ElemType e;

    printf("===== 顺序表 SqList 基本操作 =====\n");

    InitList(&L);
    printf("初始化后："); ListPrint(&L);

    /* 尾部依次插入 10 20 30 40 */
    for (i = 1; i <= 4; i++) {
        ListInsert(&L, L.length + 1, i * 10);
    }
    printf("尾插 10/20/30/40："); ListPrint(&L);

    /* 在第 2 个位置插 15 */
    ListInsert(&L, 2, 15);
    printf("在下标 2 处插入 15："); ListPrint(&L);

    /* 删掉第 3 个位置 */
    if (ListDelete(&L, 3, &e) == OK) {
        printf("删除第 3 个元素，值是 %d：", e); ListPrint(&L);
    }

    /* 按值查找 */
    printf("值 30 的位置：%d\n", LocateElem(&L, 30));
    printf("值 99 的位置：%d （0 表示没找到）\n", LocateElem(&L, 99));

    /* 按位取值 —— O(1) */
    if (GetElem(&L, 2, &e) == OK) {
        printf("第 2 个元素：%d\n", e);
    }

    /* 越界测试：插到第 100 个位置应该失败 */
    printf("越界插入返回：%s\n",
           ListInsert(&L, 100, 5) == OK ? "OK" : "ERROR(符合预期)");

    /* ---- 合并两个有序表 ---- */
    printf("\n===== 合并两个有序顺序表 =====\n");
    InitList(&La);
    InitList(&Lb);
    for (i = 1; i <= 5; i++) ListInsert(&La, La.length + 1, i * 2);       /* 2 4 6 8 10 */
    for (i = 1; i <= 4; i++) ListInsert(&Lb, Lb.length + 1, i * 3 - 1);   /* 2 5 8 11   */
    printf("La = "); ListPrint(&La);
    printf("Lb = "); ListPrint(&Lb);
    MergeList(&La, &Lb, &Lc);
    printf("Lc = "); ListPrint(&Lc);

    return 0;
}
