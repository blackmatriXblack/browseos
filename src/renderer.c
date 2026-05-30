#include "../include/browseos.h"

RenderPage* renderer_create_page(void) {
    RenderPage *page = (RenderPage*)calloc(1, sizeof(RenderPage));
    if (!page) return NULL;
    page->title[0] = '\0';
    page->description[0] = '\0';
    util_strlcpy(page->charset, "UTF-8", 32);
    page->width = 80;
    page->height = 24;
    page->loaded = 0;
    return page;
}

void renderer_free_page(RenderPage *page) {
    if (!page) return;
    if (page->document) {
        dom_free(page->document);
    }
    if (page->styles) {
        free(page->styles);
    }
    free(page);
}

DOMNode* dom_create_node(NodeType type, const char *tag) {
    DOMNode *node = (DOMNode*)calloc(1, sizeof(DOMNode));
    if (!node) return NULL;
    node->type = type;
    if (tag) util_strlcpy(node->tag_name, tag, 32);
    node->num_children = 0;
    node->capacity_children = 0;
    node->children = NULL;
    node->parent = NULL;
    node->rendered = 0;
    node->visible = 1;
    node->x = node->y = node->width = node->height = 0;
    return node;
}

int dom_add_child(DOMNode *parent, DOMNode *child) {
    if (!parent || !child) return 0;
    if (parent->num_children >= parent->capacity_children) {
        int new_cap = parent->capacity_children ? parent->capacity_children * 2 : 8;
        DOMNode **new_children = (DOMNode**)realloc(parent->children, new_cap * sizeof(DOMNode*));
        if (!new_children) return 0;
        parent->children = new_children;
        parent->capacity_children = new_cap;
    }
    parent->children[parent->num_children++] = child;
    child->parent = parent;
    return 1;
}

void dom_free(DOMNode *node) {
    if (!node) return;
    int i;
    for (i = 0; i < node->num_children; i++) {
        dom_free(node->children[i]);
    }
    if (node->children) free(node->children);
    free(node);
}

static int parse_tag(const char *html, int *pos, char *tag, int max_tag, int *is_closing, int *is_self_closing) {
    int len = strlen(html);
    int p = *pos;
    int ti = 0;

    *is_closing = 0;
    *is_self_closing = 0;

    if (p >= len || html[p] != '<') return 0;
    p++; /* skip < */

    if (p < len && html[p] == '/') {
        *is_closing = 1;
        p++;
    }

    while (p < len && html[p] && html[p] != '>' && html[p] != ' ' && html[p] != '\t' && html[p] != '\n') {
        if (ti < max_tag - 1) tag[ti++] = html[p];
        p++;
    }
    tag[ti] = '\0';

    /* Skip attributes */
    int in_attr = 0;
    while (p < len && html[p] != '>') {
        if (html[p] == '"') in_attr = !in_attr;
        if (!in_attr && html[p] == '/' && p + 1 < len && html[p + 1] == '>') {
            *is_self_closing = 1;
        }
        p++;
    }
    if (p < len && html[p] == '>') p++;

    *pos = p;
    return 1;
}

static int parse_attributes(const char *html, int *pos, DOMNode *node) {
    int p = *pos;
    int len = strlen(html);

    while (p < len && html[p] != '>' && html[p] != '/') {
        while (p < len && (html[p] == ' ' || html[p] == '\t' || html[p] == '\n')) p++;

        char name[64] = {0};
        int ni = 0;
        while (p < len && html[p] != '=' && html[p] != '>' && html[p] != ' ' && html[p] != '\t' && html[p] != '\n') {
            if (ni < 63) name[ni++] = html[p];
            p++;
        }
        name[ni] = '\0';

        if (name[0] == '\0') break;

        char value[MAX_PATH] = {0};
        if (p < len && html[p] == '=') {
            p++;
            if (p < len && (html[p] == '"' || html[p] == '\'')) {
                char quote = html[p];
                p++;
                int vi = 0;
                while (p < len && html[p] != quote) {
                    if (vi < MAX_PATH - 1) value[vi++] = html[p];
                    p++;
                }
                if (p < len) p++;
                value[vi] = '\0';
            } else {
                int vi = 0;
                while (p < len && html[p] != '>' && html[p] != ' ' && html[p] != '\t') {
                    if (vi < MAX_PATH - 1) value[vi++] = html[p];
                    p++;
                }
                value[vi] = '\0';
            }
        }

        if (node->num_attributes < MAX_PARAMS) {
            util_strlcpy(node->attr_names[node->num_attributes], name, 64);
            util_strlcpy(node->attributes[node->num_attributes], value, MAX_PATH);
            node->num_attributes++;
        }
    }

    *pos = p;
    return 1;
}

