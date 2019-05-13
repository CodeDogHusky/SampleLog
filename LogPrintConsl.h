#pragma once
#include "LogPrintBase.h"
#include "LogSet.h"
#include <sstream>

#include <thread>
#include <chrono>
#ifdef WIN32
#include <windows.h>
#endif


#define RESET   "\033[0m"
#define RED     "\033[31m" /* Red */
#define GREEN   "\033[32m" /* Green */
#define YELLOW  "\033[33m" /* Yellow */
#define BLUE    "\033[34m" /* Blue */
#define MAGENTA "\033[35m" /* Magenta */
#define WHITE   "\033[37m" /* White */
#define MAGENTA "\033[35m" /* Magenta  fatal*/

class PrintConsl:public PrintBase{
public:
    PrintConsl();
    ~PrintConsl();
protected:
    void _print();
    void _setLogColor(std::pair<LogPos, LogData> &data);

private:
#ifdef WIN32
    HANDLE hand_conls_;
#endif

    str_t  pos_str_;

};
