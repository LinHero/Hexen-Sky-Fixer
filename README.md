# Hexen-Sky-Fixer
Fixes a bug with Slade, where when creating 200 pixel skies for Hexen, it corrupts the sky texture.

This bug is caused by Slade not supporting textuers with columns more then 128 pixels in length, and thus splitting the columns into 2 parts, (128 + 72)
This program takes the .lmp files that Slade outputs when exporting a texture and reworks it to use a single strip.

Multiple files can be provided if you wish to modify multiple at once.
The program outputs files using the same name but withthe _FIXED suffix.

This does NOT work with transparent textures. This only works with fully opaque sky textures, (Which Hexen will always have, even for transparent supporting skies)


This program was made, compiled, and tested using minGW, so it should compile out of box on POSIX systems, and with MinGW on windows.
These were the compiler settings used to compile it.

g++ -std=c++17 -Werror -Wall -Wno-unused -Wno-sign-compare -Wno-narrowing main.cpp -o build/%name%
