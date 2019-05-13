#pragma once
#include "LogDefine.h"
#include "LogPrintBase.h"
#include "LogPrintConsl.h"
#include "LogPrintFile.h"
#include "LogPrintSocket.h"
#include <vector>

class PrintMgr{
public:
    ~PrintMgr()
    {
        for(auto it:vec_printer_)
            delete it;
    }

    static PrintMgr& instance(){
        static PrintMgr mgr;
        return mgr;
    }
    void pushLog(const std::pair<LogPos,LogData>& log){
        locker_t lock(mtx_);
        for(auto it:vec_printer_){
            it->pushLog(log);
        }
    }


    void setLogMode(int mode){
        locker_t lock(mtx_);
        if(mode&(int)LOG_MOD::LOG_MOD_PRINT_CONSL){
            PrintBase* printer=new PrintConsl;
            vec_printer_.push_back(printer);
        }
        if(mode& (int)LOG_MOD::LOG_MOD_PRINT_SOCK){
            PrintBase* printer=new PrintSocket;
            vec_printer_.push_back(printer);
        }
        if(mode& (int)LOG_MOD::LOG_MOD_PRINT_FILE){
            PrintBase* printer=new PrintFile;
            vec_printer_.push_back(printer);
        }
    }

private:
    PrintMgr(){}

    std::vector<PrintBase*> vec_printer_;
    mtx_t                   mtx_;
};


