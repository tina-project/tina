#include"sql.h"
#include"callbacks/sql_callbacks.h"
#include"core/global_macros.h"
#include"core/log/log.h"
#include"core/utils/errcodes.h"
#include"core/utils/time.h"

#include<cassert>

namespace sql = tina::core::sql;
namespace fs = std::filesystem;

#define TINA_DB_VERSION 1

/**
 * Current tina database version is: 1
 *
 * The format table in each database should be kept in the following form:
 *   ┌────────────────┬─────────────────────────┐
 *   │ Format Version │ First Create Time (UTC) │
 *   ├────────────────┼─────────────────────────┤
 *   │      INT       │           INT           │
 *   └────────────────┴─────────────────────────┘
 * After database creation, the format table SHOULDN'T BE CHANGED any more（unless this is a backup database).
 *
 * The header table in each database should be kept in the following form:
 *   ┌─────────────────────────────────────────────────────┬─────────────┬─────────────────────────────────────────────────────────────────────────────────────────────┐
 *   │                  Calendar Details                   │    Owner    │                                           Setting                                           │
 *   ├───────────────┬───────────────────────┬─────────────┼─────────────┼─────────────────────────────┬───────────────────────────┬───────────────────────────────────┤
 *   │ Calendar Name │ Calendar Display Name │ Description │ Owner Email │ Data Auto-delete After Days │ Enable Email Notification │ Email Notification Target Address │
 *   ├───────────────┼───────────────────────┼─────────────┼─────────────┼─────────────────────────────┼───────────────────────────┼───────────────────────────────────┤
 *   │     TEXT      │         TEXT          │    TEXT     │    TEXT     │             INT             │            INT            │               TEXT                │
 *   └───────────────┴───────────────────────┴─────────────┴─────────────┴─────────────────────────────┴───────────────────────────┴───────────────────────────────────┘
 * `Calendar Name` is a global sign of the current database. It only contains Latin letters, numbers and underscores. It should be same as the database filename, and shouldn't be changed after creation.
 * `Calendar Display Name` is the display name on each device. It can be changed locally and won't be synchronised to other devices by default.
 * `Email Notification Target Address` is a group of Email addresses separated by semicolons. When `Enable Email Notification` is set to non-zero, server will send an Email to each address on notify.
 *
 * The hash table in each database contains a group of hash codes, show the digest of the whole calendar & each month's. They're pre-calculated datas for multi-device synchronisation.
 * The table should be kept in the following form:
 *   ┌─────────────────┬───────────┐
 *   │       ID        │ Hash Code │
 *   ├─────────────────┼───────────┤
 *   │ INT PRIMARY KEY │    INT    │
 *   └─────────────────┴───────────┘
 * `ID` is the primary identifier of the hash code. If `ID` is 0, means the hash code is the 'root' hash code, which is the digest of the whole calendar. Otherwise `ID` should be kept in the form of 'YYYYMM'.
 * `Hash Code` is a SHA-256 hash of the specific data.
 *
 * The tag table in each database contains a map of tag-num and tag-name. The table should be kept in the following form:
 *   ┌─────────────────┬──────────┬─────────────┐
 *   │       ID        │ Tag Name │ Description │
 *   ├─────────────────┼──────────┼─────────────┤
 *   │ INT PRIMARY KEY │   TEXT   │    TEXT     │
 *   └─────────────────┴──────────┴─────────────┘
 *
 * The calendar table in each database should be kept in the following form:
 *   ┌────────────┬─────────────┬────────┬──────────────────┬────────────────┬─────────────┬────────────────────┬───────────────────────┐
 *   │ Event Name │ Event Place │ Tag ID │ Start Time (UTC) │ End Time (UTC) │ Description │ Notify Before Days │ Invitee Email Address │
 *   ├────────────┼─────────────┼────────┼──────────────────┼────────────────┼─────────────┼────────────────────┼───────────────────────┤
 *   │    TEXT    │    TEXT     │  INT   │       INT        │      INT       │    TEXT     │        INT         │         TEXT          │
 *   └────────────┴─────────────┴────────┴──────────────────┴────────────────┴─────────────┴────────────────────┴───────────────────────┘
 * `Start Time` and `End Time` is an time stamp storing event duration.
 * `Invitee Email Address` is a group of Email addresses separated by semicolons. When it is not empty, server will send an invitation Email to each address.
 *
 * The notes table in each database should be kept in the following form:
 *   ┌───────────┬────────┬───────────────────┬─────────────┬──────────────────────┐
 *   │ Note Name │ Tag ID │ Create Time (UTC) │ Description │ Notify Every Minutes │
 *   ├───────────┼────────┼───────────────────┼─────────────┼──────────────────────┤
 *   │   TEXT    │  INT   │        INT        │    TEXT     │         INT          │
 *   └───────────┴────────┴───────────────────┴─────────────┴──────────────────────┘
 */

