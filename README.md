# SampleLog
基于C++11 的一款轻量级的日志库,同时支持Ubuntu与Windows

Example:

LogWorker::setConfigPath("../Mod_Log/config.ini");
        int cnt =0;
        
	while (1) {
		LOG_INF << "HELLO "<<++cnt<<LOG_END;
		LOG_ERR << "this is error msg" <<++cnt<< LOG_END;
		LOG_FAT << "this is fatal msg" << ++cnt<<LOG_END;
		LOG_WAR << "this is warning msg" << ++cnt<<LOG_END;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		if (!(++cnt % 10000000))
			break;
	}
	getchar();
  
