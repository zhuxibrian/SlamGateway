/*
 * utils.h
 *
 *  Created on: 2017年8月12日
 *      Author: houxd
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <linux/limits.h>

/*
 * 执行shell命令
 * rbuf: 返回内容
 * rbuflen: 返回buf最大值
 * cmdfmt: 命令字符串
 * return: 命令退出的状态
 */
int shell_exec(char *rbuf, int rbuflen, const char *cmdfmt, ...);
/*
 * 获取当前日期和时间
 */
extern void get_date_time(uint32_t* _date,uint32_t *_time,uint16_t *_msec);
/*
 * 根据文件的路径自动创建父级各个目录
 */
extern void fmkdir_parent(const char *s);
/*
 * 打开文件,并自动创建各个级目录
 */
extern FILE* fopen_automkdir(const char *fpath, const char *mode);
/*
 * 检查文件是否存在,文件可以包含一个crc32码.
 * fpath:文件路径.例如 /opt/a.txt
 * pathbuf:返回找到的文件路径可以是下面任意一种:
 * /opt/aXXXXXXXX.txt	crc32返回XXXXXXXX
 * /opt/a_XXXXXXXX.txt	crc32返回XXXXXXXX
 * /opt/a.txt		crc32返回 0x0
 * crc32: 返回文件名包含的crc码
 * return 返回文件是否被找到.
 */
extern int fexist_or_crc32named(char (*pathbuf)[PATH_MAX],uint32_t *crc32);
extern FILE* fopen_crcnamed(const char *fpath, const char *mode, uint32_t *crc32);
/*
 * 计算文件的crc32值
 */
extern uint32_t fcalc_crc32(const char *fpath);
extern uint32_t fcalc_crc32(FILE* fp);
/*
 * 获取文件长度
 */
long fsize(FILE* f);
long fsize(const char *fpath);
/*
 * 以文本方式打开文件,并读取所有文本到buf
 * 返回文本长度,或错误<0
 */
extern int fread_alltext(FILE* f, char *buf, int buflen_max);
extern char* fread_alltext(FILE* f);
extern int fread_alltext(const char *fpath, char *buf, int buflen_max);
extern char* fread_alltext(const char *fpath);
/*
 * 拷贝文件
 */
extern int fcopy(const char *src, const char *tar);
/*
 * 检查文件是否存在
 */
extern int fexist(const char *fpath);
/*
 * 删除文件
 */
extern int fremove(const char *fpath);
/*
 * 删除目录或者文件
 */
extern int frmdir(const char *dir);
/*
 * 备份文件到 *.bak
 */
extern int fbackup(const char *fpath);
/*
 * 从 *.bak 还原文件
 */
extern int frecover(const char *fpath);
/*
 * 删除*.bak文件
 */
extern int fbackup_clear(const char* fpath);
/*
 *  将文件路径分解为 父目录 文件名 文件名不包含扩展 扩展名
 *  .a =>  			"" 		"" 		""	.a
 *  a =>  			"" 		a 		a	""
 * 	./a	=>			./ 		a  		a	""
 *  /.a => 			/ 		.a		"" 	.a
 * 	/a.txt	=> 		/ 		a 		a	.txt
 * 	/home/a.txt => 	/home/	a.txt	a	.txt
 * 	/home/a => 		/home/	a		a	""
 * 	/home/ => 		/home/	""		""	""
 * 	/home => 		/ home	""		""	""
 */
extern void path_split(const char *fpath,
		char (*parent)[PATH_MAX],
		char (*fname)[NAME_MAX],
		char (*fname_noext)[NAME_MAX],
		char (*ext)[NAME_MAX]);
extern void path_get_parent(const char *fpath, char (*parent_dir)[PATH_MAX]);
extern void path_get_filename(const char *fpath, char (*fname)[NAME_MAX]);

uint32_t crc32c(const uint8_t *data, unsigned int length);
uint32_t crc32c(uint32_t crc, const uint8_t *data, unsigned int length);

#endif /* UTILS_H_ */