/**
 * Open a database and check its legality. If it doesn't exist, create it.
 * This function does calendar cleanup on database open.
 *
 * Attention: Do db existance check before generating a sql object, since it automatically creates a new db when the db doesn't exist.
 */
sql::sql::sql( const fs::path& __p_dbpath ) :
    _p_dbpath( __p_dbpath )
{
    bool is_new = !fs::exists( this -> _p_dbpath );
    int rc = sqlite3_open( this -> _p_dbpath.string().c_str() , &( this -> _p_sql3_dbobj ) );
    if ( rc != SQLITE_OK )
    {
        LOG_E( WARNING ) << "open sql database failed. [path=\"" << this -> _p_dbpath << "\", errmsg=\"" << sqlite3_errmsg( this -> _p_sql3_dbobj ) << "\"]";
        throw sql_exception( utils::errcodes::sql::SQL_DB_NOT_OPEN , sqlite3_errmsg( this -> _p_sql3_dbobj ) );
    }
    LOG_E( INFO ) << "sql database opened. [path=\"" << this -> _p_dbpath << "\"]";
    if ( is_new )
    {
        LOG_E( INFO ) << "new sql database, create tables.";
        int rc = this -> _create_tables();
        if ( rc != 0 )
        {
            // ********************
            // TODO: error handling
            // ********************
        }
    } // new db
    else
    {
        if ( !( this -> _check_db_legality() ) )
        {
            // ********************
            // TODO: error handling
            // ********************
        } // illegal db
        this -> _cleanup_calendar();
    }
    LOG_E( INFO ) << "sql databse opened and verifyed.";
    return;
}

sql::sql::~sql(){
    sqlite3_close( this -> _p_sql3_dbobj );
    LOG_E( INFO ) << "sql database closed";
    return;
}

/**
 * Create tables in a new database.
 * If success returns 0, if not returns the table num defined in `_tablename_t`. If writing format table failed, returns 255.
 * This function doesn't change the errmsg set by `_exec_sqlcmd`.
 */
