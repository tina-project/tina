#include"core/utils/dirs.h"

#ifdef __linux__
#include<pwd.h>
#include<unistd.h>
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

fs::path utils::dirs::get_user_home_dir(){

#ifdef __linux__

    struct passwd* pw = getpwuid( getuid() );
    if ( pw )
    {
        return fs::path( pw -> pw_dir );
    }
    else
    {
        return fs::path();
        // get user home folder failed, return an empty path
    }

#endif // __linux__

}

fs::path utils::dirs::get_tina_data_dir(){
    fs::path data_folder = get_user_home_dir() / ".tina";
    fs::create_directories( data_folder );
    return data_folder;
}

fs::path utils::dirs::get_tina_log_dir(){
    fs::path log_folder = get_tina_data_dir() / "log";
    fs::create_directories( log_folder );
    return log_folder;
}
