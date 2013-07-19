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
