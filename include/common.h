#ifndef DS_COMMON_H
#define DS_COMMON_H

/*
 * 数据结构学习 - 公共定义
 * 采用严蔚敏《数据结构》教材的约定：函数用 Status 返回状态码
 */

#define OK        1
#define ERROR     0
#define TRUE      1
#define FALSE     0
#define OVERFLOW -2

typedef int Status;    /* 函数返回状态 */
typedef int ElemType;  /* 数据元素类型，需要时改成 char / float / 结构体 */

#endif /* DS_COMMON_H */
