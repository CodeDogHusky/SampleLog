#pragma once
#include "LogDefine.h"
#include <algorithm>
#include <fstream>
#ifdef WIN32
#include <windows.h>
#include <io.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif



typedef struct LogCoreSet {
	bool log_open_ = true;
	int  log_worker_ = 2;
}LogCoreSet;
typedef struct LogFormat{
    bool print_color_=true;
    bool print_fun_=true;
    bool print_file_=false;
    bool print_line_=true;
	bool print_time_ = true;
}PrintFormat;

typedef struct LogFileInfo {




	bool  cover_file_;       
    str_t  dir_war_;
    size_t size_war_;
    str_t  dir_inf_;
    size_t size_inf_;
    str_t  dir_err_;
    size_t size_err_;
    str_t  dir_fat_;
    size_t size_fat_;
}FileDir;
typedef struct Config {

	PrintFormat print_format_;
	FileDir     dir_log_;
	LogCoreSet  core_set_;
}Config;

class LogSetting{
public:
    static LogSetting& getInstance(){
        static LogSetting setting;
        return setting;
    }
    ~LogSetting();

public:
    inline const PrintFormat& getFormat()const{return log_config_.print_format_;}
    inline FileDir            getFileInfo(){ return log_config_.dir_log_; }
    inline bool               openColor()const{return log_config_.print_format_.print_color_;}

public:
    void               setConfigPath(const str_t& path);
	void               init();
	bool               mkDir(const str_t& dir);
	static str_t       getDateTimeStr(time_t time=0);
	static str_t       getDate(time_t time=0);

protected:
	LogSetting();
	void               _parseConfig(const std::string& path);
	str_t              _formatPath(const std::string& path);

private:
	Config             log_config_;

};



class IniParse {
public:
	IniParse(const std::string& file_path) :file_path_(file_path) {}
	~IniParse() {}

	std::string GetItemStr(const std::string& object_name, const std::string& itm_name, const std::string& defaults = "") {
		std::ifstream in(file_path_, std::ios::in);
		if (!in) 
			return defaults;
		std::string line;
		line.resize(1024 * 10);
		std::string root = "[" + object_name + "]";

		while (!in.eof())
		{
			in.getline((char*)line.c_str(), sizeof(char) * 1024 * 10);
			if (line.find(root) == std::string::npos) {//Ã»ÓÐÏî
				continue;
			}
			else {
				while (!in.eof()) {
					in.getline((char*)line.c_str(), sizeof(char) * 1024 * 10);
					if (line.find(itm_name) != std::string::npos) {
					re_detect:
						size_t pos = line.find(';');
						if (pos == std::string::npos) {
							size_t pos1 = line.find('=');
							if ( pos1!= std::string::npos) {
								return line.substr(pos1 + 1, line.size() - pos1);
							}
							else
								return defaults;
						}
						else
						{
							for (size_t i = pos - 1; i > 0; --i) {
								if (line[i] != ' ') {
									line = line.substr(0, i+1);
									goto re_detect;
								}
							}
						}
					}
				}
				return defaults;
			}
		}
		
		return defaults;
	}

	int GetItemInt(const std::string& object_name, const std::string& itm_name, const int& defaults) {
		return atoi(GetItemStr(object_name, itm_name, std::to_string(defaults)).c_str());
	}




	float GetItemFloat(const std::string& object_name, const std::string& itm_name, const std::string& defaults)
	{
		return atof(GetItemStr(object_name, itm_name, defaults).c_str());
	}


private:
	std::string file_path_;

};
