#define TX_NEWLINE {SendChar('\n');SendChar('\r');}

void InitUart(void);
char ReadChar(void);
int SendChar(unsigned char Ch);
int SendString(char* String);
int SendInteger(int Number);
int SendUnsignedInteger(unsigned int Number);