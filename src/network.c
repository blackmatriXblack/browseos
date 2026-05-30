#include "../include/browseos.h"

static const char* protocol_names[] = {
    "http://", "https://", "ftp://", "file://",
    "data:", "javascript:", "about:",
    "chrome://", "edge://", "blob:",
    "magnet:", "mailto:", "telnet:",
    "ssh:", "ws://", "wss://", "unknown"
};

static const char* status_text[] = {
    "100 Continue", "101 Switching Protocols",
    "200 OK", "201 Created", "204 No Content",
    "301 Moved Permanently", "302 Found", "303 See Other",
    "304 Not Modified", "307 Temporary Redirect", "308 Permanent Redirect",
    "400 Bad Request", "401 Unauthorized", "403 Forbidden",
    "404 Not Found", "405 Method Not Allowed", "408 Request Timeout",
    "500 Internal Server Error", "502 Bad Gateway",
    "503 Service Unavailable", "504 Gateway Timeout"
};

static HttpStatus status_map[] = {
    HTTP_100, HTTP_101, HTTP_200, HTTP_201, HTTP_204,
    HTTP_301, HTTP_302, HTTP_303, HTTP_304, HTTP_307, HTTP_308,
    HTTP_400, HTTP_401, HTTP_403, HTTP_404, HTTP_405, HTTP_408,
    HTTP_500, HTTP_502, HTTP_503, HTTP_504
};

static const char* sample_sites[][3] = {
    {"example.com", "Example Domain", "This domain is for use in illustrative examples in documents."},
    {"google.com", "Google", "Search the world's information, including webpages, images, videos and more."},
    {"github.com", "GitHub", "Where the world builds software. Millions of developers and companies build, ship, and maintain their software on GitHub."},
    {"wikipedia.org", "Wikipedia", "The Free Encyclopedia that anyone can edit."},
    {"stackoverflow.com", "Stack Overflow", "The largest, most trusted online community for developers to learn, share their programming knowledge."},
    {"microsoft.com", "Microsoft", "Empowering every person and every organization on the planet to achieve more."},
    {"apple.com", "Apple", "Apple designs, manufactures and markets smartphones, personal computers, tablets, wearables and accessories."},
    {"mozilla.org", "Mozilla", "The Mozilla project is a global community of people who believe openness, innovation, and opportunity are key to a healthy internet."},
    {"youtube.com", "YouTube", "Enjoy the videos and music you love, upload original content, and share it all with friends, family, and the world."},
    {"reddit.com", "Reddit", "Dive into anything. Reddit is a network of communities based on people's interests."},
    {"amazon.com", "Amazon", "Online shopping from the earth's biggest selection of books, magazines, music, DVDs, videos, electronics, and more."},
    {"facebook.com", "Facebook", "Connect with friends, share what you're up to, or see what's new from others all over the world."},
    {"twitter.com", "Twitter / X", "What's happening in the world right now."},
    {"linkedin.com", "LinkedIn", "World's largest professional network on the internet."},
    {"netflix.com", "Netflix", "Unlimited movies, TV shows, and more. Watch anywhere. Cancel anytime."},
    {"baidu.com", "Baidu \u767e\u5ea6",
     "\u5168\u7403\u6700\u5927\u7684\u4e2d\u6587\u641c\u7d22\u5f15\u64ce\u3001\u767e\u79d1\u3001\u65b0\u95fb\u3001\u8d34\u5427\u7b49\u591a\u79cd\u670d\u52a1"},
    {"bing.com", "Bing", "Microsoft Bing helps you find information and discover answers with intelligent search."},
    {"duckduckgo.com", "DuckDuckGo", "The search engine that doesn't track you. Privacy, simplified."},
    {"yahoo.com", "Yahoo", "News, email and search are just the beginning. Discover more every day."},
    {"ecosia.org", "Ecosia", "The search engine that plants trees. Ecosia uses the ad revenue from your searches to plant trees where they are needed most."}
};

static int sample_site_count = sizeof(sample_sites) / sizeof(sample_sites[0]);

int network_is_valid_url(const char *url) {
    if (!url || !*url) return 0;

    if (strstr(url, "://") || util_starts_with(url, "about:") ||
        util_starts_with(url, "data:") || util_starts_with(url, "javascript:") ||
        util_starts_with(url, "blob:") || util_starts_with(url, "magnet:"))
        return 1;

    if (util_contains(url, ".") || util_is_ip_address(url))
        return 1;

    return 0;
}