static int is_inline_element(const char *tag) {
    return (strcmp(tag, "a") == 0 || strcmp(tag, "span") == 0 || strcmp(tag, "strong") == 0 ||
            strcmp(tag, "em") == 0 || strcmp(tag, "b") == 0 || strcmp(tag, "i") == 0 ||
            strcmp(tag, "u") == 0 || strcmp(tag, "code") == 0 || strcmp(tag, "img") == 0 ||
            strcmp(tag, "br") == 0 || strcmp(tag, "input") == 0 || strcmp(tag, "label") == 0 ||
            strcmp(tag, "abbr") == 0 || strcmp(tag, "cite") == 0);
}

static int is_self_closing_tag(const char *tag) {
    return (strcmp(tag, "br") == 0 || strcmp(tag, "hr") == 0 || strcmp(tag, "img") == 0 ||
            strcmp(tag, "input") == 0 || strcmp(tag, "meta") == 0 || strcmp(tag, "link") == 0 ||
            strcmp(tag, "base") == 0 || strcmp(tag, "col") == 0 || strcmp(tag, "embed") == 0 ||
            strcmp(tag, "source") == 0 || strcmp(tag, "track") == 0 || strcmp(tag, "wbr") == 0);
}

static int parse_html_internal(const char *html, int *pos, DOMNode *parent, RenderPage *page, int depth) {
    if (depth > 200) return 0;
    int len = strlen(html);

    while (*pos < len) {
        /* Find next < */
        const char *remain = html + *pos;
        char *next_tag = strchr(remain, '<');
        if (!next_tag) break;

        int text_start = *pos;
        int text_end = next_tag - html;

        if (text_end > text_start) {
            int text_len = text_end - text_start;
            char *text = (char*)malloc(text_len + 1);
            if (text) {
                strncpy(text, html + text_start, text_len);
                text[text_len] = '\0';
                util_trim(text);
                if (text[0] != '\0') {
                    DOMNode *text_node = dom_create_node(NODE_TEXT, NULL);
                    if (text_node) {
                        util_strlcpy(text_node->text, text, MAX_LINE);
                        dom_add_child(parent, text_node);
                        page->total_text_nodes++;
                        page->total_chars += strlen(text);
                    }
                }
                free(text);
            }
        }

        *pos = text_end;
        if (*pos >= len) break;

        char tag[64] = {0};
        int is_closing = 0, is_self_closing = 0;
        if (!parse_tag(html, pos, tag, 64, &is_closing, &is_self_closing)) {
            (*pos)++;
            continue;
        }

        if (tag[0] == '\0') continue;

        if (is_closing) {
            if (strcmp(tag, parent->tag_name) == 0) {
                return 1;
            }
            continue;
        }

        if (strcmp(tag, "!DOCTYPE") == 0 || strcmp(tag, "doctype") == 0) {
            DOMNode *doctype = dom_create_node(NODE_DOCTYPE, "!DOCTYPE");
            if (doctype) dom_add_child(parent, doctype);
            continue;
        }

        if (strcmp(tag, "!--") == 0) {
            /* Find end of comment */
            char *end_comment = strstr(html + *pos, "-->");
            if (end_comment) {
                *pos = end_comment - html + 3;
            }
            continue;
        }

        if (strcmp(tag, "script") == 0) {
            page->has_scripts = 1;
            page->num_scripts++;
            DOMNode *script_node = dom_create_node(NODE_SCRIPT, "script");
            if (script_node) {
                dom_add_child(parent, script_node);
                char *end_script = strstr(html + *pos, "</script>");
                if (end_script) {
                    int content_len = end_script - (html + *pos);
                    if (content_len > 0 && content_len < MAX_LINE) {
                        strncpy(script_node->text, html + *pos, content_len);
                        script_node->text[content_len] = '\0';
                    }
                    *pos = end_script - html + 9;
                }
            }
            continue;
        }

        if (strcmp(tag, "style") == 0) {
            page->has_styles = 1;
            page->num_styles++;
            DOMNode *style_node = dom_create_node(NODE_STYLE, "style");
            if (style_node) {
                dom_add_child(parent, style_node);
                char *end_style = strstr(html + *pos, "</style>");
                if (end_style) {
                    int content_len = end_style - (html + *pos);
                    if (content_len > 0 && content_len < MAX_LINE) {
                        strncpy(style_node->text, html + *pos, content_len);
                        style_node->text[content_len] = '\0';
                    }
                    *pos = end_style - html + 8;
                }
            }
            continue;
        }

        DOMNode *node = dom_create_node(NODE_ELEMENT, tag);
        if (!node) continue;

        parse_attributes(html, pos, node);

        if (strcmp(tag, "a") == 0) page->num_links++;
        else if (strcmp(tag, "img") == 0) page->num_images++;
        else if (strcmp(tag, "form") == 0) page->num_forms++;
        else if (strcmp(tag, "table") == 0) page->num_tables++;
        else if (strcmp(tag, "h1") == 0 || strcmp(tag, "h2") == 0 || strcmp(tag, "h3") == 0 ||
                 strcmp(tag, "h4") == 0 || strcmp(tag, "h5") == 0 || strcmp(tag, "h6") == 0) page->num_headings++;
        else if (strcmp(tag, "p") == 0) page->num_paragraphs++;
        else if (strcmp(tag, "ul") == 0 || strcmp(tag, "ol") == 0) page->num_lists++;
        else if (strcmp(tag, "iframe") == 0) page->num_iframes++;
        else if (strcmp(tag, "div") == 0) page->num_divs++;
        else if (strcmp(tag, "span") == 0) page->num_spans++;
        else if (strcmp(tag, "input") == 0) page->num_inputs++;
        else if (strcmp(tag, "button") == 0) page->num_buttons++;
        else if (strcmp(tag, "canvas") == 0) page->has_canvas = 1;
        else if (strcmp(tag, "svg") == 0) page->has_svg = 1;
        else if (strcmp(tag, "video") == 0) page->has_video = 1;
        else if (strcmp(tag, "audio") == 0) page->has_audio = 1;

        page->total_elements++;

        dom_add_child(parent, node);

        if (strcmp(tag, "title") == 0) {
            int title_start = *pos;
            char *end_title = strstr(html + *pos, "</title>");
            if (end_title) {
                int title_len = end_title - (html + *pos);
                if (title_len > 0 && title_len < MAX_TITLE) {
                    strncpy(page->title, html + *pos, title_len);
                    page->title[title_len] = '\0';
                    util_trim(page->title);
                }
                *pos = end_title - html + 8;
            }
            continue;
        }

        if (strcmp(tag, "meta") == 0) {
            int i;
            for (i = 0; i < node->num_attributes; i++) {
                if (strcmp(node->attr_names[i], "name") == 0 && strcmp(node->attributes[i], "description") == 0) {
                    int j;
                    for (j = 0; j < node->num_attributes; j++) {
                        if (strcmp(node->attr_names[j], "content") == 0) {
                            util_strlcpy(page->description, node->attributes[j], MAX_LINE);
                        }
                    }
                }
                if (strcmp(node->attr_names[i], "charset") == 0) {
                    util_strlcpy(page->charset, node->attributes[i], 32);
                }
            }
            continue;
        }

        if (strcmp(tag, "link") == 0 && strcmp(tag, "br") != 0) {
            /* Check for stylesheet links */
            int i;
            for (i = 0; i < node->num_attributes; i++) {
                if (strcmp(node->attr_names[i], "rel") == 0 &&
                    strcmp(node->attributes[i], "stylesheet") == 0) {
                    page->has_styles = 1;
                    page->num_styles++;
                }
            }
        }

        if (!is_self_closing && !is_self_closing_tag(tag)) {
            parse_html_internal(html, pos, node, page, depth + 1);
        }
    }

    return 1;
}

