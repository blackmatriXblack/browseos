#include "../include/browseos.h"

static const char* engine_type_names[] = {
    "WebKit", "Gecko", "Blink", "Trident", "EdgeHTML",
    "Servo", "Goanna", "NetSurf", "LibWeb", "Flow", "Custom"
};

const char* engine_type_name(EngineType t) {
    if (t >= ENGINE_WEBKIT && t <= ENGINE_CUSTOM)
        return engine_type_names[t];
    return "Unknown";
}

static Engine default_engines[] = {
    {
        ENGINE_WEBKIT, "WebKit2", "612.4.9",
        "Apple Inc.",
        "WebKit is the browser engine used by Safari, Mail, App Store, and many other macOS, iOS, and Linux applications. "
        "It features a highly optimized rendering pipeline with Nitro JavaScript engine, support for modern web standards, "
        "and excellent performance on Apple platforms. WebKit emphasizes standards compliance, security, and energy efficiency.",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 14_5) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.5 Safari/605.1.15",
        92, 25, 95, 94, 93, 96,
        RENDER_STANDARD, JS_FULL, CSS_FULL,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    },
    {
        ENGINE_GECKO, "Gecko", "125.0",
        "Mozilla Foundation",
        "Gecko is the browser engine developed by Mozilla for Firefox. It is designed for standards compliance, "
        "security, and extensibility. Gecko features the SpiderMonkey JavaScript engine, WebRender for GPU-accelerated "
        "rendering, and extensive support for open web standards. It powers Firefox, Thunderbird, and many other applications.",
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:125.0) Gecko/20100101 Firefox/125.0",
        88, 30, 93, 92, 91, 94,
        RENDER_STANDARD, JS_FULL, CSS_FULL,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    },
    {
        ENGINE_BLINK, "Blink", "125.0.6422.142",
        "Google LLC / The Chromium Project",
        "Blink is a browser engine based on WebKit, developed by Google as part of the Chromium project. "
        "It powers Google Chrome, Microsoft Edge, Opera, Brave, Samsung Internet, and many other browsers. "
        "Blink features the V8 JavaScript engine, high-performance rendering, and broad platform support. "
        "It is the most widely used browser engine in the world.",
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/125.0.0.0 Safari/537.36",
        96, 28, 91, 97, 95, 98,
        RENDER_STANDARD, JS_FULL, CSS_FULL,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    },
    {
        ENGINE_TRIDENT, "Trident", "7.0",
        "Microsoft Corporation",
        "Trident (also known as MSHTML) is the legacy browser engine used by Internet Explorer. "
        "It was first introduced in 1997 with IE4 and has gone through several versions. Trident/7.0 was the "
        "final version included in IE11. It has been superseded by EdgeHTML and Blink. Trident is known for "
        "its proprietary extensions and quirks mode rendering.",
        "Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko",
        45, 55, 40, 35, 30, 25,
        RENDER_QUIRKS, JS_INTERMEDIATE, CSS_INTERMEDIATE,
        0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    },
    {
        ENGINE_EDGEHTML, "EdgeHTML", "18.18362",
        "Microsoft Corporation",
        "EdgeHTML is the proprietary browser engine developed by Microsoft for the original Edge browser (Spartan). "
        "It was designed as a modern replacement for Trident with improved standards support, performance, "
        "and security. EdgeHTML was used from Windows 10 build 15002 until Microsoft Edge switched to Chromium "
        "in 2020. It featured the Chakra JavaScript engine.",
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36 Edge/18.18362",
        75, 35, 80, 78, 76, 80,
        RENDER_ALMOST_STANDARD, JS_FULL, CSS_FULL,
        1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0
    },
    {
        ENGINE_SERVO, "Servo", "0.0.1",
        "Mozilla Research / Linux Foundation",
        "Servo is a modern, high-performance browser engine designed for both traditional and mobile devices. "
        "Written in Rust, it leverages memory safety and parallelism to achieve excellent performance. "
        "Servo features WebRender for GPU-based rendering, Stylo for parallel CSS processing, "
        "and is designed for multi-core architectures from the ground up.",
        "Mozilla/5.0 (Mobile; rv:65.0) Servo/1.0 Firefox/65.0",
        95, 15, 88, 85, 82, 70,
        RENDER_STANDARD, JS_INTERMEDIATE, CSS_INTERMEDIATE,
        1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0
    },
    {
        ENGINE_GOANNA, "Goanna", "5.0",
        "Moonchild Productions",
        "Goanna is an open-source browser engine that forked from Mozilla's Gecko in 2016. "
        "It is used primarily by Pale Moon and Basilisk browsers. Goanna retains much of the Firefox "
        "legacy compatibility while providing a unique rendering path. It emphasizes customization, "
        "user choice, and support for legacy web technologies.",
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:102.0) Goanna/20230101 PaleMoon/33.0.0",
        65, 40, 75, 60, 55, 50,
        RENDER_ALMOST_STANDARD, JS_INTERMEDIATE, CSS_INTERMEDIATE,
        0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0
    },
    {
        ENGINE_NETSURF, "NetSurf", "3.10",
        "The NetSurf Developers",
        "NetSurf is a lightweight, open-source web browser designed for resource-constrained environments. "
        "Its engine supports HTML 5 and CSS 2.1 with a focus on speed and low memory usage. "
        "NetSurf does not support JavaScript but provides fast rendering of static web content. "
        "It is ideal for embedded systems, legacy hardware, and minimal computing environments.",
        "NetSurf/3.10 (Windows; Win64)",
        70, 8, 55, 30, 45, 5,
        RENDER_LIMITED, JS_NONE, CSS_INTERMEDIATE,
        0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }
};

