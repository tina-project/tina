#ifndef _TINA_CORE_LOG_LOG_H_
#define _TINA_CORE_LOG_LOG_H_

#include<memory>

#include"g3log/g3log.hpp"
#include"g3log/logworker.hpp"

// *************************************
// TODO: LOG_E, LOG_IF_E & CHECK_E macro
// *************************************

namespace tina::core::log {

    class log {

        public:
            static void start_global_logger();
            static void stop_global_logger();

        private:
            static std::unique_ptr<g3::LogWorker> _up_g3lw_worker;

    }; // class log

} // namespace tina::core::log

#endif // _TINA_CORE_LOG_LOG_H_
