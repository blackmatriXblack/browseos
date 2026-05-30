#include "../include/browseos.h"

/* ============================================================
 * THEME / COLOR SYSTEM
 * ============================================================ */
static ThemeConfig theme_data[] = {
    {COLOR_WHITE, COLOR_BLACK, "default", "Default dark terminal",
     COLOR_GREEN, COLOR_CYAN, COLOR_RED, COLOR_YELLOW, COLOR_BLUE, COLOR_CYAN, COLOR_YELLOW},
    {COLOR_WHITE, COLOR_BLACK, "dark", "Pure dark theme",
     COLOR_GREEN, COLOR_CYAN, COLOR_RED, COLOR_YELLOW, COLOR_BLUE, COLOR_CYAN, COLOR_YELLOW},
    {COLOR_BLACK, COLOR_WHITE, "light", "Light background theme",
     COLOR_BLUE, COLOR_BLACK, COLOR_RED, COLOR_BLUE, COLOR_BLUE, COLOR_BLUE, COLOR_BLACK},
    {COLOR_GREEN, COLOR_BLACK, "hacker", "Matrix green hacker theme",
     COLOR_GREEN, COLOR_GREEN, COLOR_RED, COLOR_YELLOW, COLOR_CYAN, COLOR_GREEN, COLOR_GREEN},
    {COLOR_CYAN, COLOR_BLACK, "ocean", "Ocean blue theme",
     COLOR_CYAN, COLOR_BLUE, COLOR_RED, COLOR_YELLOW, COLOR_CYAN, COLOR_CYAN, COLOR_CYAN},
    {COLOR_BRIGHT_RED, COLOR_BLACK, "sunset", "Warm sunset theme",
     COLOR_BRIGHT_YELLOW, COLOR_BRIGHT_RED, COLOR_RED, COLOR_YELLOW, COLOR_BRIGHT_MAGENTA, COLOR_BRIGHT_RED, COLOR_BRIGHT_YELLOW},
    {COLOR_GREEN, COLOR_BLACK, "forest", "Forest green theme",
     COLOR_GREEN, COLOR_BRIGHT_GREEN, COLOR_RED, COLOR_YELLOW, COLOR_BRIGHT_GREEN, COLOR_GREEN, COLOR_BRIGHT_GREEN},
    {COLOR_BLUE, COLOR_BLACK, "midnight", "Deep midnight theme",
     COLOR_BRIGHT_BLUE, COLOR_BLUE, COLOR_RED, COLOR_YELLOW, COLOR_CYAN, COLOR_BLUE, COLOR_BRIGHT_BLUE},
    {COLOR_YELLOW, COLOR_BLACK, "retro", "Retro amber terminal",
     COLOR_YELLOW, COLOR_YELLOW, COLOR_RED, COLOR_BRIGHT_YELLOW, COLOR_WHITE, COLOR_YELLOW, COLOR_BRIGHT_YELLOW},
    {COLOR_BRIGHT_YELLOW, COLOR_BLACK, "amber", "Amber monochrome",
     COLOR_BRIGHT_YELLOW, COLOR_YELLOW, COLOR_RED, COLOR_BRIGHT_YELLOW, COLOR_WHITE, COLOR_YELLOW, COLOR_BRIGHT_YELLOW},
    {COLOR_MAGENTA, COLOR_BLACK, "dracula", "Dracula dark purple",
     COLOR_MAGENTA, COLOR_CYAN, COLOR_RED, COLOR_YELLOW, COLOR_GREEN, COLOR_CYAN, COLOR_MAGENTA},
    {COLOR_CYAN, COLOR_BLACK, "nord", "Nord arctic blue",
     COLOR_CYAN, COLOR_WHITE, COLOR_RED, COLOR_YELLOW, COLOR_BLUE, COLOR_CYAN, COLOR_WHITE},
    {COLOR_GREEN, COLOR_BLACK, "solarized", "Solarized light",
     COLOR_CYAN, COLOR_BLUE, COLOR_RED, COLOR_YELLOW, COLOR_GREEN, COLOR_CYAN, COLOR_BLUE},
    {COLOR_BLUE, COLOR_BLACK, "tokyo-night", "Tokyo Night theme",
     COLOR_CYAN, COLOR_MAGENTA, COLOR_RED, COLOR_YELLOW, COLOR_GREEN, COLOR_CYAN, COLOR_MAGENTA},
    {COLOR_GREEN, COLOR_BLACK, "gruvbox", "Gruvbox retro groove",
     COLOR_YELLOW, COLOR_GREEN, COLOR_RED, COLOR_BRIGHT_YELLOW, COLOR_BLUE, COLOR_GREEN, COLOR_YELLOW},
    {COLOR_MAGENTA, COLOR_BLACK, "catppuccin", "Catppuccin Mocha",
     COLOR_GREEN, COLOR_BLUE, COLOR_RED, COLOR_YELLOW, COLOR_CYAN, COLOR_BLUE, COLOR_MAGENTA},
};

const char* theme_name(Theme t) {
    if (t >= 0 && t < sizeof(theme_data)/sizeof(theme_data[0]))
        return theme_data[t].name;
    return "default";
}

void color_set(Color fg, Color bg) {
    printf("\x1b[%d;%dm", 30 + fg, 40 + bg);
}

void color_reset(void) {
    printf("\x1b[0m");
}

const char* color_escape(Color c) {
    static char buf[16];
    if (c == COLOR_RESET) return "\x1b[0m";
    if (c >= COLOR_BRIGHT_BLACK) {
        snprintf(buf, sizeof(buf), "\x1b[%d;1m", 30 + (c - COLOR_BRIGHT_BLACK));
    } else {
        snprintf(buf, sizeof(buf), "\x1b[%dm", 30 + c);
    }
    return buf;
}

void theme_apply(BrowserOS *os, Theme theme) {
    if (theme >= 0 && theme < sizeof(theme_data)/sizeof(theme_data[0])) {
        os->theme_config = theme_data[theme];
        os->config.theme = theme;
    }
}

void theme_print_all(void) {
    int i, n = sizeof(theme_data)/sizeof(theme_data[0]);
    printf("\n  Available Themes:\n");
    printf("  %s\n", "--------------------------------------------------");
    for (i = 0; i < n; i++) {
        printf("  %-3d %-20s %s\n", i + 1, theme_data[i].name, theme_data[i].desc);
    }
    printf("\n");
}

void theme_init(BrowserOS *os) {
    theme_apply(os, THEME_DEFAULT);
}

/* ============================================================
 * UTILITY FUNCTIONS
 * ============================================================ */
void util_trim(char *s) {
    if (!s) return;
    char *end;
    while (isspace((unsigned char)*s)) s++;
    if (*s == 0) return;
    end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
}

void util_lower(char *s) {
    if (!s) return;
    for (; *s; s++) *s = tolower((unsigned char)*s);
}

int util_starts_with(const char *s, const char *prefix) {
    if (!s || !prefix) return 0;
    return strncmp(s, prefix, strlen(prefix)) == 0;
}

int util_ends_with(const char *s, const char *suffix) {
    if (!s || !suffix) return 0;
    int slen = strlen(s);
    int suflen = strlen(suffix);
    if (slen < suflen) return 0;
    return strcmp(s + slen - suflen, suffix) == 0;
}

char* util_strdup(const char *s) {
    if (!s) return NULL;
    char *d = (char*)malloc(strlen(s) + 1);
    if (d) strcpy(d, s);
    return d;
}

int util_strincmp(const char *a, const char *b, int n) {
    if (!a || !b) return -1;
    int i;
    for (i = 0; i < n; i++) {
        if (tolower((unsigned char)a[i]) != tolower((unsigned char)b[i]))
            return tolower((unsigned char)a[i]) - tolower((unsigned char)b[i]);
        if (a[i] == '\0') return 0;
    }
    return 0;
}

int util_count_char(const char *s, char c) {
    int count = 0;
    while (s && *s) { if (*s == c) count++; s++; }
    return count;
}

void util_strlcpy(char *dst, const char *src, int n) {
    if (!dst || !src || n <= 0) return;
    strncpy(dst, src, n - 1);
    dst[n - 1] = '\0';
}

int util_contains(const char *haystack, const char *needle) {
    if (!haystack || !needle) return 0;
    return strstr(haystack, needle) != NULL;
}

long util_time_diff_ms(time_t start, time_t end) {
    return (long)difftime(end, start) * 1000;
}

void util_format_size(long bytes, char *out, int max_len) {
    if (bytes < 1024)
        snprintf(out, max_len, "%ld B", bytes);
    else if (bytes < 1024 * 1024)
        snprintf(out, max_len, "%.1f KB", (double)bytes / 1024);
    else if (bytes < 1024LL * 1024 * 1024)
        snprintf(out, max_len, "%.1f MB", (double)bytes / (1024 * 1024));
    else
        snprintf(out, max_len, "%.1f GB", (double)bytes / (1024 * 1024 * 1024));
}

void util_format_time(time_t t, char *out, int max_len) {
    struct tm *tm_info = localtime(&t);
    if (tm_info)
        strftime(out, max_len, "%Y-%m-%d %H:%M:%S", tm_info);
    else
        util_strlcpy(out, "Unknown", max_len);
}

void util_format_duration(int seconds, char *out, int max_len) {
    int h = seconds / 3600;
    int m = (seconds % 3600) / 60;
    int s = seconds % 60;
    if (h > 0)
        snprintf(out, max_len, "%dh %dm %ds", h, m, s);
    else if (m > 0)
        snprintf(out, max_len, "%dm %ds", m, s);
    else
        snprintf(out, max_len, "%ds", s);
}

int util_is_ip_address(const char *s) {
    if (!s) return 0;
    int dots = 0;
    int digits = 0;
    while (*s) {
        if (*s == '.') { dots++; digits = 0; }
        else if (isdigit((unsigned char)*s)) { digits++; }
        else return 0;
        s++;
    }
    return dots == 3;
}

int util_random(int min, int max) {
    static int seeded = 0;
    if (!seeded) { srand((unsigned)time(NULL) ^ (unsigned)clock()); seeded = 1; }
    return min + rand() % (max - min + 1);
}

int util_progress_bar(double progress, char *out, int width) {
    if (!out || width <= 0) return 0;
    int filled = (int)(progress * width);
    if (filled > width) filled = width;
    int i;
    out[0] = '[';
    for (i = 0; i < width; i++) {
        if (i < filled) out[i + 1] = '=';
        else if (i == filled) out[i + 1] = '>';
        else out[i + 1] = ' ';
    }
    out[width + 1] = ']';
    out[width + 2] = '\0';
    return width + 2;
}

