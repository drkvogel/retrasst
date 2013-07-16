#ifndef WinConsoleCmdH
#define WinConsoleCmdH

#define DEFAULT_TIMEOUT_VALUE 5000
#define MSG_BUF_LEN           2000

/*
DialogLogic
-----------

Implement a DialogLogic function as follows:
1) Read output from the console process. 
2) On the basis of the output, determine if the console process is waiting for an instruction from standard input.
3) If so, write an instruction to the nextCmd buffer. Ensure that the instruction terminates with a newline character.

Note that userData is the value supplied by the user in WinConsoleCmdArgs.
*/
typedef bool (*DialogLogic)( const char* output, char* nextCmd, char* userData );

/*
Callback for logging. Implementations should be threadsafe.
*/
typedef void (*LogFunc)( const char* msg );

struct WinConsoleCmdArgs
{
    WinConsoleCmdArgs();
    const char* launchString;
    DialogLogic dialogLogic;
    LogFunc log;
    //userData can be NULL. It gets passed directly through to DialogLogic.
    char* userData; 
    unsigned long timeoutMillis;
    char exceptionMsg[MSG_BUF_LEN];
    bool timedOut;
    unsigned long exitCode;
    bool success() const { return  ( exitCode == 0 ) && ( ! timedOut ) && ( exceptionMsg[0] == '\0' ); }
};

/*
To find out how the child process fared, check the following members of WinConsoleCmdArgs:
- exceptionMsg
- timedOut
- exitCode
*/
void runWinConsoleCmd( WinConsoleCmdArgs* inArgs );

/*
params:

    commandStack: a list of newline-separated strings, the last of 
                    which is considered the first (like a stack)

    lastCommand: pointer to a buffer to which will be written the 
                    last/top-most command

This function removes the last command from commandStack and copies it to lastCommand.
*/
void removeLastCommand( char* lastCommand, char* commandStack );

#endif
