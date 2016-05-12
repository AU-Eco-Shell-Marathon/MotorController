#ifndef LOGGER_H
#define LOGGER_H
    
void FS_X_Log(const char *s);
void FS_X_Warn(const char *s);
void FS_X_ErrorOut(const char *s);
    
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
/**
 * @brief Write to debug uart
 */
void Logger_WriteToDebugUART(const char* str);

#endif