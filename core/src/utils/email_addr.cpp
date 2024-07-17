#include"core/utils/email_addr.h"

#include<algorithm>

namespace utils = tina::core::utils;

utils::email_addr::email_addr( std::string __s_email ){
    this -> store( __s_email );
    return;
}

bool utils::email_addr::store( std::string __s_email ){
    this -> _s_localpart.clear();
    this -> _s_domain.clear();

    if ( ( std::count( __s_email.begin() , __s_email.end(), '@' ) != 1 ) || 
         ( __s_email.front() == '@' ) ||
         ( __s_email.back() == '@' ) ||
         ( __s_email.front() == '.' )  ||
         ( __s_email.back() == '.' ) ||
         ( __s_email.find( ".." ) != std::string::npos ) )
    {
        return false;
    }

    this -> _s_localpart = __s_email.substr( 0 , __s_email.find( '@' ) );
    this -> _s_domain = __s_email.substr( __s_email.find( '@' ) + 1 );
    return true;
}

bool utils::email_addr::is_ok(){
    return !( this -> _s_localpart.empty() || this -> _s_domain.empty() );
}

std::string utils::email_addr::localpart(){
    return this -> _s_localpart;
}

std::string utils::email_addr::domain(){
    return this -> _s_domain;
}

std::string utils::email_addr::string(){
    return this -> _s_localpart + "@" + this -> _s_domain;
}

utils::email_addr& utils::email_addr::operator=( std::string __s_email ){
    this -> store( __s_email );
    return *this;
}