int network_normalize_url(const char *url, char *out, int max_len) {
    if (!url || !*url) return 0;

    if (strstr(url, "://") || util_starts_with(url, "about:") ||
        util_starts_with(url, "data:") || util_starts_with(url, "javascript:"))
    {
        util_strlcpy(out, url, max_len);
        return 1;
    }

    if (util_starts_with(url, "//")) {
        snprintf(out, max_len, "https:%s", url);
        return 1;
    }

    snprintf(out, max_len, "https://%s", url);
    return 1;
}

Protocol network_detect_protocol(const char *url) {
    int i;
    for (i = 0; i < PROTOCOL_UNKNOWN; i++) {
        if (util_starts_with(url, protocol_names[i]))
            return (Protocol)i;
    }
    if (util_starts_with(url, "//")) return PROTOCOL_HTTPS;
    return PROTOCOL_UNKNOWN;
}

int network_parse_url(const char *url, char *host, char *path, int *port, Protocol *proto) {
    char temp[MAX_URL];
    util_strlcpy(temp, url, MAX_URL);

    *proto = network_detect_protocol(url);
    if (*proto == PROTOCOL_UNKNOWN) return 0;

    char *p = temp;
    char *proto_end = strstr(p, "://");
    if (proto_end) {
        p = proto_end + 3;
    } else {
        /* Handle protocol-less URLs like javascript: or about: */
        if (util_starts_with(temp, "about:") || util_starts_with(temp, "data:") ||
            util_starts_with(temp, "javascript:") || util_starts_with(temp, "blob:") ||
            util_starts_with(temp, "magnet:") || util_starts_with(temp, "mailto:"))
        {
            if (host) util_strlcpy(host, "local", MAX_DOMAIN);
            if (path) util_strlcpy(path, url, MAX_URL);
            if (port) *port = 0;
            return 1;
        }
    }

    char *path_start = strchr(p, '/');
    char *query_start = strchr(p, '?');
    char *frag_start = strchr(p, '#');
    char *port_colon = NULL;

    if (path_start && query_start && path_start > query_start) path_start = NULL;
    if (path_start && frag_start && path_start > frag_start) path_start = NULL;

    int host_len;
    if (path_start) {
        host_len = path_start - p;
    } else if (query_start) {
        host_len = query_start - p;
    } else if (frag_start) {
        host_len = frag_start - p;
    } else {
        host_len = strlen(p);
    }

    char host_buf[MAX_DOMAIN];
    int host_pos = 0;
    int i;
    for (i = 0; i < host_len && i < MAX_DOMAIN - 1; i++) {
        if (p[i] == ':') {
            port_colon = &p[i];
            break;
        }
        host_buf[host_pos++] = p[i];
    }
    host_buf[host_pos] = '\0';

    if (host) util_strlcpy(host, host_buf, MAX_DOMAIN);

    if (port) {
        if (port_colon) {
            *port = atoi(port_colon + 1);
        } else {
            *port = (*proto == PROTOCOL_HTTPS) ? 443 : (*proto == PROTOCOL_HTTP) ? 80 : 0;
        }
    }

    if (path) {
        if (path_start) {
            util_strlcpy(path, path_start, MAX_URL);
        } else {
            util_strlcpy(path, "/", MAX_URL);
        }
    }

    return 1;
}

int network_resolve_dns(const char *host, char *ip, int max_len) {
    if (!host || !*host) return 0;
    if (util_is_ip_address(host)) {
        util_strlcpy(ip, host, max_len);
        return 1;
    }
    /* Simulate DNS resolution */
    unsigned int hash = 0;
    const char *p = host;
    while (*p) hash = (hash * 31) + *p++;

    snprintf(ip, max_len, "%d.%d.%d.%d",
             (hash >> 24) & 0xFF,
             (hash >> 16) & 0xFF,
             (hash >> 8) & 0xFF,
             hash & 0xFF);
    return 1;
}

static int find_sample_site(const char *host) {
    int i;
    for (i = 0; i < sample_site_count; i++) {
        if (util_contains(host, sample_sites[i][0])) {
            return i;
        }
    }
    return -1;
}

static void resp_set_body(HttpResponse *resp, const char *content) {
    if (!resp || !content) return;
    int len = strlen(content) + 1;
    if (resp->body) free(resp->body);
    resp->body = (char*)malloc(len);
    if (resp->body) {
        memcpy(resp->body, content, len);
        resp->body_length = len - 1;
        resp->body_capacity = len;
    }
}

