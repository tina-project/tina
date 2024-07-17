#include"core/utils/exe_infos.h"

#ifdef __linux__
#include<unistd.h>
#include<linux/limits.h>
#include<sys/types.h>
#endif // __linux__

// ************************************
// TODO: Windows & MacOS implementation
// ************************************

// ********************
// TODO: error handling
// ********************

namespace utils = tina::core::utils;
namespace fs = std::filesystem;

fs::path utils::exe_infos::get_exe_fullpath(){

#ifdef __linux__

    char buf[PATH_MAX];
    ssize_t len = readlink( "/proc/self/exe" , buf , sizeof( buf ) - 1 );
    if ( len != -1 )
    {
        buf[len] = '\0';
        return fs::path( buf );
    }
    else
    {
        return fs::path();
        // get exe fullpath failed, return an empty path
    }

#endif // __linux__

}

fs::path utils::exe_infos::get_exe_filename(){
    return get_exe_fullpath().stem();
}

fs::path utils::exe_infos::get_exe_dir(){
    return get_exe_fullpath().parent_path();
}
