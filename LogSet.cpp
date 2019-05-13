#include "LogSet.h"
#include "LogPrintMgr.hpp"

LogSetting::LogSetting(){

}

LogSetting::~LogSetting(){

}

void LogSetting::setConfigPath(const str_t &path)
{
	_parseConfig(path);
}

void LogSetting::_parseConfig(const std::string& path)
{
	IniParse parse(path);
	log_config_.print_format_.print_file_=parse.GetItemInt("PrintFormat", "DisPlayFile", 0);
	log_config_.print_format_.print_fun_ = parse.GetItemInt("PrintFormat", "DisPlayFun", 1);
	log_config_.print_format_.print_line_ = parse.GetItemInt("PrintFormat", "DisPlayLine", 1);
    log_config_.print_format_.print_time_ = parse.GetItemInt("PrintFormat", "DisplayTime", 1);
    log_config_.print_format_.print_color_=parse.GetItemInt("PrintFormat","DisplayColor",0);
    log_config_.dir_log_.cover_file_ = parse.GetItemInt("LogFileInfo", "FileMode", 1);
	log_config_.dir_log_.dir_err_ = parse.GetItemStr("LogFileInfo", "LogDir", "./log/err");
    log_config_.dir_log_.size_err_= parse.GetItemInt("LogFileInfo", "ERR_FileBlockSize", 10);
	log_config_.dir_log_.size_err_ *= 1024*1024;

    log_config_.dir_log_.dir_inf_ = parse.GetItemStr("LogFileInfo", "LogDir", "./log/inf");
    log_config_.dir_log_.size_inf_ = parse.GetItemInt("LogFileInfo", "INF_FileBlockSize", 30);
	log_config_.dir_log_.size_inf_*= 1024 * 1024;

    log_config_.dir_log_.dir_fat_ = parse.GetItemStr("LogFileInfo", "LogDir", "./log/fat");
    log_config_.dir_log_.size_fat_ = parse.GetItemInt("LogFileInfo", "FAT_FileBlockSize", 0.1);
	log_config_.dir_log_.size_fat_ *= 1024 * 1024;

    log_config_.dir_log_.dir_war_ = parse.GetItemStr("LogFileInfo", "LogDir", "./log/war");
    log_config_.dir_log_.size_war_ = parse.GetItemInt("LogFileInfo", "WAR_FileBlockSize", 20);
	log_config_.dir_log_.size_war_ *= 1024 * 1024;

	log_config_.core_set_.log_open_ = parse.GetItemInt("SystemSet", "LogOpen", 1);
	log_config_.core_set_.log_worker_= parse.GetItemInt("SystemSet", "LogWorker", 2);
}

str_t LogSetting::_formatPath(const std::string& path) {
	std::string  new_path(std::move(path));
#ifdef _WIN32
	std::for_each(new_path.begin(), new_path.end(), [](char& c) {
		if (c == '/')
			c = '\\';
	});
#endif
	return new_path;
}

bool LogSetting::mkDir(const str_t & dir)
{
	if (dir.empty())
		return false;
	str_t new_dir(std::move(dir));
	if (new_dir[new_dir.length()] == '/' || '\\')
		new_dir.resize(new_dir.size()- 1);
	

#ifdef WIN32
	str_t commond = "md " + _formatPath(dir);
	commond.append(">nul 2>nul");
	system(commond.c_str());

#else

	int pre = 0;
    for (int i = 0; i < dir.size(); ++i) {
        if (dir[i] == '/' || dir[i] == '\\') {
            str_t sub = dir.substr(0, i);
			mkdir(sub.c_str(), 0755);
		}
	}
    if (pre != dir.size())
        mkdir(dir.c_str(), 0755);
#endif
	return true;
}

void LogSetting::init() {
	int log_mod = 0;

	if (log_config_.core_set_.log_worker_&(int)LOG_MOD::LOG_MOD_PRINT_CONSL)
		log_mod |= (int)LOG_MOD::LOG_MOD_PRINT_CONSL;
	if(log_config_.core_set_.log_worker_&(int)LOG_MOD::LOG_MOD_PRINT_FILE)
		log_mod |= (int)LOG_MOD::LOG_MOD_PRINT_FILE;
	if (log_config_.core_set_.log_worker_&(int)LOG_MOD::LOG_MOD_PRINT_SOCK)
		log_mod |= (int)LOG_MOD::LOG_MOD_PRINT_SOCK;
	PrintMgr::instance().setLogMode(log_mod);
}


str_t LogSetting::getDateTimeStr(time_t time) {

    auto cur = std::chrono::system_clock::now();
	if (!time) {
        auto cur = std::chrono::system_clock::now();;
        time= std::chrono::system_clock::to_time_t(cur);
	}
    std::tm t = *std::localtime(&time);
	time_t not_millsecond = mktime(&t);
	auto cur_not_millsecond = std::chrono::system_clock::from_time_t(not_millsecond);
    char buf[32]={0};
	int cnt = sprintf(buf, "%02d/%02d/%02d %02d:%02d:%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
	sprintf(buf + cnt, ".%03lld", (cur.time_since_epoch().count() - cur_not_millsecond.time_since_epoch().count()) / 10000);
	return buf;
}

str_t LogSetting::getDate(time_t time ) {

	if (!time) {
        auto cur = std::chrono::system_clock::now();
        time = std::chrono::system_clock::to_time_t(cur);
	}
    std::tm t = *std::localtime(&time);
	char buf[32];
	sprintf(buf, "%02d-%02d-%02d ", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
	return buf;
}
