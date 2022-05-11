# Synchronized Processes

# By Artem Shabalin (@MightyArty) and Barak Sharabi (@bsharabi)
Fifth assigment in Operating System course.

We used </b>`fcntl()` to lock the prcesses instead of </b>`pthread`

And </b>`mmap MAP_SHARED` instead of malloc, in order to manipulate memory in multi processes enviroment.

## Credits
For file locking, we was inspired from </b>`Beej's Guide to UNIX IPC` : https://beej.us/guide/bgipc/html/multi/flocking.html

For more info about Network Programming : https://beej.us/guide/bgnet/html/

# How to use
```bash
$ "PUSH [your input]" : inserting element into the stack
$ "POP" : to remove element from the top of the stack
$ "TOP" : to show the top element of the stack
$ "COUNT" : to show how much elements at the stack
$ "CLEAN" : to remove all elements from the stack
$ "EXIT" : to exit the programm and close the connection between the client and the server
```

# How to run
```bash
# Clone the repository
$ git clone "https://github.com/MightyArty/Synchronized_Processes.git"
# Go into the repository
$ cd Synchronized_Processes
# Open the terminal on linux
$ Run "make"
# Make sure you enter a valid port number both on server and client side
$ Run "./server"
$ Run "./client"
# For the test
$ Run "./test"
```
# Cleaning files
```bash
# In the repository
$ Run "make clean"