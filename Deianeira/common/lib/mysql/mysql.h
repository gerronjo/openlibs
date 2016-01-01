/* Copyright (C) 2000-2003 MySQL AB

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

/*
  This file defines the client API to MySQL and also the ABI of the
  dynamically linked libmysqlclient.

  The ABI should never be changed in a released product of MySQL
  thus you need to take great care when changing the file. In case
  the file is changed so the ABI is broken, you must also
  update the SHAREDLIB_MAJOR_VERSION in configure.in .

*/

#ifndef _mysql_h
#define _mysql_h

#ifdef _AIX           /* large-file support will break without this */
#include <standards.h>
#endif

#ifdef __CYGWIN__     /* CYGWIN implements a UNIX API */
#undef WIN
#undef _WIN
#undef _WIN32
#undef _WIN64
#undef __WIN__
#endif

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef _global_h				/* If not standard header */
#include <sys/types.h>
#ifdef __LCC__
#include <winsock2.h>				/* For windows */
#endif
typedef char my_bool;
#if (defined(_WIN32) || defined(_WIN64)) && !defined(__WIN__)
#define __WIN__
#endif
#if !defined(__WIN__)
#define STDCALL
#else
#define STDCALL __stdcall
#endif

#ifndef my_socket_defined
#ifdef __WIN__
#define my_socket SOCKET
#else
typedef int my_socket;
#endif /* __WIN__ */
#endif /* my_socket_defined */
#endif /* _global_h */

//#include "mysql_version.h"
/* Copyright Abandoned 1996, 1999, 2001 MySQL AB
   This file is public domain and comes with NO WARRANTY of any kind */

/* Version numbers for protocol & mysqld */
#ifdef _CUSTOMCONFIG_
#include <custom_conf.h>
#else
#define PROTOCOL_VERSION		10
#define MYSQL_SERVER_VERSION		"5.1.31"
#define MYSQL_BASE_VERSION		"mysqld-5.1"
#define MYSQL_SERVER_SUFFIX_DEF		"-community"
#define FRM_VER				6
#define MYSQL_VERSION_ID		50131
#define MYSQL_PORT			3306
#define MYSQL_PORT_DEFAULT		0
#define MYSQL_UNIX_ADDR			"/tmp/mysql.sock"
#define MYSQL_CONFIG_NAME		"my"
#define MYSQL_COMPILATION_COMMENT	"MySQL Community Server (GPL)"

/* mysqld compile time options */
#endif /* _CUSTOMCONFIG_ */

#ifndef LICENSE
#define LICENSE				GPL
#endif /* LICENSE */
//end #include "mysql_version.h"

//#include "mysql_com.h"
/* Copyright (C) 2000 MySQL AB

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

/*
** Common definition between mysql server & client
*/

#define HOSTNAME_LENGTH 60
#define SYSTEM_CHARSET_MBMAXLEN 3
#define NAME_CHAR_LEN	64              /* Field/table name length */
#define USERNAME_CHAR_LENGTH 16
#define NAME_LEN                (NAME_CHAR_LEN*SYSTEM_CHARSET_MBMAXLEN)
#define USERNAME_LENGTH         (USERNAME_CHAR_LENGTH*SYSTEM_CHARSET_MBMAXLEN)

#define SERVER_VERSION_LENGTH 60
#define SQLSTATE_LENGTH 5

/*
  USER_HOST_BUFF_SIZE -- length of string buffer, that is enough to contain
  username and hostname parts of the user identifier with trailing zero in
  MySQL standard format:
  user_name_part@host_name_part\0
*/
#define USER_HOST_BUFF_SIZE HOSTNAME_LENGTH + USERNAME_LENGTH + 2

#define LOCAL_HOST	"localhost"
#define LOCAL_HOST_NAMEDPIPE "."


#if defined(__WIN__) && !defined( _CUSTOMCONFIG_)
#define MYSQL_NAMEDPIPE "MySQL"
#define MYSQL_SERVICENAME "MySQL"
#endif /* __WIN__ */

/*
  You should add new commands to the end of this list, otherwise old
  servers won't be able to handle them as 'unsupported'.
*/

enum enum_server_command
{
  COM_SLEEP, COM_QUIT, COM_INIT_DB, COM_QUERY, COM_FIELD_LIST,
  COM_CREATE_DB, COM_DROP_DB, COM_REFRESH, COM_SHUTDOWN, COM_STATISTICS,
  COM_PROCESS_INFO, COM_CONNECT, COM_PROCESS_KILL, COM_DEBUG, COM_PING,
  COM_TIME, COM_DELAYED_INSERT, COM_CHANGE_USER, COM_BINLOG_DUMP,
  COM_TABLE_DUMP, COM_CONNECT_OUT, COM_REGISTER_SLAVE,
  COM_STMT_PREPARE, COM_STMT_EXECUTE, COM_STMT_SEND_LONG_DATA, COM_STMT_CLOSE,
  COM_STMT_RESET, COM_SET_OPTION, COM_STMT_FETCH, COM_DAEMON,
  /* don't forget to update const char *command_name[] in sql_parse.cc */

  /* Must be last */
  COM_END
};


/*
  Length of random string sent by server on handshake; this is also length of
  obfuscated password, recieved from client
*/
#define SCRAMBLE_LENGTH 20
#define SCRAMBLE_LENGTH_323 8
/* length of password stored in the db: new passwords are preceeded with '*' */
#define SCRAMBLED_PASSWORD_CHAR_LENGTH (SCRAMBLE_LENGTH*2+1)
#define SCRAMBLED_PASSWORD_CHAR_LENGTH_323 (SCRAMBLE_LENGTH_323*2)


#define NOT_NULL_FLAG	1		/* Field can't be NULL */
#define PRI_KEY_FLAG	2		/* Field is part of a primary key */
#define UNIQUE_KEY_FLAG 4		/* Field is part of a unique key */
#define MULTIPLE_KEY_FLAG 8		/* Field is part of a key */
#define BLOB_FLAG	16		/* Field is a blob */
#define UNSIGNED_FLAG	32		/* Field is unsigned */
#define ZEROFILL_FLAG	64		/* Field is zerofill */
#define BINARY_FLAG	128		/* Field is binary   */

/* The following are only sent to new clients */
#define ENUM_FLAG	256		/* field is an enum */
#define AUTO_INCREMENT_FLAG 512		/* field is a autoincrement field */
#define TIMESTAMP_FLAG	1024		/* Field is a timestamp */
#define SET_FLAG	2048		/* field is a set */
#define NO_DEFAULT_VALUE_FLAG 4096	/* Field doesn't have default value */
#define ON_UPDATE_NOW_FLAG 8192         /* Field is set to NOW on UPDATE */
#define NUM_FLAG	32768		/* Field is num (for clients) */
#define PART_KEY_FLAG	16384		/* Intern; Part of some key */
#define GROUP_FLAG	32768		/* Intern: Group field */
#define UNIQUE_FLAG	65536		/* Intern: Used by sql_yacc */
#define BINCMP_FLAG	131072		/* Intern: Used by sql_yacc */
#define GET_FIXED_FIELDS_FLAG (1 << 18) /* Used to get fields in item tree */
#define FIELD_IN_PART_FUNC_FLAG (1 << 19)/* Field part of partition func */
#define FIELD_IN_ADD_INDEX (1<< 20)	/* Intern: Field used in ADD INDEX */
#define FIELD_IS_RENAMED (1<< 21)       /* Intern: Field is being renamed */

#define REFRESH_GRANT		1	/* Refresh grant tables */
#define REFRESH_LOG		2	/* Start on new log file */
#define REFRESH_TABLES		4	/* close all tables */
#define REFRESH_HOSTS		8	/* Flush host cache */
#define REFRESH_STATUS		16	/* Flush status variables */
#define REFRESH_THREADS		32	/* Flush thread cache */
#define REFRESH_SLAVE           64      /* Reset master info and restart slave
					   thread */
#define REFRESH_MASTER          128     /* Remove all bin logs in the index
					   and truncate the index */

/* The following can't be set with mysql_refresh() */
#define REFRESH_READ_LOCK	16384	/* Lock tables for read */
#define REFRESH_FAST		32768	/* Intern flag */

/* RESET (remove all queries) from query cache */
#define REFRESH_QUERY_CACHE	65536
#define REFRESH_QUERY_CACHE_FREE 0x20000L /* pack query cache */
#define REFRESH_DES_KEY_FILE	0x40000L
#define REFRESH_USER_RESOURCES	0x80000L

