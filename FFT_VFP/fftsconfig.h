#ifndef _FFTSCONFIG_H
#define _FFTSCONFIG_H

// 注意：以下两种模式只能选择一种

/* 定义该库编译为Android平台 */
#define __ANDROID__ 1

/* 定义该库编译为Apple平台(iOS) */
/* #define __APPLE__ 1 */

/* 将下行取消注释可以禁用动态FFT代码生成功能
   可节约大概100KB的内存，但效率会大幅度降低
*/
/* #define DYNAMIC_DISABLED 1*/

#define restrict __restrict

#endif