void util_truncate(const char *text, int max_len, char *out) {
    if (!text || !out) return;
    int len = strlen(text);
    if (len <= max_len) {
        strcpy(out, text);
    } else {
        strncpy(out, text, max_len - 3);
        out[max_len - 3] = '.';
        out[max_len - 2] = '.';
        out[max_len - 1] = '.';
        out[max_len] = '\0';
    }
}

void util_replace_char(char *s, char old, char new) {
    if (!s) return;
    while (*s) {
        if (*s == old) *s = new;
        s++;
    }
}

/* ============================================================
 * CONFIGURATION
 * ============================================================ */
void config_init(BrowserOS *os) {
    os->config.default_engine = ENGINE_BLINK;
    os->config.language = LANG_EN;
    os->config.theme = THEME_DEFAULT;
    os->config.auto_load_images = 1;
    os->config.enable_javascript = 1;
    os->config.enable_cookies = 1;
    os->config.enable_cache = 1;
    os->config.enable_popups = 0;
    os->config.enable_plugins = 1;
    os->config.enable_extensions = 1;
    os->config.enable_downloads = 1;
    os->config.enable_history = 1;
    os->config.enable_autocomplete = 1;
    os->config.enable_spellcheck = 0;
    os->config.enable_do_not_track = 1;
    os->config.enable_privacy_mode = 0;
    os->config.enable_sync = 0;
    os->config.enable_updates = 1;
    os->config.enable_crash_reports = 0;
    os->config.enable_hardware_accel = 1;
    os->config.enable_webgl = 1;
    os->config.enable_webrtc = 1;
    os->config.enable_webgpu = 0;
    os->config.enable_wasm = 1;
    os->config.enable_service_workers = 1;
    os->config.enable_notifications = 1;
    os->config.enable_geolocation = 0;
    os->config.enable_microphone = 0;
    os->config.enable_camera = 0;
    os->config.enable_midi = 0;
    os->config.enable_vr = 0;
    os->config.enable_ar = 0;
    os->config.home_page_shown = 1;
    util_strlcpy(os->config.home_page, "about:welcome", MAX_URL);
    util_strlcpy(os->config.search_engine, "Google", 64);
    util_strlcpy(os->config.download_dir, "./downloads", MAX_PATH);
    os->config.font_size = 14;
    os->config.zoom_level = 100;
    os->config.max_tabs = MAX_TABS;
    os->config.new_tab_page = 1;
    os->config.restore_session = 1;
    os->config.warn_quit = 1;
}

int config_set(BrowserOS *os, const char *key, const char *value) {
    if (!key || !value) return 0;

    if (strcmp(key, "home") == 0 || strcmp(key, "homepage") == 0) {
        util_strlcpy(os->config.home_page, value, MAX_URL);
    } else if (strcmp(key, "search") == 0 || strcmp(key, "search_engine") == 0) {
        util_strlcpy(os->config.search_engine, value, 64);
    } else if (strcmp(key, "download_dir") == 0 || strcmp(key, "downloads") == 0) {
        util_strlcpy(os->config.download_dir, value, MAX_PATH);
    } else if (strcmp(key, "javascript") == 0 || strcmp(key, "js") == 0) {
        os->config.enable_javascript = (strcmp(value, "1") == 0 || strcmp(value, "on") == 0 || strcmp(value, "true") == 0);
    } else if (strcmp(key, "cookies") == 0) {
        os->config.enable_cookies = (strcmp(value, "1") == 0 || strcmp(value, "on") == 0 || strcmp(value, "true") == 0);
    } else if (strcmp(key, "cache") == 0) {
        os->config.enable_cache = (strcmp(value, "1") == 0 || strcmp(value, "on") == 0 || strcmp(value, "true") == 0);
    } else if (strcmp(key, "images") == 0) {
        os->config.auto_load_images = (strcmp(value, "1") == 0 || strcmp(value, "on") == 0 || strcmp(value, "true") == 0);
    } else if (strcmp(key, "popups") == 0) {
        os->config.enable_popups = (strcmp(value, "1") == 0 || strcmp(value, "on") == 0 || strcmp(value, "true") == 0);
    } else if (strcmp(key, "privacy") == 0 || strcmp(key, "incognito") == 0) {
        os->config.enable_privacy_mode = (strcmp(value, "1") == 0 || strcmp(value, "on") == 0 || strcmp(value, "true") == 0);
    } else if (strcmp(key, "dnt") == 0 || strcmp(key, "do_not_track") == 0) {
        os->config.enable_do_not_track = (strcmp(value, "1") == 0 || strcmp(value, "on") == 0 || strcmp(value, "true") == 0);
    } else if (strcmp(key, "notifications") == 0) {
        os->config.enable_notifications = (strcmp(value, "1") == 0 || strcmp(value, "on") == 0 || strcmp(value, "true") == 0);
    } else if (strcmp(key, "geolocation") == 0) {
        os->config.enable_geolocation = (strcmp(value, "1") == 0 || strcmp(value, "on") == 0 || strcmp(value, "true") == 0);
    } else if (strcmp(key, "webgl") == 0) {
        os->config.enable_webgl = (strcmp(value, "1") == 0 || strcmp(value, "on") == 0 || strcmp(value, "true") == 0);
    } else if (strcmp(key, "wasm") == 0) {
        os->config.enable_wasm = (strcmp(value, "1") == 0 || strcmp(value, "on") == 0 || strcmp(value, "true") == 0);
    } else if (strcmp(key, "webrtc") == 0) {
        os->config.enable_webrtc = (strcmp(value, "1") == 0 || strcmp(value, "on") == 0 || strcmp(value, "true") == 0);
    } else if (strcmp(key, "font_size") == 0 || strcmp(key, "fontsize") == 0) {
        os->config.font_size = atoi(value);
        if (os->config.font_size < 8) os->config.font_size = 8;
        if (os->config.font_size > 72) os->config.font_size = 72;
    } else if (strcmp(key, "zoom") == 0) {
        os->config.zoom_level = atoi(value);
        if (os->config.zoom_level < 25) os->config.zoom_level = 25;
        if (os->config.zoom_level > 500) os->config.zoom_level = 500;
    } else {
        return 0; /* Unknown setting */
    }
    return 1;
}

void config_print(BrowserOS *os) {
    printf("\n  BrowseOS Configuration:\n");
    printf("  %s\n", "==================================================");
    printf("  %-30s = %s\n", "Home Page", os->config.home_page);
    printf("  %-30s = %s\n", "Search Engine", os->config.search_engine);
    printf("  %-30s = %s\n", "Download Directory", os->config.download_dir);
    printf("  %-30s = %s\n", "JavaScript", os->config.enable_javascript ? "Enabled" : "Disabled");
    printf("  %-30s = %s\n", "Cookies", os->config.enable_cookies ? "Enabled" : "Disabled");
    printf("  %-30s = %s\n", "Cache", os->config.enable_cache ? "Enabled" : "Disabled");
    printf("  %-30s = %s\n", "Auto-load Images", os->config.auto_load_images ? "Enabled" : "Disabled");
    printf("  %-30s = %s\n", "Block Popups", os->config.enable_popups ? "Enabled" : "Disabled");
    printf("  %-30s = %s\n", "Privacy Mode", os->config.enable_privacy_mode ? "Enabled" : "Disabled");
    printf("  %-30s = %s\n", "Do Not Track", os->config.enable_do_not_track ? "Enabled" : "Disabled");
    printf("  %-30s = %s\n", "Notifications", os->config.enable_notifications ? "Enabled" : "Disabled");
    printf("  %-30s = %s\n", "Geolocation", os->config.enable_geolocation ? "Enabled" : "Disabled");
    printf("  %-30s = %s\n", "WebGL", os->config.enable_webgl ? "Enabled" : "Disabled");
    printf("  %-30s = %s\n", "WebAssembly", os->config.enable_wasm ? "Enabled" : "Disabled");
    printf("  %-30s = %s\n", "WebRTC", os->config.enable_webrtc ? "Enabled" : "Disabled");
    printf("  %-30s = %d\n", "Font Size", os->config.font_size);
    printf("  %-30s = %d%%\n", "Zoom Level", os->config.zoom_level);
    printf("  %-30s = %s\n", "Extensions", os->config.enable_extensions ? "Enabled" : "Disabled");
    printf("  %-30s = %s\n", "Plugins", os->config.enable_plugins ? "Enabled" : "Disabled");
    printf("  %-30s = %s\n", "Hardware Acceleration", os->config.enable_hardware_accel ? "Enabled" : "Disabled");
    printf("  %-30s = %s\n", "Service Workers", os->config.enable_service_workers ? "Enabled" : "Disabled");
    printf("  %-30s = %s\n", "Restore Session", os->config.restore_session ? "Enabled" : "Disabled");
    printf("  %-30s = %s\n", "Warn Before Quit", os->config.warn_quit ? "Enabled" : "Disabled");
    printf("  %s\n", "==================================================");
}

/* ============================================================
 * TAB MANAGEMENT
 * ============================================================ */
Tab* tab_create(BrowserOS *os, const char *url, EngineType engine) {
    if (os->num_tabs >= MAX_TABS) return NULL;
    int id = os->num_tabs;
    Tab *t = &os->tabs[id];
    memset(t, 0, sizeof(Tab));
    t->id = id;
    t->state = TAB_EMPTY;
    t->engine = (engine != ENGINE_CUSTOM) ? engine : os->config.default_engine;
    t->active = 0;
    t->created = time(NULL);
    t->last_active = time(NULL);
    t->loading_progress = 0;
    t->secure = 0;
    t->muted = 0;
    t->pinned = 0;
    t->discarded = 0;
    t->has_favicon = 0;
    t->memory_usage_mb = (double)util_random(10, 80) + (double)util_random(0, 99) / 100.0;

    if (url && *url) {
        tab_navigate(os, id, url);
    } else {
        util_strlcpy(t->url, "about:blank", MAX_URL);
        util_strlcpy(t->title, "New Tab", MAX_TITLE);
        t->state = TAB_LOADED;
    }

    os->num_tabs++;
    t->active = 1;
    os->active_tab = id;
    return t;
}

int tab_destroy(BrowserOS *os, int id) {
    if (id < 0 || id >= os->num_tabs) return 0;
    if (os->tabs[id].page) {
        renderer_free_page(os->tabs[id].page);
        os->tabs[id].page = NULL;
    }
    if (id < os->num_tabs - 1) {
        memmove(&os->tabs[id], &os->tabs[id + 1], sizeof(Tab) * (os->num_tabs - id - 1));
    }
    os->num_tabs--;
    int i;
    for (i = 0; i < os->num_tabs; i++) os->tabs[i].id = i;
    if (os->active_tab >= os->num_tabs) os->active_tab = os->num_tabs - 1;
    if (os->active_tab >= 0) os->tabs[os->active_tab].active = 1;
    return 1;
}

