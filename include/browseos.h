#ifndef BROWSEOS_H
#define BROWSEOS_H

#define BROWSEOS_VERSION "3.0.0"
#define BROWSEOS_CODENAME "Golden Sakura"
#define BROWSEOS_BUILD 20260529
#define MAX_INPUT 4096
#define MAX_PATH 512
#define MAX_URL 2048
#define MAX_TITLE 512
#define MAX_LINE 65536
#define MAX_CONTENT 4194304
#define MAX_CACHE 256
#define MAX_COOKIES 512
#define MAX_HISTORY 1024
#define MAX_BOOKMARKS 512
#define MAX_TABS 64
#define MAX_TAB_STACK 16
#define MAX_DOWNLOADS 64
#define MAX_EXTENSIONS 32
#define MAX_PROCESSES 64
#define MAX_LANG_STR 4096
#define MAX_ENGINES 16
#define MAX_LANGUAGES 24
#define MAX_RENDER_BUF 262144
#define MAX_BOOKMARK_FOLDERS 32
#define MAX_CERTIFICATES 64
#define MAX_UA_STR 256
#define MAX_PROXY_STR 128
#define MAX_CMD_HISTORY 256
#define MAX_DOMAIN 256
#define MAX_HEADERS 128
#define MAX_PARAMS 64
#define MAX_SCRIPT_VARS 256

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>
#include <setjmp.h>
#include <signal.h>

/* ============================================================
 * COLOR / THEME SYSTEM
 * ============================================================ */
typedef enum {
    COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW,
    COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE,
    COLOR_BRIGHT_BLACK, COLOR_BRIGHT_RED, COLOR_BRIGHT_GREEN,
    COLOR_BRIGHT_YELLOW, COLOR_BRIGHT_BLUE, COLOR_BRIGHT_MAGENTA,
    COLOR_BRIGHT_CYAN, COLOR_BRIGHT_WHITE, COLOR_RESET
} Color;

typedef enum {
    THEME_DEFAULT, THEME_DARK, THEME_LIGHT, THEME_HACKER,
    THEME_OCEAN, THEME_SUNSET, THEME_FOREST, THEME_MIDNIGHT,
    THEME_RETRO, THEME_AMBER, THEME_DRACULA, THEME_NORD,
    THEME_SOLARIZED, THEME_TOKYO_NIGHT, THEME_GRUVBOX, THEME_CATPPUCCIN
} Theme;

typedef struct {
    Color fg, bg;
    char name[32];
    char desc[64];
    Color prompt_fg;
    Color status_fg;
    Color error_fg;
    Color warn_fg;
    Color info_fg;
    Color link_fg;
    Color heading_fg;
} ThemeConfig;

/* ============================================================
 * BROWSER ENGINE TYPES
 * ============================================================ */
typedef enum {
    ENGINE_WEBKIT,       /* Safari, WebKit2 */
    ENGINE_GECKO,        /* Firefox, GeckoView */
    ENGINE_BLINK,        /* Chrome, Edge, Chromium */
    ENGINE_TRIDENT,      /* Internet Explorer */
    ENGINE_EDGEHTML,     /* Legacy Microsoft Edge */
    ENGINE_SERVO,        /* Mozilla Servo (research) */
    ENGINE_GOANNA,       /* Pale Moon, Basilisk */
    ENGINE_NETSURF,      /* NetSurf lightweight */
    ENGINE_LIBWEB,       /* LibWeb (SerenityOS) */
    ENGINE_FLOW,         /* Flow browser engine */
    ENGINE_CUSTOM
} EngineType;

typedef enum {
    RENDER_STANDARD, RENDER_QUIRKS, RENDER_ALMOST_STANDARD,
    RENDER_LIMITED
} RenderMode;

typedef enum {
    JS_NONE, JS_BASIC, JS_INTERMEDIATE, JS_FULL
} JSSupport;

typedef enum {
    CSS_NONE, CSS_BASIC, CSS_INTERMEDIATE, CSS_FULL
} CSSSupport;

