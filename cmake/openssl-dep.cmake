# openssl-dep <https://github.com/tina-project/openssl-dep>
# A CMake module that adds OpenSSL v3 dependency to your project.
#
# Check the README & the LICENSE file under the project for details.

if(UNIX)

    message(STATUS "Fetching OpenSSL Package...")

    set(TARGET_PACKAGE_VERSION "3.3.1")
    set(TARGET_PACKAGE_URL "https://github.com/openssl/openssl/releases/download/openssl-3.3.1/openssl-3.3.1.tar.gz")
    set(TARGET_PACKAGE_SHA256 "777cd596284c883375a2a7a11bf5d2786fc5413255efab20c50d6ffe6d020b7e")

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
    set(OPENSSL_DEP_LIB_DIR "${OPENSSL_DEP_ROOT_DIR}/lib64")
    set(OPENSSL_DEP_LIBCRYPTO_PATH "${OPENSSL_DEP_LIB_DIR}/libcrypto.so")
    set(OPENSSL_DEP_LIBSSL_PATH "${OPENSSL_DEP_LIB_DIR}/libssl.so")

    message(STATUS "Configuring OpenSSL Package...")
    execute_process(
        COMMAND ./config --prefix=${OPENSSL_DEP_ROOT_DIR} no-apps no-docs no-tests
        WORKING_DIRECTORY ${TARGET_EXTRACT_DIR}
    )

    if(NOT DEFINED OPENSSL_DEP_COMPILE_MULTITHREAD_NUM)
        set(COMPILE_COMMAND "make")
    else()
        message(STATUS "Enable multi-thread compilation: ${OPENSSL_DEP_COMPILE_MULTITHREAD_NUM}")
        set(COMPILE_COMMAND "make -j${OPENSSL_DEP_COMPILE_MULTITHREAD_NUM}")
    endif()

    add_custom_command(
        OUTPUT ${OPENSSL_DEP_LIBCRYPTO_PATH}
        COMMAND make clean
        COMMAND /bin/bash -c ${COMPILE_COMMAND}
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
    unset(COMPILE_COMMAND)

elseif(WIN32)

    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        message(FATAL "MSVC is currently not supported by openssl-dep.")
    endif()

    message(STATUS "Fetching OpenSSL Binary Package...")

    set(TARGET_BIN_PACKAGE_URL "https://raw.githubusercontent.com/tina-project/openssl-dep/main/bin/OpenSSL-3.3.1-MinGW_W64.zip")
    set(TARGET_BIN_PACKAGE_SHA256 "461285b6b625054c0f10c32ed6e8f13cb5ed81e0ab48a4e0cb8fc15a3ecf82e2")

    set(TARGET_DOWNLOAD_DIR "${CMAKE_SOURCE_DIR}/_deps")
    set(TARGET_DOWNLOAD_FILE "${TARGET_DOWNLOAD_DIR}/OpenSSL-3.3.1-MinGW_W64.zip")

    file(MAKE_DIRECTORY ${TARGET_DOWNLOAD_DIR})
    file(DOWNLOAD ${TARGET_BIN_PACKAGE_URL} ${TARGET_BIN_DOWNLOAD_FILE}
        EXPECTED_HASH SHA256=${TARGET_PACKAGE_SHA256}
        SHOW_PROGRESS
    )

    message(STATUS "Extracting OpenSSL Binary Package...")
    execute_process(
        COMMAND powershell -c Expand-Archive .\OpenSSL-3.3.1-MinGW_W64.zip .
        WORKING_DIRECTORY ${TARGET_DOWNLOAD_DIR}
        OUTPUT_QUIET
    )

    unset(TARGET_BIN_PACKAGE_URL)
    unset(TARGET_BIN_PACKAGE_SHA256)
    unset(TARGET_DOWNLOAD_DIR)
    unset(TARGET_DOWNLOAD_FILE)

endif()

set(OPENSSL_DEP_ROOT_DIR "${CMAKE_SOURCE_DIR}/_deps/openssl" CACHE INTERNAL "${CMAKE_SOURCE_DIR}/_deps/openssl")
set(OPENSSL_DEP_INCLUDE_DIR "${OPENSSL_DEP_ROOT_DIR}/include" CACHE INTERNAL "${OPENSSL_DEP_ROOT_DIR}/include")
if(UNIX)
    set(OPENSSL_DEP_LIB_DIR "${OPENSSL_DEP_ROOT_DIR}/lib64" CACHE INTERNAL "${OPENSSL_DEP_ROOT_DIR}/lib64")
    set(OPENSSL_DEP_LIBCRYPTO_PATH "${OPENSSL_DEP_LIB_DIR}/libcrypto.so" CACHE INTERNAL "${OPENSSL_DEP_LIB_DIR}/libcrypto.so")
    set(OPENSSL_DEP_LIBSSL_PATH "${OPENSSL_DEP_LIB_DIR}/libssl.so" CACHE INTERNAL "${OPENSSL_DEP_LIB_DIR}/libssl.so")
elseif(WIN32)
    set(OPENSSL_DEP_LIB_DIR "${OPENSSL_DEP_ROOT_DIR}/bin" CACHE INTERNAL "${OPENSSL_DEP_ROOT_DIR}/bin")
    set(OPENSSL_DEP_LIBCRYPTO_PATH "${OPENSSL_DEP_LIB_DIR}/libcrypto-3-x64.dll" CACHE INTERNAL "${OPENSSL_DEP_LIB_DIR}/libcrypto-3-x64.dll")
    set(OPENSSL_DEP_LIBSSL_PATH "${OPENSSL_DEP_LIB_DIR}/libssl-3-x64.dll" CACHE INTERNAL "${OPENSSL_DEP_LIB_DIR}/libssl-3-x64.dll")
endif()