int tab_switch_to(BrowserOS *os, int id) {
    if (id < 0 || id >= os->num_tabs) return 0;
    int i;
    for (i = 0; i < os->num_tabs; i++) os->tabs[i].active = 0;
    os->active_tab = id;
    os->tabs[id].active = 1;
    os->tabs[id].last_active = time(NULL);
    return 1;
}

Tab* tab_get_active(BrowserOS *os) {
    if (os->active_tab >= 0 && os->active_tab < os->num_tabs)
        return &os->tabs[os->active_tab];
    return NULL;
}

Tab* tab_get(BrowserOS *os, int id) {
    if (id >= 0 && id < os->num_tabs) return &os->tabs[id];
    return NULL;
}

int tab_navigate(BrowserOS *os, int id, const char *url) {
    if (id < 0 || id >= os->num_tabs) return 0;
    Tab *t = &os->tabs[id];
    char normalized[MAX_URL];

    network_normalize_url(url, normalized, MAX_URL);
    util_strlcpy(t->url, normalized, MAX_URL);

    /* Save current URL to back stack */
    if (t->url[0] && strlen(t->url) > 0 && t->back_count < MAX_TAB_STACK) {
        util_strlcpy(t->back_stack[t->back_count], t->url, MAX_URL);
        t->back_count++;
    }

    t->state = TAB_LOADING;
    t->loading_progress = 0;
    t->can_go_back = 1;

    HttpResponse resp;
    if (network_fetch_url(os, normalized, &resp)) {
        Protocol proto = network_detect_protocol(normalized);
        util_strlcpy(t->domain, resp.host, MAX_DOMAIN);
        t->secure = (proto == PROTOCOL_HTTPS);

        if (t->page) renderer_free_page(t->page);
        t->page = renderer_create_page();
        if (t->page) {
            renderer_parse_html(resp.body, t->page, os);
            t->page->load_time = resp.load_time;
            util_strlcpy(t->title, t->page->title[0] ? t->page->title : normalized, MAX_TITLE);
            t->state = TAB_LOADED;
            t->loading_progress = 100;

            /* Add to history */
            if (os->config.enable_history) {
                history_add(os, normalized, t->title);
            }
        } else {
            t->state = TAB_ERROR;
        }
    } else {
        t->state = TAB_ERROR;
        snprintf(t->title, MAX_TITLE, "Error loading: %s", normalized);
    }

    if (resp.body) free(resp.body);
    t->last_active = time(NULL);
    return (t->state == TAB_LOADED);
}

int tab_reload(BrowserOS *os, int id) {
    Tab *t = tab_get(os, id);
    if (!t) return 0;
    return tab_navigate(os, id, t->url);
}

int tab_go_back(BrowserOS *os, int id) {
    Tab *t = tab_get(os, id);
    if (!t || t->back_count <= 0) return 0;
    t->back_count--;
    if (t->forward_count < MAX_TAB_STACK) {
        util_strlcpy(t->forward_stack[t->forward_count], t->url, MAX_URL);
        t->forward_count++;
    }
    return tab_navigate(os, id, t->back_stack[t->back_count]);
}

int tab_go_forward(BrowserOS *os, int id) {
    Tab *t = tab_get(os, id);
    if (!t || t->forward_count <= 0) return 0;
    t->forward_count--;
    if (t->back_count < MAX_TAB_STACK) {
        util_strlcpy(t->back_stack[t->back_count], t->url, MAX_URL);
        t->back_count++;
    }
    return tab_navigate(os, id, t->forward_stack[t->forward_count]);
}

void tab_print_all(BrowserOS *os) {
    int i;
    printf("\n  Open Tabs: %d / %d\n", os->num_tabs, MAX_TABS);
    printf("  %s\n", "----------------------------------------------------------------------");
    for (i = 0; i < os->num_tabs; i++) {
        Tab *t = &os->tabs[i];
        char time_str[64];
        util_format_time(t->created, time_str, 64);
        printf("  %s%-3d%s | %-35s | %-10s | %s%s%s\n",
               t->active ? "\x1b[1;33m" : "", i,
               t->active ? "\x1b[0m" : "",
               t->title,
               t->state == TAB_LOADING ? "Loading" :
               t->state == TAB_LOADED ? "Loaded" :
               t->state == TAB_ERROR ? "Error" :
               t->state == TAB_CRASHED ? "Crashed" : "Empty",
               t->secure ? "\x1b[1;32m[HTTPS]\x1b[0m " : "",
               t->pinned ? "\x1b[1;34m[PIN]\x1b[0m " : "",
               t->muted ? "\x1b[1;31m[MUTE]\x1b[0m " : "");
    }
    printf("\n");
}

void tab_print(Tab *t) {
    if (!t) {
        printf("  Tab: NULL\n");
        return;
    }
    printf("  Tab #%d\n", t->id);
    printf("  Title    : %s\n", t->title);
    printf("  URL      : %s\n", t->url);
    printf("  Domain   : %s\n", t->domain);
    printf("  Engine   : %s\n", engine_type_name(t->engine));
    printf("  State    : %s\n", t->state == TAB_LOADING ? "Loading" :
           t->state == TAB_LOADED ? "Loaded" :
           t->state == TAB_ERROR ? "Error" :
           t->state == TAB_EMPTY ? "Empty" : "Crashed");
    printf("  Secure   : %s\n", t->secure ? "Yes (HTTPS)" : "No (HTTP)");
    printf("  Progress : %d%%\n", t->loading_progress);
    printf("  Pinned   : %s\n", t->pinned ? "Yes" : "No");
    printf("  Muted    : %s\n", t->muted ? "Yes" : "No");
    printf("  Memory   : %.1f MB\n", t->memory_usage_mb);
    printf("  Created  : %s", ctime(&t->created));
    printf("  Back     : %d pages\n", t->back_count);
    printf("  Forward  : %d pages\n", t->forward_count);
}

/* ============================================================
 * HISTORY MANAGEMENT
 * ============================================================ */
int history_add(BrowserOS *os, const char *url, const char *title) {
    if (!url || !*url) return 0;
    if (os->history_count >= MAX_HISTORY) {
        memmove(os->history, os->history + 1, sizeof(HistoryEntry) * (MAX_HISTORY - 1));
        os->history_count--;
    }
    HistoryEntry *entry = &os->history[os->history_count];
    util_strlcpy(entry->url, url, MAX_URL);
    util_strlcpy(entry->title, title ? title : url, MAX_TITLE);

    char host[MAX_DOMAIN] = {0}, path[MAX_URL] = {0};
    int port;
    Protocol proto;
    network_parse_url(url, host, path, &port, &proto);
    util_strlcpy(entry->domain, host, MAX_DOMAIN);

    entry->visit_time = time(NULL);
    entry->visit_count = 1;
    entry->typed_count = 0;
    entry->bookmarked = 0;
    entry->from_search = 0;
    entry->is_secure = (proto == PROTOCOL_HTTPS);
    os->history_count++;
    return 1;
}

void history_print(BrowserOS *os, int count) {
    int n = (count < os->history_count && count > 0) ? count : os->history_count;
    int start = (n < os->history_count) ? os->history_count - n : 0;
    int i;
    printf("\n  History: %d entries (showing last %d)\n", os->history_count, n);
    printf("  %s\n", "----------------------------------------------------------------------");
    for (i = start; i < os->history_count; i++) {
        char time_str[64];
        util_format_time(os->history[i].visit_time, time_str, 64);
        char title_trunc[48];
        util_truncate(os->history[i].title, 45, title_trunc);
        printf("  %-3d | %-45s | %-20s | %s%s\n",
               i + 1, title_trunc, time_str,
               os->history[i].is_secure ? "\x1b[1;32m[HTTPS]\x1b[0m " : "",
               os->history[i].bookmarked ? "\x1b[1;33m[B]\x1b[0m" : "");
    }
    printf("\n");
}

int history_search(BrowserOS *os, const char *query, HistoryEntry *results, int max_results) {
    int count = 0, i;
    for (i = os->history_count - 1; i >= 0 && count < max_results; i--) {
        if (util_contains(os->history[i].title, query) ||
            util_contains(os->history[i].url, query) ||
            util_contains(os->history[i].domain, query)) {
            results[count++] = os->history[i];
        }
    }
    return count;
}

int history_clear(BrowserOS *os) {
    os->history_count = 0;
    return 1;
}

int history_remove(BrowserOS *os, int index) {
    if (index < 0 || index >= os->history_count) return 0;
    if (index < os->history_count - 1) {
        memmove(&os->history[index], &os->history[index + 1],
                sizeof(HistoryEntry) * (os->history_count - index - 1));
    }
    os->history_count--;
    return 1;
}

/* ============================================================
 * BOOKMARK MANAGEMENT
 * ============================================================ */
int bookmark_add(BrowserOS *os, const char *url, const char *title, const char *folder) {
    if (!url || !*url || os->bookmark_count >= MAX_BOOKMARKS) return 0;
    Bookmark *b = &os->bookmarks[os->bookmark_count];
    util_strlcpy(b->url, url, MAX_URL);
    util_strlcpy(b->title, title ? title : url, MAX_TITLE);
    util_strlcpy(b->folder, folder ? folder : "Other", 128);
    b->added = time(NULL);
    b->visit_count = 1;
    b->starred = 0;
    os->bookmark_count++;
    return 1;
}

int bookmark_remove(BrowserOS *os, int index) {
    if (index < 0 || index >= os->bookmark_count) return 0;
    if (index < os->bookmark_count - 1) {
        memmove(&os->bookmarks[index], &os->bookmarks[index + 1],
                sizeof(Bookmark) * (os->bookmark_count - index - 1));
    }
    os->bookmark_count--;
    return 1;
}

void bookmark_print(BrowserOS *os, int count) {
    int n = (count < os->bookmark_count && count > 0) ? count : os->bookmark_count;
    int i;
    printf("\n  Bookmarks: %d\n", os->bookmark_count);
    printf("  %s\n", "----------------------------------------------------------------------");
    for (i = 0; i < n; i++) {
        char title_trunc[48];
        util_truncate(os->bookmarks[i].title, 45, title_trunc);
        char time_str[64];
        util_format_time(os->bookmarks[i].added, time_str, 64);
        printf("  %-3d | %-45s | %-20s | %s%s\n",
               i + 1, title_trunc, time_str,
               os->bookmarks[i].folder,
               os->bookmarks[i].starred ? " *" : "");
    }
    printf("\n");
}

int bookmark_search(BrowserOS *os, const char *query, Bookmark *results, int max_results) {
    int count = 0, i;
    for (i = 0; i < os->bookmark_count && count < max_results; i++) {
        if (util_contains(os->bookmarks[i].title, query) ||
            util_contains(os->bookmarks[i].url, query)) {
            results[count++] = os->bookmarks[i];
        }
    }
    return count;
}