typedef struct {
    EngineType type;
    char name[32];
    char version[16];
    char vendor[64];
    char description[512];
    char user_agent[MAX_UA_STR];
    int speed_rating;        /* 1-100 */
    int memory_rating;       /* 1-100 (lower = better) */
    int standard_compliance; /* 1-100 */
    int html5_score;         /* 1-100 */
    int css3_score;          /* 1-100 */
    int js_score;            /* 1-100 */
    RenderMode render_mode;
    JSSupport js_support;
    CSSSupport css_support;
    int supports_webgl;
    int supports_wasm;
    int supports_webassembly;
    int supports_webrtc;
    int supports_webgpu;
    int supports_service_workers;
    int supports_web_components;
    int supports_canvas;
    int supports_svg;
    int supports_mathml;
    int supports_pointer_events;
    int supports_intersection_observer;
    int supports_css_grid;
    int supports_css_variables;
    int supports_css_flexbox;
    int supports_css_animations;
    int supports_css_transforms;
    int supports_drag_drop;
    int supports_geolocation;
    int supports_web_workers;
    int supports_indexed_db;
    int supports_local_storage;
    int supports_session_storage;
    int supports_web_sockets;
    int supports_fetch_api;
    int supports_http2;
    int supports_http3;
    int supports_tls13;
    int active;
} Engine;

/* ============================================================
 * NETWORK / PROTOCOL SYSTEM
 * ============================================================ */
typedef enum {
    PROTOCOL_HTTP, PROTOCOL_HTTPS, PROTOCOL_FTP, PROTOCOL_FILE,
    PROTOCOL_DATA, PROTOCOL_JAVASCRIPT, PROTOCOL_ABOUT,
    PROTOCOL_CHROME, PROTOCOL_EDGE, PROTOCOL_BLOB,
    PROTOCOL_MAGNET, PROTOCOL_MAILTO, PROTOCOL_TELNET,
    PROTOCOL_SSH, PROTOCOL_WS, PROTOCOL_WSS, PROTOCOL_UNKNOWN
} Protocol;

typedef enum {
    HTTP_GET, HTTP_POST, HTTP_PUT, HTTP_DELETE, HTTP_PATCH,
    HTTP_HEAD, HTTP_OPTIONS, HTTP_TRACE, HTTP_CONNECT
} HttpMethod;

typedef enum {
    HTTP_100, HTTP_101, HTTP_200, HTTP_201, HTTP_204,
    HTTP_301, HTTP_302, HTTP_303, HTTP_304, HTTP_307, HTTP_308,
    HTTP_400, HTTP_401, HTTP_403, HTTP_404, HTTP_405, HTTP_408,
    HTTP_500, HTTP_502, HTTP_503, HTTP_504
} HttpStatus;

typedef struct {
    char name[64];
    char value[MAX_PATH];
} HttpHeader;

typedef struct {
    HttpMethod method;
    char url[MAX_URL];
    char host[MAX_DOMAIN];
    char path[MAX_URL];
    char query[MAX_URL];
    char fragment[MAX_URL];
    Protocol protocol;
    int port;
    HttpStatus status;
    HttpHeader request_headers[MAX_HEADERS];
    int num_request_headers;
    HttpHeader response_headers[MAX_HEADERS];
    int num_response_headers;
    char *body;
    int body_capacity;
    int body_length;
    double load_time;
    int cached;
    int secure;
    int compressed;
    int chunked;
} HttpResponse;

typedef struct {
    char domain[MAX_DOMAIN];
    char path[MAX_URL];
    char name[128];
    char value[MAX_PATH];
    int secure;
    int http_only;
    time_t expires;
    int session;
} Cookie;

typedef struct {
    char url[MAX_URL];
    char content_type[64];
    int size;
    time_t timestamp;
    int expiry;
    char etag[128];
    char last_modified[64];
    int fresh;
} CacheEntry;

typedef struct {
    char host[MAX_DOMAIN];
    int port;
    int enabled;
    char username[64];
    char password[64];
    int socks;
} ProxyConfig;

typedef struct {
    char issuer[128];
    char subject[128];
    char serial[64];
    time_t valid_from;
    time_t valid_to;
    char fingerprint[128];
    int trusted;
    char domain[MAX_DOMAIN];
} Certificate;

/* ============================================================
 * RENDERING / DOM SYSTEM
 * ============================================================ */
typedef enum {
    NODE_TEXT, NODE_ELEMENT, NODE_COMMENT, NODE_DOCUMENT,
    NODE_DOCTYPE, NODE_SCRIPT, NODE_STYLE
} NodeType;

