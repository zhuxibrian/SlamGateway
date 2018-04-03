/*
 * main.h
 *
 *  Created on: Apr 3, 2018
 *      Author: xd
 */

#ifndef MAIN_H_
#define MAIN_H_


extern int verbose(int v, const char *fmt, ...);
#define vfault(...)		verbose(0,__VA_ARGS__)
#define vinfo(...)		verbose(1,__VA_ARGS__)
#define vnormal(...)	verbose(2,__VA_ARGS__)

#endif /* MAIN_H_ */