int sql::sql::_create_tables(){
    int rc;

    LOG_E( DEBUG ) << "creating format table.";
    rc = this -> _exec_sqlcmd( R"(
CREATE TABLE FORMAT(
    FORMAT_VERSION                      INT                 NOT NULL,
    FIRST_CREATE_TIME_UTC               INT                 NOT NULL
);
    )" );
    if ( rc != TINA_OK )
    {
        LOG_E( WARNING ) << "create format table failed. [errmsg=\"" << this -> _s_errmsg << "\",rc=" << rc << "]";
        return static_cast<int>( _tablename_t::FORMAT );
    }
    // format

    LOG_E( DEBUG ) << "creating header table.";
    rc = this -> _exec_sqlcmd( R"(
CREATE TABLE HEADER(
    CALENDAR_NAME                       TEXT                NOT NULL,
    CALENDAR_DISPLAY_NAME               TEXT                NOT NULL,
    DESCRIPTION                         TEXT,
    OWNER_EMAIL                         TEXT,
    DATA_AUTO_DELETE_AFTER_DAYS         INT                 NOT NULL,
    ENABLE_EMAIL_NOTIFICATION           INT                 NOT NULL,
    EMAIL_NOTIFICATION_TARGET_ADDRESS   TEXT
);
    )" );
    if ( rc != TINA_OK )
    {
        LOG_E( WARNING ) << "create header table failed. [errmsg=\"" << this -> _s_errmsg << "\",rc=" << rc << "]";
        return static_cast<int>( _tablename_t::HEADER );
    }
    // header

    LOG_E( DEBUG ) << "creating hash table.";
    rc = this -> _exec_sqlcmd( R"(
CREATE TABLE HASH(
    ID                                  INT PRIMARY KEY     NOT NULL,
    HASH_CODE                           INT                 NOT NULL
);
    )" );
    if ( rc != TINA_OK )
    {
        LOG_E( WARNING ) << "creating hash table failed. [errmsg=\"" << this -> _s_errmsg << "\",rc=" << rc << "]";
        return static_cast<int>( _tablename_t::HASH );
    }
    // hash

    LOG_E( DEBUG ) << "creating tag table.";
    rc = this -> _exec_sqlcmd( R"(
CREATE TABLE TAG(
    ID                                  INT PRIMARY KEY     NOT NULL,
    TAG_NAME                            TEXT                NOT NULL,
    DESCRIPTION                         TEXT
);
    )" );
    if ( rc != TINA_OK )
    {
        LOG_E( WARNING ) << "creating tag table failed. [errmsg=\"" << this -> _s_errmsg << "\",rc=" << rc << "]";
        return static_cast<int>( _tablename_t::TAG );
    }
    // tag

    LOG_E( DEBUG ) << "creating calendar table.";
    rc = this -> _exec_sqlcmd( R"(
CREATE TABLE CALENDAR(
    EVENT_NAME                          TEXT                NOT NULL,
    EVENT_PLACE                         TEXT                NOT NULL,
    TAG_ID                              INT,
    START_TIME_UTC                      INT                 NOT NULL,
    END_TIME_UTC                        INT                 NOT NULL,
    DESCRIPTION                         TEXT,
    NOTIFY_BEFORE_DAYS                  INT                 NOT NULL,
    INVITEE_EMAIL_ADDRESS               TEXT
);
    )" );
    if ( rc != TINA_OK )
    {
        LOG_E( WARNING ) << "creating calendar table failed. [errmsg=\"" << this -> _s_errmsg << "\",rc=" << rc << "]";
        return static_cast<int>( _tablename_t::CALENDAR );
    }
    // calendar

    LOG_E( DEBUG ) << "creating notes table.";
    rc = this -> _exec_sqlcmd( R"(
CREATE TABLE NOTES(
    NOTE_NAME                           TEXT                NOT NULL,
    TAG_ID                              INT                 NOT NULL,
    CREATE_TIME_UTC                     INT,
    DESCRIPTION                         TEXT,
    NOTIFY_EVERY_MINUTES                INT
);
    )" );
    if ( rc != TINA_OK )
    {
        LOG_E( WARNING ) << "creating notes table failed. [errmsg=\"" << this -> _s_errmsg << "\",rc=" << rc << "]";
        return static_cast<int>( _tablename_t::NOTES );
    }
    // notes

    rc = this -> _write_format();
    if ( rc != TINA_OK )
    {
        return 255;
    } // failed to write format table

    return TINA_OK;
}

int sql::sql::_check_db_legality(){

    // ****************************
    // TODO: _check_db_legality def
    // ****************************

}

int sql::sql::_cleanup_calendar(){

    // ***************************
    // TODO: _cleanup_calendar def
    // ***************************

}

/**
 * Get table len in current opened database.
 * If success returns the length, if not returns -1.
 * This function doesn't change the errmsg set by `_exec_sqlcmd`.
 */