typedef struct DOMNode {
    NodeType type;
    char tag_name[32];
    char text[MAX_LINE];
    char attributes[MAX_PARAMS][MAX_PATH];
    char attr_names[MAX_PARAMS][64];
    int num_attributes;
    int num_children;
    int capacity_children;
    struct DOMNode **children;
    struct DOMNode *parent;
    void *style;
    int rendered;
    int visible;
    int x, y, width, height;
} DOMNode;

typedef struct {
    char property[64];
    char value[MAX_PATH];
} CSSProperty;

typedef struct {
    char selector[128];
    CSSProperty properties[32];
    int num_properties;
    int specificity;
} CSSRule;

typedef struct {
    CSSRule rules[256];
    int num_rules;
} StyleSheet;

typedef struct {
    char name[64];
    char value[MAX_LINE];
    int type; /* 0=string, 1=number, 2=bool, 3=function */
} JSVariable;

typedef struct JSContext {
    JSVariable vars[MAX_SCRIPT_VARS];
    int num_vars;
    int execution_depth;
    int max_depth;
    long timeout_ms;
    clock_t start_time;
} JSContext;

typedef struct {
    DOMNode *document;
    StyleSheet *styles;
    char title[MAX_TITLE];
    char description[MAX_LINE];
    char charset[32];
    int width;
    int height;
    int scroll_x;
    int scroll_y;
    int loaded;
    double load_time;
    int has_canvas;
    int has_svg;
    int has_forms;
    int has_images;
    int has_scripts;
    int has_styles;
    int has_video;
    int has_audio;
    int has_iframes;
    int has_links;
    int num_links;
    int num_images;
    int num_scripts;
    int num_styles;
    int num_forms;
    int num_tables;
    int num_headings;
    int num_paragraphs;
    int num_lists;
    int num_iframes;
    int num_divs;
    int num_spans;
    int num_inputs;
    int num_buttons;
    int total_elements;
    int total_text_nodes;
    int total_chars;
} RenderPage;

/* ============================================================
 * TAB MANAGEMENT
 * ============================================================ */
typedef enum {
    TAB_LOADING, TAB_LOADED, TAB_ERROR, TAB_EMPTY, TAB_CRASHED
} TabState;

typedef struct {
    int id;
    char url[MAX_URL];
    char title[MAX_TITLE];
    TabState state;
    EngineType engine;
    RenderPage *page;
    int active;
    time_t created;
    time_t last_active;
    int loading_progress;
    int secure;
    char domain[MAX_DOMAIN];
    int has_favicon;
    char favicon_url[MAX_URL];
    int muted;
    int pinned;
    int discarded;
    int audio_playing;
    int has_form_data;
    int can_go_back;
    int can_go_forward;
    char back_stack[MAX_TAB_STACK][MAX_URL];
    int back_count;
    char forward_stack[MAX_TAB_STACK][MAX_URL];
    int forward_count;
    double memory_usage_mb;
} Tab;

/* ============================================================
 * HISTORY
 * ============================================================ */
typedef struct {
    char url[MAX_URL];
    char title[MAX_TITLE];
    char domain[MAX_DOMAIN];
    time_t visit_time;
    int visit_count;
    int typed_count;
    int bookmarked;
    int from_search;
    int is_secure;
} HistoryEntry;

/* ============================================================
 * BOOKMARKS
 * ============================================================ */
typedef struct {
    char name[128];
    char description[MAX_LINE];
} BookmarkFolder;

typedef struct {
    char url[MAX_URL];
    char title[MAX_TITLE];
    char folder[128];
    time_t added;
    int visit_count;
    char tags[8][32];
    int num_tags;
    char description[MAX_LINE];
    int starred;
} Bookmark;

/* ============================================================
 * DOWNLOADS
 * ============================================================ */
typedef enum {
    DL_QUEUED, DL_DOWNLOADING, DL_PAUSED, DL_COMPLETE, DL_FAILED, DL_CANCELLED
} DownloadState;

typedef struct {
    int id;
    char url[MAX_URL];
    char filename[MAX_PATH];
    char path[MAX_PATH];
    char mime_type[64];
    int total_size;
    int downloaded;
    DownloadState state;
    double progress;
    double speed_kbps;
    time_t start_time;
    time_t end_time;
    int resume_supported;
    int retry_count;
    int max_retries;
} Download;

