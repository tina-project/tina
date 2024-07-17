#ifndef _TINA_CORE_LOG_G3LOG_SINK_H_
#define _TINA_CORE_LOG_G3LOG_SINK_H_

#include<filesystem>
#include<memory>
#include<sstream>
#include<string>

#include"core/utils/exception.h"
#include"g3log/loglevels.hpp"
#include"g3log/logmessage.hpp"

namespace tina::core::log {

    DECLARE_EXCEPTION( g3log_sink_exception )

    class g3log_sink {

        public:
            g3log_sink( std::size_t __ull_refresh_after_x_msgs, unsigned int __ui_log_severity );
            ~g3log_sink();

            void write_log( g3::LogMessageMover __g3lmm_msg );

        protected:
            static std::string _log_details_to_string( const g3::LogMessage& __g3lm_msg );
            static LEVELS _parse_log_severity( unsigned int __ui_log_severity );

            inline std::ofstream& _filestream(){
                return *( this -> _up_ofs_wfile.get() );
            }

        private:
            g3::LogMessage::LogDetailsFunc _g3lmldf_log_details_func;
            std::filesystem::path _p_logfile_path;
            std::unique_ptr<std::ofstream> _up_ofs_wfile;
            std::ostringstream _oss_writebuf;
            std::size_t _ull_write_counter;
            std::size_t _ull_refresh_after_x_msgs;
            LEVELS _ls_log_severity;

            g3log_sink( const g3log_sink& ) = delete;
            g3log_sink& operator=( const g3log_sink& ) = delete;

    }; // class g3log_sink

} // namespace tina::core::log

#endif // _TINA_CORE_LOG_G3LOG_SINK_H_