#define CLIENT_LONG_PASSWORD	1	/* new more secure passwords */
#define CLIENT_FOUND_ROWS	2	/* Found instead of affected rows */
#define CLIENT_LONG_FLAG	4	/* Get all column flags */
#define CLIENT_CONNECT_WITH_DB	8	/* One can specify db on connect */
#define CLIENT_NO_SCHEMA	16	/* Don't allow database.table.column */
#define CLIENT_COMPRESS		32	/* Can use compression protocol */
#define CLIENT_ODBC		64	/* Odbc client */
#define CLIENT_LOCAL_FILES	128	/* Can use LOAD DATA LOCAL */
#define CLIENT_IGNORE_SPACE	256	/* Ignore spaces before '(' */
#define CLIENT_PROTOCOL_41	512	/* New 4.1 protocol */
#define CLIENT_INTERACTIVE	1024	/* This is an interactive client */
#define CLIENT_SSL              2048	/* Switch to SSL after handshake */
#define CLIENT_IGNORE_SIGPIPE   4096    /* IGNORE sigpipes */
#define CLIENT_TRANSACTIONS	8192	/* Client knows about transactions */
#define CLIENT_RESERVED         16384   /* Old flag for 4.1 protocol  */
#define CLIENT_SECURE_CONNECTION 32768  /* New 4.1 authentication */
#define CLIENT_MULTI_STATEMENTS (1UL << 16) /* Enable/disable multi-stmt support */
#define CLIENT_MULTI_RESULTS    (1UL << 17) /* Enable/disable multi-results */

#define CLIENT_SSL_VERIFY_SERVER_CERT (1UL << 30)
#define CLIENT_REMEMBER_OPTIONS (1UL << 31)

/* Gather all possible capabilites (flags) supported by the server */
#define CLIENT_ALL_FLAGS  (CLIENT_LONG_PASSWORD | \
                           CLIENT_FOUND_ROWS | \
                           CLIENT_LONG_FLAG | \
                           CLIENT_CONNECT_WITH_DB | \
                           CLIENT_NO_SCHEMA | \
                           CLIENT_COMPRESS | \
                           CLIENT_ODBC | \
                           CLIENT_LOCAL_FILES | \
                           CLIENT_IGNORE_SPACE | \
                           CLIENT_PROTOCOL_41 | \
                           CLIENT_INTERACTIVE | \
                           CLIENT_SSL | \
                           CLIENT_IGNORE_SIGPIPE | \
                           CLIENT_TRANSACTIONS | \
                           CLIENT_RESERVED | \
                           CLIENT_SECURE_CONNECTION | \
                           CLIENT_MULTI_STATEMENTS | \
                           CLIENT_MULTI_RESULTS | \
                           CLIENT_SSL_VERIFY_SERVER_CERT | \
                           CLIENT_REMEMBER_OPTIONS)

/*
  Switch off the flags that are optional and depending on build flags
  If any of the optional flags is supported by the build it will be switched
  on before sending to the client during the connection handshake.
*/
#define CLIENT_BASIC_FLAGS (((CLIENT_ALL_FLAGS & ~CLIENT_SSL) \
                                               & ~CLIENT_COMPRESS) \
                                               & ~CLIENT_SSL_VERIFY_SERVER_CERT)

#define SERVER_STATUS_IN_TRANS     1	/* Transaction has started */
#define SERVER_STATUS_AUTOCOMMIT   2	/* Server in auto_commit mode */
#define SERVER_MORE_RESULTS_EXISTS 8    /* Multi query - next query exists */
#define SERVER_QUERY_NO_GOOD_INDEX_USED 16
#define SERVER_QUERY_NO_INDEX_USED      32
/**
  The server was able to fulfill the clients request and opened a
  read-only non-scrollable cursor for a query. This flag comes
  in reply to COM_STMT_EXECUTE and COM_STMT_FETCH commands.
*/
#define SERVER_STATUS_CURSOR_EXISTS 64
/**
  This flag is sent when a read-only cursor is exhausted, in reply to
  COM_STMT_FETCH command.
*/
#define SERVER_STATUS_LAST_ROW_SENT 128
#define SERVER_STATUS_DB_DROPPED        256 /* A database was dropped */
#define SERVER_STATUS_NO_BACKSLASH_ESCAPES 512
/**
  Sent to the client if after a prepared statement reprepare
  we discovered that the new statement returns a different 
  number of result set columns.
*/
#define SERVER_STATUS_METADATA_CHANGED 1024

/**
  Server status flags that must be cleared when starting
  execution of a new SQL statement.
  Flags from this set are only added to the
  current server status by the execution engine, but 
  never removed -- the execution engine expects them 
  to disappear automagically by the next command.
*/
#define SERVER_STATUS_CLEAR_SET (SERVER_QUERY_NO_GOOD_INDEX_USED| \
                                 SERVER_QUERY_NO_INDEX_USED|\
                                 SERVER_MORE_RESULTS_EXISTS|\
                                 SERVER_STATUS_METADATA_CHANGED)

#define MYSQL_ERRMSG_SIZE	512
#define NET_READ_TIMEOUT	30		/* Timeout on read */
#define NET_WRITE_TIMEOUT	60		/* Timeout on write */
#define NET_WAIT_TIMEOUT	8*60*60		/* Wait for new query */

#define ONLY_KILL_QUERY         1


struct st_vio;					/* Only C */
typedef struct st_vio Vio;

#define MAX_TINYINT_WIDTH       3       /* Max width for a TINY w.o. sign */
#define MAX_SMALLINT_WIDTH      5       /* Max width for a SHORT w.o. sign */
#define MAX_MEDIUMINT_WIDTH     8       /* Max width for a INT24 w.o. sign */
#define MAX_INT_WIDTH           10      /* Max width for a LONG w.o. sign */
#define MAX_BIGINT_WIDTH        20      /* Max width for a LONGLONG */
#define MAX_CHAR_WIDTH		255	/* Max length for a CHAR colum */
#define MAX_BLOB_WIDTH		16777216	/* Default width for blob */

typedef struct st_net {
#if !defined(CHECK_EMBEDDED_DIFFERENCES) || !defined(EMBEDDED_LIBRARY)
  Vio *vio;
  unsigned char *buff,*buff_end,*write_pos,*read_pos;
  my_socket fd;					/* For Perl DBI/dbd */
  /*
    The following variable is set if we are doing several queries in one
    command ( as in LOAD TABLE ... FROM MASTER ),
    and do not want to confuse the client with OK at the wrong time
  */
  unsigned long remain_in_buf,length, buf_length, where_b;
  unsigned long max_packet,max_packet_size;
  unsigned int pkt_nr,compress_pkt_nr;
  unsigned int write_timeout, read_timeout, retry_count;
  int fcntl;
  unsigned int *return_status;
  unsigned char reading_or_writing;
  char save_char;
  my_bool unused0; /* Please remove with the next incompatible ABI change. */
  my_bool unused; /* Please remove with the next incompatible ABI change */
  my_bool compress;
  my_bool unused1; /* Please remove with the next incompatible ABI change. */
  /*
    Pointer to query object in query cache, do not equal NULL (0) for
    queries in cache that have not stored its results yet
  */
#endif
  /*
    'query_cache_query' should be accessed only via query cache
    functions and methods to maintain proper locking.
  */
  unsigned char *query_cache_query;
  unsigned int last_errno;
  unsigned char error; 
  my_bool unused2; /* Please remove with the next incompatible ABI change. */
  my_bool return_errno;
  /** Client library error message buffer. Actually belongs to struct MYSQL. */
  char last_error[MYSQL_ERRMSG_SIZE];
  /** Client library sqlstate buffer. Set along with the error message. */
  char sqlstate[SQLSTATE_LENGTH+1];
  void *extension;
} NET;


#define packet_error (~(unsigned long) 0)

