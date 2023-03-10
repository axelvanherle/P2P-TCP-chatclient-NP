# P2P-TCP-chatclient-NP

Made by Axel Vanherle.

A peer-to-peer chat client in C++ using object-oriented programming principles and QT. The chat client allows users to connect with each other using TCP protocol, communicate through sending and receiving messages, and receive a list of connected peers. The chat client is designed to be extensible, with new peers being added dynamically, and with the ability to send and receive messages to and from all connected peers.

<img src="https://user-images.githubusercontent.com/94362354/219159487-90da2fb4-bf35-456f-b4fe-6007653a29f9.png" width="700"/>

## How does it work?

The program creates a server that listens on port 24042. When a client wants to connect, the server accepts the connection and adds the client's socket to a list. Whenever a new message is sent or received, the program iterates through the list of sockets and sends or receives the message accordingly.

However, there's an interesting feature: if there are two systems connected and a third one joins system one, system one will send the IP address and port number to the third system. This allows for peer-to-peer communication. This process continues recursively, so when a fourth system joins the third one, it receives the IP address and port numbers of system one and two, and so on.

Empty Example             |  Connected example
:------------------------:|:-------------------------:
![image](https://user-images.githubusercontent.com/94362354/220201597-72bdad57-3745-4d95-86eb-95c500c2e1a9.png)  |  ![image](https://user-images.githubusercontent.com/94362354/220201785-aa0acc36-6ee1-4964-9135-2ebe1e2319af.png)

## How do i run it?

1) Open Qt Creator and go to "File" > "Open File or Project".

2) Navigate to the directory where your project is located and select the project file (it should have a .pro extension).

3) Click "Open" to open the project.

4) Qt Creator should automatically detect the build settings for your project. If it doesn't, you can set them manually by clicking on "configure project" on the left-hand side of the window.

5) To build and run the project, press ctrl + r. This assumes this project is the primary QT project.

<hr></hr>

I also made a [blogpost]( https://axelvanherle.github.io/post/21-02-23_p2ptcpchatclient/ ) about this project, check it out if you want to learn more.