void engine_init_all(BrowserOS *os) {
    int num = sizeof(default_engines) / sizeof(default_engines[0]);
    int i;
    for (i = 0; i < num && i < MAX_ENGINES; i++) {
        os->engines[i] = default_engines[i];
        os->engines[i].active = (os->engines[i].type == ENGINE_BLINK);
    }
    os->num_engines = (num < MAX_ENGINES) ? num : MAX_ENGINES;

    for (i = 0; i < os->num_engines; i++) {
        if (os->engines[i].type == os->config.default_engine) {
            os->active_engine = i;
            os->engines[i].active = 1;
            break;
        }
    }
}

Engine* engine_get(BrowserOS *os, EngineType type) {
    int i;
    for (i = 0; i < os->num_engines; i++) {
        if (os->engines[i].type == type)
            return &os->engines[i];
    }
    return NULL;
}

Engine* engine_get_active(BrowserOS *os) {
    if (os->active_engine >= 0 && os->active_engine < os->num_engines)
        return &os->engines[os->active_engine];
    return NULL;
}

int engine_switch(BrowserOS *os, EngineType type) {
    int i;
    for (i = 0; i < os->num_engines; i++) {
        os->engines[i].active = 0;
    }
    for (i = 0; i < os->num_engines; i++) {
        if (os->engines[i].type == type) {
            os->active_engine = i;
            os->engines[i].active = 1;
            return 1;
        }
    }
    return 0;
}

int engine_set_active(BrowserOS *os, int index) {
    if (index < 0 || index >= os->num_engines) return 0;
    int i;
    for (i = 0; i < os->num_engines; i++)
        os->engines[i].active = 0;
    os->active_engine = index;
    os->engines[index].active = 1;
    return 1;
}

char* engine_get_user_agent(BrowserOS *os) {
    Engine *e = engine_get_active(os);
    if (e) return e->user_agent;
    return "BrowserOS/3.0 (Compatible; BrowseOS Golden Sakura)";
}

