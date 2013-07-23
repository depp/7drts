#ifndef SCREEN_HPP
#define SCREEN_HPP
#include <GLFW/glfw3.h>
namespace Seven {

class Screen {
public:
    virtual ~Screen();

    virtual void draw(double time) = 0;
    virtual void set_hidden(bool hidden);
    virtual void key(int keycode, bool state);
    virtual void mouse_button(int button, bool state);
    virtual void mouse_pos(double xpos, double ypos);
    virtual void mouse_enter(bool is_entered);
    virtual void scroll(double xoffset, double yoffset);

    static int width() { return width_; }
    static int height() { return height_; }

private:
    static int width_, height_;
    static Screen *main_screen_;
    static bool hidden_;

    static void iconify_callback(GLFWwindow *window, int state);
    static void key_callback(GLFWwindow *window, int key, int scancode,
                             int action, int mods);
    static void mouse_button_callback(GLFWwindow *window,
                                      int button, int action, int mods);
    static void cursor_pos_callback(GLFWwindow *window,
                                    double xpos, double ypos);
    static void cursor_enter_callback(GLFWwindow *window, int state);
    static void scroll_callback(GLFWwindow *window,
                                double xoffset, double yoffset);

public:
    static int main(int argc, char *argv[]);
};

}
#endif