int bookmark_folder_create(BrowserOS *os, const char *name, const char *desc) {
    if (!name || !*name || os->bookmark_folder_count >= MAX_BOOKMARK_FOLDERS) return 0;
    BookmarkFolder *f = &os->bookmark_folders[os->bookmark_folder_count];
    util_strlcpy(f->name, name, 128);
    util_strlcpy(f->description, desc ? desc : "", MAX_LINE);
    os->bookmark_folder_count++;
    return 1;
}

int bookmark_export(BrowserOS *os, const char *filepath) {
    FILE *f = fopen(filepath, "w");
    if (!f) return 0;
    fprintf(f, "<!DOCTYPE NETSCAPE-Bookmark-file-1>\n");
    fprintf(f, "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=UTF-8\">\n");
    fprintf(f, "<TITLE>Bookmarks</TITLE>\n");
    fprintf(f, "<H1>BrowseOS Bookmarks</H1>\n");
    fprintf(f, "<DL><p>\n");
    int i;
    for (i = 0; i < os->bookmark_count; i++) {
        char time_str[64];
        util_format_time(os->bookmarks[i].added, time_str, 64);
        fprintf(f, "    <DT><A HREF=\"%s\" ADD_DATE=\"%ld\">%s</A>\n",
                os->bookmarks[i].url,
                (long)os->bookmarks[i].added,
                os->bookmarks[i].title);
    }
    fprintf(f, "</DL><p>\n");
    fclose(f);
    return 1;
}

int bookmark_import(BrowserOS *os, const char *filepath) {
    FILE *f = fopen(filepath, "r");
    if (!f) return 0;
    char line[MAX_LINE];
    int count = 0;
    while (fgets(line, sizeof(line), f) && count < 100) {
        char *href = strstr(line, "HREF=\"");
        if (href) {
            href += 6;
            char *end = strchr(href, '"');
            if (end) {
                *end = '\0';
                char *title_start = strstr(end + 1, ">");
                char *title_end = strstr(end + 1, "</A>");
                if (title_start && title_end && os->bookmark_count < MAX_BOOKMARKS) {
                    title_start++;
                    *title_end = '\0';
                    bookmark_add(os, href, title_start, "Imported");
                    count++;
                }
            }
        }
    }
    fclose(f);
    return count;
}

/* ============================================================
 * DOWNLOAD MANAGEMENT
 * ============================================================ */
int download_add(BrowserOS *os, const char *url, const char *filename) {
    if (!url || os->download_count >= MAX_DOWNLOADS) return 0;
    Download *d = &os->downloads[os->download_count];
    d->id = os->download_count;
    util_strlcpy(d->url, url, MAX_URL);
    if (filename) {
        util_strlcpy(d->filename, filename, MAX_PATH);
    } else {
        const char *base = strrchr(url, '/');
        snprintf(d->filename, MAX_PATH, "%s", base ? base + 1 : "download");
    }
    snprintf(d->path, MAX_PATH, "%s/%s", os->config.download_dir, d->filename);
    util_strlcpy(d->mime_type, "application/octet-stream", 64);
    d->total_size = util_random(1024, 10485760);
    d->downloaded = 0;
    d->state = DL_QUEUED;
    d->progress = 0.0;
    d->speed_kbps = 0.0;
    d->start_time = time(NULL);
    d->end_time = 0;
    d->resume_supported = 1;
    d->retry_count = 0;
    d->max_retries = 3;
    os->download_count++;
    return 1;
}

int download_cancel(BrowserOS *os, int id) {
    Download *d = download_get(os, id);
    if (!d) return 0;
    d->state = DL_CANCELLED;
    d->end_time = time(NULL);
    return 1;
}

int download_pause(BrowserOS *os, int id) {
    Download *d = download_get(os, id);
    if (!d || d->state != DL_DOWNLOADING) return 0;
    d->state = DL_PAUSED;
    return 1;
}

int download_resume(BrowserOS *os, int id) {
    Download *d = download_get(os, id);
    if (!d || d->state != DL_PAUSED) return 0;
    d->state = DL_DOWNLOADING;
    d->retry_count = 0;
    return 1;
}

void download_print(BrowserOS *os, int count) {
    int n = (count < os->download_count && count > 0) ? count : os->download_count;
    int i;
    printf("\n  Downloads: %d\n", os->download_count);
    printf("  %s\n", "----------------------------------------------------------------------------");
    for (i = 0; i < n; i++) {
        Download *d = &os->downloads[i];
        char size_str[32], prog_str[32];
        util_format_size(d->total_size, size_str, 32);
        util_progress_bar(d->progress, prog_str, 20);
        printf("  %-3d | %-30s | %-8s | %s %s\n",
               d->id,
               d->filename,
               size_str,
               prog_str,
               d->state == DL_QUEUED ? "Queued" :
               d->state == DL_DOWNLOADING ? "Downloading" :
               d->state == DL_PAUSED ? "Paused" :
               d->state == DL_COMPLETE ? "Complete" :
               d->state == DL_FAILED ? "Failed" : "Cancelled");
    }
    printf("\n");
}

Download* download_get(BrowserOS *os, int id) {
    if (id >= 0 && id < os->download_count) return &os->downloads[id];
    return NULL;
}

/* ============================================================
 * PROCESS MANAGEMENT
 * ============================================================ */
static int next_pid = 100;

int proc_create(BrowserOS *os, const char *name, ProcessType type, int priority) {
    if (os->process_count >= MAX_PROCESSES) return -1;
    Process *p = &os->processes[os->process_count];
    p->pid = next_pid++;
    util_strlcpy(p->name, name, 64);
    p->type = type;
    p->state = PROC_RUNNING;
    p->priority = (priority >= 0 && priority <= 10) ? priority : 5;
    p->cpu_usage = (double)util_random(0, 50) + (double)util_random(0, 99) / 100.0;
    p->memory_mb = (double)util_random(5, 200) + (double)util_random(0, 99) / 100.0;
    p->start_time = time(NULL);
    p->thread_count = util_random(1, 8);
    p->handle_count = util_random(10, 200);
    p->num_children = 0;
    os->process_count++;
    return p->pid;
}

int proc_kill(BrowserOS *os, int pid) {
    int i;
    for (i = 0; i < os->process_count; i++) {
        if (os->processes[i].pid == pid) {
            if (i < os->process_count - 1) {
                memmove(&os->processes[i], &os->processes[i + 1],
                        sizeof(Process) * (os->process_count - i - 1));
            }
            os->process_count--;
            return 1;
        }
    }
    return 0;
}

void proc_print_all(BrowserOS *os) {
    int i;
    double total_mem = 0;
    printf("\n  Process List: %d running\n", os->process_count);
    printf("  %s\n", "--------------------------------------------------------------------");
    printf("  %-5s %-20s %-15s %-10s %-8s %-10s\n",
           "PID", "Name", "Type", "State", "CPU%", "Memory");
    printf("  %s\n", "--------------------------------------------------------------------");
    for (i = 0; i < os->process_count; i++) {
        Process *p = &os->processes[i];
        const char *type_name = p->type == PROC_BROWSER ? "Browser" :
                                p->type == PROC_RENDERER ? "Renderer" :
                                p->type == PROC_NETWORK ? "Network" :
                                p->type == PROC_GPU ? "GPU" :
                                p->type == PROC_STORAGE ? "Storage" :
                                p->type == PROC_EXTENSION ? "Extension" :
                                p->type == PROC_UTILITY ? "Utility" : "Sandbox";
        const char *state_name = p->state == PROC_RUNNING ? "Running" :
                                 p->state == PROC_SLEEPING ? "Sleeping" :
                                 p->state == PROC_BLOCKED ? "Blocked" :
                                 p->state == PROC_STOPPED ? "Stopped" :
                                 p->state == PROC_ZOMBIE ? "Zombie" : "Idle";
        printf("  %-5d %-20s %-15s %-10s %-8.1f %-10.1f\n",
               p->pid, p->name, type_name, state_name,
               p->cpu_usage, p->memory_mb);
        total_mem += p->memory_mb;
    }
    printf("  %s\n", "--------------------------------------------------------------------");
    printf("  Total Memory: %.1f MB | Total CPU: %.1f%%\n", total_mem, os->cpu_load);
    printf("\n");
}

/* ============================================================
 * EXTENSION MANAGEMENT
 * ============================================================ */
int ext_install(BrowserOS *os, const char *name, const char *version, const char *author, const char *desc) {
    if (os->extension_count >= MAX_EXTENSIONS) return 0;
    Extension *ext = &os->extensions[os->extension_count];
    snprintf(ext->id, 64, "ext_%d", os->extension_count);
    util_strlcpy(ext->name, name, 128);
    util_strlcpy(ext->version, version, 16);
    util_strlcpy(ext->author, author, 128);
    util_strlcpy(ext->description, desc, MAX_LINE);
    ext->enabled = 1;
    ext->install_time = (int)time(NULL);
    ext->update_time = (int)time(NULL);
    os->extension_count++;
    return 1;
}

void ext_print_all(BrowserOS *os) {
    int i;
    printf("\n  Extensions: %d installed\n", os->extension_count);
    printf("  %s\n", "----------------------------------------------------------------------");
    for (i = 0; i < os->extension_count; i++) {
        printf("  %-3d | %-25s | %-8s | %-20s | %s\n",
               i + 1,
               os->extensions[i].name,
               os->extensions[i].version,
               os->extensions[i].author,
               os->extensions[i].enabled ? "Enabled" : "Disabled");
    }
    printf("\n");
}

/* ============================================================
 * SHELL COMMAND IMPLEMENTATIONS
 * ============================================================ */
int cmd_open(BrowserOS *os, int argc, char *argv[]) {
    if (argc < 2) {
        printf("  Usage: open <url>\n");
        printf("  Open a URL in a new tab. Examples:\n");
        printf("    open google.com\n");
        printf("    open https://github.com\n");
        printf("    open about:version\n");
        return 0;
    }

    char url[MAX_URL];
    network_normalize_url(argv[1], url, MAX_URL);

    Tab *t = tab_create(os, url, os->config.default_engine);
    if (!t) {
        printf("  %s\n", lang_get(os, "page.loading"));
        return 0;
    }

    char out_buf[MAX_RENDER_BUF];
    if (t->page) {
        renderer_format_output(t->page, out_buf, MAX_RENDER_BUF, os->render_width, os);
        printf("%s\n", out_buf);
    } else {
        printf("  [%s] %s\n", url, t->state == TAB_ERROR ? "Failed to load" : "Loaded");
    }

    return 1;
}

