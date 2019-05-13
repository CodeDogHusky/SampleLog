#pragma once
#include <array>
#include <thread>
#include <fstream>
#include "LogDefine.h"
#include "LogPrintBase.h"
#define FILE_CNT 5
typedef std::string file_name_t;
typedef size_t      write_cnt_t;
typedef std::array<std::tuple<file_name_t,std::ofstream,write_cnt_t>, FILE_CNT> ary_log_t;
#define GET_WRITE_CNT(tp) std::get<2>(tp)
#define GET_FILE_NAME(tp) std::get<0>(tp)
#define GET_FILE(tp)      std::get<1>(tp)
class PrintFile:public PrintBase{
public:
    PrintFile();
    ~PrintFile();

protected:
    void _print();
    void _createLogFile();
    bool _fileFull(const file_name_t& file_name, const size_t &max_len);
    void _createnextFileName(file_name_t& pre_name);
private:
	ary_log_t ary_file_;
	str_t     pos_str_;
};
