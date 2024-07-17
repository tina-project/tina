#include"sql_callbacks.h"

#include<string>

namespace callbacks = tina::core::sql::callbacks;

int callbacks::sqlcb_get_table_length( void* __p_v_data , int __i_argc , char** __pp_c_argv , char** __pp_c_azcolname ){
    int* len = ( int* ) __p_v_data;
    if ( __i_argc != 1 )
    {
        return 1;
    } // wrong argc, stop
    *len = std::stoi( __pp_c_argv[0] );
    // this shouldn't throw exception: this callback should only be used with `SELECT COUNT(*) FROM {TABLE}` cmd
    return 0;
}