int cmd_tabs(BrowserOS *os) {
    tab_print_all(os);
    return 1;
}

int cmd_tab_new(BrowserOS *os, const char *url) {
    if (!url || !*url) url = "about:blank";
    Tab *t = tab_create(os, url, os->config.default_engine);
    if (t) {
        printf("  Created tab #%d: %s\n", t->id, t->title);
    } else {
        printf("  Failed to create tab\n");
    }
    return (t != NULL);
}

int cmd_tab_close(BrowserOS *os, int id) {
    if (tab_destroy(os, id)) {
        printf("  Closed tab #%d\n", id);
        return 1;
    }
    printf("  Tab #%d not found\n", id);
    return 0;
}

int cmd_tab_switch(BrowserOS *os, int id) {
    if (tab_switch_to(os, id)) {
        Tab *t = tab_get_active(os);
        printf("  Switched to tab #%d: %s\n", t->id, t->title);
        return 1;
    }
    printf("  Tab #%d not found\n", id);
    return 0;
}

int cmd_engine(BrowserOS *os, int argc, char *argv[]) {
    if (argc < 2) {
        Engine *e = engine_get_active(os);
        printf("  Active Engine: %s %s\n", e ? e->name : "None", e ? e->version : "");
        return 0;
    }

    int i;
    for (i = 0; i < os->num_engines; i++) {
        if (util_strincmp(os->engines[i].name, argv[1], strlen(argv[1])) == 0) {
            engine_set_active(os, i);
            printf("  %s\n", lang_tr(os, "engine.switched", os->engines[i].name, os->engines[i].version));
            return 1;
        }
    }

    /* Try matching by number */
    int idx = atoi(argv[1]) - 1;
    if (idx >= 0 && idx < os->num_engines) {
        engine_set_active(os, idx);
        printf("  %s\n", lang_tr(os, "engine.switched", os->engines[idx].name, os->engines[idx].version));
        return 1;
    }

    printf("  Engine '%s' not found. Type 'engines' to see available engines.\n", argv[1]);
    return 0;
}

int cmd_engines(BrowserOS *os, int argc, char *argv[]) {
    (void)argc; (void)argv;
    engine_print_all(os);
    return 1;
}

int cmd_engine_info(BrowserOS *os, int argc, char *argv[]) {
    Engine *e;
    if (argc >= 2) {
        int i;
        for (i = 0; i < os->num_engines; i++) {
            if (util_strincmp(os->engines[i].name, argv[1], strlen(argv[1])) == 0) {
                e = &os->engines[i];
                engine_print_info(e);
                return 1;
            }
        }
        printf("  Engine '%s' not found\n", argv[1]);
        return 0;
    }
    e = engine_get_active(os);
    engine_print_info(e);
    return 1;
}

int cmd_lang(BrowserOS *os, int argc, char *argv[]) {
    if (argc < 2) {
        printf("  Active Language: %s (%s)\n", lang_name(os), lang_code(os->active_language));
        return 0;
    }
    Language lang = lang_from_code(argv[1]);
    if (lang_switch(os, lang)) {
        printf("  %s\n", lang_tr(os, "lang.switched", os->languages[lang].name, os->languages[lang].native_name));
        return 1;
    }
    printf("  Language '%s' not found. Use 'langs' to see available languages.\n", argv[1]);
    return 0;
}

int cmd_langs(BrowserOS *os, int argc, char *argv[]) {
    (void)argc; (void)argv;
    lang_print_all(os);
    return 1;
}

int cmd_history(BrowserOS *os, int argc, char *argv[]) {
    int count = 20;
    if (argc >= 2) {
        if (strcmp(argv[1], "clear") == 0) {
            history_clear(os);
            printf("  History cleared\n");
            return 1;
        }
        if (strcmp(argv[1], "search") == 0 && argc >= 3) {
            HistoryEntry results[50];
            int n = history_search(os, argv[2], results, 50);
            printf("  Search results for '%s': %d\n", argv[2], n);
            int i;
            for (i = 0; i < n; i++) {
                printf("  %-3d %s (%s)\n", i + 1, results[i].title, results[i].url);
            }
            return 1;
        }
        if (strcmp(argv[1], "export") == 0 && argc >= 3) {
            if (history_export(os, argv[2])) {
                printf("  History exported to %s\n", argv[2]);
                return 1;
            }
            printf("  Failed to export history\n");
            return 0;
        }
        count = atoi(argv[1]);
        if (count <= 0) count = 20;
    }
    history_print(os, count);
    return 1;
}

int history_export(BrowserOS *os, const char *filepath) {
    FILE *f = fopen(filepath, "w");
    if (!f) return 0;
    fprintf(f, "URL,Title,VisitTime,VisitCount,Domain,Secure\n");
    int i;
    for (i = 0; i < os->history_count; i++) {
        char time_str[64];
        util_format_time(os->history[i].visit_time, time_str, 64);
        fprintf(f, "\"%s\",\"%s\",%s,%d,\"%s\",%d\n",
                os->history[i].url,
                os->history[i].title,
                time_str,
                os->history[i].visit_count,
                os->history[i].domain,
                os->history[i].is_secure);
    }
    fclose(f);
    return 1;
}

int cmd_bookmark(BrowserOS *os, int argc, char *argv[]) {
    if (argc < 3) {
        printf("  Usage: bookmark <add|remove|search|export|import> <args>\n");
        printf("  Examples:\n");
        printf("    bookmark add https://github.com GitHub\n");
        printf("    bookmark remove 1\n");
        printf("    bookmark search github\n");
        printf("    bookmark export bookmarks.html\n");
        return 0;
    }

    if (strcmp(argv[1], "add") == 0 && argc >= 3) {
        const char *title = (argc >= 4) ? argv[3] : argv[2];
        bookmark_add(os, argv[2], title, "Other");
        printf("  %s\n", lang_tr(os, "bookmark.added", title));
        return 1;
    }

    if (strcmp(argv[1], "remove") == 0 && argc >= 3) {
        int idx = atoi(argv[2]) - 1;
        if (bookmark_remove(os, idx)) {
            printf("  Removed bookmark #%d\n", idx + 1);
            return 1;
        }
        printf("  Bookmark #%d not found\n", idx + 1);
        return 0;
    }

    if (strcmp(argv[1], "search") == 0 && argc >= 3) {
        Bookmark results[20];
        int n = bookmark_search(os, argv[2], results, 20);
        printf("  Found %d bookmarks matching '%s':\n", n, argv[2]);
        int i;
        for (i = 0; i < n; i++) {
            printf("  %-3d %s (%s)\n", i + 1, results[i].title, results[i].url);
        }
        return 1;
    }

    if (strcmp(argv[1], "export") == 0 && argc >= 3) {
        return bookmark_export(os, argv[2]);
    }

    if (strcmp(argv[1], "import") == 0 && argc >= 3) {
        int n = bookmark_import(os, argv[2]);
        printf("  Imported %d bookmarks\n", n);
        return n > 0;
    }

    return 0;
}

int cmd_bookmarks(BrowserOS *os, int argc, char *argv[]) {
    (void)argc; (void)argv;
    bookmark_print(os, 0);
    return 1;
}

int cmd_status(BrowserOS *os, int argc, char *argv[]) {
    (void)argc; (void)argv;
    printf("\n");
    printf("  \x1b[1;36m+============================================================+\x1b[0m\n");
    printf("  \x1b[1;36m|              \x1b[1;33mBROWSEOS SYSTEM STATUS\x1b[1;36m                     |\x1b[0m\n");
    printf("  \x1b[1;36m+============================================================+\x1b[0m\n");
    printf("  \x1b[1;37mOS:\x1b[0m\n");
    printf("    Version       : %s (%s)\n", BROWSEOS_VERSION, BROWSEOS_CODENAME);
    printf("    Build         : %d\n", BROWSEOS_BUILD);
    printf("    User          : %s@%s\n", os->os.username, os->os.hostname);
    char boot_str[64];
    util_format_time(os->os.boot_time, boot_str, 64);
    printf("    Boot Time     : %s\n", boot_str);
    printf("    Uptime        : %d seconds\n", os->os.uptime_seconds);
    printf("    Session       : #%d\n", os->os.session_id);
    printf("  \x1b[1;37mEngine:\x1b[0m\n");
    Engine *e = engine_get_active(os);
    if (e) printf("    Active        : %s %s\n", e->name, e->version);
    printf("    Available     : %d engines\n", os->num_engines);
    printf("  \x1b[1;37mLanguage:\x1b[0m\n");
    printf("    Active        : %s (%s)\n", lang_name(os), lang_code(os->active_language));
    printf("    RTL Mode      : %s\n", lang_is_rtl(os) ? "Yes" : "No");
    printf("  \x1b[1;37mTabs:\x1b[0m\n");
    printf("    Open          : %d / %d\n", os->num_tabs, MAX_TABS);
    printf("    Active        : #%d\n", os->active_tab);
    printf("  \x1b[1;37mData:\x1b[0m\n");
    printf("    History       : %d entries\n", os->history_count);
    printf("    Bookmarks     : %d entries in %d folders\n", os->bookmark_count, os->bookmark_folder_count);
    printf("    Cache         : %d entries\n", os->cache_count);
    printf("    Cookies       : %d cookies\n", os->cookie_count);
    printf("    Certificates  : %d certs\n", os->certificate_count);
    printf("  \x1b[1;37mDownloads:\x1b[0m\n");
    printf("    Active        : %d\n", os->download_count);
    printf("  \x1b[1;37mSystem:\x1b[0m\n");
    printf("    Processes     : %d running\n", os->process_count);
    printf("    Extensions    : %d installed\n", os->extension_count);
    printf("    Proxy         : %s\n", os->proxy.enabled ? os->proxy.host : "None");
    printf("    Privacy Mode  : %s\n", os->config.enable_privacy_mode ? "ENABLED" : "Disabled");
    printf("    Commands Run  : %d\n", os->os.command_count);
    printf("  \x1b[1;36m+============================================================+\x1b[0m\n\n");
    return 1;
}

int cmd_processes(BrowserOS *os, int argc, char *argv[]) {
    (void)argc; (void)argv;
    proc_print_all(os);
    return 1;
}

