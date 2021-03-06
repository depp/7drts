Seven-Day Real-Time Strategy Game
=================================

We are going to make a real-time strategy game in seven days: July
22-28, 2013.

Links
-----

See the game jam announcement here:

http://www.ludumdare.com/compo/2013/07/05/minild-44-announcement/

The design documents are on Google Drive (ask for write access!):

https://drive.google.com/folderview?id=0B0UUNK515OL_eklnWGFTbWd6REE

The repository is on GitHub:

https://github.com/depp/7drts

Project Info
------------

The goals are to have fun and make a game.

Dietrich is in charge, sorry but there's not enough time for a
democracy.

Info for programmers
--------------------

The game will be written in C++11, using OpenGL and GLFW.  You'll need
Autotools for Linux, Xcode for OS X, or Visual Studio 2012 (Express
Desktop is fine) for Windows.  Also see coding style below.

All code you write for the game must be licensed under the FreeBSD
license.  Add your name to LICENSE.txt.

http://opensource.org/licenses/BSD-2-Clause

Submit your code through GitHub pull requests.  Please try to write
nice commit messages.

Compiling on Linux
------------------

You'll need GLFW 3, which you probably have to install from source.
Note that GLFW 2 is not supported.

    $ autoreconf -fi
    $ ./configure --enable-warnings=-Werror
    $ make
    $ ./seven

Compiling on OS X
-----------------

You'll need to extract the GLEW and GLFW source code in the "libs"
directory, and rename the extracted code as "glew" and "glfw", then
build the code.  Building GLFW requires CMake.

    $ cd libs
    $ unzip /path/to/glfw-3.0.1.zip
    $ mv glfw-3.0.1 glfw
    $ (cd glfw; cmake .; make -j4)
    $ tar zxvf /path/to/glew-1.10.0.tgz
    $ mv glew-1.10.0 glew
    $ (cd glew; make -j4)

Once you're done, you must also set the run directory of the game.  In
Xcode, go to Project -> Scheme -> Edit Scheme... and for the schemes
"Run Seven.app" and "Profile Seven.app", go to Options -> Working
Directory and set it to $(PROJECT_DIR).

Whew!  That's a lot of work.  But you should be able to hit command-R
to just run the application.

Info for other creative folks
-----------------------------

All non-code content for the game must be licensed under the Creative
Commons Attribution 3.0 Unported license.

http://creativecommons.org/licenses/by/3.0/

Coding style
------------

* Limit to 80 colums, indent four spaces, never use tabs, never put
  two blank lines in a row.

* Classes, namespaces, types, etc. use CapitalizedWords.

* Variables, fields, functions etc. use lowercase_underscores.  Class
  fields have a trailing underscore (e.g. member_field_).

* Constants use ALL_CAPS_WITH_UNDERSCORES.

* Write "char *ptr" and "Object &ref", note the position of spaces.

* The "Google C++ Style Guide" is good.

* Don't try to be clever.

Example:

    namespace Seven {

    // One-line description of MyClass
    class MyClass : public OtherClass {
        // Fields always go at top.
        int member_;
        std::string other_member_;

    public:
        // Description of memberFunction
        void member_function(Obj &ref) {
            switch (ref.type) {
            case TYPE_ONE:
                doSomething(1, 2, &x);
                break;
            }

            // Okay to omit braces
            if (something)
                action();

            // If one branch has braces, all branches have braces
            if (abc == 123) {
                code();
            } else {
                more();
                etc();
            }
        }
    };

    }
