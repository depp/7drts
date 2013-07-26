#include "opengl.hpp"
#include "screen.hpp"
#include "texture.hpp"
#include "resource.hpp"
#include <cstdio>
namespace Seven {

// Test, temporary
class TestScreen : public Screen {
    Texture tex;

public:
    TestScreen() {
        tex.load_file("assets/sprites/little_sprite/LSdefaultfront");
    }

    virtual ~TestScreen() { }

    virtual void draw(double time) {
        (void) time;
        glViewport(0, 0, width(), height());
        glClearColor(0.9, 0.3, 0.1, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, (double) width(),
                0, (double) height(),
                1.0, -1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glScaled((double) tex.img_width(), (double) tex.img_height(), 1.0);
        float tw = (float) tex.img_width() / (float) tex.tex_width();
        float th = (float) tex.img_height() / (float) tex.tex_height();
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex.texture());
        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(0.0f,   th); glVertex2f(0.0f, 0.0f);
        glTexCoord2f(  tw,   th); glVertex2f(1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 1.0f);
        glTexCoord2f(  tw, 0.0f); glVertex2f(1.0f, 1.0f);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_BLEND);
    }
};

int Screen::width_, Screen::height_;
Screen *Screen::main_screen_ = nullptr;
bool Screen::hidden_ = false;

Screen::~Screen()
{ }

void Screen::resize() { }

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

void Screen::resize_callback(GLFWwindow *window, int width, int height) {
    (void) window;
    width_ = width;
    height_ = height;
    main_screen_->resize();
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
        std::fputs("error initializing GLFW\n", stderr);
        glfwTerminate();
        return 1;
    }

    glfwSetWindowSizeCallback(window, resize_callback);
    glfwSetWindowIconifyCallback(window, iconify_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetCursorEnterCallback(window, cursor_enter_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwGetWindowSize(window, &width_, &height_);
    std::printf("window: %dx%d\n", width_, height_);

    glfwMakeContextCurrent(window);

    main_screen_ = new TestScreen();
    Resource::load_all();

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
