#include <Logger.h>

char cellSeparator_ = ';';
char lineSepartor_ = '\n';

char* logFileName_ = "log%d.txt";
char* dataFileName_ = "data%d.csv";

int maxFileNumber_ = 20;
int maxFileNameLength_ = 100;

FS_FILE* logFile_;
FS_FILE* dataFile_;

void Logger_Init(void)
{
    char buffer[100];
    char volumeName[20];
    
    FS_Init();
    
    
    if(0 != FS_GetVolumeName(0u, volumeName, 19))
    {
        Logger_WriteToDebugUART("Volume: ");
        Logger_WriteToDebugUART(buffer);
        Logger_WriteToDebugUART("\n");
    }
    else
    {
        Logger_WriteToDebugUART("failed to get sd card volume\n");
    }
    
    FS_FormatLLIfRequired(volumeName);
    
    Logger_WriteToDebugUART("Initializing\n");
         
        
    if(FS_IsHLFormatted(volumeName) == 0)
    {
        Logger_WriteToDebugUART("High level formatting\n");
        FS_Format(volumeName, NULL);
    }
    
    U32 freeSpace = FS_GetVolumeFreeSpaceKB(volumeName);
    
    sprintf(buffer, "%lu", freeSpace);
    Logger_WriteToDebugUART(buffer);
    Logger_WriteToDebugUART("kB Free\n");
    
    DEBUG_UART_Start();
    
    
    sprintf(buffer, "%d", FS_DEBUG_LEVEL);
    
    Logger_WriteToDebugUART("FS Debug level: ");
    Logger_WriteToDebugUART(buffer);
    Logger_WriteToDebugUART("\n");
    
    if(0 == FS_MkDir("Dir"))
    {
        /* Display successful directory creation message */
        Logger_WriteToDebugUART("\"Dir\" created\n");
    }
    else
    {
        /* Display failure message */
        Logger_WriteToDebugUART("Failed to create dir\n");
    }
    
        
    logFile_ = FS_FOpen("testlog.txt", "w");
    
    if(!logFile_)
        Logger_WriteToDebugUART("Could not open log file\n");
    
    dataFile_ = FS_FOpen("testdata.txt", "w");
    
    if(!dataFile_)
        Logger_WriteToDebugUART("Could not open data file\n");
        
    //Open files
    /*for(i = 0; i < maxFileNumber_; i++)
    {
        sprintf(buffer, logFileName_, i);
        
        if(FS_GetFileAttributes(buffer) != 0xFF) //Check if file exists
        {
            logFile_ = FS_FOpen(buffer, "w");
        }
    }
    
    
    for(i = 0; i < maxFileNumber_; i++)
    {
        sprintf(buffer, dataFileName_, i);
        
        if(FS_GetFileAttributes(buffer) != 0xFF) //Check if file exists
        {
            dataFile_ = FS_FOpen(buffer, "w");
        }
    }*/
}

void Logger_Write(char* str)
{
    if(logFile_ != 0)
    {
        FS_Write(logFile_, str, strlen(str)); //Hope for the best, in case of any errors ignore it
    }
}

void Logger_LogData(int argc,...)
{
    if(dataFile_)
    {
        va_list valist;
        int i;
        int count;
        char buff[100];
        
        va_start(valist, argc);
        
        for(i = 0; i < argc; i++)
        {
            FS_Write(dataFile_, &cellSeparator_, 1); // kan vi ikke bare udføre alt dette i en?
            count = sprintf(buff, "%d", va_arg(valist, int));
            FS_Write(dataFile_, buff, count);
            FS_Write(dataFile_, &lineSepartor_, 1);
        }
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
    Logger_WriteToDebugUART(s);
}

void FS_X_Warn(const char *s)
{
    Logger_WriteToDebugUART(s);
}

void FS_X_ErrorOut(const char *s)
{
    Logger_WriteToDebugUART(s);
}

void Logger_WriteToDebugUART(const char* str)
{
    DEBUG_UART_PutString(str);
}