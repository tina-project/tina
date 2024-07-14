add_library(tinacore SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/log/g3log_sink.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/log/log.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sql/sql.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/folders.cpp
)
target_include_directories(tinacore
    PUBLIC
        ${TINA_CORE_PUBLIC_INCLUDE_DIR}
        ${G3LOG_INCLUDE_SRC_DIR}
        ${SQLITE3_INCLUDE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/sql
        ${G3LOG_INCLUDE_BIN_DIR}
)
target_link_libraries(tinacore
    jsoncpp_lib
    sqlite3
)
set_target_properties(tinacore PROPERTIES
    OUTPUT_NAME tinacore
    SOVERSION ${TINA_CORE_SOVERSION}
    POSITION_INDEPENDENT_CODE ON
)