/* ============================================================
 * PROCESSES
 * ============================================================ */
typedef enum {
    PROC_IDLE, PROC_RUNNING, PROC_BLOCKED, PROC_SLEEPING, PROC_ZOMBIE, PROC_STOPPED
} ProcessState;

typedef enum {
    PROC_BROWSER, PROC_RENDERER, PROC_NETWORK, PROC_GPU,
    PROC_STORAGE, PROC_EXTENSION, PROC_UTILITY, PROC_SANDBOX
} ProcessType;

typedef struct {
    int pid;
    char name[64];
    ProcessType type;
    ProcessState state;
    int priority;
    double cpu_usage;
    double memory_mb;
    time_t start_time;
    int thread_count;
    int handle_count;
    int child_pids[8];
    int num_children;
} Process;

/* ============================================================
 * EXTENSIONS
 * ============================================================ */
typedef struct {
    char id[64];
    char name[128];
    char version[16];
    char author[128];
    char description[MAX_LINE];
    int enabled;
    int has_permissions;
    int has_background_script;
    int has_content_script;
    int has_browser_action;
    int has_page_action;
    int has_options_page;
    int has_popup;
    int permissions[16];
    int num_permissions;
    int install_time;
    int update_time;
} Extension;

/* ============================================================
 * LANGUAGE SYSTEM
 * ============================================================ */
typedef enum {
    LANG_EN,    /* English */
    LANG_ZH_CN, /* Chinese (Simplified) */
    LANG_ZH_TW, /* Chinese (Traditional) */
    LANG_JA,    /* Japanese */
    LANG_KO,    /* Korean */
    LANG_FR,    /* French */
    LANG_DE,    /* German */
    LANG_ES,    /* Spanish */
    LANG_RU,    /* Russian */
    LANG_AR,    /* Arabic */
    LANG_PT,    /* Portuguese */
    LANG_IT,    /* Italian */
    LANG_NL,    /* Dutch */
    LANG_PL,    /* Polish */
    LANG_TR,    /* Turkish */
    LANG_SV,    /* Swedish */
    LANG_DA,    /* Danish */
    LANG_FI,    /* Finnish */
    LANG_CS,    /* Czech */
    LANG_HU,    /* Hungarian */
    LANG_RO,    /* Romanian */
    LANG_VI,    /* Vietnamese */
    LANG_TH,    /* Thai */
    LANG_HE     /* Hebrew */
} Language;

typedef struct {
    char code[8];
    char name[32];
    char native_name[64];
    char flag[8];
    int rtl;
    int decimal_sep;
    int date_format;
    char font[32];
} LanguageInfo;

/* ============================================================
 * CONFIGURATION
 * ============================================================ */
typedef struct {
    EngineType default_engine;
    Language language;
    Theme theme;
    int auto_load_images;
    int enable_javascript;
    int enable_cookies;
    int enable_cache;
    int enable_popups;
    int enable_plugins;
    int enable_extensions;
    int enable_downloads;
    int enable_history;
    int enable_autocomplete;
    int enable_spellcheck;
    int enable_do_not_track;
    int enable_privacy_mode;
    int enable_sync;
    int enable_updates;
    int enable_crash_reports;
    int enable_hardware_accel;
    int enable_webgl;
    int enable_webrtc;
    int enable_webgpu;
    int enable_wasm;
    int enable_service_workers;
    int enable_notifications;
    int enable_geolocation;
    int enable_microphone;
    int enable_camera;
    int enable_midi;
    int enable_vr;
    int enable_ar;
    int home_page_shown;
    char home_page[MAX_URL];
    char search_engine[64];
    char download_dir[MAX_PATH];
    int font_size;
    int zoom_level;
    int max_tabs;
    int new_tab_page;
    int restore_session;
    int warn_quit;
} Config;

/* ============================================================
 * STATISTICS & INFO TYPES
 * ============================================================ */
typedef struct {
    int total_requests;
    int total_transfer_kb;
    int total_cache_hits;
    int total_cache_misses;
    int total_errors;
    int total_dns_queries;
    int total_ssl_handshakes;
    double avg_load_time;
    double total_load_time;
    int active_connections;
} NetworkStats;

