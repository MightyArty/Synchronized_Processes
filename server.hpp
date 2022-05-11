#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <fcntl.h>
#include <sys/unistd.h>
#include <signal.h>
#include <cmath>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ostream>
#include <cstdlib>
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/stat.h>
#define em 5
#define PORT 30090
#define MAX_STACK 40000
#define MAX_PORT 65535
#define MIN_PORT 2000
#define ARG_N 2
#define MAX_CLIENT 50
#define BUFFSIZE 1024
typedef struct Stack
{
   char data[BUFFSIZE];
   struct Stack *next;
} Stack;
typedef struct info_Stack
{
   struct Stack *mmap_address;
   struct Stack *head_address;
   int *size;
} info;
int listenFd;
int  portNo;
struct info_Stack *global_info;
socklen_t len; // store size of the address
struct sockaddr_in svrAdd, clntAdd;
// for locking the file
struct flock fl;
int fd;
/**
 * @brief Deallocates the space previously allocated by malloc(), calloc(), aligned_alloc(), (since C11) or realloc().

If ptr is a null pointer, the function does nothing.

The behavior is undefined if the value of ptr does not equal a value returned earlier by malloc(), calloc(), realloc(),
 *
 * @param root : A reference to the top element in the stack.
 * @return none.
 */
void free_stack(struct info_Stack **);
/**
 * @brief Colloring the text in red color
 * just for fun :)
 * @return none.
 */
void red();
/**
 * @brief Colloring the text in yellow color
 * just for fun :)
 * @return none.
 */
void yellow();
/**
 * @brief Colloring the text in green color
 * just for fun :)
 * @return none.
 */
void green();
/**
 * @brief Colloring the text in blue color
 * just for fun :)
 * @return none.
 */
void blue();
/**
 * @brief Reseting the text color back to default
 * just for fun :)
 * @return none.
 */
void reset();
/**
 * @brief
 *
 * @param signum : number's signal
 * @return non.
 */
void sig_handler(int);
/**
 * @brief Test whether container is empty
Returns whether the stack is empty: i.e. whether its size is zero.

This member function effectively calls member empty of the underlying container object.
 *
 * @param root : root stack
 * @return true if the underlying container's size is 0, false otherwise.
 */
int isEmpty(struct info_Stack **);
/**
 * @brief Remove top element
          Removes the element on top of the stack, effectively reducing its size by one.

          The element removed is the latest element inserted into the stack, whose value can be retrieved by calling member stack::top.

          This calls the removed element's destructor.

          This member function effectively calls the member function pop_back of the underlying container object.
 *
 * @param root : ref root stack
 * @return A reference to the top element in the stack.
 */
Stack *pop(info_Stack **);
/**
 * @brief Inserts a new element at the top of the stack, above its current top element. The content of this new element is initialized to a copy of val.

    This member function effectively calls the member function push_back of the underlying container object.
 *
 * @param root : ref root stack
 * @param val :Value to which the inserted element is initialized.
                Member type value_type is the type of the elements in the container.
 * @return none
 */
void push(info_Stack **, char *);
/**
 * @brief Returns a reference to the top element in the stack.

    Since stacks are last-in first-out containers, the top element is the last element inserted into the stack.

    This member function effectively calls member back of the underlying container object.
 *
 * @param root : root stack
 * @return A reference to the top element in the stack.
 */
char *top(struct info_Stack **);

/**
 * @brief Connecting the client inputes : push/pop/top
 * Checking if the input is valid
 * If no -> throiwng an error
 * If yes -> dealing with the input in the needed way
 * @return void*
 */
void task1(int,pid_t,struct info_Stack **);

/**
 * @brief Initiallize the server side
 * @return int = 1 on success, 0 on failure
 */
int server(int argc, char *argv[]);