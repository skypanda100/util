//
// Created by zhengdongtian on 4/22/19.
//

#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <assert.h>
#include <ctype.h>
#include <regex.h>

#define gettidv1() syscall(__NR_gettid)
#define gettidv2() syscall(SYS_gettid)

/**
 * system函数内部是调用fork，如果在调用system之前程序分配了1G内存，
 * fork后子进程以及父进程总过内存就是2G，因为我的程序会分配大量内存，
 * 并且有好几个线程都要调用system，所以造成内存不必要的大量的浪费，
 * 所以重新写一个system函数（内部使用vfork）
 * @param command
 * @return
 */
int v_system(const char *command);

/**
 * 创建目录以及子目录
 * @param path_ptr
 * @return
 */
int make_dir(const char *path_ptr);

/**
 * 调用bz2进行解压
 * @param src_file_path_ptr
 * @param dst_file_path_ptr
 * @return
 */
int bz2_decompress(const char *src_file_path_ptr, const char *dst_file_path_ptr);

/**
 * 去除左侧space
 * @param output_ptr
 * @param input_ptr
 */
void l_trim(const char *input_ptr, char *output_ptr);

/**
 * 去除左侧以及右侧的space
 * @param input_ptr
 * @param output_ptr
 */
void a_trim(const char *input_ptr, char *output_ptr);

/**
 * 提取符合pattern_ptr规则的开始以及结束索引
 * @param input_ptr
 * @param pattern_ptr
 * @param so_ptr
 * @param eo_ptr
 * @return
 */
int reg_exec(const char *input_ptr, const char *pattern_ptr, int *so_ptr, int *eo_ptr);

/**
 * 根据ascii排序
 * @param c_ptr_ptr
 * @param len
 * @param is_desc
 */
void sort_by_ascii(char **c_ptr_ptr, int len, int is_desc);

/**
 * 查找所有子目录
 * @param path_ptr
 * @param dir_ptr_ptr_ptr
 * @return
 */
int list_dir(const char *path_ptr,  char ***dir_ptr_ptr_ptr);

/**
 * 查找所有文件包括子目录中的的文件
 * @param path_ptr
 * @param file_ptr_ptr_ptr
 * @return
 */
int list_file(const char *path_ptr, char ***file_ptr_ptr_ptr);

/**
 * 拷贝文件（会自动创建目标目录）
 * @param src_ptr
 * @param dst_ptr
 * @return
 */
int copy_file(const char *src_ptr, const char *dst_ptr);

/**
 * 根据分隔符将源字符串拆分为源数组，再获取目标字符串在源数组中的位置
 * @param source_ptr
 * @param split_ptr
 * @param target_ptr
 * @return
 */
int index_of_str_with_separator(const char *source_ptr, const char *split_ptr, const char *target_ptr);

/**
 * 根据分隔符将源字符串拆分为源数组，再从源数组中获取目标索引位置的字符串
 * @param source_ptr
 * @param split_ptr
 * @param target_index
 * @param target_ptr
 */
void str_at_str_with_separator(const char *source_ptr, const char *split_ptr, int target_index, char *target_ptr);

/**
 * 将字符串转化为时间戳
 * @param time_ptr
 * @param fmt_str
 * @param timestamp_ptr
 */
void parse_str_to_timestamp(const char *time_ptr, const char *fmt_str, time_t *timestamp_ptr);

/**
 * 将时间戳转化为字符串
 * @param timestamp
 * @param fmt_str
 * @param time_ptr
 * @param time_len
 */
void format_timestamp_to_str(time_t timestamp, const char *fmt_str, char *time_ptr, int time_len);

/**
 * 将字符串转化为时间
 * @param time_ptr
 * @param fmt_str
 * @param tm_time_ptr
 */
void parse_str_to_tm(const char *time_ptr, const char *fmt_str, struct tm *tm_time_ptr);

/**
 * 将时间转化为字符串
 * @param tm_time_ptr
 * @param fmt_str
 * @param time_ptr
 * @param time_len
 */
void format_tm_to_str(const struct tm *tm_time_ptr, const char *fmt_str, char *time_ptr, int time_len);

#endif //UTIL_H