enum enum_field_types { MYSQL_TYPE_DECIMAL, MYSQL_TYPE_TINY,
			MYSQL_TYPE_SHORT,  MYSQL_TYPE_LONG,
			MYSQL_TYPE_FLOAT,  MYSQL_TYPE_DOUBLE,
			MYSQL_TYPE_NULL,   MYSQL_TYPE_TIMESTAMP,
			MYSQL_TYPE_LONGLONG,MYSQL_TYPE_INT24,
			MYSQL_TYPE_DATE,   MYSQL_TYPE_TIME,
			MYSQL_TYPE_DATETIME, MYSQL_TYPE_YEAR,
			MYSQL_TYPE_NEWDATE, MYSQL_TYPE_VARCHAR,
			MYSQL_TYPE_BIT,
                        MYSQL_TYPE_NEWDECIMAL=246,
			MYSQL_TYPE_ENUM=247,
			MYSQL_TYPE_SET=248,
			MYSQL_TYPE_TINY_BLOB=249,
			MYSQL_TYPE_MEDIUM_BLOB=250,
			MYSQL_TYPE_LONG_BLOB=251,
			MYSQL_TYPE_BLOB=252,
			MYSQL_TYPE_VAR_STRING=253,
			MYSQL_TYPE_STRING=254,
			MYSQL_TYPE_GEOMETRY=255

};

/* For backward compatibility */
#define CLIENT_MULTI_QUERIES    CLIENT_MULTI_STATEMENTS    
#define FIELD_TYPE_DECIMAL     MYSQL_TYPE_DECIMAL
#define FIELD_TYPE_NEWDECIMAL  MYSQL_TYPE_NEWDECIMAL
#define FIELD_TYPE_TINY        MYSQL_TYPE_TINY
#define FIELD_TYPE_SHORT       MYSQL_TYPE_SHORT
#define FIELD_TYPE_LONG        MYSQL_TYPE_LONG
#define FIELD_TYPE_FLOAT       MYSQL_TYPE_FLOAT
#define FIELD_TYPE_DOUBLE      MYSQL_TYPE_DOUBLE
#define FIELD_TYPE_NULL        MYSQL_TYPE_NULL
#define FIELD_TYPE_TIMESTAMP   MYSQL_TYPE_TIMESTAMP
#define FIELD_TYPE_LONGLONG    MYSQL_TYPE_LONGLONG
#define FIELD_TYPE_INT24       MYSQL_TYPE_INT24
#define FIELD_TYPE_DATE        MYSQL_TYPE_DATE
#define FIELD_TYPE_TIME        MYSQL_TYPE_TIME
#define FIELD_TYPE_DATETIME    MYSQL_TYPE_DATETIME
#define FIELD_TYPE_YEAR        MYSQL_TYPE_YEAR
#define FIELD_TYPE_NEWDATE     MYSQL_TYPE_NEWDATE
#define FIELD_TYPE_ENUM        MYSQL_TYPE_ENUM
#define FIELD_TYPE_SET         MYSQL_TYPE_SET
#define FIELD_TYPE_TINY_BLOB   MYSQL_TYPE_TINY_BLOB
#define FIELD_TYPE_MEDIUM_BLOB MYSQL_TYPE_MEDIUM_BLOB
#define FIELD_TYPE_LONG_BLOB   MYSQL_TYPE_LONG_BLOB
#define FIELD_TYPE_BLOB        MYSQL_TYPE_BLOB
#define FIELD_TYPE_VAR_STRING  MYSQL_TYPE_VAR_STRING
#define FIELD_TYPE_STRING      MYSQL_TYPE_STRING
#define FIELD_TYPE_CHAR        MYSQL_TYPE_TINY
#define FIELD_TYPE_INTERVAL    MYSQL_TYPE_ENUM
#define FIELD_TYPE_GEOMETRY    MYSQL_TYPE_GEOMETRY
#define FIELD_TYPE_BIT         MYSQL_TYPE_BIT


/* Shutdown/kill enums and constants */ 

/* Bits for THD::killable. */
#define MYSQL_SHUTDOWN_KILLABLE_CONNECT    (unsigned char)(1 << 0)
#define MYSQL_SHUTDOWN_KILLABLE_TRANS      (unsigned char)(1 << 1)
#define MYSQL_SHUTDOWN_KILLABLE_LOCK_TABLE (unsigned char)(1 << 2)
#define MYSQL_SHUTDOWN_KILLABLE_UPDATE     (unsigned char)(1 << 3)

enum mysql_enum_shutdown_level {
  /*
    We want levels to be in growing order of hardness (because we use number
    comparisons). Note that DEFAULT does not respect the growing property, but
    it's ok.
  */
  SHUTDOWN_DEFAULT = 0,
  /* wait for existing connections to finish */
  SHUTDOWN_WAIT_CONNECTIONS= MYSQL_SHUTDOWN_KILLABLE_CONNECT,
  /* wait for existing trans to finish */
  SHUTDOWN_WAIT_TRANSACTIONS= MYSQL_SHUTDOWN_KILLABLE_TRANS,
  /* wait for existing updates to finish (=> no partial MyISAM update) */
  SHUTDOWN_WAIT_UPDATES= MYSQL_SHUTDOWN_KILLABLE_UPDATE,
  /* flush InnoDB buffers and other storage engines' buffers*/
  SHUTDOWN_WAIT_ALL_BUFFERS= (MYSQL_SHUTDOWN_KILLABLE_UPDATE << 1),
  /* don't flush InnoDB buffers, flush other storage engines' buffers*/
  SHUTDOWN_WAIT_CRITICAL_BUFFERS= (MYSQL_SHUTDOWN_KILLABLE_UPDATE << 1) + 1,
  /* Now the 2 levels of the KILL command */
#if MYSQL_VERSION_ID >= 50000
  KILL_QUERY= 254,
#endif
  KILL_CONNECTION= 255
};


enum enum_cursor_type
{
  CURSOR_TYPE_NO_CURSOR= 0,
  CURSOR_TYPE_READ_ONLY= 1,
  CURSOR_TYPE_FOR_UPDATE= 2,
  CURSOR_TYPE_SCROLLABLE= 4
};


/* options for mysql_set_option */
enum enum_mysql_set_option
{
  MYSQL_OPTION_MULTI_STATEMENTS_ON,
  MYSQL_OPTION_MULTI_STATEMENTS_OFF
};

#define net_new_transaction(net) ((net)->pkt_nr=0)

#ifdef __cplusplus
extern "C" {
#endif

my_bool	my_net_init(NET *net, Vio* vio);
void	my_net_local_init(NET *net);
void	net_end(NET *net);
  void	net_clear(NET *net, my_bool clear_buffer);
my_bool net_realloc(NET *net, size_t length);
my_bool	net_flush(NET *net);
my_bool	my_net_write(NET *net,const unsigned char *packet, size_t len);
my_bool	net_write_command(NET *net,unsigned char command,
			  const unsigned char *header, size_t head_len,
			  const unsigned char *packet, size_t len);
int	net_real_write(NET *net,const unsigned char *packet, size_t len);
unsigned long my_net_read(NET *net);

#ifdef _global_h
void my_net_set_write_timeout(NET *net, uint timeout);
void my_net_set_read_timeout(NET *net, uint timeout);
#endif

/*
  The following function is not meant for normal usage
  Currently it's used internally by manager.c
*/
struct sockaddr;
int my_connect(my_socket s, const struct sockaddr *name, unsigned int namelen,
	       unsigned int timeout);

struct rand_struct {
  unsigned long seed1,seed2,max_value;
  double max_value_dbl;
};

#ifdef __cplusplus
}
#endif

  /* The following is for user defined functions */

enum Item_result {STRING_RESULT=0, REAL_RESULT, INT_RESULT, ROW_RESULT,
                  DECIMAL_RESULT};

typedef struct st_udf_args
{
  unsigned int arg_count;		/* Number of arguments */
  enum Item_result *arg_type;		/* Pointer to item_results */
  char **args;				/* Pointer to argument */
  unsigned long *lengths;		/* Length of string arguments */
  char *maybe_null;			/* Set to 1 for all maybe_null args */
  char **attributes;                    /* Pointer to attribute name */
  unsigned long *attribute_lengths;     /* Length of attribute arguments */
  void *extension;
} UDF_ARGS;

  /* This holds information about the result */

typedef struct st_udf_init
{
  my_bool maybe_null;          /* 1 if function can return NULL */
  unsigned int decimals;       /* for real functions */
  unsigned long max_length;    /* For string functions */
  char *ptr;                   /* free pointer for function data */
  my_bool const_item;          /* 1 if function always returns the same value */
  void *extension;
} UDF_INIT;
/* 
  TODO: add a notion for determinism of the UDF. 
  See Item_udf_func::update_used_tables ()
*/

  /* Constants when using compression */
#define NET_HEADER_SIZE 4		/* standard header size */
#define COMP_HEADER_SIZE 3		/* compression header extra size */

  /* Prototypes to password functions */

