/* 
* Copyright (C) 2009,2010 Toni Gundogdu.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file quvi.h
 * @mainpage libquvi C API
 *
 * This documentation describes the libquvi C API.
 *
 * @author Toni Gundogdu
 * @version 0.1.3
 * @example quvi.c
 * @example simple.c
 */

#ifndef quvi_h
#define quvi_h

#ifndef DOXY_SKIP
#include <stdlib.h>
#include <stdint.h> /* C99 header */
#endif

/** @defgroup libquvi_types Types
 * Describes the constants and the types used with the API.
 * @{
 */

/** @enum QUVIversion Types used with quvi_version() */
typedef enum {
QUVI_VERSION = 0x00,    /**< Version string only */
QUVI_VERSION_LONG,      /**< Version string, build date and misc. features */
} QUVIversion;

/** @enum QUVIcode Return codes */
typedef enum {
/* Static errors */
QUVI_OK  = 0x00,        /**< OK */
QUVI_MEM,               /**< Memory allocation failed */
QUVI_BADHANDLE,         /**< Bad session handle */
QUVI_INVARG,            /**< Invalid function argument */
QUVI_CURLINIT,          /**< libcurl initialization failed */
QUVI_LAST,              /**< Indicates end of list iteration */
QUVI_ABORTEDBYCALLBACK, /**< Aborted by callback function */
_INTERNAL_QUVI_LAST,    /**< For library internal use only */
/* Dynamically generated errors */
QUVI_PCRE = 0x40, /**< libpcre error occurred */
QUVI_NOSUPPORT,   /**< libquvi does not support the video host */
QUVI_CURL,        /**< libcurl error occurred */
QUVI_ICONV,       /**< libiconv error occurred */
} QUVIcode;

/** @enum QUVIstatus Status codes */
typedef enum {
QUVISTATUS_FETCH  = 0x00,  /**< Status changed to fetch (page, config, etc.) */
QUVISTATUS_VERIFY,         /**< Status changed to verify video link */
} QUVIstatus;

/** @enum QUVIstatusType Status type codes */
typedef enum {
/* QUVISTATUS_FETCH: */
QUVISTATUSTYPE_PAGE = 0x00,   /**< Fetching video page */
QUVISTATUSTYPE_CONFIG,        /**< Fetching config */
QUVISTATUSTYPE_PLAYLIST,      /**< Fetching playlist */
/* Generic types: */
QUVISTATUSTYPE_DONE,          /**< General purpose "done" status type */
} QUVIstatusType;

/** @enum QUVIoption Option codes to be used with quvi_setopt()
*
* If you use QUVIOPT_NOVERIFY, the library will return zero for video
* file length and an empty string for video suffix. Both of them are
* parsed during the video link verification process from the server
* returned HTTP header fields content-length and content-type.
*/
typedef enum {
QUVIOPT_FORMAT = 0x00,  /**< Requested video file format */
QUVIOPT_NOVERIFY,       /**< Do not verify video link */
QUVIOPT_STATUSFUNCTION, /**< Callback function for status updates */
QUVIOPT_WRITEFUNCTION,   /**< Callback function for writing data */
} QUVIoption;

#define QUVIINFO_VOID       0x100000 /**< void type */
#define QUVIINFO_LONG       0x200000 /**< long type */
#define QUVIINFO_STRING     0x300000 /**< string type */
#define QUVIINFO_DOUBLE     0x400000 /**< double type */
#define QUVIINFO_TYPEMASK   0xf00000 /**< type mask */

/** @enum QUVIinfo Info codes to be used with quvi_getinfo()
*
* The library creates a cURL handle which is used to fetch and
* verify parsed video details. The cURL handle is initialized
* with the following libcurl options:
*   - CURLOPT_USERAGENT         ("Mozilla/5.0")
*   - CURLOPT_FOLLOWLOCATION    (1)
*   - CURLOPT_NOBODY            (0)
* 
* You can, of course, override those settings in your program. You
* can even use the cURL handle in your program until quvi_close()
* is called which will release the handle. See the src/quvi.c for
* an example of this. Note that libquvi uses the libcurl easy
* interface and not the multi interface.
*
* @warning If you use the libquvi created cURL handle in your program,
* leave the releasing of the handle for the library to do.
*/
typedef enum {
QUVIINFO_NONE        = 0x00,             /**< Unused */
QUVIINFO_CURL        = QUVIINFO_VOID + 1,/**< Session libcurl handle */
QUVIINFO_CURLCODE    = QUVIINFO_LONG + 2,/**< Last libcurl returned code */
QUVIINFO_HTTPCODE    = QUVIINFO_LONG + 3,/**< Last libcurl returned HTTP code */
/* Add new ones below. */
_QUVIINFO_LAST        = 3                 /**< Placeholder */
} QUVIinfo;

