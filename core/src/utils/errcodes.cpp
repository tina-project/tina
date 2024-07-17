/**
 * This file is auto-generated by core/tools/update_utils_errcodes.py
 * 
 * DO NOT EDIT!!!
 * 
 * If something is wrong, please fix it in core/tools/templates/update_utils_errcodes/errcodes.cpp.in first
 */

#include"core/utils/errcodes.h"

#include<stdexcept>

namespace utils = tina::core::utils;

std::string utils::errcodes::get_name( utils::errcodes::g3log_sink __ec_code ){
    try {
        return _um_g3log_sink_namemap.at( __ec_code );
    }
    catch ( std::out_of_range )
    {
        return "";
    }
}

std::string utils::errcodes::get_name( utils::errcodes::sql __ec_code ){
    try {
        return _um_sql_namemap.at( __ec_code );
    }
    catch ( std::out_of_range )
    {
        return "";
    }
}

const std::unordered_map<utils::errcodes::g3log_sink,std::string> utils::errcodes::_um_g3log_sink_namemap = {
    { G3LOG_SINK_LOGFILE_NOT_OPEN , "G3LOG_SINK_LOGFILE_NOT_OPEN" },
};

const std::unordered_map<utils::errcodes::sql,std::string> utils::errcodes::_um_sql_namemap = {
    { SQL_DB_NOT_OPEN , "SQL_DB_NOT_OPEN" },
    { SQL_ILLEGAL_DB , "SQL_ILLEGAL_DB" },
    { SQL_CREATE_TABLE_FAILED , "SQL_CREATE_TABLE_FAILED" },
    { SQL_UNEXCEPTED , "SQL_UNEXCEPTED" },
};