int network_fetch_url(BrowserOS *os, const char *url, HttpResponse *resp) {
    if (!resp) return 0;

    memset(resp, 0, sizeof(HttpResponse));
    resp->body = NULL;
    resp->body_capacity = 0;
    resp->body_length = 0;
    resp->method = HTTP_GET;
    resp->status = HTTP_200;

    char normalized[MAX_URL];
    if (!network_normalize_url(url, normalized, MAX_URL)) {
        resp->status = HTTP_400;
        resp_set_body(resp, "Invalid URL");
        return 0;
    }
    util_strlcpy(resp->url, normalized, MAX_URL);

    Protocol proto;
    char host[MAX_DOMAIN], path[MAX_URL];
    int port;

    if (!network_parse_url(normalized, host, path, &port, &proto)) {
        resp->status = HTTP_400;
        resp_set_body(resp, "Malformed URL");
        return 0;
    }

    resp->protocol = proto;
    util_strlcpy(resp->host, host, MAX_DOMAIN);
    util_strlcpy(resp->path, path, MAX_URL);
    resp->port = port;
    resp->secure = (proto == PROTOCOL_HTTPS);
    resp->cached = 0;

    /* Check cache first */
    CacheEntry ce;
    if (os->config.enable_cache && cache_get(os, normalized, &ce)) {
        resp->cached = 1;
        resp->body_length = ce.size;
        os->net_stats.total_cache_hits++;
    } else {
        os->net_stats.total_cache_misses++;
    }

    os->net_stats.active_connections++;
    os->net_stats.total_requests++;

    char body_buf[MAX_CONTENT];

    /* Check if about: or javascript: protocol */
    if (proto == PROTOCOL_ABOUT) {
        if (strcmp(path, "about:blank") == 0 || strcmp(normalized, "about:blank") == 0) {
            resp_set_body(resp, "<html><head><title>about:blank</title></head><body></body></html>");
        } else {
            snprintf(body_buf, MAX_CONTENT,
                "<html><head><title>About BrowseOS</title></head>"
                "<body><h1>BrowseOS v%s</h1>"
                "<p>Golden Sakura - Build %d</p>"
                "<p>A Browser Operating System by GoldenSakuraZero</p>"
                "<p>Active Engine: %s</p>"
                "<p>Language: %s</p>"
                "<hr/><p>Type 'help' for available commands</p></body></html>",
                BROWSEOS_VERSION, BROWSEOS_BUILD,
                engine_get_active(os) ? engine_get_active(os)->name : "None",
                lang_name(os));
            resp_set_body(resp, body_buf);
        }
        os->net_stats.active_connections--;
        return 1;
    }

    if (proto == PROTOCOL_JAVASCRIPT) {
        resp_set_body(resp, "");
        os->net_stats.active_connections--;
        return 1;
    }

    /* DNS lookup */
    char ip[64];
    network_resolve_dns(host, ip, sizeof(ip));
    os->net_stats.total_dns_queries++;

    /* SSL handshake for HTTPS */
    if (resp->secure) {
        os->net_stats.total_ssl_handshakes++;
        network_check_certificate(os, host);
    }

    /* Set response headers */
    snprintf(resp->response_headers[0].name, sizeof(resp->response_headers[0].name), "Server");
    snprintf(resp->response_headers[0].value, sizeof(resp->response_headers[0].value), "BrowseOS/%s (%s)", BROWSEOS_VERSION, engine_get_active(os) ? engine_get_active(os)->name : "Unknown");
    resp->num_response_headers = 1;

    snprintf(resp->response_headers[resp->num_response_headers].name, sizeof(resp->response_headers[0].name), "Content-Type");
    snprintf(resp->response_headers[resp->num_response_headers].value, sizeof(resp->response_headers[0].value), "text/html; charset=utf-8");
    resp->num_response_headers++;

    snprintf(resp->response_headers[resp->num_response_headers].name, sizeof(resp->response_headers[0].name), "X-BrowseOS-Engine");
    snprintf(resp->response_headers[resp->num_response_headers].value, sizeof(resp->response_headers[0].value), "%s %s", engine_get_active(os) ? engine_get_active(os)->name : "Unknown", engine_get_active(os) ? engine_get_active(os)->version : "");
    resp->num_response_headers++;

    resp->load_time = (double)(util_random(10, 500)) / 1000.0;

    /* Check for known sites */
    int site_idx = find_sample_site(host);
    if (site_idx >= 0) {
        generate_sample_html(sample_sites[site_idx][1], body_buf, MAX_CONTENT, os);
        resp_set_body(resp, body_buf);
        resp->status = HTTP_200;
    } else if (util_contains(host, "search") || util_contains(path, "/search")) {
        snprintf(body_buf, MAX_CONTENT,
            "<html><head><title>Search Results</title></head>"
            "<body><h1>Search Results</h1><p>Your search produced no results on this simulated network.</p>"
            "<p>BrowseOS is a CLI-based browser operating system simulator.</p>"
            "<p>Try visiting: google.com, github.com, wikipedia.org, or about:version</p></body></html>");
        resp_set_body(resp, body_buf);
        resp->status = HTTP_200;
    } else {
        /* Generate a dynamic page for unknown sites */
        snprintf(body_buf, MAX_CONTENT,
            "<html><head><title>%s - BrowseOS</title></head>"
            "<body><h1>%s</h1>"
            "<p><strong>URL:</strong> %s</p>"
            "<p><strong>Host:</strong> %s</p>"
            "<p><strong>IP Address:</strong> %s</p>"
            "<p><strong>Protocol:</strong> %s</p>"
            "<p><strong>Port:</strong> %d</p>"
            "<p><strong>Path:</strong> %s</p>"
            "<p><strong>Engine:</strong> %s %s</p>"
            "<p><strong>User-Agent:</strong> %s</p>"
            "<p><strong>Load Time:</strong> %.3fs</p>"
            "<p><strong>Secure:</strong> %s</p>"
            "<hr/>"
            "<p>Welcome to %s! This page was dynamically generated by BrowseOS.</p>"
            "<p>The site you requested is not in the built-in directory, but a simulated page has been created.</p>"
            "<ul>"
            "<li><a href='https://github.com'>Visit GitHub</a></li>"
            "<li><a href='https://google.com'>Visit Google</a></li>"
            "<li><a href='https://wikipedia.org'>Visit Wikipedia</a></li>"
            "<li><a href='https://stackoverflow.com'>Visit Stack Overflow</a></li>"
            "</ul>"
            "<p>Try 'open google.com' or 'help' for more commands.</p>"
            "</body></html>",
            host, host,
            normalized, host, ip,
            protocol_names[proto], port, path,
            engine_get_active(os) ? engine_get_active(os)->name : "N/A",
            engine_get_active(os) ? engine_get_active(os)->version : "",
            engine_get_user_agent(os),
            resp->load_time,
            resp->secure ? "Yes (TLS 1.3)" : "No",
            host);
        resp_set_body(resp, body_buf);
        resp->status = HTTP_200;
    }

    os->net_stats.total_transfer_kb += resp->body_length / 1024;
    os->net_stats.avg_load_time = (os->net_stats.avg_load_time * (os->net_stats.total_requests - 1) + resp->load_time) / os->net_stats.total_requests;
    os->net_stats.active_connections--;

    /* Add to cache */
    if (os->config.enable_cache) {
        cache_put(os, normalized, "text/html", resp->body_length, "");
    }

    return 1;
}

