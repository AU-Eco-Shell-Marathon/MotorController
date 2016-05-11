#ifndef LOGGER_H
#define LOGGER_H

#include <project.h>
#include <FS.h>
#include <string.h>
#include <Global.h>
#include <stdarg.h>
#include <stdio.h>

/**
 * @brief Initialize logger
 */
void Logger_Init(void);
/**
 * @brief Write to log
 */
void Logger_Write(char* str);
/**
 * @brief Log data
 */
void Logger_LogData(int argc,...);
/**
 * @brief Close logger
 */
void Logger_Exit(void);

#endif