typedef struct {
    int total_pages_rendered;
    int total_elements_rendered;
    int total_layouts;
    int total_paints;
    int total_reflow;
    double avg_render_time;
    double total_render_time;
} RenderStats;

typedef struct {
    int year, month, day, hour, minute, second;
    int timezone_offset;
    char timezone_name[32];
    int daylight_saving;
} TimeInfo;

/* ============================================================
 * MAIN OS STATE
 * ============================================================ */
typedef struct {
    char running;
    char username[32];
    char hostname[32];
    time_t boot_time;
    int uptime_seconds;
    int logged_in;
    int session_id;
    int command_count;
    char last_error[MAX_LINE];
    int error_code;
} OSState;

typedef struct {
    Config config;
    OSState os;
    Engine engines[MAX_ENGINES];
    int num_engines;
    int active_engine;
    LanguageInfo languages[MAX_LANGUAGES];
    int num_languages;
    int active_language;
    Tab tabs[MAX_TABS];
    int num_tabs;
    int active_tab;
    HistoryEntry history[MAX_HISTORY];
    int history_count;
    Bookmark bookmarks[MAX_BOOKMARKS];
    int bookmark_count;
    BookmarkFolder bookmark_folders[MAX_BOOKMARK_FOLDERS];
    int bookmark_folder_count;
    Download downloads[MAX_DOWNLOADS];
    int download_count;
    Process processes[MAX_PROCESSES];
    int process_count;
    Extension extensions[MAX_EXTENSIONS];
    int extension_count;
    CacheEntry cache[MAX_CACHE];
    int cache_count;
    Cookie cookies[MAX_COOKIES];
    int cookie_count;
    Certificate certificates[MAX_CERTIFICATES];
    int certificate_count;
    ProxyConfig proxy;
    ThemeConfig theme_config;
    char cmd_history[MAX_CMD_HISTORY][MAX_INPUT];
    int cmd_history_count;
    char current_dir[MAX_PATH];
    int render_width;
    int render_height;
    long total_memory_kb;
    long used_memory_kb;
    double cpu_load;
    TimeInfo time_info;
    NetworkStats net_stats;
    RenderStats render_stats;
    int argc;
    char **argv;
} BrowserOS;

/* ============================================================
 * ENGINE FUNCTION PROTOTYPES
 * ============================================================ */
void engine_init_all(BrowserOS *os);
Engine* engine_get(BrowserOS *os, EngineType type);
Engine* engine_get_active(BrowserOS *os);
int engine_switch(BrowserOS *os, EngineType type);
int engine_set_active(BrowserOS *os, int index);
char* engine_get_user_agent(BrowserOS *os);
void engine_print_info(const Engine *e);
void engine_print_all(BrowserOS *os);
void engine_compare(BrowserOS *os);
const char* engine_type_name(EngineType t);
int engine_render_html(BrowserOS *os, const char *html, char *output, int max_out);

/* ============================================================
 * LANGUAGE FUNCTION PROTOTYPES
 * ============================================================ */
void lang_init_all(BrowserOS *os);
int lang_switch(BrowserOS *os, Language lang);
const char* lang_get(BrowserOS *os, const char *key);
const char* lang_tr(BrowserOS *os, const char *key, ...);
void lang_print_all(BrowserOS *os);
const char* lang_code(Language l);
Language lang_from_code(const char *code);
const char* lang_name(BrowserOS *os);
const char* lang_key(const char *key);
int lang_is_rtl(BrowserOS *os);

/* ============================================================
 * NETWORK FUNCTION PROTOTYPES
 * ============================================================ */
int network_fetch_url(BrowserOS *os, const char *url, HttpResponse *resp);
int network_resolve_dns(const char *host, char *ip, int max_len);
Protocol network_detect_protocol(const char *url);
int network_parse_url(const char *url, char *host, char *path, int *port, Protocol *proto);
int network_build_request(BrowserOS *os, HttpResponse *resp);
int network_send_request(const HttpMethod method, const char *url, HttpHeader *headers, int num_headers, const char *body, HttpResponse *resp);
int network_handle_response(HttpResponse *resp);
int network_check_certificate(BrowserOS *os, const char *domain);
int network_init_ssl(void *ctx, const char *host, int port);
void network_proxy_set(BrowserOS *os, const char *host, int port);
void network_proxy_disable(BrowserOS *os);
int network_is_valid_url(const char *url);
int network_normalize_url(const char *url, char *out, int max_len);
int network_is_valid_domain(const char *domain);
int network_detect_encoding(const char *content, char *encoding, int max_len);

