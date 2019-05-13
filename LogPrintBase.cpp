#include "LogPrintBase.h"
#include "LogSet.h"
PrintBase::PrintBase()
{
    exit_.store(false);
}


str_t PrintBase::_setLogFormat(std::pair<LogPos, LogData> &data)
{
	std::stringstream ss;
    str_t time_str(LogSetting::getDateTimeStr(data.first.getTime()));
	ss << time_str << " [";
	if (LogSetting::getInstance().getFormat().print_file_)
        ss << data.first.getFile()<< " :";
	if (LogSetting::getInstance().getFormat().print_fun_)
        ss << data.first.getFun();
	if (LogSetting::getInstance().getFormat().print_line_)
        ss << ":" << data.first.getLine();
	ss << "] ";
	return ss.str();
}


void PrintBase::_getLogItem(std::pair<LogPos, LogData>& item)
{
	un_lock_t lock(mtx_);
	if (!que_log_.empty()) {
		item = std::move(que_log_.front());
		que_log_.pop_front();
	}
	else {
		cv_.wait(lock, [&]() {
			if (exit_)
				return true;
			else if (!que_log_.empty()) {
				item = std::move(que_log_.front());
				que_log_.pop_front();
				return true;
			}
			return false;
		});
	}
	
}
