#ifndef _TINA_CORE_UTILS_EMAIL_ADDR_H_
#define _TINA_CORE_UTILS_EMAIL_ADDR_H_

#include<string>

namespace tina::core::utils {

    class email_addr {
        
        public:
            email_addr(){}
            email_addr( std::string __s_email );
            ~email_addr(){}

            bool store( std::string __s_email );
            bool is_ok();
            std::string localpart();
            std::string domain();
            std::string string();
            email_addr& operator=( std::string __s_email );

        private:
            std::string _s_localpart;
            std::string _s_domain;

    }; // class email_addr

} // namespace tina::core::utils

#endif // _TINA_CORE_UTILS_EMAIL_ADDR_H_