int cmd_about(BrowserOS *os, int argc, char *argv[]) {
    (void)argc; (void)argv;
    printf("\n");
    printf("  \x1b[1;36m+============================================================+\x1b[0m\n");
    printf("  \x1b[1;36m|        \x1b[1;33mBrowseOS - The Browser Operating System\x1b[1;36m            |\x1b[0m\n");
    printf("  \x1b[1;36m+============================================================+\x1b[0m\n");
    printf("  \x1b[1;37mVersion:\x1b[0m  %s\n", BROWSEOS_VERSION);
    printf("  \x1b[1;37mCodename:\x1b[0m %s\n", BROWSEOS_CODENAME);
    printf("  \x1b[1;37mBuild:\x1b[0m    %d\n", BROWSEOS_BUILD);
    printf("  \x1b[1;37mAuthor:\x1b[0m   GoldenSakuraZero\n");
    printf("  \x1b[1;37mLicense:\x1b[0m  Open Source\n");
    printf("  \n");
    printf("  BrowseOS is a feature-rich, command-line based browser operating\n");
    printf("  system simulator. It includes multiple browser engines, a full\n");
    printf("  language localization system, network stack simulation, DOM\n");
    printf("  rendering engine, tab management, history, bookmarks, downloads,\n");
    printf("  process management, extensions, and more.\n");
    printf("  \n");
    printf("  \x1b[1;37mEngines:\x1b[0m    WebKit, Gecko, Blink, Trident, EdgeHTML, Servo, Goanna, NetSurf\n");
    printf("  \x1b[1;37mLanguages:\x1b[0m  24 languages with full RTL support\n");
    printf("  \x1b[1;37mFeatures:\x1b[0m   HTML rendering, CSS, JS, DOM, Cache, Cookies, SSL/TLS\n");
    printf("  \x1b[1;36m+============================================================+\x1b[0m\n\n");
    return 1;
}

int cmd_version(BrowserOS *os, int argc, char *argv[]) {
    (void)argc; (void)argv;
    printf("  BrowseOS v%s (%s) Build %d\n", BROWSEOS_VERSION, BROWSEOS_CODENAME, BROWSEOS_BUILD);
    printf("  Active Engine: %s\n", engine_get_active(os) ? engine_get_active(os)->name : "None");
    printf("  Language: %s\n", lang_name(os));
    return 1;
}

int cmd_clear(BrowserOS *os, int argc, char *argv[]) {
    (void)argc; (void)argv;
    system("cls");
    shell_print_banner(os);
    return 1;
}

int cmd_echo(BrowserOS *os, int argc, char *argv[]) {
    (void)os;
    int i;
    for (i = 1; i < argc; i++) {
        printf("%s%c", argv[i], (i < argc - 1) ? ' ' : '\n');
    }
    if (argc < 2) printf("\n");
    return 1;
}

int cmd_date(BrowserOS *os, int argc, char *argv[]) {
    (void)argc; (void)argv;
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char buf[128];
    strftime(buf, sizeof(buf), "%A, %B %d, %Y %H:%M:%S %Z", tm_info);
    printf("  %s\n", buf);
    return 1;
}

int cmd_whoami(BrowserOS *os, int argc, char *argv[]) {
    (void)argc; (void)argv;
    printf("  %s@%s\n", os->os.username, os->os.hostname);
    return 1;
}

int cmd_help(BrowserOS *os, int argc, char *argv[]) {
    (void)argc; (void)argv;
    (void)os;
    const char *help_text = "\n"
        "  \x1b[1;36m+============================================================+\x1b[0m\n"
        "  \x1b[1;36m|              \x1b[1;33mBrowseOS COMMAND REFERENCE\x1b[1;36m                  |\x1b[0m\n"
        "  \x1b[1;36m+============================================================+\x1b[0m\n"
        "\n"
        "  \x1b[1;37mNAVIGATION:\x1b[0m\n"
        "    open <url>         Open a URL in a new tab\n"
        "    visit <url>        Alias for open\n"
        "    search <query>     Search the web\n"
        "\n"
        "  \x1b[1;37mTAB MANAGEMENT:\x1b[0m\n"
        "    tabs               List all open tabs\n"
        "    tab new <url>      Create a new tab\n"
        "    tab close <id>     Close a tab\n"
        "    tab switch <id>    Switch to a tab\n"
        "    tab info <id>      Show tab details\n"
        "    tab reload         Reload current tab\n"
        "    tab back           Go back in history\n"
        "    tab forward        Go forward in history\n"
        "\n"
        "  \x1b[1;37mENGINE MANAGEMENT:\x1b[0m\n"
        "    engines            List all browser engines\n"
        "    engine <name>      Switch browser engine\n"
        "    engine info <name> Show engine details\n"
        "    engine compare     Compare all engines\n"
        "\n"
        "  \x1b[1;37mLANGUAGE:\x1b[0m\n"
        "    langs              List all languages\n"
        "    lang <code>        Switch language (e.g., lang ja)\n"
        "\n"
        "  \x1b[1;37mHISTORY:\x1b[0m\n"
        "    history            Show browsing history\n"
        "    history <n>        Show last n entries\n"
        "    history clear      Clear history\n"
        "    history search <q> Search history\n"
        "    history export <f> Export history to CSV\n"
        "\n"
        "  \x1b[1;37mBOOKMARKS:\x1b[0m\n"
        "    bookmarks          List bookmarks\n"
        "    bookmark add <url> [title]  Add bookmark\n"
        "    bookmark remove <n>        Remove bookmark\n"
        "    bookmark search <q>        Search bookmarks\n"
        "    bookmark export <file>     Export bookmarks\n"
        "    bookmark import <file>     Import bookmarks\n"
        "\n"
        "  \x1b[1;37mSETTINGS:\x1b[0m\n"
        "    settings           Show current settings\n"
        "    set <key> <value>  Change a setting\n"
        "    theme <name>       Change theme\n"
        "    themes             List available themes\n"
        "\n"
        "  \x1b[1;37mDOWNLOAD & DATA:\x1b[0m\n"
        "    downloads          Show download list\n"
        "    download <url>     Download a file\n"
        "    cache              Show cache entries\n"
        "    cache clear        Clear cache\n"
        "    cookies            Show cookies\n"
        "    cookies clear      Clear cookies\n"
        "\n"
        "  \x1b[1;37mNETWORK:\x1b[0m\n"
        "    proxy set <h:p>    Set HTTP proxy\n"
        "    proxy off          Disable proxy\n"
        "    useragent          Show user agent\n"
        "    useragent set <s>  Set user agent\n"
        "    dns <host>         DNS lookup\n"
        "    ping <host>        Ping a host\n"
        "\n"
        "  \x1b[1;37mSYSTEM:\x1b[0m\n"
        "    status             Show system status\n"
        "    processes          Show running processes\n"
        "    about              About BrowseOS\n"
        "    version            Show version\n"
        "    date               Show current date/time\n"
        "    whoami             Show current user\n"
        "    clear              Clear screen\n"
        "    echo <text>        Print text\n"
        "    help [command]     Show this help\n"
        "    exit               Exit BrowseOS\n"
        "    quit               Alias for exit\n"
        "\n"
        "  \x1b[1;36m+============================================================+\x1b[0m\n";

    printf("%s", help_text);
    return 1;
}

int cmd_settings(BrowserOS *os, int argc, char *argv[]) {
    (void)argc; (void)argv;
    config_print(os);
    return 1;
}

int cmd_set(BrowserOS *os, int argc, char *argv[]) {
    if (argc < 3) {
        printf("  Usage: set <key> <value>\n");
        printf("  Common settings: home, search, javascript, cookies, cache, images,\n");
        printf("    popups, privacy, dnt, font_size, zoom, notifications, webgl\n");
        return 0;
    }
    if (config_set(os, argv[1], argv[2])) {
        printf("  %s\n", lang_tr(os, "setting.changed", argv[1], argv[2]));
        return 1;
    }
    printf("  Unknown setting: %s. Use 'settings' to see current values.\n", argv[1]);
    return 0;
}

int cmd_theme(BrowserOS *os, int argc, char *argv[]) {
    if (argc < 2) {
        printf("  Current theme: %s\n", theme_name(os->config.theme));
        printf("  Use 'themes' to see all available themes.\n");
        return 0;
    }
    int i, n = sizeof(theme_data)/sizeof(theme_data[0]);
    for (i = 0; i < n; i++) {
        if (strcmp(theme_data[i].name, argv[1]) == 0) {
            theme_apply(os, (Theme)i);
            printf("  Theme changed to: %s\n", argv[1]);
            return 1;
        }
    }
    /* Try by number */
    int idx = atoi(argv[1]) - 1;
    if (idx >= 0 && idx < n) {
        theme_apply(os, (Theme)idx);
        printf("  Theme changed to: %s\n", theme_data[idx].name);
        return 1;
    }
    printf("  Theme '%s' not found. Use 'themes' to list available themes.\n", argv[1]);
    return 0;
}

int cmd_themes(BrowserOS *os, int argc, char *argv[]) {
    (void)argc; (void)argv;
    theme_print_all();
    return 1;
}

int cmd_proxy(BrowserOS *os, int argc, char *argv[]) {
    if (argc < 2) {
        printf("  Proxy: %s\n", os->proxy.enabled ? os->proxy.host : "None (direct connection)");
        if (os->proxy.enabled) printf("  Port: %d\n", os->proxy.port);
        return 0;
    }
    if (strcmp(argv[1], "set") == 0 && argc >= 3) {
        char *colon = strchr(argv[2], ':');
        if (colon) {
            *colon = '\0';
            network_proxy_set(os, argv[2], atoi(colon + 1));
            printf("  Proxy set to %s:%d\n", os->proxy.host, os->proxy.port);
        } else {
            network_proxy_set(os, argv[2], 8080);
            printf("  Proxy set to %s:%d\n", os->proxy.host, os->proxy.port);
        }
        return 1;
    }
    if (strcmp(argv[1], "off") == 0 || strcmp(argv[1], "disable") == 0) {
        network_proxy_disable(os);
        printf("  Proxy disabled\n");
        return 1;
    }
    printf("  Usage: proxy set <host:port> | proxy off\n");
    return 0;
}

int cmd_useragent(BrowserOS *os, int argc, char *argv[]) {
    if (argc < 2) {
        printf("  User-Agent: %s\n", engine_get_user_agent(os));
        return 0;
    }
    if (strcmp(argv[1], "set") == 0 && argc >= 3) {
        Engine *e = engine_get_active(os);
        if (e) {
            util_strlcpy(e->user_agent, argv[2], MAX_UA_STR);
            printf("  User-Agent updated\n");
        }
        return 1;
    }
    if (strcmp(argv[1], "reset") == 0) {
        engine_init_all(os);
        printf("  User-Agent reset to default\n");
        return 1;
    }
    return 0;
}

int cmd_dns(BrowserOS *os, const char *host) {
    if (!host) {
        printf("  Usage: dns <hostname>\n");
        return 0;
    }
    char ip[64];
    if (network_resolve_dns(host, ip, sizeof(ip))) {
        printf("  DNS Lookup for %s:\n", host);
        printf("    IPv4: %s\n", ip);
        return 1;
    }
    printf("  DNS lookup failed\n");
    return 0;
}