#define QUVIPROPERTY_STRING     0x100000 /**< string type */
#define QUVIPROPERTY_LONG       0x200000 /**< long type */
#define QUVIPROPERTY_DOUBLE     0x300000 /**< double type */
#define QUVIPROPERTY_TYPEMASK   0xf00000 /**< type mask */

/** @enum QUVIproperty Video property codes to be used with quvi_getprop() */
typedef enum {
QUVIPROP_NONE          = 0x00, /**< Unused */
QUVIPROP_HOSTID        = QUVIPROPERTY_STRING + 1, /**< Host ID */
QUVIPROP_PAGEURL       = QUVIPROPERTY_STRING + 2, /**< Video page link */
QUVIPROP_PAGETITLE     = QUVIPROPERTY_STRING + 3, /**< Video title */
QUVIPROP_VIDEOID       = QUVIPROPERTY_STRING + 4, /**< Video ID */
QUVIPROP_VIDEOURL      = QUVIPROPERTY_STRING + 5, /**< Video URL */
QUVIPROP_VIDEOFILELENGTH       = QUVIPROPERTY_DOUBLE + 6, /**< Video file length */
QUVIPROP_VIDEOFILECONTENTTYPE  = QUVIPROPERTY_STRING + 7, /**< Video file content-type */
QUVIPROP_VIDEOFILESUFFIX       = QUVIPROPERTY_STRING + 8, /**< Video file suffix */
QUVIPROP_HTTPCODE      = QUVIPROPERTY_LONG   + 9, /**< Last libcurl returned HTTP code */
QUVIPROP_VIDEOFORMAT   = QUVIPROPERTY_STRING + 10, /**< Requested video format, set using QUVIOPT_FORMAT */
/* Add new ones below. */
_QUVIPROP_LAST          = 10 /**< Placeholder */
} QUVIproperty;

/** @brief libquvi session handle */
typedef void *quvi_t;
/** @brief Video parsing session handle */
typedef void *quvi_video_t;

/**
 * @brief Status callback function
 *
 * Callback function for status changes.
 *
 * @note Returning a non-zero value from the callback function will stop libquvi
 *
 * Example:
 * @code
 * static int
 * status_callback (long param, void *data) {
 *   quvi_word status, type;
 *   status = quvi_loword(param);
 *   type   = quvi_hiword(param);
 *   switch (status) {
 *   case QUVISTATUS_FETCH:
 *     switch (type) {
 *        default: printf(":: Fetch %s ...", (char *)data); break;
 *        case QUVISTATUSTYPE_CONFIG  : printf(":: Fetch config ..."); break;
 *        case QUVISTATUSTYPE_PLAYLIST: printf(":: Fetch playlist ..."); break;
 *        case QUVISTATUSTYPE_DONE    : puts("done."); break;
 *      }
 *      break;
 *   case QUVISTATUS_VERIFY:
 *     switch (type) {
 *     default: printf(":: Verify video link ..."); break;
 *     case QUVISTATUSTYPE_DONE: puts("done."); break;
 *     }
 *     break;
 *   }
 *   fflush(stdout);
 * }
 * int
 * main (int argc, char *argv[]) {
 *   quvi_t quvi;
 *   quvi_init(&quvi);
 *   quvi_setopt(quvi, QUVIOPT_STATUSFUNCTION, status_callback);
 *   ...
 * }
 * @endcode
 */
typedef int (*quvi_callback_status) (long param, void *data);

#define QUVI_WRITEFUNC_ABORT 0x10000000 /**< Abort writing */

/**
 * @brief Write callback function
 *
 * This function gets called by libquvi (or libcurl) as soon as
 * there is data received that needs to be saved.
 *
 * @note Returning QUVI_WRITEFUNC_ABORT from the callback function will stop write
 */
