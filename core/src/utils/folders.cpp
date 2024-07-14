#include"core/utils/folders.h"

#ifdef __linux__
#include<pwd.h>
#include<unistd.h>
#include<sys/types.h>
#endif // __linux__

namespace utils = tina::core::utils;
namespace fs = std::filesystem;

fs::path utils::folders::get_user_home_folder(){

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

fs::path utils::folders::get_tina_data_folder(){
    return get_user_home_folder() / ".tina";
}
