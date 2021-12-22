# PaintTogether
[![4](https://user-images.githubusercontent.com/45181211/147136570-0bd5b0f1-ad03-48c4-a483-8c74e234aca1.PNG)](https://youtu.be/O0215dgTq24?t=245)

A Project for CS5500 Fundamentals of Software Engineering. 

# What is this?
PaintTogether is a collaborative painting application where multiple clients can connect to a server and work together. This collaboration is possible because changes made on one user's canvas are reflected on another user's canvas. Clients can change brush color, brush size, undo, redo, or even reset the entire canvas to one color!

# Functionality
This app has 6 main functionalities
1) Paint
2) Undo Paint
3) Redo Paint
4) Change Color
5) Reset Canvas (to selected color)
6) Change Brush Size
7) Networked Chat

## Purpose
The purpose of this project was to demonstrate our understanding of C++, the Command pattern, GUIs, networking, and working with an Agile project management methodology. This project was produced under a simulated work contract, with the customer being our professor, our Scrum Master being our TA, and our teammates being our co-workers. 

## How to Run
1) Download SFML (Simple and Fast Multimedia Library)
2) Download this repository
3) (For Windows), it is best to open this project in CLION. Run "untitled5_server", type "s" for server, then run "untitled5_client1" and "untitled5_client2", and type "c" for client.
4) (For Linux), download freeglut3 with `sudo apt-get install freeglut3-dev`. In the root directory, run `g++ -std=c++17 ./src/*.cpp -I./include/ -o App -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -lpthread -lglut -lGLU -lGL`. 


## Credits
To my teammates Edgar, Calvin, and Yatharth.
