#include"core/utils/time.h"

#include<chrono>

namespace utils = tina::core::utils;

std::time_t utils::time::get_ts(){
    return std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
}

std::tm* utils::time::get_local_tm(){
    std::time_t ts = get_ts();
    return std::localtime( &ts );
}

std::tm* utils::time::get_utc_tm(){
    std::time_t ts = get_ts();
    return std::gmtime( &ts );
}

/**
 * Get local timediff to UTC.
 */
utils::time::timediff utils::time::get_local_timediff(){
    std::tm* local_tm = get_local_tm();
    return { static_cast<short>( local_tm -> tm_gmtoff / 3600 ) , static_cast<short>( local_tm -> tm_gmtoff % 3600 ) };
}