/* Cache */
int cache_get(BrowserOS *os, const char *url, CacheEntry *entry);
int cache_put(BrowserOS *os, const char *url, const char *content_type, int size, const char *etag);
int cache_clear(BrowserOS *os);
int cache_remove(BrowserOS *os, int index);
void cache_print(BrowserOS *os);
int cache_size(BrowserOS *os);
int cache_hit_rate(BrowserOS *os);

/* Cookies */
int cookie_get(BrowserOS *os, const char *domain, const char *path, const char *name, char *value, int max_len);
int cookie_set(BrowserOS *os, const char *domain, const char *path, const char *name, const char *value, int secure, int http_only, time_t expires);
int cookie_clear(BrowserOS *os);
int cookie_clear_domain(BrowserOS *os, const char *domain);
void cookie_print(BrowserOS *os);
int cookie_count_for_domain(BrowserOS *os, const char *domain);

/* ============================================================
 * RENDERER FUNCTION PROTOTYPES
 * ============================================================ */
RenderPage* renderer_create_page(void);
void renderer_free_page(RenderPage *page);
int renderer_parse_html(const char *html, RenderPage *page, BrowserOS *os);
int renderer_render_page(RenderPage *page, char *output, int max_out, BrowserOS *os);
int renderer_process_dom(DOMNode *node, char *output, int max_out, int depth, BrowserOS *os);
void renderer_analyze_content(RenderPage *page);
DOMNode* dom_create_node(NodeType type, const char *tag);
int dom_add_child(DOMNode *parent, DOMNode *child);
void dom_free(DOMNode *node);
int renderer_apply_css(RenderPage *page, const char *css);
int renderer_execute_js(RenderPage *page, const char *script, BrowserOS *os);
char* renderer_get_title(RenderPage *page);
void renderer_print_stats(RenderPage *page);
int renderer_extract_links(RenderPage *page, char links[][MAX_URL], int max_links);
int renderer_extract_text(RenderPage *page, char *text, int max_len);
int renderer_format_output(RenderPage *page, char *output, int max_out, int width, BrowserOS *os);

/* ============================================================
 * SHELL / UI FUNCTION PROTOTYPES
 * ============================================================ */
void shell_init(BrowserOS *os);
void shell_run(BrowserOS *os);
int shell_handle_command(BrowserOS *os, const char *input);
void shell_print_banner(BrowserOS *os);
void shell_print_prompt(BrowserOS *os);
void shell_print_help(BrowserOS *os);
int shell_parse_command(const char *input, char *cmd, char args[][MAX_INPUT], int *num_args);
void shell_print_welcome(BrowserOS *os);
void shell_auto_complete(BrowserOS *os, const char *input, char *output, int max_len);
void shell_sigint_handler(int sig);

/* Tab commands */
int cmd_open(BrowserOS *os, int argc, char *argv[]);
int cmd_tabs(BrowserOS *os);
int cmd_tab_new(BrowserOS *os, const char *url);
int cmd_tab_close(BrowserOS *os, int id);
int cmd_tab_switch(BrowserOS *os, int id);
int cmd_tab_info(BrowserOS *os, int id);
int cmd_tab_reload(BrowserOS *os);
int cmd_tab_duplicate(BrowserOS *os);
int cmd_tab_move(BrowserOS *os, int id, int new_pos);

/* Engine commands */
int cmd_engine(BrowserOS *os, int argc, char *argv[]);
int cmd_engines(BrowserOS *os, int argc, char *argv[]);
int cmd_engine_info(BrowserOS *os, int argc, char *argv[]);

/* Language commands */
int cmd_lang(BrowserOS *os, int argc, char *argv[]);
int cmd_langs(BrowserOS *os, int argc, char *argv[]);

/* History/Bookmark commands */
int cmd_history(BrowserOS *os, int argc, char *argv[]);
int cmd_history_clear(BrowserOS *os);
int cmd_history_search(BrowserOS *os, const char *query);
int cmd_bookmark(BrowserOS *os, int argc, char *argv[]);
int cmd_bookmarks(BrowserOS *os, int argc, char *argv[]);
int cmd_bookmark_add(BrowserOS *os, const char *url, const char *title);
int cmd_bookmark_remove(BrowserOS *os, int index);
int cmd_bookmark_export(BrowserOS *os, const char *filepath);
int cmd_bookmark_import(BrowserOS *os, const char *filepath);

