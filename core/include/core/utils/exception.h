#ifndef _TINA_CORE_UTILS_EXCEPTION_H_
#define _TINA_CORE_UTILS_EXCEPTION_H_

#include<exception>
#include<string>

#define DECLARE_EXCEPTION( sub_exception )                                  \
    class sub_exception final : public tina::core::utils::exception {       \
                                                                            \
        public:                                                             \
            sub_exception( int __i_errno , const char* __p_c_errmsg )       \
                : exception( __i_errno , __p_c_errmsg ){};                  \
            sub_exception( int __i_errno , const std::string& __s_errmsg )  \
                : exception( __i_errno , __s_errmsg ){};                    \
                                                                            \
    };

namespace tina::core::utils {

    class exception : public std::exception {

        public:
            exception( int __i_errno , const char* __p_c_errmsg );
            exception( int __i_errno , const std::string& __s_errmsg );

            const char* what() const noexcept override;

        private:
            std::string _s_errmsg;

    }; // class exception

} // namespace tina::core::utils

#endif // _TINA_CORE_UTILS_EXCEPTION_H_
