This a 2D top down game using C++ and SFML 3.0, includes 8 directional movement and animation, it is still a work in progress but is planned to have combat, health and enemies and be similar to top down zelda games. I am very new to SFML so I'm learning as I go.

This app requires the SMFL 3.0 library to work, specifically the graphics, window and system components.

To run the app on Windows I am using MSVC, the command to run the app is something similar to the command the below however the file path changes depending on where the SFML 3.0 library is installed on your computer.
cl /EHsc /std:c++17 /I 'C:\C++ Libraries\SFML-3.0.0-windows-vc17-32-bit\SFML-3.0.0\include' C:\Users\callu\2D-Game-C++\main.cpp /link '/LIBPATH:C:\C++ Libraries\SFML-3.0.0-windows-vc17-32-bit\SFML-3.0.0\lib' sfml-graphics.lib sfml-window.lib sfml-system.lib