#ifndef _TINA_CORE_UTILS_EXE_INFOS_H_
#define _TINA_CORE_UTILS_EXE_INFOS_H_

#include<filesystem>

namespace tina::core::utils {

    class exe_infos {

        public:
            exe_infos() = delete;
            exe_infos( const exe_infos& ) = delete;
            ~exe_infos() = delete;
            exe_infos& operator=( const exe_infos& ) = delete;

            static std::filesystem::path get_exe_fullpath();
            static std::filesystem::path get_exe_filename();
            static std::filesystem::path get_exe_dir();

    }; // class executable_infos

} // namespace tina::core::utils

#endif // _TINA_CORE_UTILS_EXE_INFOS_H_
