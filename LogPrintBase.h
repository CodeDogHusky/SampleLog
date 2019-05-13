#pragma once
#include "LogDefine.h"
#include <condition_variable>
#include <deque>
#include <atomic>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctime>




class  LogPos{
public:
    LogPos():line_(0),is_empty_(true){memset(&cur_,0,sizeof(time_t));}
    LogPos(const LogPos& oth):line_(oth.line_),is_empty_(oth.is_empty_){func_=oth.func_;file_=oth.file_;memcpy(&cur_,&oth.cur_,sizeof(time_t));}
    LogPos(LogPos&& ref):is_empty_(ref.is_empty_),line_(ref.line_){func_=std::move(ref.func_);file_=std::move(ref.file_);;memcpy(&cur_,&ref.cur_,sizeof(time_t));}
    LogPos&operator=(LogPos&& ref){is_empty_=ref.is_empty_;line_=ref.line_;func_=std::move(ref.func_);file_=std::move(ref.file_);;memcpy(&cur_,&ref.cur_,sizeof(time_t));return *this;}
    LogPos&operator=(const LogPos& ref){is_empty_=ref.is_empty_;line_=ref.line_;func_=ref.func_;file_=ref.file_;memcpy(&cur_,&ref.cur_,sizeof(time_t));return *this;}

public:
	void clear() { file_ = std::string(); func_ = std::string(); line_ = 0; is_empty_ = true; cur_ = 0; }
    inline void setFile(const str_t& file){file_=file;is_empty_=false;}
    inline void setLine(const int& line){line_=line;is_empty_=false;}
    inline void setFunc(const str_t& fun){func_=fun;is_empty_=false;}
    inline void setTime(time_t time){cur_=time;is_empty_=false;}
    const str_t&      getFile()const{return file_;}
    const int&        getLine()const{return line_;}
    const str_t&      getFun()const{return func_;}
    const time_t&     getTime()const{return cur_;}
    const bool        isEmpty()const{return is_empty_;}
private:
    bool        is_empty_;
    std::string file_;
    std::string func_;
    int         line_;
    time_t      cur_;

};

struct LogData{
    LogData(){}
	LogData(const LogData& oth) :level_(oth.level_) { data_<< oth.data_.str(); }
    LogData(LogData&& ref):level_(ref.level_){data_=std::move(ref.data_);}
	LogData& operator =(LogData&& ref) { level_ = ref.level_; data_ = std::move(ref.data_); return *this; }
    LogData& operator =(const LogData& ref){level_=ref.level_;data_<<ref.data_.str();return *this;}

    bool isEmpty(){return (data_.tellp()<0);}
	void clear() { level_ = 0; data_.str(""); }
    int        level_;
	ss_t       data_;
};

class PrintBase{
public:
    PrintBase();
    virtual ~PrintBase(){}


    void pushLog(const std::pair<LogPos,LogData>& pair){
        un_lock_t lock(mtx_);
        que_log_.push_back(pair);
        cv_.notify_one();
    }

	void pushLog(std::pair<LogPos, LogData>&& pair) {
		un_lock_t lock(mtx_);
		que_log_.push_back(pair);
		cv_.notify_one();
	}
 
protected:
    virtual void _print()=0;
	void         _getLogItem(std::pair<LogPos, LogData>& itm);
    str_t        _setLogFormat(std::pair<LogPos,LogData> &data);
	

protected:
    std::atomic_bool                      exit_;
    mtx_t                                 mtx_;
    std::condition_variable               cv_;
    std::deque<std::pair<LogPos,LogData>> que_log_;

};
