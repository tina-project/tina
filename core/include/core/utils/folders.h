#ifndef _TINA_CORE_UTILS_FOLDERS_H_
#define _TINA_CORE_UTILS_FOLDERS_H_

#include<filesystem>

namespace tina::core::utils {

    class folders {

        public:
            static std::filesystem::path get_user_home_folder();
            static std::filesystem::path get_tina_data_folder();

    }; // class folders

} // namespace tina::core::utils

#endif // _TINA_CORE_UTILS_FOLDERS_H_
