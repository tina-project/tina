#ifndef _TINA_CORE_UTILS_TIME_H_
#define _TINA_CORE_UTILS_TIME_H_

#include<ctime>

namespace tina::core::utils {

    class time {

        public:
            time() = delete;
            time( const time& ) = delete;
            ~time() = delete;
            time& operator=( const time& ) = delete;

        public:
            typedef struct timediff {
                short hour;
                short min;
            } timediff;

            static std::time_t get_ts();
            static std::tm* get_local_tm();
            static std::tm* get_utc_tm();
            static timediff get_local_timediff();

    }; // class time

} // namespace tina::core::utils

#endif // _TINA_CORE_UTILS_TIME_H_
