#include"core/utils/exception.h"

#include<sstream>

namespace utils = tina::core::utils;

utils::exception::exception( int __i_errno , const char* __p_c_errmsg ){
    std::ostringstream oss;
    oss << "[" << __i_errno << "] " << __p_c_errmsg;
    this -> _s_errmsg = oss.str();
    return;
}

utils::exception::exception( int __i_errno , const std::string& __s_errmsg ){
    std::ostringstream oss;
    oss << "[" << __i_errno << "] " << __s_errmsg;
    this -> _s_errmsg = oss.str();
    return;
}

const char* utils::exception::what() const noexcept {
    return this -> _s_errmsg.c_str();
}
