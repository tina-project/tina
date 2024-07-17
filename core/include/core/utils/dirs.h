#ifndef _TINA_CORE_UTILS_FOLDERS_H_
#define _TINA_CORE_UTILS_FOLDERS_H_

#include<filesystem>

namespace tina::core::utils {

    class dirs {

        public:
            dirs() = delete;
            dirs( const dirs& ) = delete;
            ~dirs() = delete;
            dirs& operator=( const dirs& ) = delete;

            static std::filesystem::path get_user_home_dir();
            static std::filesystem::path get_tina_data_dir();
            static std::filesystem::path get_tina_log_dir();

    }; // class dirs

} // namespace tina::core::utils

#endif // _TINA_CORE_UTILS_FOLDERS_H_