int network_check_certificate(BrowserOS *os, const char *domain) {
    if (os->certificate_count >= MAX_CERTIFICATES) return 0;

    Certificate *cert = &os->certificates[os->certificate_count];
    util_strlcpy(cert->domain, domain, MAX_DOMAIN);
    util_strlcpy(cert->issuer, "BrowseOS Root CA", 128);
    util_strlcpy(cert->subject, domain, 128);
    snprintf(cert->serial, 64, "%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X",
             util_random(0, 255), util_random(0, 255), util_random(0, 255), util_random(0, 255),
             util_random(0, 255), util_random(0, 255), util_random(0, 255), util_random(0, 255));
    cert->valid_from = time(NULL) - 86400 * 365;
    cert->valid_to = time(NULL) + 86400 * 365;
    snprintf(cert->fingerprint, 128,
             "SHA256: %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
             util_random(0, 255), util_random(0, 255), util_random(0, 255), util_random(0, 255),
             util_random(0, 255), util_random(0, 255), util_random(0, 255), util_random(0, 255),
             util_random(0, 255), util_random(0, 255), util_random(0, 255), util_random(0, 255),
             util_random(0, 255), util_random(0, 255), util_random(0, 255), util_random(0, 255));
    cert->trusted = 1;
    os->certificate_count++;
    return 1;
}

void network_proxy_set(BrowserOS *os, const char *host, int port) {
    util_strlcpy(os->proxy.host, host, MAX_DOMAIN);
    os->proxy.port = port;
    os->proxy.enabled = 1;
}

void network_proxy_disable(BrowserOS *os) {
    os->proxy.enabled = 0;
    os->proxy.host[0] = '\0';
    os->proxy.port = 0;
}

/* Cache Functions */
int cache_get(BrowserOS *os, const char *url, CacheEntry *entry) {
    int i;
    for (i = 0; i < os->cache_count; i++) {
        if (strcmp(os->cache[i].url, url) == 0) {
            time_t now = time(NULL);
            if (difftime(now, os->cache[i].timestamp) < os->cache[i].expiry) {
                if (entry) *entry = os->cache[i];
                return 1;
            }
            /* Expired - remove it */
            cache_remove(os, i);
            return 0;
        }
    }
    return 0;
}

