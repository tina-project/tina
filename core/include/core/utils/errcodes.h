/**
 * This file is auto-generated by core/tools/update_utils_errcodes.py
 * 
 * DO NOT EDIT!!!
 * 
 * If something is wrong, please fix it in core/tools/templates/update_utils_errcodes/errcodes.h.in first
 */

#ifndef _TINA_CORE_UTILS_ERRCODES_H_
#define _TINA_CORE_UTILS_ERRCODES_H_

#include<string>
#include<unordered_map>

namespace tina::core::utils {

    class errcodes {

        public:
            errcodes() = delete;
            errcodes( const errcodes& ) = delete;
            ~errcodes() = delete;
            errcodes& operator=( const errcodes& ) = delete;

        public:
            typedef enum g3log_sink : int {
                G3LOG_SINK_LOGFILE_NOT_OPEN = 1,
            } g3log_sink;

            typedef enum sql : int {
                SQL_DB_NOT_OPEN = 1,
                SQL_ILLEGAL_DB = 2,
                SQL_CREATE_TABLE_FAILED = 3,
                SQL_WRITE_FORMAT_FAILED = 4,
                SQL_WRITE_HEADER_FAILED = 5,
                SQL_WRITE_HASH_FAILED = 6,
                SQL_WRITE_TAG_FAILED = 7,
                SQL_WRITE_CALENDAR_FAILED = 8,
                SQL_WRITE_NOTES_FAILED = 9,
                SQL_UNEXCEPTED = 255,
            } sql;

            typedef enum sql_cb : int {
                SQL_CB_ILLEGAL_ARGC = 1,
                SQL_CB_OVERWRITE_EXISTING_DATA = 2,
                SQL_CB_ROOT_HASH_REDEFINE = 3,
                SQL_CB_ILLEGAL_HASH_ID = 4,
                SQL_CB_ILLEGAL_HASH = 5,
                SQL_CB_UNEXCEPTED = 255,
            } sql_cb;

            static std::string get_name( g3log_sink __ec_code );
            static std::string get_name( sql __ec_code );
            static std::string get_name( sql_cb __ec_code );

        private:
            static const std::unordered_map<g3log_sink,std::string> _um_g3log_sink_namemap;
            static const std::unordered_map<sql,std::string> _um_sql_namemap;
            static const std::unordered_map<sql_cb,std::string> _um_sql_cb_namemap;

    }; // class errcodes

} // namespace tina::core::utils

#endif // _TINA_CORE_UTILS_ERRCODES_H_