void engine_print_info(const Engine *e) {
    if (!e) return;
    printf("  Engine      : %s %s\n", e->name, e->version);
    printf("  Vendor      : %s\n", e->vendor);
    printf("  Description : %s\n", e->description);
    printf("  User-Agent  : %s\n", e->user_agent);
    printf("  Performance : Speed=%d/100  Memory=%d/100  Compliance=%d/100\n",
           e->speed_rating, e->memory_rating, e->standard_compliance);
    printf("  Benchmarks  : HTML5=%d/100  CSS3=%d/100  JS=%d/100\n",
           e->html5_score, e->css3_score, e->js_score);
    printf("  Rendering   : %s\n", e->render_mode == RENDER_STANDARD ? "Standard" :
           e->render_mode == RENDER_QUIRKS ? "Quirks" :
           e->render_mode == RENDER_ALMOST_STANDARD ? "Almost Standards" : "Limited");
    printf("  JS Support  : %s\n", e->js_support == JS_NONE ? "None" :
           e->js_support == JS_BASIC ? "Basic" :
           e->js_support == JS_INTERMEDIATE ? "Intermediate" : "Full");
    printf("  CSS Support : %s\n", e->css_support == CSS_NONE ? "None" :
           e->css_support == CSS_BASIC ? "Basic" :
           e->css_support == CSS_INTERMEDIATE ? "Intermediate" : "Full");
    printf("  Features    :");
    if (e->supports_webgl) printf(" WebGL");
    if (e->supports_wasm) printf(" WASM");
    if (e->supports_webrtc) printf(" WebRTC");
    if (e->supports_webgpu) printf(" WebGPU");
    if (e->supports_canvas) printf(" Canvas");
    if (e->supports_svg) printf(" SVG");
    if (e->supports_css_grid) printf(" CSS-Grid");
    if (e->supports_css_flexbox) printf(" Flexbox");
    if (e->supports_css_animations) printf(" CSS-Animations");
    if (e->supports_web_sockets) printf(" WebSockets");
    if (e->supports_fetch_api) printf(" FetchAPI");
    if (e->supports_http2) printf(" HTTP/2");
    if (e->supports_http3) printf(" HTTP/3");
    if (e->supports_tls13) printf(" TLS1.3");
    if (e->supports_service_workers) printf(" ServiceWorkers");
    if (e->supports_web_components) printf(" WebComponents");
    if (e->supports_indexed_db) printf(" IndexedDB");
    if (e->supports_local_storage) printf(" LocalStorage");
    printf("\n");
}

void engine_print_all(BrowserOS *os) {
    int i;
    printf("\n");
    printf("  %-3s %-16s %-10s %-30s %-8s %-8s %-8s %s\n",
           "#", "Engine", "Version", "Vendor", "Speed", "Memory", "Compliance", "Active");
    printf("  %s\n", "----------------------------------------------------------------------------------------------------------------------");
    for (i = 0; i < os->num_engines; i++) {
        Engine *e = &os->engines[i];
        printf("  %-3d %-16s %-10s %-30s %-8d %-8d %-8d %s\n",
               i + 1,
               e->name,
               e->version,
               e->vendor,
               e->speed_rating,
               e->memory_rating,
               e->standard_compliance,
               e->active ? "*ACTIVE*" : "");
    }
    printf("\n");
}

void engine_compare(BrowserOS *os) {
    int i;
    printf("\n  == ENGINE COMPARISON ==\n\n");
    printf("  %-16s", "Engine");
    for (i = 0; i < os->num_engines; i++)
        printf(" | %-12s", os->engines[i].name);
    printf("\n  %s\n", "-------------------" "---------------------" "---------------------" "---------------------" "---------------------");
    printf("  %-16s", "Speed");
    for (i = 0; i < os->num_engines; i++)
        printf(" | %-12d", os->engines[i].speed_rating);
    printf("\n  %-16s", "Memory Eff.");
    for (i = 0; i < os->num_engines; i++)
        printf(" | %-12d", 100 - os->engines[i].memory_rating);
    printf("\n  %-16s", "Compliance");
    for (i = 0; i < os->num_engines; i++)
        printf(" | %-12d", os->engines[i].standard_compliance);
    printf("\n  %-16s", "HTML5 Score");
    for (i = 0; i < os->num_engines; i++)
        printf(" | %-12d", os->engines[i].html5_score);
    printf("\n  %-16s", "CSS3 Score");
    for (i = 0; i < os->num_engines; i++)
        printf(" | %-12d", os->engines[i].css3_score);
    printf("\n  %-16s", "JS Score");
    for (i = 0; i < os->num_engines; i++)
        printf(" | %-12d", os->engines[i].js_score);
    printf("\n\n");
}

int engine_render_html(BrowserOS *os, const char *html, char *output, int max_out) {
    Engine *e = engine_get_active(os);
    if (!e) {
        util_strlcpy(output, "[ERROR] No active engine", max_out);
        return 0;
    }

    RenderPage *page = renderer_create_page();
    if (!page) {
        util_strlcpy(output, "[ERROR] Failed to create render page", max_out);
        return 0;
    }

    int ok = renderer_parse_html(html, page, os);
    if (!ok) {
        util_strlcpy(output, "[ERROR] Failed to parse HTML", max_out);
        renderer_free_page(page);
        return 0;
    }

    int out_len = renderer_format_output(page, output, max_out, os->render_width, os);
    renderer_free_page(page);
    return out_len;
}
