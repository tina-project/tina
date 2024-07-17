#ifndef _TINA_CORE_SQL_CALLBACKS_SQL_CALLBACKS_H_
#define _TINA_CORE_SQL_CALLBACKS_SQL_CALLBACKS_H_

#define SQL_CB( cb ) { &cb , #cb }

namespace tina::core::sql::callbacks {

    int sqlcb_get_table_length( void* __p_v_data , int __i_argc , char** __pp_c_argv , char** __pp_c_azcolname );

} // namespace tina::core::sql::callbacks

#endif // _TINA_CORE_SQL_CALLBACKS_SQL_CALLBACKS_H_
