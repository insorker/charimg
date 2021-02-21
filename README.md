# C语言实现BMP图片转字符画

## 简介

- insorker
- 2021/2/21

实现BMP图片转字符画输出到命令行或文件。

## 使用

包含头文件 "charimg.h"

函数 chim_cmd 提供命令行输出

函数 chim_file 提供文件输出

dw、dh 指横向、纵向除数，如 dh 为2表示纵向字符像素数除以2

头文件 "charimg_test.h" 提供了几个样例供测试