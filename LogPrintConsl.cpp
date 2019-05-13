#include "LogPrintConsl.h"



PrintConsl::PrintConsl()
{
#ifdef WIN32
    hand_conls_ = GetStdHandle(STD_OUTPUT_HANDLE);
    if(!hand_conls_)
        throw std::runtime_error(" wind handl get faild");
#endif
    std::thread(&PrintConsl::_print,this).detach();
}

PrintConsl::~PrintConsl()
{
    exit_.store(true);
    while(1){
		if (!exit_)
			break;
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}



void PrintConsl::_setLogColor(std::pair<LogPos,LogData>& data)
{
#ifdef WIN32
    switch (data.second.level_) {
    case LOG_LEVEL::LOG_INF:{
        SetConsoleTextAttribute(hand_conls_, FOREGROUND_GREEN);
        break;
    }
    case LOG_LEVEL::LOG_WAR:{
        SetConsoleTextAttribute(hand_conls_, FOREGROUND_GREEN|FOREGROUND_RED);
        break;
    }
    case LOG_LEVEL::LOG_ERR:{
        SetConsoleTextAttribute(hand_conls_, FOREGROUND_RED);
        break;
    }
    case LOG_LEVEL::LOG_FAT:{
        SetConsoleTextAttribute(hand_conls_, FOREGROUND_RED|BACKGROUND_BLUE);
        break;
    }
    }
#else
    std::stringstream ss;
    switch (data.second.level_) {
    case LOG_LEVEL::LOG_INF:{
        ss<<GREEN<<pos_str_;
        break;
    }
    case LOG_LEVEL::LOG_WAR:{
        ss<<YELLOW<<pos_str_;
        break;
    }
    case LOG_LEVEL::LOG_ERR:{
        ss<<RED<<pos_str_;
        break;
    }
    case LOG_LEVEL::LOG_FAT:{
        ss<<MAGENTA<<pos_str_;
        break;
    }
    }
    pos_str_=std::move(ss.str());
#endif
}




void PrintConsl::_print()
{
    while(1){
        std::pair<LogPos,LogData> item;
		_getLogItem(item);
		if (exit_) {
			exit_.store(false);
			return;
		}
        pos_str_=std::move(_setLogFormat(item));
        if(LogSetting::getInstance().getFormat().print_color_)
            _setLogColor(item);
#ifdef WIN32
        std::cout<<pos_str_<<item.second.data_.str()<<std::endl;
#else
        if(LogSetting::getInstance().getFormat().print_color_)
            std::cout<<pos_str_<<item.second.data_<<RESET<<std::endl;
        else
            std::cout<<pos_str_<<item.second.data_<<std::endl;
#endif

#ifdef WIN32
        if(LogSetting::getInstance().getFormat().print_color_&&hand_conls_)
            SetConsoleTextAttribute(hand_conls_, FOREGROUND_GREEN| FOREGROUND_BLUE| FOREGROUND_RED);
#endif

    }
}