#ifdef __cplusplus
extern "C" {
#endif

/*
  These functions are used for authentication by client and server and
  implemented in sql/password.c
*/

void randominit(struct rand_struct *, unsigned long seed1,
                unsigned long seed2);
double my_rnd(struct rand_struct *);
void create_random_string(char *to, unsigned int length, struct rand_struct *rand_st);

void hash_password(unsigned long *to, const char *password, unsigned int password_len);
void make_scrambled_password_323(char *to, const char *password);
void scramble_323(char *to, const char *message, const char *password);
my_bool check_scramble_323(const char *, const char *message,
                           unsigned long *salt);
void get_salt_from_password_323(unsigned long *res, const char *password);
void make_password_from_salt_323(char *to, const unsigned long *salt);

void make_scrambled_password(char *to, const char *password);
void scramble(char *to, const char *message, const char *password);
my_bool check_scramble(const char *reply, const char *message,
                       const unsigned char *hash_stage2);
void get_salt_from_password(unsigned char *res, const char *password);
void make_password_from_salt(char *to, const unsigned char *hash_stage2);
char *octet2hex(char *to, const char *str, unsigned int len);

/* end of password.c */

char *get_tty_password(const char *opt_message);
const char *mysql_errno_to_sqlstate(unsigned int mysql_errno);

/* Some other useful functions */

my_bool my_thread_init(void);
void my_thread_end(void);

#ifdef _global_h
ulong STDCALL net_field_length(uchar **packet);
my_ulonglong net_field_length_ll(uchar **packet);
uchar *net_store_length(uchar *pkg, ulonglong length);
#endif

#ifdef __cplusplus
}
#endif

#define NULL_LENGTH ((unsigned long) ~0) /* For net_store_length */
#define MYSQL_STMT_HEADER       4
#define MYSQL_LONG_DATA_HEADER  6
//end #include "mysql_com.h"

//#include "mysql_time.h"
/* Copyright (C) 2004 MySQL AB

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2 of the License.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

/*
  Time declarations shared between the server and client API:
  you should not add anything to this header unless it's used
  (and hence should be visible) in mysql.h.
  If you're looking for a place to add new time-related declaration,
  it's most likely my_time.h. See also "C API Handling of Date
  and Time Values" chapter in documentation.
*/

enum enum_mysql_timestamp_type
{
  MYSQL_TIMESTAMP_NONE= -2, MYSQL_TIMESTAMP_ERROR= -1,
  MYSQL_TIMESTAMP_DATE= 0, MYSQL_TIMESTAMP_DATETIME= 1, MYSQL_TIMESTAMP_TIME= 2
};


/*
  Structure which is used to represent datetime values inside MySQL.

  We assume that values in this structure are normalized, i.e. year <= 9999,
  month <= 12, day <= 31, hour <= 23, hour <= 59, hour <= 59. Many functions
  in server such as my_system_gmt_sec() or make_time() family of functions
  rely on this (actually now usage of make_*() family relies on a bit weaker
  restriction). Also functions that produce MYSQL_TIME as result ensure this.
  There is one exception to this rule though if this structure holds time
  value (time_type == MYSQL_TIMESTAMP_TIME) days and hour member can hold
  bigger values.
*/
typedef struct st_mysql_time
{
  unsigned int  year, month, day, hour, minute, second;
  unsigned long second_part;
  my_bool       neg;
  enum enum_mysql_timestamp_type time_type;
} MYSQL_TIME;

//end #include "mysql_time.h"




//#include "my_list.h" /* for LISTs used in 'MYSQL' and 'MYSQL_STMT' */
/* Copyright (C) 2000 MySQL AB

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct st_list {
  struct st_list *prev,*next;
  void *data;
} LIST;

typedef int (*list_walk_action)(void *,void *);

extern LIST *list_add(LIST *root,LIST *element);
extern LIST *list_delete(LIST *root,LIST *element);
extern LIST *list_cons(void *data,LIST *root);
extern LIST *list_reverse(LIST *root);
extern void list_free(LIST *root,unsigned int free_data);
extern unsigned int list_length(LIST *);
extern int list_walk(LIST *,list_walk_action action,unsigned char * argument);

#define list_rest(a) ((a)->next)
#define list_push(a,b) (a)=list_cons((b),(a))
#define list_pop(A) {LIST *old=(A); (A)=list_delete(old,old) ; my_free((unsigned char *) old,MYF(MY_FAE)); }

#ifdef	__cplusplus
}
#endif

//end #include "my_list.h" 


extern unsigned int mysql_port;
extern char *mysql_unix_port;

#define CLIENT_NET_READ_TIMEOUT		365*24*3600	/* Timeout on read */
#define CLIENT_NET_WRITE_TIMEOUT	365*24*3600	/* Timeout on write */

#ifdef __NETWARE__
#pragma pack(push, 8)		/* 8 byte alignment */
#endif

#define IS_PRI_KEY(n)	((n) & PRI_KEY_FLAG)
#define IS_NOT_NULL(n)	((n) & NOT_NULL_FLAG)
#define IS_BLOB(n)	((n) & BLOB_FLAG)
#define IS_NUM(t)	((t) <= MYSQL_TYPE_INT24 || (t) == MYSQL_TYPE_YEAR || (t) == MYSQL_TYPE_NEWDECIMAL)
#define IS_NUM_FIELD(f)	 ((f)->flags & NUM_FLAG)
#define INTERNAL_NUM_FIELD(f) (((f)->type <= MYSQL_TYPE_INT24 && ((f)->type != MYSQL_TYPE_TIMESTAMP || (f)->length == 14 || (f)->length == 8)) || (f)->type == MYSQL_TYPE_YEAR)
#define IS_LONGDATA(t) ((t) >= MYSQL_TYPE_TINY_BLOB && (t) <= MYSQL_TYPE_STRING)


typedef struct st_mysql_field {
  char *name;                 /* Name of column */
  char *org_name;             /* Original column name, if an alias */
  char *table;                /* Table of column if column was a field */
  char *org_table;            /* Org table name, if table was an alias */
  char *db;                   /* Database for table */
  char *catalog;	      /* Catalog for table */
  char *def;                  /* Default value (set by mysql_list_fields) */
  unsigned long length;       /* Width of column (create length) */
  unsigned long max_length;   /* Max width for selected set */
  unsigned int name_length;
  unsigned int org_name_length;
  unsigned int table_length;
  unsigned int org_table_length;
  unsigned int db_length;
  unsigned int catalog_length;
  unsigned int def_length;
  unsigned int flags;         /* Div flags */
  unsigned int decimals;      /* Number of decimals in field */
  unsigned int charsetnr;     /* Character set */
  enum enum_field_types type; /* Type of field. See mysql_com.h for types */
  void *extension;
} MYSQL_FIELD;

typedef char **MYSQL_ROW;		/* return data as array of strings */
typedef unsigned int MYSQL_FIELD_OFFSET; /* offset to current field */

#ifndef _global_h
#if defined(NO_CLIENT_LONG_LONG)
typedef unsigned long my_ulonglong;
#elif defined (__WIN__)
typedef unsigned __int64 my_ulonglong;
#else
typedef unsigned long long my_ulonglong;
#endif
#endif

//#include "my_alloc.h"
/* Copyright (C) 2000 MySQL AB

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

/* 
   Data structures for mysys/my_alloc.c (root memory allocator)
*/

#define ALLOC_MAX_BLOCK_TO_DROP			4096
#define ALLOC_MAX_BLOCK_USAGE_BEFORE_DROP	10

typedef struct st_used_mem
{				   /* struct for once_alloc (block) */
  struct st_used_mem *next;	   /* Next block in use */
  unsigned int	left;		   /* memory left in block  */
  unsigned int	size;		   /* size of block */
} USED_MEM;


typedef struct st_mem_root
{
  USED_MEM *free;                  /* blocks with free memory in it */
  USED_MEM *used;                  /* blocks almost without free memory */
  USED_MEM *pre_alloc;             /* preallocated block */
  /* if block have less memory it will be put in 'used' list */
  size_t min_malloc;
  size_t block_size;               /* initial block size */
  unsigned int block_num;          /* allocated blocks counter */
  /* 
     first free block in queue test counter (if it exceed 
     MAX_BLOCK_USAGE_BEFORE_DROP block will be dropped in 'used' list)
  */
  unsigned int first_block_usage;

  void (*error_handler)(void);
} MEM_ROOT;