int renderer_parse_html(const char *html, RenderPage *page, BrowserOS *os) {
    if (!html || !page) return 0;

    if (page->document) {
        dom_free(page->document);
        page->document = NULL;
    }

    page->document = dom_create_node(NODE_DOCUMENT, "document");
    if (!page->document) return 0;

    int pos = 0;
    int ok = parse_html_internal(html, &pos, page->document, page, 0);
    page->loaded = ok;

    return ok;
}

static void render_text_node(DOMNode *node, char *output, int *out_pos, int max_out, int depth, int width) {
    if (!node || !node->text || !node->text[0]) return;
    if (*out_pos >= max_out - 1) return;

    char indent[128];
    int i;
    for (i = 0; i < depth && i < 127; i++) indent[i] = ' ';
    indent[i] = '\0';

    int remaining = max_out - *out_pos - 1;
    if (remaining > 0) {
        *out_pos += snprintf(output + *out_pos, remaining, "%s%s", indent, node->text);
    }
}

static void render_element_node(DOMNode *node, char *output, int *out_pos, int max_out, int depth, int width, RenderPage *page) {
    if (*out_pos >= max_out - 1) return;

    char indent[128];
    int i;
    for (i = 0; i < depth && i < 127; i++) indent[i] = ' ';
    indent[i] = '\0';

    int remaining = max_out - *out_pos - 1;

    /* Heading elements */
    if (node->tag_name[0] == 'h' && strlen(node->tag_name) == 2 &&
        node->tag_name[1] >= '1' && node->tag_name[1] <= '6') {
        char line[MAX_LINE] = {0};
        int text_pos = 0;
        for (i = 0; i < node->num_children; i++) {
            if (node->children[i]->type == NODE_TEXT) {
                util_strlcpy(line + text_pos, node->children[i]->text, MAX_LINE - text_pos);
                text_pos += strlen(node->children[i]->text);
            }
        }
        util_trim(line);

        if (line[0]) {
            char sep[128];
            int s = 0;
            int line_len = strlen(line);
            int total_width = line_len + 4;
            if (total_width > width) total_width = width;
            int h_level = node->tag_name[1] - '0';
            char heading_char = (h_level <= 2) ? '=' : '-';
            for (s = 0; s < total_width && s < 127; s++) sep[s] = heading_char;
            sep[s] = '\0';
            *out_pos += snprintf(output + *out_pos, remaining, "%s%s\n%s  %s\n%s%s\n",
                                 indent, sep, indent, line, indent, sep);
        }
        return;
    }

    /* Title element rendered earlier */
    if (strcmp(node->tag_name, "title") == 0) return;

    /* Paragraph */
    if (strcmp(node->tag_name, "p") == 0) {
        char line[MAX_LINE] = {0};
        int text_pos = 0;
        for (i = 0; i < node->num_children; i++) {
            if (node->children[i]->type == NODE_TEXT) {
                util_strlcpy(line + text_pos, node->children[i]->text, MAX_LINE - text_pos);
                text_pos += strlen(node->children[i]->text);
            }
        }
        util_trim(line);
        if (line[0]) {
            *out_pos += snprintf(output + *out_pos, remaining, "\n%s%s\n\n", indent, line);
        }
        return;
    }

    /* Horizontal rule */
    if (strcmp(node->tag_name, "hr") == 0) {
        char sep[128];
        for (i = 0; i < width && i < 127; i++) sep[i] = '-';
        sep[(width < 127) ? width : 127] = '\0';
        *out_pos += snprintf(output + *out_pos, remaining, "%s%s\n", indent, sep);
        return;
    }

    /* Line break */
    if (strcmp(node->tag_name, "br") == 0) {
        *out_pos += snprintf(output + *out_pos, remaining, "\n");
        return;
    }

    /* List items */
    if (strcmp(node->tag_name, "li") == 0) {
        char line[MAX_LINE] = {0};
        int text_pos = 0;
        for (i = 0; i < node->num_children; i++) {
            if (node->children[i]->type == NODE_TEXT) {
                util_strlcpy(line + text_pos, node->children[i]->text, MAX_LINE - text_pos);
                text_pos += strlen(node->children[i]->text);
            }
        }
        util_trim(line);
        if (line[0]) {
            *out_pos += snprintf(output + *out_pos, remaining, "%s  * %s\n", indent, line);
        }
        return;
    }

    /* Table - simple rendering */
    if (strcmp(node->tag_name, "table") == 0) {
        *out_pos += snprintf(output + *out_pos, remaining, "\n");
        for (i = 0; i < node->num_children; i++) {
            render_element_node(node->children[i], output, out_pos, max_out, depth + 1, width, page);
        }
        *out_pos += snprintf(output + *out_pos, max_out - *out_pos - 1, "\n");
        return;
    }

    if (strcmp(node->tag_name, "tr") == 0) {
        *out_pos += snprintf(output + *out_pos, remaining, "%s|", indent);
        for (i = 0; i < node->num_children; i++) {
            if (strcmp(node->children[i]->tag_name, "td") == 0 ||
                strcmp(node->children[i]->tag_name, "th") == 0) {
                char cell[MAX_LINE] = {0};
                int ti;
                int text_pos = 0;
                for (ti = 0; ti < node->children[i]->num_children; ti++) {
                    if (node->children[i]->children[ti]->type == NODE_TEXT) {
                        util_strlcpy(cell + text_pos, node->children[i]->children[ti]->text, MAX_LINE - text_pos);
                        text_pos += strlen(node->children[i]->children[ti]->text);
                    }
                }
                util_trim(cell);
                *out_pos += snprintf(output + *out_pos, max_out - *out_pos - 1, " %-20s |", cell);
            }
        }
        *out_pos += snprintf(output + *out_pos, max_out - *out_pos - 1, "\n");
        return;
    }

    /* Anchor/link */
    if (strcmp(node->tag_name, "a") == 0) {
        char href[MAX_URL] = {0};
        for (i = 0; i < node->num_attributes; i++) {
            if (strcmp(node->attr_names[i], "href") == 0) {
                util_strlcpy(href, node->attributes[i], MAX_URL);
            }
        }
        char line[MAX_LINE] = {0};
        int text_pos = 0;
        for (i = 0; i < node->num_children; i++) {
            if (node->children[i]->type == NODE_TEXT) {
                util_strlcpy(line + text_pos, node->children[i]->text, MAX_LINE - text_pos);
                text_pos += strlen(node->children[i]->text);
            }
        }
        util_trim(line);
        if (line[0]) {
            if (href[0]) {
                *out_pos += snprintf(output + *out_pos, remaining, "%s[%s](%s)\n", indent, line, href);
            } else {
                *out_pos += snprintf(output + *out_pos, remaining, "%s%s\n", indent, line);
            }
        }
        return;
    }

    /* Image */
    if (strcmp(node->tag_name, "img") == 0) {
        char alt[128] = {0};
        char src[MAX_URL] = {0};
        for (i = 0; i < node->num_attributes; i++) {
            if (strcmp(node->attr_names[i], "alt") == 0)
                util_strlcpy(alt, node->attributes[i], 128);
            if (strcmp(node->attr_names[i], "src") == 0)
                util_strlcpy(src, node->attributes[i], MAX_URL);
        }
        if (alt[0]) {
            *out_pos += snprintf(output + *out_pos, remaining, "%s[IMG: %s]\n", indent, alt);
        } else if (src[0]) {
            *out_pos += snprintf(output + *out_pos, remaining, "%s[IMAGE]\n", indent);
        }
        return;
    }

    /* Div, span, and other containers - render children */
    for (i = 0; i < node->num_children; i++) {
        if (node->children[i]->type == NODE_TEXT) {
            render_text_node(node->children[i], output, out_pos, max_out, depth, width);
        } else if (node->children[i]->type == NODE_ELEMENT) {
            render_element_node(node->children[i], output, out_pos, max_out, depth, width, page);
        }
    }
}

