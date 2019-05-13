#pragma once
#include <iostream>
#include <string>
#include <deque>
#include <thread>
#include <mutex>
#include <type_traits>
#include <typeinfo>
#include <atomic>
typedef std::lock_guard<std::mutex> locker_t;

//this class is thread salf
template <typename OBJ>
class ObjPool{
public:
    ObjPool(){
	}

    ~ObjPool(){
		exit_.store(true);
        locker_t lock(mtx_);
        for(auto it:que_obj_){
            auto obj=dynamic_cast<OBJ*>(it);
            if(obj)
                delete obj;
        }
    }

    void pushObj(OBJ* obj){
        if(!obj)return;
        locker_t lock(mtx_);
		if (que_obj_.size() < 10)
			que_obj_.push_back(obj);
		else
			delete obj;
        
    }

    OBJ* getObj(){
        OBJ* ret_obj=nullptr;
        {
            locker_t lock(mtx_);
            if(!que_obj_.empty()){
				ret_obj =que_obj_.front();
                que_obj_.pop_front();
            }
        }
        return ret_obj;
    }
private:
	std::atomic_bool exit_;
    std::mutex       mtx_;
    std::deque<OBJ*> que_obj_;
};