//end #include "my_alloc.h"

//#include "typelib.h"
/* Copyright (C) 2000 MySQL AB

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

//#include "my_alloc.h"

typedef struct st_typelib {	/* Different types saved here */
  unsigned int count;		/* How many types */
  const char *name;		/* Name of typelib */
  const char **type_names;
  unsigned int *type_lengths;
} TYPELIB;

extern my_ulonglong find_typeset(char *x, TYPELIB *typelib,int *error_position);
extern int find_type_or_exit(const char *x, TYPELIB *typelib,
                             const char *option);
extern int find_type(char *x, const TYPELIB *typelib, unsigned int full_name);
extern void make_type(char *to,unsigned int nr,TYPELIB *typelib);
extern const char *get_type(TYPELIB *typelib,unsigned int nr);
extern TYPELIB *copy_typelib(MEM_ROOT *root, TYPELIB *from);

extern TYPELIB sql_protocol_typelib;

//end #include "typelib.h"

#define MYSQL_COUNT_ERROR (~(my_ulonglong) 0)

/* backward compatibility define - to be removed eventually */
#define ER_WARN_DATA_TRUNCATED WARN_DATA_TRUNCATED

typedef struct st_mysql_rows {
  struct st_mysql_rows *next;		/* list of rows */
  MYSQL_ROW data;
  unsigned long length;
} MYSQL_ROWS;

typedef MYSQL_ROWS *MYSQL_ROW_OFFSET;	/* offset to current row */

typedef struct embedded_query_result EMBEDDED_QUERY_RESULT;
typedef struct st_mysql_data {
  MYSQL_ROWS *data;
  struct embedded_query_result *embedded_info;
  MEM_ROOT alloc;
  my_ulonglong rows;
  unsigned int fields;
  /* extra info for embedded library */
  void *extension;
} MYSQL_DATA;

enum mysql_option 
{
  MYSQL_OPT_CONNECT_TIMEOUT, MYSQL_OPT_COMPRESS, MYSQL_OPT_NAMED_PIPE,
  MYSQL_INIT_COMMAND, MYSQL_READ_DEFAULT_FILE, MYSQL_READ_DEFAULT_GROUP,
  MYSQL_SET_CHARSET_DIR, MYSQL_SET_CHARSET_NAME, MYSQL_OPT_LOCAL_INFILE,
  MYSQL_OPT_PROTOCOL, MYSQL_SHARED_MEMORY_BASE_NAME, MYSQL_OPT_READ_TIMEOUT,
  MYSQL_OPT_WRITE_TIMEOUT, MYSQL_OPT_USE_RESULT,
  MYSQL_OPT_USE_REMOTE_CONNECTION, MYSQL_OPT_USE_EMBEDDED_CONNECTION,
  MYSQL_OPT_GUESS_CONNECTION, MYSQL_SET_CLIENT_IP, MYSQL_SECURE_AUTH,
  MYSQL_REPORT_DATA_TRUNCATION, MYSQL_OPT_RECONNECT,
  MYSQL_OPT_SSL_VERIFY_SERVER_CERT
};

struct st_mysql_options {
  unsigned int connect_timeout, read_timeout, write_timeout;
  unsigned int port, protocol;
  unsigned long client_flag;
  char *host,*user,*password,*unix_socket,*db;
  struct st_dynamic_array *init_commands;
  char *my_cnf_file,*my_cnf_group, *charset_dir, *charset_name;
  char *ssl_key;				/* PEM key file */
  char *ssl_cert;				/* PEM cert file */
  char *ssl_ca;					/* PEM CA file */
  char *ssl_capath;				/* PEM directory of CA-s? */
  char *ssl_cipher;				/* cipher to use */
  char *shared_memory_base_name;
  unsigned long max_allowed_packet;
  my_bool use_ssl;				/* if to use SSL or not */
  my_bool compress,named_pipe;
 /*
   On connect, find out the replication role of the server, and
   establish connections to all the peers
 */
  my_bool rpl_probe;
 /*
   Each call to mysql_real_query() will parse it to tell if it is a read
   or a write, and direct it to the slave or the master
 */
  my_bool rpl_parse;
 /*
   If set, never read from a master, only from slave, when doing
   a read that is replication-aware
 */
  my_bool no_master_reads;
#if !defined(CHECK_EMBEDDED_DIFFERENCES) || defined(EMBEDDED_LIBRARY)
  my_bool separate_thread;
#endif
  enum mysql_option methods_to_use;
  char *client_ip;
  /* Refuse client connecting to server if it uses old (pre-4.1.1) protocol */
  my_bool secure_auth;
  /* 0 - never report, 1 - always report (default) */
  my_bool report_data_truncation;

  /* function pointers for local infile support */
  int (*local_infile_init)(void **, const char *, void *);
  int (*local_infile_read)(void *, char *, unsigned int);
  void (*local_infile_end)(void *);
  int (*local_infile_error)(void *, char *, unsigned int);
  void *local_infile_userdata;
  void *extension;
};

enum mysql_status 
{
  MYSQL_STATUS_READY,MYSQL_STATUS_GET_RESULT,MYSQL_STATUS_USE_RESULT
};

enum mysql_protocol_type 
{
  MYSQL_PROTOCOL_DEFAULT, MYSQL_PROTOCOL_TCP, MYSQL_PROTOCOL_SOCKET,
  MYSQL_PROTOCOL_PIPE, MYSQL_PROTOCOL_MEMORY
};
/*
  There are three types of queries - the ones that have to go to
  the master, the ones that go to a slave, and the adminstrative
  type which must happen on the pivot connectioin
*/
enum mysql_rpl_type 
{
  MYSQL_RPL_MASTER, MYSQL_RPL_SLAVE, MYSQL_RPL_ADMIN
};

typedef struct character_set
{
  unsigned int      number;     /* character set number              */
  unsigned int      state;      /* character set state               */
  const char        *csname;    /* collation name                    */
  const char        *name;      /* character set name                */
  const char        *comment;   /* comment                           */
  const char        *dir;       /* character set directory           */
  unsigned int      mbminlen;   /* min. length for multibyte strings */
  unsigned int      mbmaxlen;   /* max. length for multibyte strings */
} MY_CHARSET_INFO;

struct st_mysql_methods;
struct st_mysql_stmt;

typedef struct st_mysql
{
  NET		net;			/* Communication parameters */
  unsigned char	*connector_fd;		/* ConnectorFd for SSL */
  char		*host,*user,*passwd,*unix_socket,*server_version,*host_info;
  char          *info, *db;
  struct charset_info_st *charset;
  MYSQL_FIELD	*fields;
  MEM_ROOT	field_alloc;
  my_ulonglong affected_rows;
  my_ulonglong insert_id;		/* id if insert on table with NEXTNR */
  my_ulonglong extra_info;		/* Not used */
  unsigned long thread_id;		/* Id for connection in server */
  unsigned long packet_length;
  unsigned int	port;
  unsigned long client_flag,server_capabilities;
  unsigned int	protocol_version;
  unsigned int	field_count;
  unsigned int 	server_status;
  unsigned int  server_language;
  unsigned int	warning_count;
  struct st_mysql_options options;
  enum mysql_status status;
  my_bool	free_me;		/* If free in mysql_close */
  my_bool	reconnect;		/* set to 1 if automatic reconnect */

  /* session-wide random string */
  char	        scramble[SCRAMBLE_LENGTH+1];

 /*
   Set if this is the original connection, not a master or a slave we have
   added though mysql_rpl_probe() or mysql_set_master()/ mysql_add_slave()
 */
  my_bool rpl_pivot;
  /*
    Pointers to the master, and the next slave connections, points to
    itself if lone connection.
  */
  struct st_mysql* master, *next_slave;

  struct st_mysql* last_used_slave; /* needed for round-robin slave pick */
 /* needed for send/read/store/use result to work correctly with replication */
  struct st_mysql* last_used_con;

  LIST  *stmts;                     /* list of all statements */
  const struct st_mysql_methods *methods;
  void *thd;
  /*
    Points to boolean flag in MYSQL_RES  or MYSQL_STMT. We set this flag 
    from mysql_stmt_close if close had to cancel result set of this object.
  */
  my_bool *unbuffered_fetch_owner;
  /* needed for embedded server - no net buffer to store the 'info' */
  char *info_buffer;
  void *extension;
} MYSQL;


