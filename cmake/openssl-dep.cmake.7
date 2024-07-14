# openssl-dep <https://github.com/tina-project/openssl-dep>
# A CMake module that downloads OpenSSL source code & compiles it with your project.
#
# Check the README & the LICENSE file under the project for details.

if(NOT UNIX)
    message(FATAL "This module can only be run on a unix-like system.")
endif()

message(STATUS "Fetching OpenSSL Package...")

if(NOT DEFINED OPENSSL_DEP_NOT_USE_OPENSSL3)
    set(TARGET_PACKAGE_VERSION "3.3.1")
    set(TARGET_PACKAGE_URL "https://github.com/openssl/openssl/releases/download/openssl-3.3.1/openssl-3.3.1.tar.gz")
    set(TARGET_PACKAGE_SHA256 "777cd596284c883375a2a7a11bf5d2786fc5413255efab20c50d6ffe6d020b7e")
else()
    set(TARGET_PACKAGE_VERSION "1.1.1w")
    set(TARGET_PACKAGE_URL "https://github.com/openssl/openssl/releases/download/OpenSSL_1_1_1w/openssl-1.1.1w.tar.gz")
    set(TARGET_PACKAGE_SHA256 "cf3098950cb4d853ad95c0841f1f9c6d3dc102dccfcacd521d93925208b76ac8")
endif()

set(TARGET_DOWNLOAD_DIR "${CMAKE_SOURCE_DIR}/_deps")
set(TARGET_EXTRACT_DIR "${TARGET_DOWNLOAD_DIR}/openssl-${TARGET_PACKAGE_VERSION}")
set(TARGET_DOWNLOAD_FILE "${TARGET_DOWNLOAD_DIR}/openssl-${TARGET_PACKAGE_VERSION}.tar.gz")

message(STATUS "OpenSSL Package Version: ${TARGET_PACKAGE_VERSION}")
message(STATUS "OpenSSL Package Download Directory: ${TARGET_DOWNLOAD_DIR}")
message(STATUS "OpenSSL Package Extract Directory: ${TARGET_EXTRACT_DIR}")

file(MAKE_DIRECTORY ${TARGET_DOWNLOAD_DIR})
file(DOWNLOAD ${TARGET_PACKAGE_URL} ${TARGET_DOWNLOAD_FILE}
    EXPECTED_HASH SHA256=${TARGET_PACKAGE_SHA256}
    SHOW_PROGRESS
)

message(STATUS "Extracting OpenSSL Package...")
execute_process(
    COMMAND tar -zxvf openssl-${TARGET_PACKAGE_VERSION}.tar.gz
    WORKING_DIRECTORY ${TARGET_DOWNLOAD_DIR}
    OUTPUT_QUIET
)

set(OPENSSL_DEP_ROOT_DIR "${CMAKE_SOURCE_DIR}/_deps/openssl")
set(OPENSSL_DEP_INCLUDE_DIR "${OPENSSL_DEP_ROOT_DIR}/include")
set(OPENSSL_DEP_LIB_DIR "${OPENSSL_DEP_ROOT_DIR}/lib")
set(OPENSSL_DEP_LIBCRYPTO_PATH "${OPENSSL_DEP_LIB_DIR}/libcrypto.so")
set(OPENSSL_DEP_LIBSSL_PATH "${OPENSSL_DEP_LIB_DIR}/libssl.so")

message(STATUS "Configuring OpenSSL Package...")
execute_process(
    COMMAND ./config --prefix=${OPENSSL_DEP_ROOT_DIR} no-apps no-docs no-tests
    WORKING_DIRECTORY ${TARGET_EXTRACT_DIR}
)

add_custom_command(
    OUTPUT ${OPENSSL_DEP_LIBCRYPTO_PATH}
    COMMAND make install
    WORKING_DIRECTORY ${TARGET_EXTRACT_DIR}
    COMMENT "Compiling OpenSSL"
)
add_custom_target(compile_openssl
    DEPENDS ${OPENSSL_DEP_LIBCRYPTO_PATH}
    WORKING_DIRECTORY ${TARGET_EXTRACT_DIR}
    COMMENT "Compiling OpenSSL"
    VERBATIM
)

unset(TARGET_PACKAGE_VERSION)
unset(TARGET_PACKAGE_URL)
unset(TARGET_PACKAGE_SHA256)
unset(TARGET_DOWNLOAD_DIR)
unset(TARGET_EXTRACT_DIR)
unset(TARGET_DOWNLOAD_FILE)
