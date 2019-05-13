#include "../header/LogWorkerImpl.h"


LogWorkerImpl::LogWorkerImpl(LOG_LEVEL level, const char *file, const char *fun, int line, time_t cur)
{
    pos_.setTime(cur);
    pos_.setFile(file);
    pos_.setFunc(fun);
    pos_.setLine(line);
    data_.level_ = level;
}

LogWorkerImpl::~LogWorkerImpl()
{

}


bool LogWorkerImpl::isEnd(const char *end)
{
    if(!end)
        return false;
    if(!memcmp(end,LOG_END,strlen(LOG_END)))
        return true;
    return false;
}

void LogWorkerImpl::pushLog()
{
    if(pos_.isEmpty()||data_.isEmpty())
        return;
	auto it = std::make_pair(pos_, data_);
    PrintMgr::instance().pushLog(it);
	pos_.clear();
	data_.clear();
}

