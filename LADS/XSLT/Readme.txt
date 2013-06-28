To use the XSLT static library:

1) Load the XSLT project into C++Builder and build it. This creates a '.lib' file.
2) Add this '.lib' file to your project.
3) Edit the Library Path for your project to include the XSLT project directory.
4) Edit the Include Path for your project also to include the XSLT project directory.
5) #include "Source.h" and use the 'transform' method.

There is an example in the 'example' subdirectory of the XSLT project itself.

Paul Storey, December 2010