int cmd_downloads(BrowserOS *os, int argc, char *argv[]) {
    (void)argc; (void)argv;
    download_print(os, 0);
    return 1;
}

int cmd_download(BrowserOS *os, int argc, char *argv[]) {
    const char *url = (argc >= 2) ? argv[1] : NULL;
    if (!url) {
        printf("  Usage: download <url>\n");
        return 0;
    }
    return download_add(os, url, NULL);
}

int cmd_cache(BrowserOS *os, int argc, char *argv[]) {
    (void)argc; (void)argv;
    cache_print(os);
    return 1;
}

int cmd_cache_clear(BrowserOS *os) {
    cache_clear(os);
    printf("  Cache cleared (%d entries removed)\n", os->cache_count);
    return 1;
}

int cmd_cookies(BrowserOS *os, int argc, char *argv[]) {
    (void)argc; (void)argv;
    cookie_print(os);
    return 1;
}

int cmd_cookies_clear(BrowserOS *os) {
    int n = os->cookie_count;
    cookie_clear(os);
    printf("  %d cookies cleared\n", n);
    return 1;
}

int cmd_search(BrowserOS *os, int argc, char *argv[]) {
    if (argc < 2) {
        printf("  Usage: search <query>\n");
        return 0;
    }
    char query[MAX_INPUT] = "";
    int i;
    for (i = 1; i < argc; i++) {
        if (i > 1) strcat(query, "+");
        strcat(query, argv[i]);
    }
    char url[MAX_URL];
    if (strcmp(os->config.search_engine, "Google") == 0)
        snprintf(url, MAX_URL, "https://google.com/search?q=%s", query);
    else if (strcmp(os->config.search_engine, "Bing") == 0)
        snprintf(url, MAX_URL, "https://bing.com/search?q=%s", query);
    else if (strcmp(os->config.search_engine, "DuckDuckGo") == 0)
        snprintf(url, MAX_URL, "https://duckduckgo.com/?q=%s", query);
    else
        snprintf(url, MAX_URL, "https://google.com/search?q=%s", query);

    return cmd_open(os, 2, (char*[]){"open", url});
}

/* ============================================================
 * SHELL PARSER AND DISPATCHER
 * ============================================================ */
int shell_parse_command(const char *input, char *cmd, char args[][MAX_INPUT], int *num_args) {
    char buf[MAX_INPUT];
    util_strlcpy(buf, input, MAX_INPUT);
    util_trim(buf);

    if (buf[0] == '\0') return 0;

    *num_args = 0;
    cmd[0] = '\0';

    char *p = buf;
    int in_quotes = 0;
    char current[MAX_INPUT] = "";
    int ci = 0;

    while (*p) {
        if (*p == '"') {
            in_quotes = !in_quotes;
            p++;
            continue;
        }
        if (*p == ' ' && !in_quotes) {
            if (ci > 0) {
                current[ci] = '\0';
                if (*num_args == 0) {
                    util_strlcpy(cmd, current, MAX_INPUT);
                } else if (*num_args < MAX_PARAMS) {
                    util_strlcpy(args[*num_args - 1], current, MAX_INPUT);
                }
                (*num_args)++;
                ci = 0;
            }
            p++;
            continue;
        }
        if (ci < MAX_INPUT - 1) current[ci++] = *p;
        p++;
    }
    if (ci > 0) {
        current[ci] = '\0';
        if (*num_args == 0) {
            util_strlcpy(cmd, current, MAX_INPUT);
        } else if (*num_args < MAX_PARAMS) {
            util_strlcpy(args[*num_args - 1], current, MAX_INPUT);
        }
        (*num_args)++;
    }

    util_lower(cmd);
    return 1;
}

static const struct {
    const char *name;
    const char *aliases[4];
    int (*handler)(BrowserOS *os, int argc, char *argv[]);
} cmd_table[] = {
    {"open", {"visit", "", ""}, cmd_open},
    {"help", {"?", "h", ""}, cmd_help},
    {"tabs", {"tablist", "", ""}, NULL},
    {"tab", {"", "", ""}, NULL},
    {"engine", {"eng", "", ""}, cmd_engine},
    {"engines", {"engine ls", "", ""}, cmd_engines},
    {"lang", {"language", "", ""}, cmd_lang},
    {"langs", {"languages", "", ""}, cmd_langs},
    {"history", {"hist", "h", ""}, cmd_history},
    {"bookmarks", {"bm", "b", ""}, cmd_bookmarks},
    {"bookmark", {"bma", "", ""}, cmd_bookmark},
    {"status", {"sysinfo", "info", "stats"}, cmd_status},
    {"processes", {"ps", "procs", ""}, cmd_processes},
    {"about", {"", "", ""}, cmd_about},
    {"version", {"ver", "--version", "-v"}, cmd_version},
    {"clear", {"cls", "clean", ""}, cmd_clear},
    {"echo", {"print", "", ""}, cmd_echo},
    {"date", {"time", "now", ""}, cmd_date},
    {"whoami", {"user", "who", ""}, cmd_whoami},
    {"settings", {"config", "conf", ""}, cmd_settings},
    {"set", {"configure", "", ""}, cmd_set},
    {"theme", {"themes", "", ""}, cmd_theme},
    {"themes", {"", "", ""}, cmd_themes},
    {"proxy", {"", "", ""}, cmd_proxy},
    {"useragent", {"ua", "user-agent", ""}, cmd_useragent},
    {"search", {"find", "s", ""}, cmd_search},
    {"downloads", {"dls", "dl", ""}, cmd_downloads},
    {"download", {"dl", "get", ""}, cmd_download},
    {"cache", {"", "", ""}, cmd_cache},
    {"cookies", {"", "", ""}, cmd_cookies},
};

static int handle_tab_command(BrowserOS *os, int argc, char *argv[]) {
    if (argc < 2) {
        tab_print_all(os);
        return 1;
    }
    if (strcmp(argv[1], "new") == 0) {
        const char *url = (argc >= 3) ? argv[2] : "about:blank";
        return cmd_tab_new(os, url);
    }
    if (strcmp(argv[1], "close") == 0 && argc >= 3) {
        return cmd_tab_close(os, atoi(argv[2]));
    }
    if (strcmp(argv[1], "switch") == 0 && argc >= 3) {
        return cmd_tab_switch(os, atoi(argv[2]));
    }
    if (strcmp(argv[1], "switch") == 0 && argc >= 3) {
        return cmd_tab_switch(os, atoi(argv[2]));
    }
    if (strcmp(argv[1], "info") == 0 && argc >= 3) {
        Tab *t = tab_get(os, atoi(argv[2]));
        if (t) tab_print(t);
        else printf("  Tab #%s not found\n", argv[2]);
        return 1;
    }
    if (strcmp(argv[1], "reload") == 0) {
        Tab *t = tab_get_active(os);
        if (t) { tab_reload(os, t->id); printf("  Reloaded tab #%d\n", t->id); }
        else printf("  No active tab\n");
        return 1;
    }
    if (strcmp(argv[1], "back") == 0) {
        Tab *t = tab_get_active(os);
        if (t && tab_go_back(os, t->id)) printf("  Went back\n");
        else printf("  No back history\n");
        return 1;
    }
    if (strcmp(argv[1], "forward") == 0) {
        Tab *t = tab_get_active(os);
        if (t && tab_go_forward(os, t->id)) printf("  Went forward\n");
        else printf("  No forward history\n");
        return 1;
    }
    return 0;
}

int shell_handle_command(BrowserOS *os, const char *input) {
    char cmd[64];
    char args[MAX_PARAMS][MAX_INPUT];
    int num_args = 0;

    if (!shell_parse_command(input, cmd, args, &num_args)) return 1;

    os->os.command_count++;

    /* Save to command history */
    if (os->cmd_history_count < MAX_CMD_HISTORY) {
        util_strlcpy(os->cmd_history[os->cmd_history_count], input, MAX_INPUT);
        os->cmd_history_count++;
    } else {
        memmove(os->cmd_history, os->cmd_history + 1, sizeof(os->cmd_history[0]) * (MAX_CMD_HISTORY - 1));
        util_strlcpy(os->cmd_history[MAX_CMD_HISTORY - 1], input, MAX_INPUT);
    }

    /* Build argv array */
    char *argv[MAX_PARAMS + 1];
    argv[0] = cmd;
    int i;
    for (i = 0; i < num_args - 1; i++) argv[i + 1] = args[i];
    int argc = num_args;

    /* Handle exit/quit */
    if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0 || strcmp(cmd, "q") == 0) {
        if (os->config.warn_quit) {
            printf("  Are you sure you want to exit? (y/N): ");
            char response[16];
            if (fgets(response, sizeof(response), stdin)) {
                if (response[0] == 'y' || response[0] == 'Y') {
                    os->os.running = 0;
                    printf("  Shutting down BrowseOS...\n");
                }
            }
        } else {
            os->os.running = 0;
            printf("  Shutting down BrowseOS...\n");
        }
        return 1;
    }

    /* Handle tab command separately */
    if (strcmp(cmd, "tab") == 0) {
        if (!handle_tab_command(os, argc, argv)) {
            tab_print_all(os);
        }
        return 1;
    }

    /* Handle engine info/comparison */
    if (strcmp(cmd, "engine") == 0 && argc >= 2) {
        if (strcmp(argv[1], "info") == 0) {
            if (argc >= 3) {
                cmd_engine_info(os, argc, (char*[]){"engine", argv[2]});
            } else {
                cmd_engine_info(os, 1, (char*[]){"engine"});
            }
            return 1;
        }
        if (strcmp(argv[1], "compare") == 0) {
            engine_compare(os);
            return 1;
        }
    }

    /* Dispatch commands */
    for (i = 0; i < (int)(sizeof(cmd_table) / sizeof(cmd_table[0])); i++) {
        if (strcmp(cmd, cmd_table[i].name) == 0) {
            if (cmd_table[i].handler) {
                return cmd_table[i].handler(os, argc, argv);
            }
            break;
        }
        int j;
        for (j = 0; j < 4 && cmd_table[i].aliases[j][0]; j++) {
            if (strcmp(cmd, cmd_table[i].aliases[j]) == 0) {
                if (cmd_table[i].handler) {
                    return cmd_table[i].handler(os, argc, argv);
                }
                break;
            }
        }
    }

    /* Try handling as a URL */
    if (network_is_valid_url(cmd) || util_contains(cmd, ".")) {
        char *new_argv[3] = {"open", cmd, NULL};
        return cmd_open(os, 2, new_argv);
    }

    printf("  %s\n", lang_tr(os, "cmd.notfound", cmd));
    return 0;
}

