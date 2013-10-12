/* libquvi
 * Copyright (C) 2012-2013  Toni Gundogdu <legatvs@gmail.com>
 *
 * This file is part of libquvi <http://quvi.sourceforge.net/>.
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General
 * Public License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef l_def_h
#define l_def_h

#define USERDATA_QUVI_T   "_quvi_t"
#define GS_EMPTY_S        ""

/* Media script: Media properties */

#define MS_CAN_PARSE_URL    "can_parse_url" /* Script can parse URL */
#define MS_START_TIME_MS    "start_time_ms" /* Start streaming at */
#define MS_DURATION_MS      "duration_ms"   /* Media duration in ms */
#define MS_INPUT_URL        "input_url"     /* App req. URL */
#define MS_THUMB_URL        "thumb_url"     /* Image thumbnail URL */
#define MS_GOTO_URL         "goto_url"      /* Redirect to URL */
#define MS_DOMAINS          "domains"       /* Accepted domains */
#define MS_TITLE            "title"         /* Media title */
#define MS_ID               "id"            /* Media ID */
#define MS_STREAMS          "streams"       /* Media streams */

/* Media script: stream properties */

#define MSS_VIDEO                 "video"
#define MSS_VIDEO_BITRATE_KBIT_S  "bitrate_kbit_s"
#define MSS_VIDEO_ENCODING        "encoding"
#define MSS_VIDEO_HEIGHT          "height"
#define MSS_VIDEO_WIDTH           "width"

#define MSS_AUDIO                 "audio"
#define MSS_AUDIO_BITRATE_KBIT_S  MSS_VIDEO_BITRATE_KBIT_S
#define MSS_AUDIO_ENCODING        MSS_VIDEO_ENCODING

#define MSS_FLAGS                 "flags"
#define MSS_FLAGS_BEST            "best"

#define MSS_CONTAINER             "container"
#define MSS_URL                   "url"
#define MSS_ID                    "id"

/* Subtitle export script */

#define SUES_CAN_EXPORT_DATA  "can_export_data" /* Script can export data */
#define SUES_EXPORT_FORMAT    "export_format"   /* Script generates this format */
#define SUES_FROM_FORMAT      "from_format"     /* Export from this format */
#define SUES_INPUT_URL        "input_url"       /* Subtitle language URL */
#define SUES_TO_FORMAT        "to_format"       /* Export to this format */
#define SUES_DATA             "data"            /* Exported data */

/* Subtitle script */

#define SUS_CAN_PARSE_URL "can_parse_url"   /* Script can parse URL */
#define SUS_INPUT_URL     "input_url"       /* App req. URL */
#define SUS_DOMAINS       "domains"         /* Domains */
#define SUS_SUBTITLES     "subtitles"       /* Subtitles */

/* Subtitles - qargs.subtitles */

#define SUSS_FORMAT       "format"          /* Internal subtitle format */
#define SUSS_TYPE         "type"            /* Subtitle type {cc,tts,...} */
#define SUSS_LANG         "lang"            /* Subtitles: Language */

/* Subtitles - qargs.subtitles[n].lang */

#define SUSSL_TRANSLATED "translated"  /* Translated (English) language */
#define SUSSL_ORIGINAL   "original"    /* Original language */
#define SUSSL_CODE       "code"        /* Code {en,fi,...} */
#define SUSSL_URL        "url"         /* URL */
#define SUSSL_ID         "id"          /* ID {cc_en,tts_fi,...} */

/* Playlist script */

#define PS_CAN_PARSE_URL  "can_parse_url"   /* Script can parse URL */
#define PS_INPUT_URL      "input_url"       /* App req. URL */
#define PS_THUMB_URL      "thumb_url"       /* Playlist thumbnail URL */
#define PS_DOMAINS        "domains"         /* Domains */
#define PS_TITLE          "title"           /* Title */
#define PS_MEDIA          "media"           /* Found media */
#define PS_ID             "id"              /* Playlist ID */

/* Playlist script: Media properties */

#define PSM_DURATION_MS "duration_ms" /* Duration in ms */
#define PSM_TITLE       "title"       /* Title */
#define PSM_URL         "url"         /* URL */

/* Scan script */

#define SS_INPUT_URL  "input_url" /* App req. URL */
#define SS_MEDIA_URL  "media_url" /* Array of found media URLs */
#define SS_CONTENT    "content"   /* Content to scan */

/* Utility script */

#define US_INPUT_URL "input_url" /* App req. URL */

/* quvi object */

  /* http */
#define QO_CONTENT_LENGTH "content_length"
#define QO_RESPONSE_CODE  "response_code"
#define QO_CONTENT_TYPE   "content_type"
#define QO_RESOLVED_URL   "resolved_url"
#define QO_DATA           "data"
  /* crypto */
#define QO_CIPHERTEXT     "ciphertext"
#define QO_PLAINTEXT      "plaintext"
#define QO_DIGEST         "digest"
  /* base64 */
#define QO_BASE64         "base64"
  /* common */
#define QO_ERROR_MESSAGE  "error_message"
#define QO_QUVI_CODE      "quvi_code"

/* Human-readable quvi object crypto option names */
#define HRE_CIPHER_FLAGS "cipher flags"
#define HRE_CIPHER_MODE  "cipher mode"
#define HRE_CIPHER_KEY   "cipher key"
#define HRE_ALGORITHM    "algorithm"
/* Human-readable quvi object HTTP cookie option names */
#define HRE_COOKIE_MODE  "cookie mode"

/* Lua indexes.
 *
 * Used as 'key' (at index -2) and 'value' (at index -1). Many of
 * the libquvi functions that call lua_next use these indexes. This is
 * similar to (in Lua):
 *   for k,v in pairs(t) do ... end
 */

enum {LI_VALUE = -1, LI_KEY = -2};

#endif /* l_def_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