typedef struct st_mysql_res {
  my_ulonglong  row_count;
  MYSQL_FIELD	*fields;
  MYSQL_DATA	*data;
  MYSQL_ROWS	*data_cursor;
  unsigned long *lengths;		/* column lengths of current row */
  MYSQL		*handle;		/* for unbuffered reads */
  const struct st_mysql_methods *methods;
  MYSQL_ROW	row;			/* If unbuffered read */
  MYSQL_ROW	current_row;		/* buffer to current row */
  MEM_ROOT	field_alloc;
  unsigned int	field_count, current_field;
  my_bool	eof;			/* Used by mysql_fetch_row */
  /* mysql_stmt_close() had to cancel this result */
  my_bool       unbuffered_fetch_cancelled;  
  void *extension;
} MYSQL_RES;

#define MAX_MYSQL_MANAGER_ERR 256  
#define MAX_MYSQL_MANAGER_MSG 256

#define MANAGER_OK           200
#define MANAGER_INFO         250
#define MANAGER_ACCESS       401
#define MANAGER_CLIENT_ERR   450
#define MANAGER_INTERNAL_ERR 500

#if !defined(MYSQL_SERVER) && !defined(MYSQL_CLIENT)
#define MYSQL_CLIENT
#endif


typedef struct st_mysql_manager
{
  NET net;
  char *host, *user, *passwd;
  char *net_buf, *net_buf_pos, *net_data_end;
  unsigned int port;
  int cmd_status;
  int last_errno;
  int net_buf_size;
  my_bool free_me;
  my_bool eof;
  char last_error[MAX_MYSQL_MANAGER_ERR];
  void *extension;
} MYSQL_MANAGER;

typedef struct st_mysql_parameters
{
  unsigned long *p_max_allowed_packet;
  unsigned long *p_net_buffer_length;
  void *extension;
} MYSQL_PARAMETERS;

#if !defined(MYSQL_SERVER) && !defined(EMBEDDED_LIBRARY)
#define max_allowed_packet (*mysql_get_parameters()->p_max_allowed_packet)
#define net_buffer_length (*mysql_get_parameters()->p_net_buffer_length)
#endif

/*
  Set up and bring down the server; to ensure that applications will
  work when linked against either the standard client library or the
  embedded server library, these functions should be called.
*/
int STDCALL mysql_server_init(int argc, char **argv, char **groups);
void STDCALL mysql_server_end(void);

/*
  mysql_server_init/end need to be called when using libmysqld or
  libmysqlclient (exactly, mysql_server_init() is called by mysql_init() so
  you don't need to call it explicitely; but you need to call
  mysql_server_end() to free memory). The names are a bit misleading
  (mysql_SERVER* to be used when using libmysqlCLIENT). So we add more general
  names which suit well whether you're using libmysqld or libmysqlclient. We
  intend to promote these aliases over the mysql_server* ones.
*/
#define mysql_library_init mysql_server_init
#define mysql_library_end mysql_server_end

MYSQL_PARAMETERS *STDCALL mysql_get_parameters(void);

/*
  Set up and bring down a thread; these function should be called
  for each thread in an application which opens at least one MySQL
  connection.  All uses of the connection(s) should be between these
  function calls.
*/
my_bool STDCALL mysql_thread_init(void);
void STDCALL mysql_thread_end(void);

/*
  Functions to get information from the MYSQL and MYSQL_RES structures
  Should definitely be used if one uses shared libraries.
*/

my_ulonglong STDCALL mysql_num_rows(MYSQL_RES *res);
unsigned int STDCALL mysql_num_fields(MYSQL_RES *res);
my_bool STDCALL mysql_eof(MYSQL_RES *res);
MYSQL_FIELD *STDCALL mysql_fetch_field_direct(MYSQL_RES *res,
					      unsigned int fieldnr);
MYSQL_FIELD * STDCALL mysql_fetch_fields(MYSQL_RES *res);
MYSQL_ROW_OFFSET STDCALL mysql_row_tell(MYSQL_RES *res);
MYSQL_FIELD_OFFSET STDCALL mysql_field_tell(MYSQL_RES *res);

unsigned int STDCALL mysql_field_count(MYSQL *mysql);
my_ulonglong STDCALL mysql_affected_rows(MYSQL *mysql);
my_ulonglong STDCALL mysql_insert_id(MYSQL *mysql);
unsigned int STDCALL mysql_errno(MYSQL *mysql);
const char * STDCALL mysql_error(MYSQL *mysql);
const char *STDCALL mysql_sqlstate(MYSQL *mysql);
unsigned int STDCALL mysql_warning_count(MYSQL *mysql);
const char * STDCALL mysql_info(MYSQL *mysql);
unsigned long STDCALL mysql_thread_id(MYSQL *mysql);
const char * STDCALL mysql_character_set_name(MYSQL *mysql);
int          STDCALL mysql_set_character_set(MYSQL *mysql, const char *csname);

MYSQL *		STDCALL mysql_init(MYSQL *mysql);
my_bool		STDCALL mysql_ssl_set(MYSQL *mysql, const char *key,
				      const char *cert, const char *ca,
				      const char *capath, const char *cipher);
const char *    STDCALL mysql_get_ssl_cipher(MYSQL *mysql);
my_bool		STDCALL mysql_change_user(MYSQL *mysql, const char *user, 
					  const char *passwd, const char *db);
MYSQL *		STDCALL mysql_real_connect(MYSQL *mysql, const char *host,
					   const char *user,
					   const char *passwd,
					   const char *db,
					   unsigned int port,
					   const char *unix_socket,
					   unsigned long clientflag);
int		STDCALL mysql_select_db(MYSQL *mysql, const char *db);
int		STDCALL mysql_query(MYSQL *mysql, const char *q);
int		STDCALL mysql_send_query(MYSQL *mysql, const char *q,
					 unsigned long length);
int		STDCALL mysql_real_query(MYSQL *mysql, const char *q,
					unsigned long length);
MYSQL_RES *     STDCALL mysql_store_result(MYSQL *mysql);
MYSQL_RES *     STDCALL mysql_use_result(MYSQL *mysql);

/* perform query on master */
my_bool		STDCALL mysql_master_query(MYSQL *mysql, const char *q,
					   unsigned long length);
my_bool		STDCALL mysql_master_send_query(MYSQL *mysql, const char *q,
						unsigned long length);
/* perform query on slave */  
my_bool		STDCALL mysql_slave_query(MYSQL *mysql, const char *q,
					  unsigned long length);
my_bool		STDCALL mysql_slave_send_query(MYSQL *mysql, const char *q,
					       unsigned long length);
void        STDCALL mysql_get_character_set_info(MYSQL *mysql,
                           MY_CHARSET_INFO *charset);

/* local infile support */

#define LOCAL_INFILE_ERROR_LEN 512

void
mysql_set_local_infile_handler(MYSQL *mysql,
                               int (*local_infile_init)(void **, const char *,
                            void *),
                               int (*local_infile_read)(void *, char *,
							unsigned int),
                               void (*local_infile_end)(void *),
                               int (*local_infile_error)(void *, char*,
							 unsigned int),
                               void *);

void
mysql_set_local_infile_default(MYSQL *mysql);


/*
  enable/disable parsing of all queries to decide if they go on master or
  slave
*/
void            STDCALL mysql_enable_rpl_parse(MYSQL* mysql);
void            STDCALL mysql_disable_rpl_parse(MYSQL* mysql);
/* get the value of the parse flag */  
int             STDCALL mysql_rpl_parse_enabled(MYSQL* mysql);

/*  enable/disable reads from master */
void            STDCALL mysql_enable_reads_from_master(MYSQL* mysql);
void            STDCALL mysql_disable_reads_from_master(MYSQL* mysql);
/* get the value of the master read flag */  
my_bool		STDCALL mysql_reads_from_master_enabled(MYSQL* mysql);

enum mysql_rpl_type     STDCALL mysql_rpl_query_type(const char* q, int len);  

/* discover the master and its slaves */  
my_bool		STDCALL mysql_rpl_probe(MYSQL* mysql);

/* set the master, close/free the old one, if it is not a pivot */
int             STDCALL mysql_set_master(MYSQL* mysql, const char* host,
					 unsigned int port,
					 const char* user,
					 const char* passwd);
int             STDCALL mysql_add_slave(MYSQL* mysql, const char* host,
					unsigned int port,
					const char* user,
					const char* passwd);

int		STDCALL mysql_shutdown(MYSQL *mysql,
                                       enum mysql_enum_shutdown_level
                                       shutdown_level);
