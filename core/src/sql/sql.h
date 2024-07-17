#ifndef _TINA_CORE_SQL_SQL_H_
#define _TINA_CORE_SQL_SQL_H_

#include<filesystem>
#include<string>

#include"sql_types.h"
#include"core/utils/exception.h"
#include"sqlite3.h"

namespace tina::core::sql {

    DECLARE_EXCEPTION( sql_exception )

    class sql {

        public:
            sql( const std::filesystem::path& __p_dbpath );
            ~sql();

        protected:
            typedef enum _tablename_t {
                NONE        = 0,
                // the nums are also used for identifying which table creation went wrong by `_create_tables`
                // therefore 0 should be retained, it is returned if success.
                FORMAT      = 1,
                HEADER      = 2,
                HASH        = 3,
                TAG         = 4,
                CALENDAR    = 5,
                NOTES       = 6,
            } _tablename_t;

        protected:
            int _create_tables();
            int _check_db_legality();
            int _cleanup_calendar();

            int _get_table_len( _tablename_t __e_table );
            int _exec_sqlcmd( const std::string& __s_cmd );
            int _exec_sqlcmd( const std::string& __s_cmd , const callback_data_t& __scd_cb , void* __p_v_data );

        private:
            std::filesystem::path _p_dbpath;
            sqlite3* _p_sql3_dbobj;
            std::string _s_errmsg;

            int _write_format();

    }; // class sql

} // namespace tina::core::sql

#endif // _TINA_CORE_SQL_SQL_H_
