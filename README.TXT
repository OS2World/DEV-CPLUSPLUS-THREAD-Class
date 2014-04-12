THREAD-Class
============

Authors:    Markus Hof                mhof@iiic.ethz.ch
            Roman Fischer         rofische@iiic.ethz.ch

Purpose:    Hide details of creation and handling for OS/2-Threads.
            Possibility to pass arguments to the newly startet
            thread.

Compiler:   We developed under TopSpeed C++ V3.01. There shouldn't
            be any compiler-dependencies.

Notes:      - The original class definition includes the inheritance
              of a class called 'ERROR'. 
              We designed the class ERROR as a base class for all our
              classes to simplify error-handling. If somebody requests,
              I can post this class too.
            - I had to remove all code referencing ERROR by hand. I had
              no chance to test, if it still compiles ok. So...
            - We release this class "as it is". So if you encounter any
              problems or bugs, please let us know.
            - If you have any suggestions or enhancements, please mail
              us a copy of the modyfied code or a note describing your
              idea.           

rf / 02.01.93
