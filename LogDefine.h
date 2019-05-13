#pragma once
#include <iostream>
#include <string>
#include <mutex>
#define LOG_END  "LogEnd"

#ifdef WIN32

#ifndef EXPORT_CLASS
#define EXPORT_CLASS __declspec(dllexport)
#else
#define EXPORT_CLASS __declspec(dllimport)
#endif

#else

#define  EXPORT_CLASS

#endif


enum LOG_LEVEL{
    LOG_INF  =0,
    LOG_WAR  =1,
    LOG_ERR  =2,
    LOG_FAT  =3,
};

enum class LOG_MOD{
    LOG_MOD_PRINT_FILE  =1,
    LOG_MOD_PRINT_CONSL =2,
    LOG_MOD_PRINT_SOCK  =4,
};



typedef std::mutex                  mtx_t;
typedef std::unique_lock<mtx_t>     un_lock_t;
typedef std::lock_guard<mtx_t>      locker_t;
typedef std::string                 str_t;
typedef std::stringstream           ss_t;