/* Settings commands */
int cmd_settings(BrowserOS *os, int argc, char *argv[]);
int cmd_set(BrowserOS *os, int argc, char *argv[]);
int cmd_theme(BrowserOS *os, int argc, char *argv[]);
int cmd_themes(BrowserOS *os, int argc, char *argv[]);

/* Network commands */
int cmd_proxy(BrowserOS *os, int argc, char *argv[]);
int cmd_useragent(BrowserOS *os, int argc, char *argv[]);
int cmd_dns(BrowserOS *os, const char *host);
int cmd_ping(BrowserOS *os, const char *host);
int cmd_traceroute(BrowserOS *os, const char *host);

/* System commands */
int cmd_status(BrowserOS *os, int argc, char *argv[]);
int cmd_processes(BrowserOS *os, int argc, char *argv[]);
int cmd_about(BrowserOS *os, int argc, char *argv[]);
int cmd_version(BrowserOS *os, int argc, char *argv[]);
int cmd_clear(BrowserOS *os, int argc, char *argv[]);
int cmd_echo(BrowserOS *os, int argc, char *argv[]);
int cmd_date(BrowserOS *os, int argc, char *argv[]);
int cmd_whoami(BrowserOS *os, int argc, char *argv[]);
int cmd_help(BrowserOS *os, int argc, char *argv[]);

/* Download commands */
int cmd_downloads(BrowserOS *os, int argc, char *argv[]);
int cmd_download(BrowserOS *os, int argc, char *argv[]);
int cmd_download_cancel(BrowserOS *os, int id);
int cmd_download_pause(BrowserOS *os, int id);
int cmd_download_resume(BrowserOS *os, int id);

/* Cache/Cookie commands */
int cmd_cache(BrowserOS *os, int argc, char *argv[]);
int cmd_cache_clear(BrowserOS *os);
int cmd_cookies(BrowserOS *os, int argc, char *argv[]);
int cmd_cookies_clear(BrowserOS *os);

/* Search command */
int cmd_search(BrowserOS *os, int argc, char *argv[]);

/* ============================================================
 * CONFIG FUNCTION PROTOTYPES
 * ============================================================ */
void config_init(BrowserOS *os);
int config_set(BrowserOS *os, const char *key, const char *value);
int config_get(BrowserOS *os, const char *key, char *value, int max_len);
void config_print(BrowserOS *os);
int config_save(BrowserOS *os, const char *filepath);
int config_load(BrowserOS *os, const char *filepath);
int config_reset(BrowserOS *os);
int config_import(BrowserOS *os, const char *filepath);
int config_export(BrowserOS *os, const char *filepath);

/* ============================================================
 * UI / COLOR SYSTEM
 * ============================================================ */
void color_set(Color fg, Color bg);
void color_reset(void);
const char* color_escape(Color c);
void theme_apply(BrowserOS *os, Theme theme);
void theme_print_all(void);
void theme_init(BrowserOS *os);
const char* theme_name(Theme t);

/* ============================================================
 * PROCESS MANAGEMENT
 * ============================================================ */
int proc_create(BrowserOS *os, const char *name, ProcessType type, int priority);
int proc_kill(BrowserOS *os, int pid);
int proc_suspend(BrowserOS *os, int pid);
int proc_resume(BrowserOS *os, int pid);
void proc_print_all(BrowserOS *os);
Process* proc_get(BrowserOS *os, int pid);
void proc_update_cpu(BrowserOS *os);

/* ============================================================
 * EXTENSION MANAGEMENT
 * ============================================================ */
int ext_install(BrowserOS *os, const char *name, const char *version, const char *author, const char *desc);
int ext_uninstall(BrowserOS *os, int index);
int ext_enable(BrowserOS *os, int index);
int ext_disable(BrowserOS *os, int index);
void ext_print_all(BrowserOS *os);

/* ============================================================
 * TAB MANAGEMENT
 * ============================================================ */
