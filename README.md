# Meeting Manager 
Implements a simple scheduling application for rooms, persons, and meetings. Serves as a playground for working with various languages and design patterns, and so different versions are implemented for C, C++, Java and Python (in progress).

## Installing
Choose a language, navigate to the directory and build / run as needed.

* For the C implementation, 'make' will build two versions: one that uses a linked list and one that uses an array as the underlying data structure. See the Makefile for more details.
* C++ compilation requires '-std=c++14'

## Running
The application is console based and controlled by a simple command language consisting of a two letter command followed by the appropriate arguments for the command. The first letter of a command denotes an action, the second denotes the kind of object for the action.

The action letters are:
p - print
a - add
r - reschedule (meetings only)
d - delete
s - save
l - (lower-case L) load

The object letters are:
i - individual person
r - room
m - meeting
p - participant
s - schedule (all meetings - delete and print commands only)
g - group (all people - delete and print commands only)
a - all for the delete command, allocations in the print command (memory information); 
d - data (all people, rooms, and meetings - save and load commands only)

## Exiting
Enter command 'qq' to quit gracefully.