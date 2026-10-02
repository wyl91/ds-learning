/*
 * 02 - 线性表之单链表（LinkList）
 *
 * 核心思想：节点**离散**分布，靠指针串起来。
 * 插入/删除只要改指针，O(1)（前提是已经拿到前驱）—— 不用搬数据。
 * 代价：不能随机访问，找第 i 个要跑 O(n)。
 *
 * 重点看两个"教科书必考"操作：
 *   1. 头插法建表（结果逆序） vs 尾插法建表（结果正序）
 *   2. 就地逆置（不申请新节点，只改指针方向）
 *
 * 带**头结点**：第一个节点不存数据，专门用来省掉"在表头插入"的特殊判断。
 */

#include <stdio.h>
#include <stdlib.h>
#include "common.h"

typedef struct LNode {
    ElemType data;
    struct LNode *next;
} LNode, *LinkList;

/* 初始化：创建头结点，next 置空 */
Status InitList(LinkList *L)
{
    *L = (LNode *)malloc(sizeof(LNode));
    if (*L == NULL) {
        return OVERFLOW;
    }
    (*L)->next = NULL;
    return OK;
}

/*
 * 头插法：每次把新节点接在头结点后面
 * 输入 1 2 3，得到 3 2 1 —— 逆序
 * 时间 O(n)，不用找尾，写起来最省事
 */
void CreateListHead(LinkList L, const ElemType *a, int n)
{
    int i;
    LNode *p;
    for (i = 0; i < n; i++) {
        p = (LNode *)malloc(sizeof(LNode));
        if (p == NULL) return;
        p->data = a[i];
        p->next = L->next;
        L->next = p;
    }
}

/*
 * 尾插法：维护一个尾指针 r，新节点挂 r 后面
 * 输入 1 2 3，得到 1 2 3 —— 正序
 * 时间 O(n)
 */
void CreateListTail(LinkList L, const ElemType *a, int n)
{
    int i;
    LNode *r = L, *p; /* r 初始指向头结点 */
    for (i = 0; i < n; i++) {
        p = (LNode *)malloc(sizeof(LNode));
        if (p == NULL) return;
        p->data = a[i];
        r->next = p;
        r = p; /* r 后移 */
    }
    r->next = NULL; /* 尾巴必须收口，否则野指针 */
}

/* 求表长：带头结点，从 L->next 开始数 */
int ListLength(const LinkList L)
{
    int len = 0;
    LNode *p = L->next;
    while (p != NULL) {
        len++;
        p = p->next;
    }
    return len;
}

/* 按位查找：返回第 i 个节点的指针，找不到返回 NULL */
LNode *GetElem(const LinkList L, int i)
{
    int j = 1;
    LNode *p = L->next;
    if (i < 1) return NULL;
    while (p != NULL && j < i) {
        p = p->next;
        j++;
    }
    return p;
}

/* 按值查找：返回第一个匹配的节点 */
LNode *LocateElem(const LinkList L, ElemType e)
{
    LNode *p = L->next;
    while (p != NULL && p->data != e) {
        p = p->next;
    }
    return p;
}

/* 在第 i 个位置插入 e。带头结点的好处：插到表头也不用改 L 本身 */
Status ListInsert(LinkList L, int i, ElemType e)
{
    LNode *p = GetElem(L, i - 1); /* 找前驱 */
    LNode *s;
    if (p == NULL) {
        return ERROR;
    }
    s = (LNode *)malloc(sizeof(LNode));
    if (s == NULL) {
        return OVERFLOW;
    }
    s->data = e;
    s->next = p->next; /* 先接后 */
    p->next = s;       /* 再断前 —— 顺序反了会丢链 */
    return OK;
}

