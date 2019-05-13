#pragma once
#include "LogDefine.h"
#include "LogPrintBase.h"
#include "LogPrintMgr.hpp"
class LogWorker;


class LogWorkerImpl{
    friend class LogWorker;
public:

    LogWorkerImpl(LOG_LEVEL level,const char* file,const char* fun,int line,time_t cur);
    ~LogWorkerImpl();
	template<typename DATA_TYPE >
	void appendData(DATA_TYPE data) {
		data_.data_ << data;
	}
   
    inline void _setLogInfo(LOG_LEVEL level,const char* file,const char* fun,int line,time_t cur){
        pos_.setFile(file);
        pos_.setLine(line);
        pos_.setFunc(fun);
        pos_.setTime(cur);
        data_.level_=level;	
    }
	bool isEnd(const char *end);
	void pushLog();

protected:
    
    
private:
    mtx_t       mtx_;
    LogPos      pos_;
    LogData     data_;
};



