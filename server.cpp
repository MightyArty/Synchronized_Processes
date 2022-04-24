#include "server.hpp"
#include "malloc.h"
#define em 5

/**
 * @brief The functions welcome,red,yellow,blue,green and reset are just for fun
 * We want you to enjoy the proccess :)
 */
void welcome()
{
    printf("\033[1;31m    $$      $$$$$  $$$$$$$$$ $     $         \033[1;34m $$$$$         $$     $$$$$       $$      $  $    \n");
    printf("\033[1;31m   $  $     $   $      $      $   $          \033[1;34m $   $        $  $    $   $      $  $     $ $    \n");
    printf("\033[1;31m  $ -- $    $$$$$      $        $    \033[1;33m @@@@@@ \033[1;34m $$$$$$$     $ -- $   $$$$$     $ -- $    $$        \n");
    printf("\033[1;31m $      $   $    $     $        $            \033[1;34m $     $    $      $  $    $   $      $   $ $         \n");
    printf("\033[1;31m$        $  $     $    $        $            \033[1;34m $$$$$$$   $        $ $     $ $        $  $  $       \n");
}
void red()
{
    printf("\033[1;31m");
}
void yellow()
{
    printf("\033[1;33m");
}
void blue()
{
    printf("\033[0;34m");
}
void green()
{
    printf("\033[0;32m");
}
void reset()
{
    printf("\033[0m");
}

void sig_handler(int signum)
{
    free_stack(&my_stack);
    switch (signum)
    {
    case SIGTSTP:
        red();
        puts("");
        printf("Trying to exit on CONTROL-Z command\n");
    case SIGINT:
        yellow();
        printf("Trying to exit on CONTROL-C command\n");
    case SIGQUIT:
        green();
        printf("Trying to exit on CONTROL-/ command\n");
    default:
        close(listenFd);
        std::cout << "Closing Server" << std::endl;
    }
}

Stack *newNode(char *data)
{
    Stack *stack = new Stack();
    stack->data = strcpy((char *)_malloc(BUFFSIZE), data);
    stack->next = NULL;
    return stack;
}

int isEmpty(Stack *root)
{
    return !root;
}

Stack *pop(Stack **root)
{
    if (isEmpty(*root))
    {
        return NULL;
    }
    Stack *temp = *root;
    *root = (*root)->next;
    size--;
    return temp;
}

void push(Stack **root, char *data)
{
    size++;
    Stack *Stack = newNode(data);
    Stack->next = *root;
    *root = Stack;
}

char *top(Stack *root)
{
    char *s;
    if (isEmpty(root))
    {
        return NULL;
    }
    s = root->data;
    return s;
}

int server(int argc, char *argv[]){
    if (argc >= 2)
    {
        try
        {
            portNo = atoi(argv[1]);
            if ((portNo > 65535) || (portNo < 2000))
            {
                throw std::invalid_argument("Please enter a port number between 2000 - 65535");
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            portNo = htons(3000);
            std::cout << "Port :" << portNo << std::endl;
        }
    }
    else
    {
        portNo = htons(3000);
        std::cout << "Port :" << portNo << std::endl;
    }

    // create socket
    listenFd = socket(AF_INET, SOCK_STREAM, 0);

    if (listenFd < 0)
    {
        std::cerr << "Cannot open socket" << std::endl;
        return 0;
    }

    bzero((char *)&svrAdd, sizeof(svrAdd));

    svrAdd.sin_family = AF_INET;
    svrAdd.sin_addr.s_addr = INADDR_ANY;
    svrAdd.sin_port = htons(portNo);

    // bind socket
    if (bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0)
    {
        std::cerr << "Cannot bind" << std::endl;
        return 0;
    }

    if (listen(listenFd, 5) == -1)
    {
        printf("\n listen has failed\n");
        return 0;
    }
    return 1;
}