int renderer_format_output(RenderPage *page, char *output, int max_out, int width, BrowserOS *os) {
    if (!page || !output || max_out <= 0) return 0;
    output[0] = '\0';
    int out_pos = 0;

    Engine *e = engine_get_active(os);

    /* Render engine header */
    int remaining = max_out - out_pos - 1;
    out_pos += snprintf(output + out_pos, remaining,
        "\x1b[1;36m+==============================================================================+\x1b[0m\n");

    /* Title bar */
    if (page->title[0]) {
        remaining = max_out - out_pos - 1;
        out_pos += snprintf(output + out_pos, remaining,
            "\x1b[1;37m|  \x1b[1;33m%s\x1b[0m\n", page->title);
    }

    /* URL and engine info */
    remaining = max_out - out_pos - 1;
    out_pos += snprintf(output + out_pos, remaining,
        "\x1b[1;36m|  Engine: %s %s | Render: %s | Load: %.2fs\x1b[0m\n",
        e ? e->name : "N/A",
        e ? e->version : "",
        page->loaded ? "Complete" : "Failed",
        page->load_time);

    remaining = max_out - out_pos - 1;
    out_pos += snprintf(output + out_pos, remaining,
        "\x1b[1;36m+==============================================================================+\x1b[0m\n\n");

    /* Render page content */
    if (page->document) {
        int i;
        for (i = 0; i < page->document->num_children; i++) {
            if (page->document->children[i]->type == NODE_ELEMENT) {
                render_element_node(page->document->children[i], output, &out_pos, max_out, 0, width, page);
            }
        }
    }

    /* Render page stats footer */
    remaining = max_out - out_pos - 1;
    out_pos += snprintf(output + out_pos, remaining,
        "\n\x1b[1;36m+==============================================================================+\x1b[0m\n");
    remaining = max_out - out_pos - 1;
    out_pos += snprintf(output + out_pos, remaining,
        "\x1b[1;37m|  Elements: %d | Text Nodes: %d | Chars: %d | Links: %d | Images: %d | Forms: %d\x1b[0m\n",
        page->total_elements, page->total_text_nodes, page->total_chars,
        page->num_links, page->num_images, page->num_forms);
    remaining = max_out - out_pos - 1;
    out_pos += snprintf(output + out_pos, remaining,
        "\x1b[1;36m+==============================================================================+\x1b[0m\n");

    return out_pos;
}

