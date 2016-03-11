# Vintage Toy Synthesiser #

Vintage Toy Synthesiser is my project for the [Element14 Music Tech Design Challenge](https://www.element14.com/community/community/design-challenges/musictech) where I am converting a wooden vintage toy piano into a standalone digital synthesiser. See the [project blog](https://www.element14.com/community/community/design-challenges/musictech/blog/tags#/?tags=project_vintage) to stay up-to-date with the project activity. 

## Project Proposal ##

At the time of writing this the project is still under development, however here is the original project proposal specification:
+ Develop a digital synthesis engine that can generate and modify synthesised sounds
+ Modify the existing key mechanism so that it can trigger sounds and control velocity modulation on the synthesis engine
+ Add a set of potentiometers, switches and other controls to the existing piano enclosure for controlling the synthesis engine
+ Add an audio output for connecting the instrument to external speakers and recording equipment
+ Add MIDI I/O so that the instrument can be integrated into existing music production systems
+ Keep the existing aesthetic character of the vintage toy piano

For more info see the full [project proposal](https://www.element14.com/community/community/design-challenges/musictech/blog/2015/12/20/my-music-tech-design-challenge-project-proposal--vintage-toy-synthesiser).

## Hardware/Software Architecture ##

The synthesiser uses a couple of different hardware software platforms:
+ The brain and sound engine of the synthesiser are seperate Linux applications running on a [BeagleBone Black](https://beagleboard.org/black) board, writen in C/C++, which communicate via software sockets. See the [BeagleBone Black directory](https://github.com/liamlacey/Vintage-Toy-Synthesiser/tree/master/Code/BBB/) for the code for these applications.
+ The keyboard and panel use [Arduino Pro Mini](https://www.arduino.cc/en/Main/ArduinoBoardProMini) boards for sensor/control scanning. The Arduinos communicate with the BeagleBone Black via serial. See the [Arduino directory](https://github.com/liamlacey/Vintage-Toy-Synthesiser/tree/master/Code/Arduino) for the keyboard and panel code.
+ There is a Mac OS X editor application for saving and loading patches on the synth, written in C++ using the [JUCE](https://www.juce.com) framework. See the [VtsEditor directory](https://github.com/liamlacey/Vintage-Toy-Synthesiser/tree/master/Code/VtsEditor) for the code.

For more info on the software architecture see [here](https://www.element14.com/community/community/design-challenges/musictech/blog/2016/02/12/vintage-toy-synthesiser-software-architecture).