void shell_print_banner(BrowserOS *os) {
    printf("\n");
    printf("  \x1b[1;36m  ____                                          \x1b[0m\n");
    printf("  \x1b[1;36m |  _ \\ ___ _ __ ___  ___  _ __   ___  ___  ___ \x1b[0m\n");
    printf("  \x1b[1;36m | |_) / _ \\ '__/ __|/ _ \\| '_ \\ / _ \\/ __|/ _ \\\x1b[0m\n");
    printf("  \x1b[1;36m |  _ <  __/ |  \\__ \\ (_) | | | | (_) \\__ \\  __/\x1b[0m\n");
    printf("  \x1b[1;36m |_| \\_\\___|_|  |___/\\___/|_| |_|\\___/|___/\\___|\x1b[0m\n");
    printf("  \x1b[1;36m                                                \x1b[0m\n");
    printf("\n");
    printf("  \x1b[1;33m%s\x1b[0m\n", lang_get(os, "banner.title"));
    printf("  \x1b[1;37m%s\x1b[0m\n", lang_get(os, "welcome"));
    Engine *e = engine_get_active(os);
    if (e) printf("  \x1b[1;32mEngine: %s %s  |  Language: %s  |  Theme: %s\x1b[0m\n",
                  e->name, e->version, lang_name(os), theme_name(os->config.theme));
    printf("  \x1b[1;32mType 'help' for a list of commands.\x1b[0m\n");
    printf("\n");
}

void shell_print_prompt(BrowserOS *os) {
    printf("\x1b[1;33m%s@%s\x1b[0m:\x1b[1;34m~\x1b[0m%s ",
           os->os.username, os->os.hostname, lang_get(os, "prompt"));
    fflush(stdout);
}

/* ============================================================
 * INITIALIZATION
 * ============================================================ */
void browseos_init(BrowserOS *os, int argc, char **argv) {
    memset(os, 0, sizeof(BrowserOS));
    os->argc = argc;
    os->argv = argv;

    /* OS State */
    util_strlcpy(os->os.username, "user", 32);
    util_strlcpy(os->os.hostname, "browseos", 32);
    os->os.boot_time = time(NULL);
    os->os.running = 1;
    os->os.logged_in = 1;
    os->os.session_id = util_random(1000, 9999);
    os->os.command_count = 0;
    util_strlcpy(os->os.last_error, "", MAX_LINE);
    os->os.error_code = 0;

    os->active_tab = -1;
    os->num_tabs = 0;
    os->history_count = 0;
    os->bookmark_count = 0;
    os->download_count = 0;
    os->process_count = 0;
    os->extension_count = 0;
    os->cache_count = 0;
    os->cookie_count = 0;
    os->certificate_count = 0;
    os->cmd_history_count = 0;
    os->render_width = 78;
    os->render_height = 24;
    os->total_memory_kb = 8388608;  /* 8 GB */
    os->used_memory_kb = 0;
    os->cpu_load = 0.0;
    os->proxy.enabled = 0;

    util_strlcpy(os->current_dir, "~", MAX_PATH);

    /* Initialize subsystems */
    config_init(os);
    theme_init(os);
    engine_init_all(os);
    lang_init_all(os);

    /* Load default bookmarks */
    bookmark_folder_create(os, "Web Development", "Web development resources");
    bookmark_folder_create(os, "Search Engines", "Search engines");
    bookmark_add(os, "https://github.com", "GitHub", "Web Development");
    bookmark_add(os, "https://stackoverflow.com", "Stack Overflow", "Web Development");
    bookmark_add(os, "https://google.com", "Google", "Search Engines");
    bookmark_add(os, "https://wikipedia.org", "Wikipedia", "Other");

    /* Create initial processes */
    proc_create(os, "browser", PROC_BROWSER, 5);
    proc_create(os, "renderer", PROC_RENDERER, 5);
    proc_create(os, "network", PROC_NETWORK, 4);
    proc_create(os, "storage", PROC_STORAGE, 3);
    proc_create(os, "gpu-process", PROC_GPU, 3);

    /* Install default extensions */
    ext_install(os, "Ad Blocker", "1.2.0", "BrowseOS Team", "Blocks unwanted advertisements");
    ext_install(os, "Dark Reader", "2.1.0", "BrowseOS Team", "Dark mode for all websites");
    ext_install(os, "Password Manager", "1.0.0", "BrowseOS Team", "Secure password storage");
    ext_install(os, "Download Manager", "1.5.0", "BrowseOS Team", "Advanced download management");
}

void browseos_shutdown(BrowserOS *os) {
    int i;
    for (i = 0; i < os->num_tabs; i++) {
        if (os->tabs[i].page) {
            renderer_free_page(os->tabs[i].page);
            os->tabs[i].page = NULL;
        }
    }
    os->num_tabs = 0;
    os->os.running = 0;
}

static int util_max(int a, int b) { return a > b ? a : b; }
static int util_min(int a, int b) { return a < b ? a : b; }

void browseos_update(BrowserOS *os) {
    time_t now = time(NULL);
    os->os.uptime_seconds = (int)difftime(now, os->os.boot_time);
    os->cpu_load = (double)util_random(5, 35) + (double)util_random(0, 99) / 100.0;
    os->used_memory_kb = os->total_memory_kb - (unsigned long long)util_random(1000000, 2000000);
    os->net_stats.active_connections = util_max(0, os->net_stats.active_connections);
}

void browseos_print_error(BrowserOS *os, const char *msg) {
    printf("\x1b[1;31m[ERROR]\x1b[0m %s\n", msg);
    util_strlcpy(os->os.last_error, msg, MAX_LINE);
}

void browseos_print_info(BrowserOS *os, const char *msg) {
    (void)os;
    printf("\x1b[1;36m[INFO]\x1b[0m %s\n", msg);
}

void browseos_print_warning(BrowserOS *os, const char *msg) {
    (void)os;
    printf("\x1b[1;33m[WARN]\x1b[0m %s\n", msg);
}

void browseos_print_success(BrowserOS *os, const char *msg) {
    (void)os;
    printf("\x1b[1;32m[OK]\x1b[0m %s\n", msg);
}

/* ============================================================
 * NETWORK HELPERS (simple helpers)
 * ============================================================ */
int network_build_request(BrowserOS *os, HttpResponse *resp) {
    if (!resp) return 0;
    Engine *e = engine_get_active(os);
    const char *ua = e ? e->user_agent : "BrowseOS/3.0";

    /* Set request headers */
    resp->num_request_headers = 0;
    snprintf(resp->request_headers[resp->num_request_headers].name, 64, "User-Agent");
    snprintf(resp->request_headers[resp->num_request_headers].value, MAX_PATH, "%s", ua);
    resp->num_request_headers++;

    snprintf(resp->request_headers[resp->num_request_headers].name, 64, "Accept");
    snprintf(resp->request_headers[resp->num_request_headers].value, MAX_PATH, "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    resp->num_request_headers++;

    snprintf(resp->request_headers[resp->num_request_headers].name, 64, "Accept-Language");
    snprintf(resp->request_headers[resp->num_request_headers].value, MAX_PATH, "%s", lang_code(os->active_language));
    resp->num_request_headers++;

    if (os->config.enable_do_not_track) {
        snprintf(resp->request_headers[resp->num_request_headers].name, 64, "DNT");
        snprintf(resp->request_headers[resp->num_request_headers].value, MAX_PATH, "1");
        resp->num_request_headers++;
    }

    if (os->config.enable_privacy_mode) {
        snprintf(resp->request_headers[resp->num_request_headers].name, 64, "Sec-GPC");
        snprintf(resp->request_headers[resp->num_request_headers].value, MAX_PATH, "1");
        resp->num_request_headers++;
    }

    return 1;
}

int network_send_request(const HttpMethod method, const char *url, HttpHeader *headers, int num_headers, const char *body, HttpResponse *resp) {
    if (!resp) return 0;
    memset(resp, 0, sizeof(HttpResponse));
    resp->method = method;
    (void)method; (void)headers; (void)num_headers; (void)body;
    return 0;
}

int network_handle_response(HttpResponse *resp) {
    if (!resp) return 0;
    /* Simulate response handling */
    return (resp->status >= HTTP_200 && resp->status < HTTP_400);
}

int network_is_valid_domain(const char *domain) {
    if (!domain || !*domain) return 0;
    int len = strlen(domain);
    if (len < 3 || len > 253) return 0;
    if (domain[0] == '.' || domain[len - 1] == '.') return 0;
    return util_contains(domain, ".");
}

int network_detect_encoding(const char *content, char *encoding, int max_len) {
    if (!content || !encoding) return 0;
    const char *charset = strstr(content, "charset=");
    if (charset) {
        charset += 8;
        int i = 0;
        while (charset[i] && charset[i] != '"' && charset[i] != '>' && charset[i] != ' ' && i < max_len - 1) {
            encoding[i] = charset[i];
            i++;
        }
        encoding[i] = '\0';
        return 1;
    }
    util_strlcpy(encoding, "UTF-8", max_len);
    return 0;
}

int network_init_ssl(void *ctx, const char *host, int port) {
    /* Simulated SSL - always succeeds */
    return 1;
}

/* ============================================================
 * ENTRY POINT
 * ============================================================ */
static BrowserOS g_os;

void shell_sigint_handler(int sig) {
    printf("\n");
    shell_print_prompt(&g_os);
    fflush(stdout);
}

void shell_run(BrowserOS *os) {
    char input[MAX_INPUT];
    os->os.running = 1;

    shell_print_banner(os);

    /* Create an initial tab */
    Tab *t = tab_create(os, os->config.home_page, os->config.default_engine);
    if (t) {
        printf("  Opened start page in tab #%d\n", t->id);
    }

    while (os->os.running) {
        browseos_update(os);
        shell_print_prompt(os);

        if (!fgets(input, sizeof(input), stdin)) break;

        input[strcspn(input, "\n")] = '\0';

        if (input[0] == '\0') continue;

        shell_handle_command(os, input);
    }

    browseos_shutdown(os);
}

int main(int argc, char **argv) {
    signal(SIGINT, shell_sigint_handler);
    signal(SIGTERM, shell_sigint_handler);

    browseos_init(&g_os, argc, argv);

    /* Check for command-line URL argument */
    if (argc > 1) {
        char url[MAX_URL];
        network_normalize_url(argv[1], url, MAX_URL);
        printf("  Opening: %s\n", url);
        Tab *t = tab_create(&g_os, url, g_os.config.default_engine);
        if (t && t->page) {
            char out[MAX_RENDER_BUF];
            renderer_format_output(t->page, out, MAX_RENDER_BUF, g_os.render_width, &g_os);
            printf("%s\n", out);
        }
    }

    shell_run(&g_os);

    printf("  Goodbye!\n");
    return 0;
}
