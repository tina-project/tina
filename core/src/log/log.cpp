#include"core/log/log.h"
#include"g3log_sink.h"

namespace log = tina::core::log;

std::unique_ptr<g3::LogWorker> log::log::_up_g3lw_worker = nullptr;

void log::log::start_global_logger(){
    if ( _up_g3lw_worker )
    {
        return;
    } // logger has already been inited;
    _up_g3lw_worker = g3::LogWorker::createLogWorker();
    _up_g3lw_worker -> addSink( std::make_unique<g3log_sink>( 20 , 1 ) , &g3log_sink::write_log );
    // *****************************************
    // TODO: datas above should be set in config
    // *****************************************
    g3::initializeLogging( _up_g3lw_worker.get() );
    return;
}

void log::log::stop_global_logger(){
    g3::internal::shutDownLogging();
    return;
}