int		STDCALL mysql_dump_debug_info(MYSQL *mysql);
int		STDCALL mysql_refresh(MYSQL *mysql,
				     unsigned int refresh_options);
int		STDCALL mysql_kill(MYSQL *mysql,unsigned long pid);
int		STDCALL mysql_set_server_option(MYSQL *mysql,
						enum enum_mysql_set_option
						option);
int		STDCALL mysql_ping(MYSQL *mysql);
const char *	STDCALL mysql_stat(MYSQL *mysql);
const char *	STDCALL mysql_get_server_info(MYSQL *mysql);
const char *	STDCALL mysql_get_client_info(void);
unsigned long	STDCALL mysql_get_client_version(void);
const char *	STDCALL mysql_get_host_info(MYSQL *mysql);
unsigned long	STDCALL mysql_get_server_version(MYSQL *mysql);
unsigned int	STDCALL mysql_get_proto_info(MYSQL *mysql);
MYSQL_RES *	STDCALL mysql_list_dbs(MYSQL *mysql,const char *wild);
MYSQL_RES *	STDCALL mysql_list_tables(MYSQL *mysql,const char *wild);
MYSQL_RES *	STDCALL mysql_list_processes(MYSQL *mysql);
int		STDCALL mysql_options(MYSQL *mysql,enum mysql_option option,
				      const void *arg);
void		STDCALL mysql_free_result(MYSQL_RES *result);
void		STDCALL mysql_data_seek(MYSQL_RES *result,
					my_ulonglong offset);
MYSQL_ROW_OFFSET STDCALL mysql_row_seek(MYSQL_RES *result,
						MYSQL_ROW_OFFSET offset);
MYSQL_FIELD_OFFSET STDCALL mysql_field_seek(MYSQL_RES *result,
					   MYSQL_FIELD_OFFSET offset);
MYSQL_ROW	STDCALL mysql_fetch_row(MYSQL_RES *result);
unsigned long * STDCALL mysql_fetch_lengths(MYSQL_RES *result);
MYSQL_FIELD *	STDCALL mysql_fetch_field(MYSQL_RES *result);
MYSQL_RES *     STDCALL mysql_list_fields(MYSQL *mysql, const char *table,
					  const char *wild);
unsigned long	STDCALL mysql_escape_string(char *to,const char *from,
					    unsigned long from_length);
unsigned long	STDCALL mysql_hex_string(char *to,const char *from,
                                         unsigned long from_length);
unsigned long STDCALL mysql_real_escape_string(MYSQL *mysql,
					       char *to,const char *from,
					       unsigned long length);
void		STDCALL mysql_debug(const char *debug);
void 		STDCALL myodbc_remove_escape(MYSQL *mysql,char *name);
unsigned int	STDCALL mysql_thread_safe(void);
my_bool		STDCALL mysql_embedded(void);
MYSQL_MANAGER*  STDCALL mysql_manager_init(MYSQL_MANAGER* con);  
MYSQL_MANAGER*  STDCALL mysql_manager_connect(MYSQL_MANAGER* con,
					      const char* host,
					      const char* user,
					      const char* passwd,
					      unsigned int port);
void            STDCALL mysql_manager_close(MYSQL_MANAGER* con);
int             STDCALL mysql_manager_command(MYSQL_MANAGER* con,
						const char* cmd, int cmd_len);
int             STDCALL mysql_manager_fetch_line(MYSQL_MANAGER* con,
						  char* res_buf,
						 int res_buf_size);
my_bool         STDCALL mysql_read_query_result(MYSQL *mysql);


/*
  The following definitions are added for the enhanced 
  client-server protocol
*/

/* statement state */
enum enum_mysql_stmt_state
{
  MYSQL_STMT_INIT_DONE= 1, MYSQL_STMT_PREPARE_DONE, MYSQL_STMT_EXECUTE_DONE,
  MYSQL_STMT_FETCH_DONE
};


/*
  This structure is used to define bind information, and
  internally by the client library.
  Public members with their descriptions are listed below
  (conventionally `On input' refers to the binds given to
  mysql_stmt_bind_param, `On output' refers to the binds given
  to mysql_stmt_bind_result):

  buffer_type    - One of the MYSQL_* types, used to describe
                   the host language type of buffer.
                   On output: if column type is different from
                   buffer_type, column value is automatically converted
                   to buffer_type before it is stored in the buffer.
  buffer         - On input: points to the buffer with input data.
                   On output: points to the buffer capable to store
                   output data.
                   The type of memory pointed by buffer must correspond
                   to buffer_type. See the correspondence table in
                   the comment to mysql_stmt_bind_param.

  The two above members are mandatory for any kind of bind.

  buffer_length  - the length of the buffer. You don't have to set
                   it for any fixed length buffer: float, double,
                   int, etc. It must be set however for variable-length
                   types, such as BLOBs or STRINGs.

  length         - On input: in case when lengths of input values
                   are different for each execute, you can set this to
                   point at a variable containining value length. This
                   way the value length can be different in each execute.
                   If length is not NULL, buffer_length is not used.
                   Note, length can even point at buffer_length if
                   you keep bind structures around while fetching:
                   this way you can change buffer_length before
                   each execution, everything will work ok.
                   On output: if length is set, mysql_stmt_fetch will
                   write column length into it.

  is_null        - On input: points to a boolean variable that should
                   be set to TRUE for NULL values.
                   This member is useful only if your data may be
                   NULL in some but not all cases.
                   If your data is never NULL, is_null should be set to 0.
                   If your data is always NULL, set buffer_type
                   to MYSQL_TYPE_NULL, and is_null will not be used.

  is_unsigned    - On input: used to signify that values provided for one
                   of numeric types are unsigned.
                   On output describes signedness of the output buffer.
                   If, taking into account is_unsigned flag, column data
                   is out of range of the output buffer, data for this column
                   is regarded truncated. Note that this has no correspondence
                   to the sign of result set column, if you need to find it out
                   use mysql_stmt_result_metadata.
  error          - where to write a truncation error if it is present.
                   possible error value is:
                   0  no truncation
                   1  value is out of range or buffer is too small

  Please note that MYSQL_BIND also has internals members.
*/

typedef struct st_mysql_bind
{
  unsigned long	*length;          /* output length pointer */
  my_bool       *is_null;	  /* Pointer to null indicator */
  void		*buffer;	  /* buffer to get/put data */
  /* set this if you want to track data truncations happened during fetch */
  my_bool       *error;
  unsigned char *row_ptr;         /* for the current data position */
  void (*store_param_func)(NET *net, struct st_mysql_bind *param);
  void (*fetch_result)(struct st_mysql_bind *, MYSQL_FIELD *,
                       unsigned char **row);
  void (*skip_result)(struct st_mysql_bind *, MYSQL_FIELD *,
		      unsigned char **row);
  /* output buffer length, must be set when fetching str/binary */
  unsigned long buffer_length;
  unsigned long offset;           /* offset position for char/binary fetch */
  unsigned long	length_value;     /* Used if length is 0 */
  unsigned int	param_number;	  /* For null count and error messages */
  unsigned int  pack_length;	  /* Internal length for packed data */
  enum enum_field_types buffer_type;	/* buffer type */
  my_bool       error_value;      /* used if error is 0 */
  my_bool       is_unsigned;      /* set if integer type is unsigned */
  my_bool	long_data_used;	  /* If used with mysql_send_long_data */
  my_bool	is_null_value;    /* Used if is_null is 0 */
  void *extension;
} MYSQL_BIND;


