#include "LogPrintFile.h"
#include "LogSet.h"
PrintFile::PrintFile()
{

	_createLogFile();
	std::thread(&PrintFile::_print, this).detach();
}

PrintFile::~PrintFile()
{

	exit_.store(true);
	cv_.notify_one();
	while (1) {
		if (!exit_)
			break;
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}

	for (int i = 0; i < FILE_CNT; ++i) {
		if (GET_FILE(ary_file_[i])) {
			GET_FILE(ary_file_[i]).flush();
			GET_FILE(ary_file_[i]).close();
		}
	}
}


void PrintFile::_createLogFile() {



	str_t info_dir = LogSetting::getInstance().getFileInfo().dir_inf_ + "/info";
	LogSetting::getInstance().getFileInfo().dir_inf_ = info_dir;
	if (!LogSetting::getInstance().mkDir(info_dir)) {
		throw std::runtime_error("dir path:" + info_dir + " cant create ");
	}
	str_t info_path = info_dir + std::string("/info.log");


	GET_FILE_NAME(ary_file_[LOG_INF]) = info_path;

	//循环检测文件是否写满
NEXT_INFO_FILE:
	if (_fileFull(GET_FILE_NAME(ary_file_[LOG_INF]), LogSetting::getInstance().getFileInfo().size_inf_)) {
		_createnextFileName(GET_FILE_NAME(ary_file_[LOG_INF]));
		goto NEXT_INFO_FILE;
	}
	else {
		GET_FILE(ary_file_[LOG_INF]) = std::ofstream(GET_FILE_NAME(ary_file_[LOG_INF]), std::ios::out | std::ios::app);
	}


	str_t err_dir = LogSetting::getInstance().getFileInfo().dir_err_ + "/error";
	LogSetting::getInstance().getFileInfo().dir_err_ = err_dir;
	if (!LogSetting::getInstance().mkDir(err_dir)) {
		throw std::runtime_error(err_dir + "cant create");
	}
	str_t err_path = err_dir + "/error.log";
	GET_FILE_NAME(ary_file_[LOG_ERR]) = err_path;
		//循环检测文件是否写满
	NEXT_ERR_FILE:
	if (_fileFull(GET_FILE_NAME(ary_file_[LOG_ERR]), LogSetting::getInstance().getFileInfo().size_err_)) {
		_createnextFileName(GET_FILE_NAME(ary_file_[LOG_ERR]));
		goto NEXT_ERR_FILE;
	}
	else {
		GET_FILE(ary_file_[LOG_ERR])=std::ofstream(GET_FILE_NAME(ary_file_[LOG_ERR]), std::ios::out | std::ios::app);
	}



	str_t war_dir = LogSetting::getInstance().getFileInfo().dir_war_ + "/warning";
	LogSetting::getInstance().getFileInfo().dir_war_ = war_dir;
	if (!LogSetting::getInstance().mkDir(war_dir)) {
		throw std::runtime_error("dir : " + war_dir + " cant create");
	}
	str_t war_path = war_dir + "/warning.log";
	GET_FILE_NAME(ary_file_[LOG_WAR]) = war_path;

NEXT_WAR_FILE:
	if (_fileFull(GET_FILE_NAME(ary_file_[LOG_WAR]), LogSetting::getInstance().getFileInfo().size_war_)) {
		_createnextFileName(GET_FILE_NAME(ary_file_[LOG_WAR]));
		goto NEXT_WAR_FILE;
	}
	else {
		GET_FILE(ary_file_[LOG_WAR])=std::ofstream(GET_FILE_NAME(ary_file_[LOG_WAR]), std::ios::out | std::ios::app);
	}



	str_t fat_dir = LogSetting::getInstance().getFileInfo().dir_fat_ + "/fatal";
	LogSetting::getInstance().getFileInfo().dir_fat_ = fat_dir;
	if (!LogSetting::getInstance().mkDir(fat_dir)) {
		throw std::runtime_error(fat_dir + " cant create");
	}
	str_t fat_path = fat_dir + "/fatal.log";
	GET_FILE_NAME(ary_file_[LOG_FAT]) = fat_path;

NEXT_FAT_FILE:
	if (_fileFull(GET_FILE_NAME(ary_file_[LOG_FAT]), LogSetting::getInstance().getFileInfo().size_fat_)) {
		_createnextFileName(GET_FILE_NAME(ary_file_[LOG_FAT]));
		goto NEXT_FAT_FILE;
	}
	else {
		GET_FILE(ary_file_[LOG_FAT])=std::ofstream(GET_FILE_NAME(ary_file_[LOG_FAT]), std::ios::app | std::ios::out);
	}

}

void PrintFile::_print()
{
	while (1) {
		std::pair<LogPos, LogData> item;
		_getLogItem(item);
		if (exit_) {
			exit_.store(false);
			return;
		}
		else if (item.second.isEmpty())
			continue;
		pos_str_ = std::move(_setLogFormat(item));
		GET_FILE(ary_file_[item.second.level_]) << pos_str_ << item.second.data_.str() << "\n";
		if (!(++GET_WRITE_CNT(ary_file_[item.second.level_]) % 10)) {
			size_t size = 0;
			if ((LOG_LEVEL)item.second.level_ == LOG_LEVEL::LOG_INF)
				size = LogSetting::getInstance().getFileInfo().size_inf_;
			else if (LOG_LEVEL::LOG_ERR == (LOG_LEVEL)item.second.level_)
				size = LogSetting::getInstance().getFileInfo().size_err_;
			else if (LOG_LEVEL::LOG_WAR == (LOG_LEVEL)item.second.level_)
				size = LogSetting::getInstance().getFileInfo().size_war_;
			else if (LOG_LEVEL::LOG_FAT == (LOG_LEVEL)item.second.level_)
				size = LogSetting::getInstance().getFileInfo().size_fat_;

			if (_fileFull(GET_FILE_NAME(ary_file_[item.second.level_]), size)) {
				GET_FILE(ary_file_[item.second.level_]).close();
				_createnextFileName(GET_FILE_NAME(ary_file_[item.second.level_]));
				GET_FILE(ary_file_[item.second.level_]).open(GET_FILE_NAME(ary_file_[item.second.level_]), std::ios::out | std::ios::app);
			}
		}

	}
}

bool PrintFile::_fileFull(const file_name_t& name, const size_t& max_len) {

	if (name.empty())
		return false;
	FILE* file = fopen(name.c_str(), "ab+");
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	fclose(file);
	if (size >= max_len)
		return true;
	return false;
}

void PrintFile::_createnextFileName(file_name_t &pre_name)
{
	static std::string sufix = std::string(".log");
	auto pos = pre_name.find(sufix);
	if (pos == std::string::npos) {
		pre_name.append(".log");
	}
	else {
		pos += 4;
		if (pos == pre_name.size()) {
			pre_name.append("1");
		}
		else {
			str_t sub = std::string(pre_name.c_str() + pos, pre_name.size() - pos);
			int cnt = 0;
			sscanf(sub.c_str(), "%d", &cnt);
			cnt++;
			pos = pre_name.find(sufix);
			pos += 4;
			pre_name.resize(pos);
			pre_name.append(std::to_string(cnt));
		}
	}
}