int sql::sql::_get_table_len( sql::sql::_tablename_t __e_table ){
    LOG_E( DEBUG ) << "getting table len. [table=" << static_cast<int>( __e_table ) << "]";
    std::ostringstream oss;
    oss << "SELECT COUNT(*) FROM ";
    switch ( __e_table )
    {
        case FORMAT:    oss << "FORMAT;";   break;
        case HEADER:    oss << "HEADER;";   break;
        case HASH:      oss << "HASH;";     break;
        case TAG:       oss << "TAG;";      break;
        case CALENDAR:  oss << "CALENDAR;"; break;
        case NOTES:     oss << "NOTES;";    break;
        default: 
            // ********************
            // TODO: error handling
            // ********************
            break;
    }
    int len;
    int rc = this -> _exec_sqlcmd(
        oss.str() ,
        SQL_CB( callbacks::sqlcb_get_table_length ) ,
        ( void* ) &len 
    );
    if ( rc != TINA_OK )
    {
        LOG_E( WARNING ) << "get table len failed. [errmsg=\"" << this -> _s_errmsg << "\",rc=" << rc << "]";
        return -1;
    } // exec sqlcmd failed
    return len;
}

/**
 * Execute sql command without callback function.
 * If success returns 0, if not returns the rc from `sqlite3_exec` function.
 */
int sql::sql::_exec_sqlcmd( const std::string& __s_cmd ){
    LOG_E( DEBUG ) << "executing sql command. [cmd=\"" << __s_cmd << "\"]";
    char* errmsg = { 0 };
    int rc = sqlite3_exec( this -> _p_sql3_dbobj , __s_cmd.c_str() , nullptr , nullptr , &errmsg );
    if ( rc != SQLITE_OK )
    {
        LOG_E( DEBUG ) << "sql command execute failed. [errmsg=\"" << errmsg << "\",rc=" << rc << "]";
        this -> _s_errmsg = errmsg;
        sqlite3_free( errmsg );
        return rc;
    }
    return TINA_OK;
}

/**
 * Execute sql command with callback function.
 * If success returns 0, if not returns the rc from `sqlite3_exec` function.
 */
int sql::sql::_exec_sqlcmd( const std::string& __s_cmd , const callback_data_t& __scd_cb , void* __p_v_data ){
    LOG_E( DEBUG ) << "executing sql command. [cmd=\"" << __s_cmd << "\",callback=\"" << __scd_cb.s_cbname << "\"]";
    char* errmsg = { 0 };
    int rc = sqlite3_exec( this -> _p_sql3_dbobj , __s_cmd.c_str() , __scd_cb.p_fu_cb , __p_v_data , &errmsg );
    if ( rc != SQLITE_OK )
    {
        LOG_E( DEBUG ) << "sql command execute failed. [errmsg=\"" << errmsg << "\",rc=" << rc << "]";
        this -> _s_errmsg = errmsg;
        sqlite3_free( errmsg );
        return rc;
    }
    return TINA_OK;
}

/**
 * Write Tina database format to format table.
 * This function is private and should only be called by `_create_tables` function.
 * This function doesn't change the errmsg set by `_exec_sqlcmd`.
 */
int sql::sql::_write_format(){
    assert( this -> _get_table_len( FORMAT ) == 0 );

    std::ostringstream oss;
    oss << "INSERT INTO FORMAT VALUES (" << TINA_DB_VERSION << ","
                                         << utils::time::get_ts() << ");";
    LOG_E( DEBUG ) << "writing format table.";
    int rc = this -> _exec_sqlcmd(
        oss.str()
    );
    if ( rc != TINA_OK )
    {
        LOG_E( WARNING ) << "write format table failed. [errmsg=\"" << this -> _s_errmsg << "\",rc=" << rc << "]";
        return rc;
    }

    return TINA_OK;                                         
}
