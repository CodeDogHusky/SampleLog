#pragma once
#include "LogPrintBase.h"
class PrintSocket:public PrintBase{
public:
    PrintSocket();
    ~PrintSocket();

protected:
    void _print();

private:

};
