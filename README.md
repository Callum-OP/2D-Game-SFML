# How to run
To run the game on Windows I am using MSVC, the command will be similar to:
  cl /EHsc /std:c++17 /I 'C:\C++ Libraries\SFML-3.0.0-windows-vc17-32-bit\SFML-3.0.0\include' C:\Users\callu\2D-Game-C++\main.cpp /link '/LIBPATH:C:\C++ Libraries\SFML-3.0.0-     windows-vc17-32-bit\SFML-3.0.0\lib' sfml-graphics.lib sfml-window.lib sfml-system.lib
  
However the command changes based on the file path of where the SFML 3.0 library is installed on your computer.
If you have MSVC and SFML 3.0 installed and edited the command to have the right sfml file path then you should be able to run the game.

You only need to run the main file.
You may want to edit the c_cpp_properties.json file with the correct sfml file path.

# About the app
This a 2D top down game using C++ and SFML 3.0, includes 8 directional movement and animation, it is still a work in progress but is planned to have combat, health and enemies and be similar to top down zelda games. I am very new to SFML so I'm learning as I go.

This app requires the SMFL 3.0 library to work, specifically the graphics, window and system components.

Below is a screenshot of the game in its current state, further below is a video.

<img width="395" alt="2D Game SFML" src="https://github.com/user-attachments/assets/78444bac-4418-441f-b92b-858ecac660cd" />

![video3279694462](https://github.com/user-attachments/assets/a0c55427-53c4-4239-acf4-ca926e631543)


