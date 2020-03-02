files: receiver sender

receiver: recv.cpp
	g++ -o receiver recv.cpp

sender: sender.cpp
	g++ -o sender sender.cpp
