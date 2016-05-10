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
    int i;
    FS_Init();
    
    char buffer[maxFileNameLength_];
        
    //Open files
    for(i = 0; i < maxFileNumber_; i++)
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
    }
}

void Logger_Write(char* str)
{
    if(logFile_)
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