int renderer_extract_links(RenderPage *page, char links[][MAX_URL], int max_links) {
    if (!page || !page->document) return 0;
    int count = 0;

    /* Simple recursive link extraction */
    DOMNode *stack[1024];
    int stack_pos = 0;
    stack[stack_pos++] = page->document;

    while (stack_pos > 0 && count < max_links) {
        DOMNode *node = stack[--stack_pos];
        if (node->type == NODE_ELEMENT && strcmp(node->tag_name, "a") == 0) {
            int i;
            for (i = 0; i < node->num_attributes; i++) {
                if (strcmp(node->attr_names[i], "href") == 0) {
                    util_strlcpy(links[count++], node->attributes[i], MAX_URL);
                    break;
                }
            }
        }
        int i;
        for (i = node->num_children - 1; i >= 0 && stack_pos < 1024; i--) {
            stack[stack_pos++] = node->children[i];
        }
    }

    return count;
}

int renderer_extract_text(RenderPage *page, char *text, int max_len) {
    if (!page || !text) return 0;
    text[0] = '\0';
    int pos = 0;

    DOMNode *stack[1024];
    int stack_pos = 0;
    stack[stack_pos++] = page->document;

    while (stack_pos > 0) {
        DOMNode *node = stack[--stack_pos];
        if (node->type == NODE_TEXT && node->text[0]) {
            int remaining = max_len - pos - 1;
            if (remaining > 0) {
                pos += snprintf(text + pos, remaining, "%s ", node->text);
            }
        }
        int i;
        for (i = node->num_children - 1; i >= 0 && stack_pos < 1024; i--) {
            stack[stack_pos++] = node->children[i];
        }
    }

    return pos;
}

