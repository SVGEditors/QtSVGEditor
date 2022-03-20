/**
 * \mainpage SuperDog Licensing API
 * \file dog_api.h SuperDog Licensing API declarations
 *
 * Copyright (C) 2016 SafeNet, Inc. All rights reserved.
 *
 */


#ifndef __DOG_API_H__
#define __DOG_API_H__

#if !defined(WITH_AKSTYPES) && !defined(WITH_OEFTYPES)
#  if defined(_MSC_VER) || defined(__WATCOMC__) || defined(__BORLANDC__)
typedef unsigned __int64 dog_u64_t;
typedef signed __int64 dog_s64_t;
#  else
typedef unsigned long long dog_u64_t;
typedef signed long long dog_s64_t;
#  endif
#  if defined(_MSC_VER)
typedef unsigned long dog_u32_t;
typedef signed long dog_s32_t;
#  else
typedef unsigned int dog_u32_t;
typedef signed int dog_s32_t;
#  endif
typedef unsigned short dog_u16_t;
typedef signed short dog_s16_t;
typedef signed char dog_s8_t;
typedef unsigned char dog_u8_t;
#endif

#if defined(_WIN32) || defined(WIN32) || defined(_MSC_VER) || defined(__BORLANDC__)
#  define DOG_CALLCONV __stdcall
#else
#  define DOG_CALLCONV
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup dog_feature_ids Feature ID defines
 *
 * @{
 */

/**
 * \brief The SuperDog default Feature ID
 *
 * Available in every SuperDog
 */
#define DOG_DEFAULT_FID 0

/**
 * @}
 */



/**
 * @defgroup dog_file_ids Data File ID defines
 *
 * @{
 */

/**
 * \brief File ID for default read-write data file
 *
 * File ID for default read-write data file.
 */

#define DOG_FILEID_RW 0xfff4

/**
 * @}
 */



/**
 * @defgroup dog_error_codes Licensing API Status Codes
 *
 * @{
 */

enum dog_error_codes
{
    /** Request successfully completed */
    DOG_STATUS_OK = 0,

    /** Request exceeds data file range */
    DOG_MEM_RANGE = 1,

    /** System is out of memory */
    DOG_INSUF_MEM = 3,

    /** Too many open login sessions */
    DOG_TMOF = 4,

    /** Access denied */
    DOG_ACCESS_DENIED = 5,

    /** Required SuperDog not found */
    DOG_NOT_FOUND = 7,

    /** Encryption/decryption data length is too short */
    DOG_TOO_SHORT = 8,

    /** Invalid input handle */
    DOG_INV_HND = 9,

    /** Specified File ID not recognized by API */
    DOG_INV_FILEID = 10,

    /** Invalid XML format */
    DOG_INV_FORMAT = 15,

    /** Unable to execute function in this context */
    DOG_REQ_NOT_SUPP = 16,

    /** Binary data passed to function does not contain valid update */
    DOG_INV_UPDATE_OBJ = 17,

    /** SuperDog to be updated not found */
    DOG_KEYID_NOT_FOUND = 18,

    /** Required XML tags not found; Contents in binary data are missing
     * or invalid */
    DOG_INV_UPDATE_DATA = 19,

    /** Update not supported by SuperDog */
    DOG_INV_UPDATE_NOTSUPP = 20,

    /** Update counter is set incorrectly */
    DOG_INV_UPDATE_CNTR = 21,

    /** Invalid Vendor Code passed */
    DOG_INV_VCODE = 22,

    /** Passed time value is outside supported value range */
    DOG_INV_TIME = 24,

    /** Acknowledge data requested by the update, however the ack_data
     * input parameter is NULL */
    DOG_NO_ACK_SPACE = 26,

    /** Program running on a terminal server */
    DOG_TS_DETECTED = 27,

    /** Unknown algorithm used in V2C file */
    DOG_UNKNOWN_ALG = 29,

    /** Signature verification failed */
    DOG_INV_SIG = 30,

    /** Requested Feature not available */
    DOG_FEATURE_NOT_FOUND = 31,

    /** Communication error between API and local SuperDog License Manager */
    DOG_LOCAL_COMM_ERR = 33,

    /** Vendor Code not recognized by API */
    DOG_UNKNOWN_VCODE = 34,

    /** Invalid XML specification */
    DOG_INV_SPEC = 35,

    /** Invalid XML scope */
    DOG_INV_SCOPE = 36,

    /** Too many SuperDog currently connected */
    DOG_TOO_MANY_KEYS = 37,

    /** Session was interrupted */
    DOG_BROKEN_SESSION = 39,

    /** Feature has expired */
    DOG_FEATURE_EXPIRED = 41,

    /** SuperDog License Manager version too old */
    DOG_OLD_LM = 42,

    /** USB error occurred when communicating with a SuperDog */
    DOG_DEVICE_ERR = 43,

    /** System time has been tampered with */
    DOG_TIME_ERR = 45,

    /** Communication error occurred in secure channel */
    DOG_SCHAN_ERR = 46,

    /** Corrupt data exists in secure storage area of SuperDog SL */
    DOG_STORAGE_CORRUPT = 47,

    /** Unable to locate a Feature matching the scope */
    DOG_SCOPE_RESULTS_EMPTY = 50,

    /** SuperDog SL incompatible with machine hardware; SuperDog SL is locked
     * to different hardware. OR:
     * In the case of a V2C file, conflict between SuperDog SL data and machine
     * hardware data; SuperDog SL locked to different hardware */
    DOG_HARDWARE_MODIFIED = 52,

    /** Trying to install a V2C file with an update counter that is out
     * of sequence with the update counter in the SuperDog.
     * The values of the update counter in the file are lower than
     * those in the SuperDog. */
    DOG_UPDATE_TOO_OLD = 54,

    /** Trying to install a V2C file with an update counter that is out
     * of sequence with the update counter in the SuperDog.
     * The first value of the update counter in the file is greater than
     * the value in the SuperDog. */
    DOG_UPDATE_TOO_NEW = 55,

    /** Cloned SuperDog SL secure storage detected */
    DOG_CLONE_DETECTED = 64,

    /** Specified V2C update already installed */
    DOG_UPDATE_ALREADY_ADDED = 65,

    /** Secure storage ID mismatch */
    DOG_SECURE_STORE_ID_MISMATCH = 78,

    /** Unable to locate dynamic library for API */
    DOG_NO_API_DYLIB = 400,

    /** Dynamic library for API is invalid */
    DOG_INV_API_DYLIB = 401,

    /** Object incorrectly initialized */
    DOG_INVALID_OBJECT = 500,

    /** Invalid function parameter */
    DOG_INVALID_PARAMETER = 501,

    /** Logging in twice to the same object */
    DOG_ALREADY_LOGGED_IN = 502,

    /** Logging out twice of the same object */
    DOG_ALREADY_LOGGED_OUT = 503,

    /** Incorrect use of system or platform */
    DOG_OPERATION_FAILED = 525,

    /** Requested function not implemented */
    DOG_NOT_IMPL = 698,

    /** Internal error occurred in API */
    DOG_INT_ERR = 699,

    DOG_NEXT_FREE_VALUES = 7001,
};

/**
 * @}
 */



/**
 * @defgroup dog_general SuperDog typedefs and macros
 *
 * @{
 */

/** A SuperDog status code */
typedef enum dog_error_codes dog_status_t;

/** SuperDog size type */
typedef dog_u32_t dog_size_t;

/** SuperDog connection handle */
typedef dog_u32_t dog_handle_t;

/** SuperDog Feature ID */
typedef dog_u32_t dog_feature_t;

/** SuperDog File ID */
typedef dog_u32_t dog_fileid_t;

/** SuperDog timestamp, representing elapsed seconds since
 * Jan-01-1970 0:00:00 GMT */
typedef dog_u64_t dog_time_t;

/** SuperDog Vendor Code buffer */
typedef const void *dog_vendor_code_t;

/** format to retrieve update info (C2V) */
#define DOG_UPDATEINFO     "<dogformat format=\"updateinfo\"/>"

/** format to retrieve session info */
#define DOG_SESSIONINFO    "<dogformat format=\"sessioninfo\"/>"

/** format to retrieve hardware info */
#define DOG_KEYINFO        "<dogformat format=\"keyinfo\"/>"

/** format to retrieve host fingerprint info */
#define DOG_FINGERPRINT    "<dogformat format=\"host_fingerprint\"/>"

/**
 * Invalid handle value for dog_login() and dog_login_scope() functions.
 */
#define DOG_INVALID_HANDLE_VALUE 0

/**
 * \brief Minimum block size for dog_encrypt() and dog_decrypt() functions.
 */
#define DOG_MIN_BLOCK_SIZE 16

/**
 * @}
 */



/**
 * @defgroup dog_basic The Basic SuperDog Licensing API
 *
 * @{
 */


dog_status_t DOG_CALLCONV dog_login(dog_feature_t feature_id,
                dog_vendor_code_t vendor_code,
                dog_handle_t *handle);


dog_status_t DOG_CALLCONV dog_login_scope(dog_feature_t feature_id,
                const char *scope,
                dog_vendor_code_t vendor_code,
                dog_handle_t *handle);


dog_status_t DOG_CALLCONV dog_logout(dog_handle_t handle);


dog_status_t DOG_CALLCONV dog_encrypt(dog_handle_t handle,
                void *buffer,
                dog_size_t length);


dog_status_t DOG_CALLCONV dog_decrypt(dog_handle_t handle,
                void *buffer,
                dog_size_t length);


dog_status_t DOG_CALLCONV dog_read(dog_handle_t handle,
                dog_fileid_t fileid,
                dog_size_t offset,
                dog_size_t length,
                void *buffer);


dog_status_t DOG_CALLCONV dog_write(dog_handle_t handle,
                dog_fileid_t fileid,
                dog_size_t offset,
                dog_size_t length,
                const void *buffer);


dog_status_t DOG_CALLCONV dog_get_size(dog_handle_t handle,
                dog_fileid_t fileid,
                dog_size_t *size);


dog_status_t DOG_CALLCONV dog_get_time(dog_handle_t handle,
                dog_time_t *time);


dog_status_t DOG_CALLCONV dog_get_info(const char *scope,
                const char *format,
                dog_vendor_code_t vendor_code,
                char **info);


dog_status_t DOG_CALLCONV dog_get_sessioninfo(dog_handle_t handle,
                const char *format,
                char **info);


void DOG_CALLCONV dog_free(char *info);
   

dog_status_t DOG_CALLCONV dog_update(const char *update_data,
                char **ack_data);


dog_status_t DOG_CALLCONV dog_get_version(unsigned int *major_version,
                unsigned int *minor_version,
                unsigned int *build_server,
                unsigned int *build_number,
                dog_vendor_code_t vendor_code);


dog_status_t DOG_CALLCONV dog_datetime_to_dogtime(unsigned int day,
                unsigned int month,
                unsigned int year,
                unsigned int hour,
                unsigned int minute,
                unsigned int second,
                dog_time_t *time);


dog_status_t DOG_CALLCONV dog_dogtime_to_datetime(dog_time_t time,
                unsigned int *day,
                unsigned int *month,
                unsigned int *year,
                unsigned int *hour,
                unsigned int *minute,
                unsigned int *second);

/**
 * @}
 */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __DOG_API_H__ */
