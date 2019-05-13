#include "LogWorker.h"
#include "LogWorkerImpl.h"
#include "LogObjPool.hpp"
#define HAND(obj) ((LogWorkerImpl*)obj)

ObjPool<LogWorkerImpl> g_pool;

void LogWorker::setConfigPath(const str_t& conf) {
	LogSetting::getInstance().setConfigPath(conf);
	LogSetting::getInstance().init();
}

LogWorker::LogWorker(LOG_LEVEL level, const char *file, const char *fun, int line):hand_(nullptr)
{
    time_t cur=time(NULL);
    hand_=g_pool.getObj();
    if(!hand_)
        hand_=new LogWorkerImpl(level,file,fun,line,cur);
    else
        HAND(hand_)->_setLogInfo(level,file,fun,line,cur);
}


LogWorker::~LogWorker()
{
	HAND(hand_)->pushLog();
	g_pool.pushObj((LogWorkerImpl*)hand_);
	hand_ = nullptr;
}


LogWorker &LogWorker::operator<<(const char *log)
{
	if (HAND(hand_)->isEnd(log))
		HAND(hand_)->pushLog();
	else
		HAND(hand_)->appendData(log);
    return *this;
}

LogWorker &LogWorker::operator<<(char *log)
{
	if (HAND(hand_)->isEnd(log))
		HAND(hand_)->pushLog();
	else
		HAND(hand_)->appendData(log);
	return *this;
}

LogWorker &LogWorker::operator<<(const char &log)
{
	HAND(hand_)->appendData(log);
	return *this;
}
LogWorker& LogWorker::operator<<(const unsigned char& log) {
	HAND(hand_)->appendData(log);
	return *this;
}

LogWorker &LogWorker::operator<<(const str_t &log)
{
	if (HAND(hand_)->isEnd(log.c_str()))
		HAND(hand_)->pushLog();
	else
		HAND(hand_)->appendData(log);
    return *this;
}

LogWorker &LogWorker::operator<<(const int &log)
{
	HAND(hand_)->appendData(log);
    return *this;
}

LogWorker &LogWorker::operator<<(const size_t &log)
{
	HAND(hand_)->appendData(log);
    return *this;
}

LogWorker &LogWorker::operator<<(const float &log)
{
	HAND(hand_)->appendData(log);
    return *this;
}

LogWorker &LogWorker::operator<<(const double &log)
{
	HAND(hand_)->appendData(log);
    return *this;
}
LogWorker& LogWorker::operator<<(const bool&   log) {
	HAND(hand_)->appendData(log);
	return *this;
}

LogWorker& LogWorker::operator<<(void*         log) {
	char buf[12] = { 0 };
	sprintf(buf, "%p", log);
	HAND(hand_)->appendData(buf);
	return *this;
}

LogWorker& LogWorker::operator<<(const unsigned int& log) {
	HAND(hand_)->appendData(log);
	return *this;
}
LogWorker& LogWorker::operator<<(const short&  log) {
	HAND(hand_)->appendData(log);
	return *this;
}
LogWorker& LogWorker::operator<<(const unsigned short& log) {
	HAND(hand_)->appendData(log);
	return *this;
}

