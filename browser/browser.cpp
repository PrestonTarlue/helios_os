// HELIOS 1.0 Web Browser
// Simple built-in web browser for HELIOS

extern void print_string(const char* str, unsigned char color);

class WebPage {
public:
    char title[256];
    char url[512];
    char content[4096];
    
    WebPage() {
        title[0] = 0;
        url[0] = 0;
        content[0] = 0;
    }
};

class Browser {
private:
    WebPage current_page;
    WebPage history[50];
    int history_index;
    
public:
    Browser() : history_index(0) {
        // Initialize browser
    }
    
    void navigate(const char* url) {
        // Parse URL and load page
        copy_string(current_page.url, url);
        load_page(url);
    }
    
    void load_page(const char* url) {
        // Load page content based on URL
        if (compare_strings(url, "about:home")) {
            load_home_page();
        } else if (compare_strings(url, "about:about")) {
            load_about_page();
        } else if (compare_strings(url, "about:version")) {
            load_version_page();
        } else {
            load_error_page(url);
        }
    }
    
private:
    void load_home_page() {
        copy_string(current_page.title, "HELIOS Browser - Home");
        const char* html = 
            "<html>\n"
            "<head><title>HELIOS Browser</title></head>\n"
            "<body>\n"
            "<h1>Welcome to HELIOS Browser</h1>\n"
            "<p>This is a simple web browser built into HELIOS OS 1.0</p>\n"
            "<h2>Available Pages:</h2>\n"
            "<ul>\n"
            "<li><a href='about:home'>Home</a></li>\n"
            "<li><a href='about:about'>About</a></li>\n"
            "<li><a href='about:version'>Version</a></li>\n"
            "</ul>\n"
            "</body>\n"
            "</html>";
        copy_string(current_page.content, html);
    }
    
    void load_about_page() {
        copy_string(current_page.title, "About HELIOS");
        const char* html =
            "<html>\n"
            "<head><title>About HELIOS</title></head>\n"
            "<body>\n"
            "<h1>About HELIOS 1.0</h1>\n"
            "<p>HELIOS is a modern operating system built from scratch in C++</p>\n"
            "<h2>Features:</h2>\n"
            "<ul>\n"
            "<li>Multi-tasking kernel</li>\n"
            "<li>Virtual memory management</li>\n"
            "<li>File system support</li>\n"
            "<li>Glass effect GUI</li>\n"
            "<li>Built-in browser</li>\n"
            "<li>Linux-compatible shell</li>\n"
            "</ul>\n"
            "<p><a href='about:home'>Back to Home</a></p>\n"
            "</body>\n"
            "</html>";
        copy_string(current_page.content, html);
    }
    
    void load_version_page() {
        copy_string(current_page.title, "Version Info");
        const char* html =
            "<html>\n"
            "<head><title>Version</title></head>\n"
            "<body>\n"
            "<h1>HELIOS Version Information</h1>\n"
            "<table border='1'>\n"
            "<tr><td>OS Name</td><td>HELIOS</td></tr>\n"
            "<tr><td>Version</td><td>1.0</td></tr>\n"
            "<tr><td>Architecture</td><td>x86-64</td></tr>\n"
            "<tr><td>Build Date</td><td>2026</td></tr>\n"
            "</table>\n"
            "<p><a href='about:home'>Back to Home</a></p>\n"
            "</body>\n"
            "</html>";
        copy_string(current_page.content, html);
    }
    
    void load_error_page(const char* url) {
        copy_string(current_page.title, "Page Not Found");
        copy_string(current_page.content,
            "<html>\n"
            "<head><title>404 Error</title></head>\n"
            "<body>\n"
            "<h1>404 - Page Not Found</h1>\n"
            "<p>The page you requested could not be found.</p>\n"
            "<p><a href='about:home'>Back to Home</a></p>\n"
            "</body>\n"
            "</html>");
    }
    
    void copy_string(char* dest, const char* src) {
        int i = 0;
        while (src[i] && i < 255) {
            dest[i] = src[i];
            i++;
        }
        dest[i] = 0;
    }
    
    bool compare_strings(const char* s1, const char* s2) {
        while (*s1 && *s2) {
            if (*s1 != *s2) return false;
            s1++;
            s2++;
        }
        return *s1 == *s2;
    }
    
public:
    void render() {
        // Render current page
        print_string("==============================\n", 0x0F);
        print_string(current_page.title, 0x0F);
        print_string("\n", 0x0F);
        print_string("==============================\n\n", 0x0F);
        print_string(current_page.content, 0x0F);
        print_string("\n", 0x0F);
    }
    
    void back() {
        if (history_index > 0) {
            history_index--;
            current_page = history[history_index];
            render();
        }
    }
    
    void forward() {
        if (history_index < 49) {
            history_index++;
            current_page = history[history_index];
            render();
        }
    }
};

Browser* browser_instance = nullptr;

void browser_init() {
    browser_instance = new Browser();
}

void browser_launch() {
    if (!browser_instance) {
        browser_init();
    }
    browser_instance->navigate("about:home");
    browser_instance->render();
}

void browser_navigate(const char* url) {
    if (browser_instance) {
        browser_instance->navigate(url);
        browser_instance->render();
    }
}

void browser_back() {
    if (browser_instance) {
        browser_instance->back();
    }
}

void browser_forward() {
    if (browser_instance) {
        browser_instance->forward();
    }
}
