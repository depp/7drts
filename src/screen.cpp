#include "opengl.hpp"
#include "screen.hpp"
#include <cstdio>
namespace Seven {

// Test, temporary
class TestScreen : public Screen {
public:
    virtual ~TestScreen()
    { }

    virtual void draw(double time)
    {
        (void) time;
        glClearColor(0.9, 0.3, 0.1, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
    }
};

Screen *Screen::main_screen_ = nullptr;
bool Screen::hidden_ = false;

Screen::~Screen()
{ }

void Screen::set_hidden(bool hidden) {
    (void) hidden;
}

void Screen::key(int keycode, bool state) {
    (void) keycode;
    (void) state;
}

void Screen::mouse_button(int button, bool state) {
    (void) button;
    (void) state;
}

void Screen::mouse_pos(double xpos, double ypos) {
    (void) xpos;
    (void) ypos;
}

void Screen::mouse_enter(bool is_entered) {
    (void) is_entered;
}

void Screen::scroll(double xoffset, double yoffset) {
    (void) xoffset;
    (void) yoffset;
}


void Screen::iconify_callback(GLFWwindow *window, int state) {
    (void) window;
    hidden_ = state != 0;
    main_screen_->set_hidden(hidden_);
}

void Screen::key_callback(GLFWwindow *window, int key, int scancode,
                          int action, int mods) {
    (void) window;
    (void) scancode;
    (void) mods;
    switch (action) {
    case GLFW_PRESS: main_screen_->key(key, true); break;
    case GLFW_RELEASE: main_screen_->key(key, false); break;
    default: break;
    }
}

void Screen::mouse_button_callback(GLFWwindow *window,
                                   int button, int action, int mods) {
    (void) window;
    (void) mods;
    main_screen_->mouse_button(button, action != GLFW_RELEASE);
}

void Screen::cursor_pos_callback(GLFWwindow *window,
                                 double xpos, double ypos) {
    (void) window;
    main_screen_->mouse_pos(xpos, ypos);
}

void Screen::cursor_enter_callback(GLFWwindow *window, int state) {
    (void) window;
    main_screen_->mouse_enter(state != GL_FALSE);
}

void Screen::scroll_callback(GLFWwindow *window,
                             double xoffset, double yoffset) {
    (void) window;
    main_screen_->scroll(xoffset, yoffset);
}

int Screen::main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    if (!glfwInit())
        return 1;

    GLFWwindow *window = glfwCreateWindow(
        1280, 720, "7drts", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return 1;
    }

    main_screen_ = new TestScreen();

    glfwSetWindowIconifyCallback(window, iconify_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetCursorEnterCallback(window, cursor_enter_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwMakeContextCurrent(window);

    GLenum glewErr = glewInit();
    if (glewErr != GLEW_OK) {
        std::fprintf(stderr, "error in glewInit(): %s\n",
                     glewGetErrorString(glewErr));
        glfwTerminate();
        return 1;
    }

    while (!glfwWindowShouldClose(window)) {
        if (hidden_) {
            glfwWaitEvents();
        } else {
            main_screen_->draw(glfwGetTime());
            gl_error_check("main");
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}

}

int main(int argc, char *argv[])
{
    Seven::Screen::main(argc, argv);
}