/* statement handler */
typedef struct st_mysql_stmt
{
  MEM_ROOT       mem_root;             /* root allocations */
  LIST           list;                 /* list to keep track of all stmts */
  MYSQL          *mysql;               /* connection handle */
  MYSQL_BIND     *params;              /* input parameters */
  MYSQL_BIND     *bind;                /* output parameters */
  MYSQL_FIELD    *fields;              /* result set metadata */
  MYSQL_DATA     result;               /* cached result set */
  MYSQL_ROWS     *data_cursor;         /* current row in cached result */
  /*
    mysql_stmt_fetch() calls this function to fetch one row (it's different
    for buffered, unbuffered and cursor fetch).
  */
  int            (*read_row_func)(struct st_mysql_stmt *stmt, 
                                  unsigned char **row);
  /* copy of mysql->affected_rows after statement execution */
  my_ulonglong   affected_rows;
  my_ulonglong   insert_id;            /* copy of mysql->insert_id */
  unsigned long	 stmt_id;	       /* Id for prepared statement */
  unsigned long  flags;                /* i.e. type of cursor to open */
  unsigned long  prefetch_rows;        /* number of rows per one COM_FETCH */
  /*
    Copied from mysql->server_status after execute/fetch to know
    server-side cursor status for this statement.
  */
  unsigned int   server_status;
  unsigned int	 last_errno;	       /* error code */
  unsigned int   param_count;          /* input parameter count */
  unsigned int   field_count;          /* number of columns in result set */
  enum enum_mysql_stmt_state state;    /* statement state */
  char		 last_error[MYSQL_ERRMSG_SIZE]; /* error message */
  char		 sqlstate[SQLSTATE_LENGTH+1];
  /* Types of input parameters should be sent to server */
  my_bool        send_types_to_server;
  my_bool        bind_param_done;      /* input buffers were supplied */
  unsigned char  bind_result_done;     /* output buffers were supplied */
  /* mysql_stmt_close() had to cancel this result */
  my_bool       unbuffered_fetch_cancelled;  
  /*
    Is set to true if we need to calculate field->max_length for 
    metadata fields when doing mysql_stmt_store_result.
  */
  my_bool       update_max_length;     
  void *extension;
} MYSQL_STMT;

enum enum_stmt_attr_type
{
  /*
    When doing mysql_stmt_store_result calculate max_length attribute
    of statement metadata. This is to be consistent with the old API, 
    where this was done automatically.
    In the new API we do that only by request because it slows down
    mysql_stmt_store_result sufficiently.
  */
  STMT_ATTR_UPDATE_MAX_LENGTH,
  /*
    unsigned long with combination of cursor flags (read only, for update,
    etc)
  */
  STMT_ATTR_CURSOR_TYPE,
  /*
    Amount of rows to retrieve from server per one fetch if using cursors.
    Accepts unsigned long attribute in the range 1 - ulong_max
  */
  STMT_ATTR_PREFETCH_ROWS
};


typedef struct st_mysql_methods
{
  my_bool (*read_query_result)(MYSQL *mysql);
  my_bool (*advanced_command)(MYSQL *mysql,
			      enum enum_server_command command,
			      const unsigned char *header,
			      unsigned long header_length,
			      const unsigned char *arg,
			      unsigned long arg_length,
			      my_bool skip_check,
                              MYSQL_STMT *stmt);
  MYSQL_DATA *(*read_rows)(MYSQL *mysql,MYSQL_FIELD *mysql_fields,
			   unsigned int fields);
  MYSQL_RES * (*use_result)(MYSQL *mysql);
  void (*fetch_lengths)(unsigned long *to, 
			MYSQL_ROW column, unsigned int field_count);
  void (*flush_use_result)(MYSQL *mysql);
#if !defined(MYSQL_SERVER) || defined(EMBEDDED_LIBRARY)
  MYSQL_FIELD * (*list_fields)(MYSQL *mysql);
  my_bool (*read_prepare_result)(MYSQL *mysql, MYSQL_STMT *stmt);
  int (*stmt_execute)(MYSQL_STMT *stmt);
  int (*read_binary_rows)(MYSQL_STMT *stmt);
  int (*unbuffered_fetch)(MYSQL *mysql, char **row);
  void (*free_embedded_thd)(MYSQL *mysql);
  const char *(*read_statistics)(MYSQL *mysql);
  my_bool (*next_result)(MYSQL *mysql);
  int (*read_change_user_result)(MYSQL *mysql, char *buff, const char *passwd);
  int (*read_rows_from_cursor)(MYSQL_STMT *stmt);
#endif
} MYSQL_METHODS;


MYSQL_STMT * STDCALL mysql_stmt_init(MYSQL *mysql);
int STDCALL mysql_stmt_prepare(MYSQL_STMT *stmt, const char *query,
                               unsigned long length);
int STDCALL mysql_stmt_execute(MYSQL_STMT *stmt);
int STDCALL mysql_stmt_fetch(MYSQL_STMT *stmt);
int STDCALL mysql_stmt_fetch_column(MYSQL_STMT *stmt, MYSQL_BIND *bind_arg, 
                                    unsigned int column,
                                    unsigned long offset);
int STDCALL mysql_stmt_store_result(MYSQL_STMT *stmt);
unsigned long STDCALL mysql_stmt_param_count(MYSQL_STMT * stmt);
my_bool STDCALL mysql_stmt_attr_set(MYSQL_STMT *stmt,
                                    enum enum_stmt_attr_type attr_type,
                                    const void *attr);
my_bool STDCALL mysql_stmt_attr_get(MYSQL_STMT *stmt,
                                    enum enum_stmt_attr_type attr_type,
                                    void *attr);
my_bool STDCALL mysql_stmt_bind_param(MYSQL_STMT * stmt, MYSQL_BIND * bnd);
my_bool STDCALL mysql_stmt_bind_result(MYSQL_STMT * stmt, MYSQL_BIND * bnd);
my_bool STDCALL mysql_stmt_close(MYSQL_STMT * stmt);
my_bool STDCALL mysql_stmt_reset(MYSQL_STMT * stmt);
my_bool STDCALL mysql_stmt_free_result(MYSQL_STMT *stmt);
my_bool STDCALL mysql_stmt_send_long_data(MYSQL_STMT *stmt, 
                                          unsigned int param_number,
                                          const char *data, 
                                          unsigned long length);
MYSQL_RES *STDCALL mysql_stmt_result_metadata(MYSQL_STMT *stmt);
MYSQL_RES *STDCALL mysql_stmt_param_metadata(MYSQL_STMT *stmt);
unsigned int STDCALL mysql_stmt_errno(MYSQL_STMT * stmt);
const char *STDCALL mysql_stmt_error(MYSQL_STMT * stmt);
const char *STDCALL mysql_stmt_sqlstate(MYSQL_STMT * stmt);
MYSQL_ROW_OFFSET STDCALL mysql_stmt_row_seek(MYSQL_STMT *stmt, 
                                             MYSQL_ROW_OFFSET offset);
MYSQL_ROW_OFFSET STDCALL mysql_stmt_row_tell(MYSQL_STMT *stmt);
void STDCALL mysql_stmt_data_seek(MYSQL_STMT *stmt, my_ulonglong offset);
my_ulonglong STDCALL mysql_stmt_num_rows(MYSQL_STMT *stmt);
my_ulonglong STDCALL mysql_stmt_affected_rows(MYSQL_STMT *stmt);
my_ulonglong STDCALL mysql_stmt_insert_id(MYSQL_STMT *stmt);
unsigned int STDCALL mysql_stmt_field_count(MYSQL_STMT *stmt);

my_bool STDCALL mysql_commit(MYSQL * mysql);
my_bool STDCALL mysql_rollback(MYSQL * mysql);
my_bool STDCALL mysql_autocommit(MYSQL * mysql, my_bool auto_mode);
my_bool STDCALL mysql_more_results(MYSQL *mysql);
int STDCALL mysql_next_result(MYSQL *mysql);
void STDCALL mysql_close(MYSQL *sock);


/* status return codes */
#define MYSQL_NO_DATA        100
#define MYSQL_DATA_TRUNCATED 101

#define mysql_reload(mysql) mysql_refresh((mysql),REFRESH_GRANT)

#ifdef USE_OLD_FUNCTIONS
MYSQL *		STDCALL mysql_connect(MYSQL *mysql, const char *host,
				      const char *user, const char *passwd);
int		STDCALL mysql_create_db(MYSQL *mysql, const char *DB);
int		STDCALL mysql_drop_db(MYSQL *mysql, const char *DB);
#define	 mysql_reload(mysql) mysql_refresh((mysql),REFRESH_GRANT)
#endif
#define HAVE_MYSQL_REAL_CONNECT

/*
  The following functions are mainly exported because of mysqlbinlog;
  They are not for general usage
*/

#define simple_command(mysql, command, arg, length, skip_check) \
  (*(mysql)->methods->advanced_command)(mysql, command, 0,  \
                                        0, arg, length, skip_check, NULL)
#define stmt_command(mysql, command, arg, length, stmt) \
  (*(mysql)->methods->advanced_command)(mysql, command, 0,  \
                                        0, arg, length, 1, stmt)

#ifdef __NETWARE__
#pragma pack(pop)		/* restore alignment */
#endif

#ifdef	__cplusplus
}
#endif

#endif /* _mysql_h */