int cache_put(BrowserOS *os, const char *url, const char *content_type, int size, const char *etag) {
    if (os->cache_count >= MAX_CACHE) return 0;
    int idx = os->cache_count;
    util_strlcpy(os->cache[idx].url, url, MAX_URL);
    util_strlcpy(os->cache[idx].content_type, content_type, 64);
    os->cache[idx].size = size;
    os->cache[idx].timestamp = time(NULL);
    os->cache[idx].expiry = 300; /* 5 minutes */
    util_strlcpy(os->cache[idx].etag, etag, 128);
    util_strlcpy(os->cache[idx].last_modified, "Thu, 01 Jan 2026 00:00:00 GMT", 64);
    os->cache[idx].fresh = 1;
    os->cache_count++;
    return 1;
}

int cache_clear(BrowserOS *os) {
    os->cache_count = 0;
    return 1;
}

int cache_remove(BrowserOS *os, int index) {
    if (index < 0 || index >= os->cache_count) return 0;
    if (index < os->cache_count - 1) {
        memmove(&os->cache[index], &os->cache[index + 1], sizeof(CacheEntry) * (os->cache_count - index - 1));
    }
    os->cache_count--;
    return 1;
}

void cache_print(BrowserOS *os) {
    int i;
    printf("\n  Cache Entries: %d / %d\n", os->cache_count, MAX_CACHE);
    printf("  %s\n", "------------------------------------------------------------");
    for (i = 0; i < os->cache_count; i++) {
        char time_str[64];
        util_format_time(os->cache[i].timestamp, time_str, 64);
        printf("  %-3d | %-40s | %-6dKB | %s%s\n",
               i + 1, os->cache[i].url, os->cache[i].size / 1024,
               time_str, os->cache[i].fresh ? "" : " (stale)");
    }
    printf("\n");
}

/* Cookie Functions */
int cookie_get(BrowserOS *os, const char *domain, const char *path, const char *name, char *value, int max_len) {
    int i;
    for (i = 0; i < os->cookie_count; i++) {
        if (util_contains(domain, os->cookies[i].domain) &&
            strcmp(os->cookies[i].name, name) == 0)
        {
            time_t now = time(NULL);
            if (!os->cookies[i].session && difftime(now, os->cookies[i].expires) > 0) {
                /* Expired */
                return 0;
            }
            util_strlcpy(value, os->cookies[i].value, max_len);
            return 1;
        }
    }
    return 0;
}

int cookie_set(BrowserOS *os, const char *domain, const char *path, const char *name, const char *value, int secure, int http_only, time_t expires) {
    if (os->cookie_count >= MAX_COOKIES) return 0;
    int idx = os->cookie_count;
    util_strlcpy(os->cookies[idx].domain, domain, MAX_DOMAIN);
    util_strlcpy(os->cookies[idx].path, path, MAX_URL);
    util_strlcpy(os->cookies[idx].name, name, 128);
    util_strlcpy(os->cookies[idx].value, value, MAX_PATH);
    os->cookies[idx].secure = secure;
    os->cookies[idx].http_only = http_only;
    os->cookies[idx].expires = expires;
    os->cookies[idx].session = (expires == 0);
    os->cookie_count++;
    return 1;
}

int cookie_clear(BrowserOS *os) {
    os->cookie_count = 0;
    return 1;
}

int cookie_clear_domain(BrowserOS *os, const char *domain) {
    int i, removed = 0;
    for (i = os->cookie_count - 1; i >= 0; i--) {
        if (util_contains(os->cookies[i].domain, domain)) {
            if (i < os->cookie_count - 1) {
                memmove(&os->cookies[i], &os->cookies[i + 1], sizeof(Cookie) * (os->cookie_count - i - 1));
            }
            os->cookie_count--;
            removed++;
        }
    }
    return removed;
}

void cookie_print(BrowserOS *os) {
    int i;
    printf("\n  Cookies: %d\n", os->cookie_count);
    printf("  %s\n", "----------------------------------------------------------------------");
    for (i = 0; i < os->cookie_count; i++) {
        printf("  %-3d | %-20s | %-15s | %-20s | %s%s\n",
               i + 1,
               os->cookies[i].domain,
               os->cookies[i].name,
               os->cookies[i].value,
               os->cookies[i].session ? "Session" : "Persistent",
               os->cookies[i].secure ? " | Secure" : "");
    }
    printf("\n");
}
