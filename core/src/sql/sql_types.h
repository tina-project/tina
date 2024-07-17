#ifndef _TINA_CORE_SQL_SQL_TYPES_H_
#define _TINA_CORE_SQL_SQL_TYPES_H_

#include<string>

namespace tina::core::sql {

    typedef int ( *sql_callback )( void* , int , char** , char** );

    typedef struct {
        sql_callback p_fu_cb;
        std::string s_cbname;
    } callback_data_t;

} // namespace tina::core::sql

#endif // _TINA_CORE_SQL_SQL_TYPES_H_
