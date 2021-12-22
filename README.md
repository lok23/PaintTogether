# PaintTogether
[![4](https://user-images.githubusercontent.com/45181211/147136570-0bd5b0f1-ad03-48c4-a483-8c74e234aca1.PNG)](https://youtu.be/O0215dgTq24?t=245)

A Project for CS5500 Fundamentals of Software Engineering. 

# What is this?
PaintTogether is a collaborative painting application where multiple clients can connect to a server and work together. This collaboration is possible because changes made on one user's canvas are reflected on another user's canvas. Clients can change brush color, undo, redo, or even reset the entire canvas to one color!




# Functionality
This app has 6 main functionalities
1) Paint
https://user-images.githubusercontent.com/45181211/147141232-78553552-39b4-43da-912b-a480d543231d.mp4

3) Undo Paint
https://user-images.githubusercontent.com/45181211/147141386-cec36dfc-2a60-4e40-98c9-44b0103c6dc6.mp4


5) Redo Paint
https://user-images.githubusercontent.com/45181211/147142437-86877571-18f0-4076-abf6-c764c4d93dc0.mp4



7) Change Color
https://user-images.githubusercontent.com/45181211/147142477-7ed6f0b6-7816-4495-98ad-e7f9b64ef93b.mp4


9) Reset Canvas (to selected color)
https://user-images.githubusercontent.com/45181211/147142485-53b45979-ee11-4ba0-b6ce-b6b665a329e1.mp4


12) Networked Chat
[![4](https://user-images.githubusercontent.com/45181211/147136570-0bd5b0f1-ad03-48c4-a483-8c74e234aca1.PNG)](https://youtu.be/O0215dgTq24?t=280)


## Purpose
The purpose of this project was to demonstrate our understanding of C++, the Command pattern, GUIs, networking, and working with an Agile project management methodology. This project was produced under a simulated work contract, with the customer being our professor, our Scrum Master being our TA, and our teammates being our co-workers. 

## How to Run
1) Download SFML (Simple and Fast Multimedia Library)
2) Download this repository
3) (For Windows), it is best to open this project in CLION. Run "untitled5_server", type "s" for server, then run "untitled5_client1" and "untitled5_client2", and type "c" for client.
4) (For Linux), download freeglut3 with `sudo apt-get install freeglut3-dev`. In the root directory, run `g++ -std=c++17 ./src/*.cpp -I./include/ -o App -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -lpthread -lglut -lGLU -lGL`. 


## Credits
To my teammates Edgar, Calvin, and Yatharth.