void renderer_analyze_content(RenderPage *page) {
    if (!page) return;
    printf("  Content Analysis:\n");
    printf("  ========================================\n");
    printf("  Title: %s\n", page->title[0] ? page->title : "(none)");
    printf("  Description: %s\n", page->description[0] ? page->description : "(none)");
    printf("  Charset: %s\n", page->charset);
    printf("  ========================================\n");
    printf("  Element Statistics:\n");
    printf("  - Total Elements: %d\n", page->total_elements);
    printf("  - Text Nodes:     %d\n", page->total_text_nodes);
    printf("  - Total Chars:    %d\n", page->total_chars);
    printf("  - Headings:       %d\n", page->num_headings);
    printf("  - Paragraphs:     %d\n", page->num_paragraphs);
    printf("  - Links:          %d\n", page->num_links);
    printf("  - Images:         %d\n", page->num_images);
    printf("  - Tables:         %d\n", page->num_tables);
    printf("  - Forms:          %d\n", page->num_forms);
    printf("  - Lists:          %d\n", page->num_lists);
    printf("  - Divs:           %d\n", page->num_divs);
    printf("  - Spans:          %d\n", page->num_spans);
    printf("  - Inputs:         %d\n", page->num_inputs);
    printf("  - Buttons:        %d\n", page->num_buttons);
    printf("  - Iframes:        %d\n", page->num_iframes);
    printf("  ========================================\n");
    printf("  Features: %s%s%s%s%s%s%s\n",
           page->has_canvas ? "Canvas " : "",
           page->has_svg ? "SVG " : "",
           page->has_scripts ? "Scripts " : "",
           page->has_styles ? "Styles " : "",
           page->has_forms ? "Forms " : "",
           page->has_video ? "Video " : "",
           page->has_audio ? "Audio " : "");
}