typedef int (*quvi_callback_write) (char *buffer,
                                    size_t size,
                                    size_t nitems,
                                    void *instream);

/** @brief Word type */
typedef uint32_t quvi_word;
/** @brief Byte type */
typedef uint8_t  quvi_byte;

/** @} */ /* End of libquvi_types group. */


/** @defgroup libquvi_macros Macros
 * Describes the available macros.
 * @{
 */

/** @brief Return a low byte from a word type variable  */
#define quvi_lobyte(w) ((quvi_byte)((uint64_t)(w) & 0xff))
/** @brief Return a high byte from a word type variable */
#define quvi_hibyte(w) ((quvi_byte)((uint64_t)(w) >> 8))

/** @brief Return a low word from a long type variable */
#define quvi_loword(l) ((quvi_word)((uint64_t)(l) & 0xffff))
/** @brief Return a high word from a long type variable */
#define quvi_hiword(l) ((quvi_word)((uint64_t)(l) >> 16))

/** @} */ /* End of libquvi_macros group. */


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @defgroup libquvi_api Functions
 * Describes the available C API functions.
 * @{
 */

/** @defgroup libquvi_init Initializing
 * @{
 */

/**
* @brief Start a new libquvi session
*
* @param quvi Pointer to a new handle
*
* @note 
*   - Close each created handle with quvi_close() when done using it
*   - libcurl will use the http_proxy environment setting if it is set
*   - See http://curl.haxx.se/libcurl/c/curl_easy_setopt.html on how to override that
*
* @return Non-zero if an error occurred
*
* @sa quvi_close
*
* Example:
* @code
* #include <quvi/quvi.h>
* ...
* quvi_t quvi;
* quvi_init(&quvi);
* @endcode
*/
QUVIcode quvi_init(quvi_t *quvi);


/** @defgroup libquvi_release Releasing
 * @{
 */

/**
* @brief End a libquvi session
*
* @param quvi Handle to a session
*
* @sa quvi_init
*
* Example:
* @code
* quvi_t quvi;
* quvi_init(&quvi);
* ...
* quvi_close(&quvi);
* @endcode
*/
void quvi_close(quvi_t *quvi);

/** @} */ /* End of libquvi_release group. */
/** @} */ /* End of libquvi_init group. */


/** @defgroup libquvi_setup Setting up
 * @{
 */

/** 
* @brief Set options for a libquvi session handle
* 
* @param quvi Handle to a libquvi session
* @param opt Option ID
* @param ... Parameter
* 
* @return Non-zero if an error occurred
*
* @sa QUVIoption
*
* Example:
* @code
* quvi_t quvi;
* quvi_init(&quvi);
* quvi_setopt(quvi, QUVIOPT_FORMAT, "hd");
* ...
* @endcode
*/
QUVIcode quvi_setopt(quvi_t quvi, QUVIoption opt, ...);

/**
 * @brief Default write callback.
 *
 * This is the default write callback function that libquvi
 * uses to write data received using libcurl.
 *
 * @return The number of bytes actually taken care of
 */
size_t quvi_write_callback_default(void *ptr, size_t size, size_t nmemb, void *stream);

/** @} */ /* End of libquvi_setup group. */


/** @defgroup libquvi_getinfo Getting session information
 * @{
 */

/** 
* @brief Get information from a libquvi session handle
* 
* @param quvi Handle to a libquvi session
* @param info Info ID
* @param ... Parameter
* 
* @return Non-zero if an error occurred
*
* @warning Do not attempt to free the memory returned by this function
*
* @sa QUVIinfo
*
* Example:
* @code
* CURL curl;
* long last_httpcode;
* ...
* quvi_getinfo(quvi, QUVIINFO_CURL, &curl);
* quvi_getinfo(quvi, QUVIINFO_HTTPCODE, &last_httpcode);
* ...
* @endcode
*/
QUVIcode quvi_getinfo(quvi_t quvi, QUVIinfo info, ...);

/** @} */ /* End of libquvi_getinfo group. */


/** @defgroup libquvi_parse Parsing video links
 * @{
 */

/**
* @brief Start a new video parsing session
*
* @param quvi Handle to a session
* @param url Null-terminated string to an URL
* @param video Pointer to a new video session handle
*
* @note Close each created handle when done and/or before reusing it
*
* @return Non-zero if an error occurred
*
* @sa quvi_parse_close
*
* Example:
* @code
* quvi_t quvi;
* quvi_video_t video;
* quvi_init(&quvi);
* quvi_parse(quvi, "http://www.youtube.com/watch?v=DeWsZ2b_pK4", &video);
* ...
* @endcode
*/
QUVIcode quvi_parse(quvi_t quvi, char *url, quvi_video_t *video);


/** @defgroup libquvi_getprop Getting parsed video properties
 * @{
 */

/** 
* @brief Get video property information from a video session handle
* 
* @param video Handle to a video session
* @param prop Property ID
* @param ... Parameter
* 
* @return Non-zero if an error occurred
*
* @warning Do not attempt to free the memory returned by this function
*
* @sa QUVIproperty
*
* Example:
* @code
* char *url;
* ...
* quvi_getprop(video, QUVIPROP_VIDEOURL, &url);
* puts(url);
* ...
* @endcode
*/
QUVIcode quvi_getprop(quvi_video_t video, QUVIproperty prop, ...);

/** @} */ /* End of libquvi_getprop group. */


/** @defgroup libquvi_parse_util Utility functions
 * @{
 */

/**
* @brief Move to the next video link (if any)
*
* Used to iterate video links for the parsed video page.
* While most of the supported websites typically do not
* split videos into segments, some, like cctv are known
* to do so.
*
* @param video Handle to a video session
*
* @return Non-zero if end of list was reached (QUVI_LAST) or an error occurred
*
* @sa quvi_getprop
*
* Example:
* @code
* char *url;
* do {
*   quvi_getprop(video, QUVIPROP_VIDEOURL, &url);
*   puts(url);
* } while (quvi_next_videolink(video) == QUVI_OK);
* @endcode
*/
QUVIcode quvi_next_videolink(quvi_video_t video);

/** @} */ /* End of libquvi_parse_util group. */


/** @defgroup libquvi_parse_release Releasing
 * @{
 */

/**
* @brief End a video parsing session
*
* @param video Pointer to a video session
*
* @sa quvi_parse
*
* Example:
* @code
* quvi_t quvi;
* quvi_video_t video;
* ...
* quvi_init(&quvi);
* quvi_parse(quvi, ..., &video);
* quvi_parse_close(&video);
* quvi_close(&quvi);
* @endcode
*/
void quvi_parse_close(quvi_video_t *video);

/** @} */ /* End of libquvi_parse_release group */
/** @} */ /* End of libquvi_parse group */


/** @defgroup libquvi_misc Misc. utility functions
 * @{
 */

/** 
* @brief Next supported video host
*
* Iterate over the list of the supported video hosts.
* 
* @param domain Pointer to a null-terminated string (e.g. youtube.com)
* @param formats Pointer to a null-terminated string (e.g. flv, delim. '|')
*
* @return Non-zero if end of list was reached (QUVI_LAST) or an error occurred
*
* @warning Do not attempt to free the memory returned by this function
*
* Example:
* @code
* char *domain=0, *formats=0;
* while (quvi_next_host(&domain, &formats) == QUVI_OK)
*   printf("%s\t%s\n", domain, formats);
* @endcode
*/
QUVIcode quvi_next_host(char **domain, char **formats);

/** 
* @brief Return a string describing the error code
* 
* @param quvi Handle to a libquvi session
* @param code Error code
* 
* @return Null-terminated string
*
* @warning Do not attempt to free the memory returned by this function
*
* Example:
* @code
* quvi_t quvi;
* QUVIcode rc = quvi_init(&quvi);
* if (rc != QUVI_OK) {
*   fprintf(stderr, "error: %s\n", quvi_strerror(quvi,rc));
*   exit (rc);
* }
* quvi_close(&quvi);
* @endcode
*/
char *quvi_strerror(quvi_t quvi, QUVIcode code);

/** 
* @brief Return libquvi version
*
* @param type Version type
* 
* @return Null-terminated string
*
* @warning Do not attempt to free the memory returned by this function
*
* Example:
* @code
* puts( quvi_version(QUVI_VERSION_LONG) );
* @endcode
*/
char *quvi_version(QUVIversion type);

/** @} */ /* End of libquvi_misc group. */
/** @} */ /* End of libquvi_api group. */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* quvi_h */