/* 删除第 i 个节点，值用 *e 带出。记得 free，不然内存泄漏 */
Status ListDelete(LinkList L, int i, ElemType *e)
{
    LNode *p = GetElem(L, i - 1);
    LNode *q;
    if (p == NULL || p->next == NULL) {
        return ERROR;
    }
    q = p->next;
    p->next = q->next;
    if (e != NULL) {
        *e = q->data;
    }
    free(q);
    return OK;
}

/*
 * 就地逆置：不新建节点，把指针逐个掉头
 * 三指针法：pre / p / next，一趟走完 O(n)，空间 O(1)
 *
 *   pre=NULL  p=①->②->③
 *   循环一次：①->NULL, p=②->③
 *   再循环：  ②->①->NULL, p=③
 */
void ReverseList(LinkList L)
{
    LNode *pre = NULL, *p = L->next, *next;
    while (p != NULL) {
        next = p->next; /* 先存住后继，不然断链就找不回来了 */
        p->next = pre;  /* 掉头 */
        pre = p;        /* pre 前移 */
        p = next;       /* p 前移 */
    }
    L->next = pre; /* 头结点接上新的首节点 */
}

/*
 * 经典练习：找倒数第 k 个节点
 * 快慢指针，快指针先走 k 步，然后一起走，快指针到尾时慢指针就是答案
 * 一趟 O(n)，不要求表长
 */
LNode *FindKthFromEnd(const LinkList L, int k)
{
    LNode *fast = L->next, *slow = L->next;
    int i;
    if (k < 1) return NULL;
    for (i = 0; i < k && fast != NULL; i++) {
        fast = fast->next;
    }
    if (i < k) return NULL; /* 表长不足 k */
    while (fast != NULL) {
        fast = fast->next;
        slow = slow->next;
    }
    return slow;
}

/* 打印 */
void ListPrint(const LinkList L)
{
    LNode *p = L->next;
    printf("LinkList(len=%d): ", ListLength(L));
    if (p == NULL) {
        printf("(空表)");
    }
    while (p != NULL) {
        printf("%d", p->data);
        p = p->next;
        if (p != NULL) printf(" -> ");
    }
    printf("\n");
}

/* 整表销毁，学习阶段也要养成习惯 */
void DestroyList(LinkList L)
{
    LNode *p = L->next, *q;
    while (p != NULL) {
        q = p->next;
        free(p);
        p = q;
    }
    L->next = NULL;
}

/* ---------------- 测试 ---------------- */

int main(void)
{
    LinkList L1, L2;
    ElemType a[] = {1, 2, 3, 4, 5};
    ElemType e;
    int n = sizeof(a) / sizeof(a[0]);

    printf("===== 单链表 LinkList =====\n");
    printf("原始数组：1 2 3 4 5\n\n");

    /* 头插 vs 尾插 */
    InitList(&L1);
    CreateListHead(L1, a, n);
    printf("头插法建表（逆序）："); ListPrint(L1);

    InitList(&L2);
    CreateListTail(L2, a, n);
    printf("尾插法建表（正序）："); ListPrint(L2);

    /* 插入 / 删除 */
    ListInsert(L2, 3, 99);
    printf("\n在第 3 位插入 99："); ListPrint(L2);

    ListDelete(L2, 1, &e);
    printf("删除第 1 位（值 %d）：", e); ListPrint(L2);

    /* 按值查找 */
    printf("值 4 的节点地址：%p\n", (void *)LocateElem(L2, 4));
    printf("值 88 的节点地址：%p （NULL 表示没找到）\n", (void *)LocateElem(L2, 88));

    /* 就地逆置 */
    printf("\n逆置前："); ListPrint(L2);
    ReverseList(L2);
    printf("逆置后："); ListPrint(L2);

    /* 倒数第 k 个 */
    printf("\n倒数第 1 个：%d\n", FindKthFromEnd(L2, 1)->data);
    printf("倒数第 3 个：%d\n", FindKthFromEnd(L2, 3)->data);

    DestroyList(L1);
    DestroyList(L2);
    printf("\n链表已销毁。\n");
    return 0;
}