void renderer_print_stats(RenderPage *page) {
    if (!page) return;
    printf("  Render Stats:\n");
    printf("  - Loaded: %s\n", page->loaded ? "Yes" : "No");
    printf("  - Elements: %d\n", page->total_elements);
    printf("  - DOM Depth: %d\n", page->num_divs + page->num_paragraphs > 0 ? 3 : 1);
    printf("  - Load Time: %.3fs\n", page->load_time);
}

/* Generate sample HTML for known sites */
char* generate_sample_html(const char *title, char *buf, int max_len, BrowserOS *os) {
    Engine *e = engine_get_active(os);
    const char *eng_name = e ? e->name : "BrowserOS";
    const char *eng_ver = e ? e->version : "3.0";

    snprintf(buf, max_len,
        "<!DOCTYPE html><html><head>"
        "<title>%s - BrowseOS</title>"
        "<meta charset=\"UTF-8\">"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
        "</head><body>"
        "<h1>%s</h1>"
        "<hr/>"
        "<p><strong>Rendered by:</strong> %s %s</p>"
        "<p><strong>Operating System:</strong> BrowseOS v%s (Golden Sakura)</p>"
        "<p><strong>User-Agent:</strong> %s</p>"
        "<hr/>"
        "<h2>Welcome to %s</h2>"
        "<p>This is a simulated web page being rendered by BrowseOS.</p>"
        "<p>BrowseOS is a feature-rich browser operating system simulator running in your terminal.</p>"
        "<h3>Available Actions:</h3>"
        "<ul>"
        "<li><a href='https://github.com'>Visit GitHub</a> - Explore open source projects</li>"
        "<li><a href='https://google.com'>Visit Google</a> - Search the web</li>"
        "<li><a href='https://wikipedia.org'>Visit Wikipedia</a> - Browse knowledge</li>"
        "<li><a href='https://stackoverflow.com'>Visit Stack Overflow</a> - Developer community</li>"
        "</ul>"
        "<h3>System Status:</h3>"
        "<table border='1'>"
        "<tr><th>Component</th><th>Status</th><th>Version</th></tr>"
        "<tr><td>Browser Engine</td><td>Active</td><td>%s %s</td></tr>"
        "<tr><td>JavaScript Engine</td><td>%s</td><td>v1.0</td></tr>"
        "<tr><td>CSS Engine</td><td>%s</td><td>v3.0</td></tr>"
        "<tr><td>Network Stack</td><td>Connected</td><td>v2.0</td></tr>"
        "<tr><td>Cache</td><td>%s</td><td>%d entries</td></tr>"
        "<tr><td>Cookies</td><td>%s</td><td>%d cookies</td></tr>"
        "</table>"
        "<hr/>"
        "<p><em>Generated by BrowseOS Build %d | Type 'help' for commands</em></p>"
        "</body></html>",
        title, title,
        eng_name, eng_ver,
        BROWSEOS_VERSION,
        engine_get_user_agent(os),
        title,
        eng_name, eng_ver,
        os->config.enable_javascript ? "Enabled" : "Disabled",
        os->config.enable_javascript ? "Full" : "Limited",
        os->config.enable_cache ? "Enabled" : "Disabled",
        os->cache_count,
        os->config.enable_cookies ? "Enabled" : "Disabled",
        os->cookie_count,
        BROWSEOS_BUILD);

    return buf;
}
