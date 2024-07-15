#include"g3log_sink.h"
#include"core/utils/errcodes.h"
#include"core/utils/folders.h"
#include"filesinkhelper.ipp"

#include<cstring>

#define LOG_PREFIX      "ExpenseTracker"
#define LOG_TIMEFORMAT  "%Y-%m-%d %H:%M:%S.%f3"

namespace log = tina::core::log;
namespace fs = std::filesystem;

log::g3log_sink::g3log_sink( std::size_t __ull_refresh_after_x_msgs, unsigned int __ui_log_severity ) :
    _g3lmldf_log_details_func( &( this -> _log_details_to_string ) ) ,
    _p_logfile_path( this -> _get_log_folder() ) ,
    _up_ofs_wfile( new std::ofstream ) ,
    _ull_write_counter( 0 ) ,
    _ull_refresh_after_x_msgs( __ull_refresh_after_x_msgs ) ,
    _ls_log_severity( this -> _parse_log_severity( __ui_log_severity ) )
{
    if ( !fs::exists( this -> _p_logfile_path ) )
    {
        fs::create_directories( this -> _p_logfile_path );
        // here: _p_logfile_path is still the log folder path
    }
    std::string logfile_name = g3::internal::createLogFileName( LOG_PREFIX , "" );
    this -> _p_logfile_path /= logfile_name;
    this -> _up_ofs_wfile -> open( this -> _p_logfile_path , std::ios::out | std::ios::trunc );
    if ( !( this -> _up_ofs_wfile -> is_open() ) )
    {
        this -> _up_ofs_wfile.reset();
        throw g3log_sink_exception( utils::errcodes::g3log_sink::G3LOG_SINK_LOGFILE_NOT_OPEN , strerror( errno ) );
    }
    return;
}

log::g3log_sink::~g3log_sink(){
    std::ostringstream oss;
    oss << "\nlogger sink shutdown at: ";
    auto now = std::chrono::system_clock::now();
    oss << g3::localtime_formatted( now , LOG_TIMEFORMAT ) << "\n";
    this -> _filestream() << this -> _oss_writebuf.str() << oss.str() << std::flush;
    // write exit msg to log
    return;
}

void log::g3log_sink::write_log( g3::LogMessageMover __g3lmm_msg ){
    if ( __g3lmm_msg.get()._level.value < this -> _ls_log_severity.value )
    {
        return;
    } // no need to log due to low log msg severity
    std::string data = __g3lmm_msg.get().toString( this -> _g3lmldf_log_details_func );
    this -> _oss_writebuf << data;
    this -> _ull_write_counter++;
    if ( this -> _ull_write_counter % this -> _ull_refresh_after_x_msgs == 0 )
    {
        this -> _filestream() << this -> _oss_writebuf.str() << std::flush;
        this -> _oss_writebuf.str( "" );
    } // write all msgs in buf to log file; refresh buf
    return;
}

fs::path log::g3log_sink::_get_log_folder(){
    return utils::folders::get_tina_data_folder() / "log";
}

std::string log::g3log_sink::_log_details_to_string( const g3::LogMessage& __g3lm_msg ){
    std::ostringstream oss;
    oss << "[" << __g3lm_msg.timestamp( LOG_TIMEFORMAT ) << "] "
        << __g3lm_msg.level() << ( __g3lm_msg._level.value == g3::kWarningValue ? "\t" : "\t\t" )
        << std::left << std::setw( 64 )
        << "(" << __g3lm_msg.threadID() << ": " << __g3lm_msg.file() << " -> "
        << __g3lm_msg.function() << ":" << __g3lm_msg.line() << ")";
    return oss.str();
}

LEVELS log::g3log_sink::_parse_log_severity( unsigned int __ui_log_severity ){
    switch ( __ui_log_severity )
    {
        case 0: return DEBUG;   break;
        case 1: return INFO;    break;
        case 2: return WARNING; break;
        case 3: return FATAL;   break;
        default: return INFO;   break;
    }
    return INFO;
}
