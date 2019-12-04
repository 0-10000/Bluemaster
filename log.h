#ifndef __PROXIMITY_LOG_H__
#define __PROXIMITY_LOG_H__

#include <stdio.h>

#define COLOR_RED "\033[31m"                                                              
#define COLOR_GREEN "\033[32m"                                                            
#define COLOR_BROWN "\033[33m"                                                            
#define COLOR_BLUE "\033[34m"                                                             
#define COLOR_END "\033[0m"                                                               

#define PRINT(fmt, args...) printf(fmt "\n", ##args)
#define PRINT_RED(fmt, args...) printf(COLOR_RED fmt COLOR_END "\n", ##args)
#define PRINT_GREEN(fmt, args...) printf(COLOR_GREEN fmt COLOR_END "\n", ##args)
#define PRINT_BROWN(fmt, args...) printf(COLOR_BROWN fmt COLOR_END "\n", ##args)
#define PRINT_BLUE(fmt, args...) printf(COLOR_BLUE fmt COLOR_END "\n", ##args)

#endif // __PROXIMITY_LOG_H__ 
