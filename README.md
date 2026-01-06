# Demo Video

https://github.com/user-attachments/assets/1028a2f5-8e7c-4651-9ff3-18818390168c


# How to run
To run the game on Windows I am using MSVC, the command will be similar to:
  cl /EHsc /std:c++17 /I 'C:\C++ Libraries\SFML-3.0.0-windows-vc17-32-bit\SFML-3.0.0\include' C:\Users\callu\2D-Game-C++\main.cpp /link '/LIBPATH:C:\C++ Libraries\SFML-3.0.0-     windows-vc17-32-bit\SFML-3.0.0\lib' sfml-graphics.lib sfml-window.lib sfml-system.lib
  
However the command changes based on the file path of where the SFML 3.0 library is installed on your computer.
If you have MSVC and SFML 3.0 installed and edited the command to have the right sfml file path then you should be able to run the game.
You only need to run the main file.
You may want to edit the c_cpp_properties.json file with the correct sfml file path.

This may change in the future.

# About the app
This a 2D top down game using C++ and SFML 3.0, includes 8 directional movement, combat, animation and enemies, it is still a work in progress, but is planned to be similar to top down zelda games. I am very new to SFML so I'm learning as I go.

This app requires the SMFL 3.0 library to work, specifically the graphics, audio, window and system components.

# Features to consider
Add more tile options
Ranged attack
More enemy options
Make enemies more intelligent and dangerous
Audio and sfx
Damage animation that delays next attack for enemies

Stop enemy AI and any other animations or movement when far outside the view of the player/camera
Lighting system?
Multiple more levels, Procedurally generated dungeons and levels?
Death screen
Main menu when paused

Finished assets (not the template ones being used at the moment)

# Issues
Character animation is a bit stuttery when sprinting
Sometimes get the errors: 
  "miniaudio ERROR: [WASAPI] Failed to initialize device.""
  "Failed to initialize the audio playback device: Invalid argument"
  "Failed to initialize audio device or engine"   
As a result sometimes audio works and sometimes it does not.
Enemies do not always spawn where they should spawn, some are outside of the map or seemingly absent.