Tab* tab_create(BrowserOS *os, const char *url, EngineType engine);
int tab_destroy(BrowserOS *os, int id);
int tab_switch_to(BrowserOS *os, int id);
Tab* tab_get_active(BrowserOS *os);
Tab* tab_get(BrowserOS *os, int id);
int tab_navigate(BrowserOS *os, int id, const char *url);
int tab_reload(BrowserOS *os, int id);
int tab_go_back(BrowserOS *os, int id);
int tab_go_forward(BrowserOS *os, int id);
void tab_print_all(BrowserOS *os);
void tab_print(Tab *t);

/* ============================================================
 * HISTORY MANAGEMENT
 * ============================================================ */
int history_add(BrowserOS *os, const char *url, const char *title);
void history_print(BrowserOS *os, int count);
int history_search(BrowserOS *os, const char *query, HistoryEntry *results, int max_results);
int history_clear(BrowserOS *os);
int history_remove(BrowserOS *os, int index);
int history_count(BrowserOS *os);
int history_export(BrowserOS *os, const char *filepath);
int history_import(BrowserOS *os, const char *filepath);
char** history_suggestions(BrowserOS *os, const char *partial, int *count);

/* ============================================================
 * BOOKMARK MANAGEMENT
 * ============================================================ */
int bookmark_add(BrowserOS *os, const char *url, const char *title, const char *folder);
int bookmark_remove(BrowserOS *os, int index);
int bookmark_update(BrowserOS *os, int index, const char *title, const char *folder);
void bookmark_print(BrowserOS *os, int count);
int bookmark_search(BrowserOS *os, const char *query, Bookmark *results, int max_results);
int bookmark_count_in_folder(BrowserOS *os, const char *folder);
int bookmark_folder_create(BrowserOS *os, const char *name, const char *desc);
int bookmark_folder_remove(BrowserOS *os, int index);
void bookmark_folders_print(BrowserOS *os);
int bookmark_export(BrowserOS *os, const char *filepath);
int bookmark_import(BrowserOS *os, const char *filepath);

/* ============================================================
 * DOWNLOAD MANAGEMENT
 * ============================================================ */
int download_add(BrowserOS *os, const char *url, const char *filename);
int download_cancel(BrowserOS *os, int id);
int download_pause(BrowserOS *os, int id);
int download_resume(BrowserOS *os, int id);
void download_print(BrowserOS *os, int count);
Download* download_get(BrowserOS *os, int id);
int download_remove_completed(BrowserOS *os);

/* ============================================================
 * UTILITY FUNCTIONS
 * ============================================================ */
void util_trim(char *s);
void util_lower(char *s);
int util_starts_with(const char *s, const char *prefix);
int util_ends_with(const char *s, const char *suffix);
char* util_strdup(const char *s);
int util_strincmp(const char *a, const char *b, int n);
int util_count_char(const char *s, char c);
void util_replace_char(char *s, char old, char new);
void util_strlcpy(char *dst, const char *src, int n);
int util_contains(const char *haystack, const char *needle);
long util_time_diff_ms(time_t start, time_t end);
void util_format_size(long bytes, char *out, int max_len);
void util_format_time(time_t t, char *out, int max_len);
void util_format_duration(int seconds, char *out, int max_len);
void util_url_decode(char *dst, const char *src);
void util_url_encode(char *dst, const char *src);
int util_is_ip_address(const char *s);
int util_random(int min, int max);
int util_progress_bar(double progress, char *out, int width);
void util_center_text(const char *text, int width, char *out, int max_len);
void util_truncate(const char *text, int max_len, char *out);
int util_word_wrap(const char *text, int line_width, char *out, int max_out);
void util_strip_html(const char *html, char *text, int max_len);

/* ============================================================
 * INITIALIZATION
 * ============================================================ */
void browseos_init(BrowserOS *os, int argc, char **argv);
void browseos_shutdown(BrowserOS *os);
void browseos_update(BrowserOS *os);
void browseos_print_error(BrowserOS *os, const char *msg);
void browseos_print_info(BrowserOS *os, const char *msg);
void browseos_print_warning(BrowserOS *os, const char *msg);
void browseos_print_success(BrowserOS *os, const char *msg);

/* ============================================================
 * HTML GENERATORS (for simulation)
 * ============================================================ */
char* generate_sample_html(const char *url, char *buf, int max_len, BrowserOS *os);

#endif /* BROWSEOS_H */
