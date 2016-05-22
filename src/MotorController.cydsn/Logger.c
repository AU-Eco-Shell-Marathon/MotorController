#include <Logger.h>

char cellSeparator_ = ';';
char lineSepartor_ = '\n';

char* logFileName_ = "log%d.txt";
char* dataFileName_ = "data%d.csv";

int maxFileNumber_ = 50;

FS_FILE* logFile_;
FS_FILE* dataFile_;

char buffer[255];

void Logger_Init(void)
{
    char volumeName[20];
    
    FS_Init();
    DEBUG_UART_Start();
    
    Logger_Write("\nInitializing SD Card\n");
    
    if(0 != FS_GetVolumeName(0u, volumeName, 19))
    {
        Logger_Write("\tVolume: %s\n", volumeName);
    }
    else
    {
        Logger_Write("\tFailed to get sd card volume\n");
    }
    
    FS_FormatLLIfRequired(volumeName);
         
    if(FS_IsHLFormatted(volumeName) == 0)
    {
        Logger_Write("\tHigh level formatting sd card\n");
        FS_Format(volumeName, NULL);
    }
    
    U32 freeSpace = FS_GetVolumeFreeSpaceKB(volumeName);
    
    Logger_Write("\t%lu kB Free\n", freeSpace);
    Logger_Write("\tFS Debug level: %d \n", FS_DEBUG_LEVEL);
    
    //Open files
    int i;
    for(i = 0; i < maxFileNumber_; i++)
    {
        sprintf(buffer, logFileName_, i);
        
        if(FS_GetFileAttributes(buffer) == 0xFF) //Check if file exists
        {
            logFile_ = FS_FOpen(buffer, "w");
            Logger_Write("\tLog file number: %d\n", i);
            break;
        }
    }
    
    if(!logFile_)
        Logger_Write("\tCould not open log file\n");
    
    for(i = 0; i < maxFileNumber_; i++)
    {
        sprintf(buffer, dataFileName_, i);
        
        if(FS_GetFileAttributes(buffer) == 0xFF) //Check if file exists
        {
            dataFile_ = FS_FOpen(buffer, "w");
            Logger_Write("\tData file number: %d\n", i);
            break;
        }
    }
    
    if(!dataFile_)
        Logger_Write("\tCould not open data file\n");
}

void Logger_Write(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int length = vsprintf(buffer, format, args);
    va_end(args);
    
    if(logFile_ != 0)
    {
        FS_Write(logFile_, buffer, length); //Hope for the best, in case of any errors ignore it
    }
    
    if(LOGGER_WriteLogToUart)
    {
        DEBUG_UART_PutString(buffer);   
    }
}

void Logger_LogData(int argc,...)
{
    if(dataFile_)
    {
        va_list args;
        int i;
        int count;
        
        va_start(args, argc);
        
        for(i = 0; i < argc; i++)
        {
            count = sprintf(buffer, "%c%d", cellSeparator_, va_arg(args, int));
            FS_Write(dataFile_, buffer, count);
        }
        
        FS_Write(dataFile_, &lineSepartor_, 1);
        
        va_end(args);
    }
}

void Logger_LogDataHeaderLine(int argc,...)
{
    if(dataFile_)
    {
        va_list args;
        int i;
        int count;
        
        va_start(args, argc);
        
        for(i = 0; i < argc; i++)
        {
            count = sprintf(buffer, "%c%s", cellSeparator_, va_arg(args, const char*));
            FS_Write(dataFile_, buffer, count);
        }
        
        FS_Write(dataFile_, &lineSepartor_, 1);
        
        va_end(args);
    }
}

void Logger_Exit(void)
{
    if(dataFile_)
    {
        FS_FClose(dataFile_);
    }
    
    if(logFile_)
    {
        FS_FClose(dataFile_);
    }
}

void FS_X_Log(const char *s)
{
    Logger_Write(s);
}

void FS_X_Warn(const char *s)
{
    Logger_Write(s);
}

void FS_X_ErrorOut(const char *s)
{
    Logger_Write(s);
}