// HELIOS 1.0 GUI System
// Graphics system with glass effects and wallpaper support

// Color definitions
typedef unsigned int Color;

const Color BLACK = 0x000000;
const Color WHITE = 0xFFFFFF;
const Color BLUE = 0x0066CC;
const Color TRANSPARENT = 0xFFFFFFFF;

class GUISystem {
private:
    unsigned char* framebuffer;
    int screen_width;
    int screen_height;
    int bytes_per_pixel;
    
public:
    GUISystem() : screen_width(1024), screen_height(768), bytes_per_pixel(4) {
        framebuffer = (unsigned char*) 0xE0000000;  // Linear framebuffer address
    }
    
    void initialize() {
        setup_framebuffer();
        draw_wallpaper();
        draw_glass_effect();
        draw_taskbar();
    }
    
private:
    void setup_framebuffer() {
        // Initialize framebuffer to black
        for (int i = 0; i < screen_width * screen_height * bytes_per_pixel; i++) {
            framebuffer[i] = 0;
        }
    }
    
    void draw_wallpaper() {
        // Draw gradient wallpaper (blue to darker blue)
        for (int y = 0; y < screen_height; y++) {
            for (int x = 0; x < screen_width; x++) {
                unsigned int pixel_offset = (y * screen_width + x) * bytes_per_pixel;
                
                // Calculate gradient
                unsigned char blue = 100 + (y * 50) / screen_height;
                unsigned char green = 50 + (y * 30) / screen_height;
                unsigned char red = 30 + (y * 20) / screen_height;
                
                framebuffer[pixel_offset] = blue;
                framebuffer[pixel_offset + 1] = green;
                framebuffer[pixel_offset + 2] = red;
                framebuffer[pixel_offset + 3] = 255;  // Alpha (opaque)
            }
        }
    }
    
    void draw_glass_effect() {
        // Draw semi-transparent glass panel
        int panel_x = 50;
        int panel_y = 50;
        int panel_width = 400;
        int panel_height = 300;
        
        // Draw panel area
        for (int y = panel_y; y < panel_y + panel_height; y++) {
            for (int x = panel_x; x < panel_x + panel_width; x++) {
                unsigned int pixel_offset = (y * screen_width + x) * bytes_per_pixel;
                
                // Glass effect - semi-transparent white
                framebuffer[pixel_offset] = 220;      // Blue
                framebuffer[pixel_offset + 1] = 220;  // Green
                framebuffer[pixel_offset + 2] = 220;  // Red
                framebuffer[pixel_offset + 3] = 180;  // Alpha (semi-transparent)
            }
        }
        
        // Draw glass border (light blue)
        draw_rectangle_outline(panel_x, panel_y, panel_width, panel_height, 0x00CCFF, 2);
    }
    
    void draw_taskbar() {
        // Draw taskbar at bottom of screen
        int taskbar_height = 50;
        int taskbar_y = screen_height - taskbar_height;
        
        // Draw taskbar background
        for (int y = taskbar_y; y < screen_height; y++) {
            for (int x = 0; x < screen_width; x++) {
                unsigned int pixel_offset = (y * screen_width + x) * bytes_per_pixel;
                
                // Dark glass taskbar
                framebuffer[pixel_offset] = 60;       // Blue
                framebuffer[pixel_offset + 1] = 60;   // Green
                framebuffer[pixel_offset + 2] = 60;   // Red
                framebuffer[pixel_offset + 3] = 200;  // Alpha
            }
        }
    }
    
    void draw_rectangle_outline(int x, int y, int width, int height, unsigned int color, int thickness) {
        // Draw rectangle border
        for (int i = 0; i < thickness; i++) {
            // Top edge
            draw_line(x, y + i, x + width, y + i, color);
            // Bottom edge
            draw_line(x, y + height - i, x + width, y + height - i, color);
            // Left edge
            draw_line(x + i, y, x + i, y + height, color);
            // Right edge
            draw_line(x + width - i, y, x + width - i, y + height, color);
        }
    }
    
    void draw_line(int x1, int y1, int x2, int y2, unsigned int color) {
        // Bresenham line algorithm (simple version)
        int dx = x2 - x1;
        int dy = y2 - y1;
        
        if (dx == 0) {
            // Vertical line
            int start_y = (y1 < y2) ? y1 : y2;
            int end_y = (y1 > y2) ? y1 : y2;
            for (int y = start_y; y <= end_y; y++) {
                set_pixel(x1, y, color);
            }
        } else if (dy == 0) {
            // Horizontal line
            int start_x = (x1 < x2) ? x1 : x2;
            int end_x = (x1 > x2) ? x1 : x2;
            for (int x = start_x; x <= end_x; x++) {
                set_pixel(x, y1, color);
            }
        }
    }
    
    void set_pixel(int x, int y, unsigned int color) {
        if (x >= 0 && x < screen_width && y >= 0 && y < screen_height) {
            unsigned int pixel_offset = (y * screen_width + x) * bytes_per_pixel;
            framebuffer[pixel_offset] = (color >> 16) & 0xFF;      // Red
            framebuffer[pixel_offset + 1] = (color >> 8) & 0xFF;   // Green
            framebuffer[pixel_offset + 2] = color & 0xFF;          // Blue
            framebuffer[pixel_offset + 3] = 255;                   // Alpha
        }
    }
};

GUISystem* gui = nullptr;

void gui_init() {
    gui = new GUISystem();
    gui->initialize();
}

void gui_draw_window(int x, int y, int width, int height, const char* title) {
    // Draw a GUI window with glass effect
    // To be implemented
}

void gui_draw_button(int x, int y, int width, int height, const char* label) {
    // Draw a button
    // To be implemented
}

void gui_draw_text(int x, int y, const char* text, unsigned int color) {
    // Draw text on GUI
    // To be implemented
}
