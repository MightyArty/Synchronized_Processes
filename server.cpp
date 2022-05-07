#include "server.hpp"

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

void free_stack(Stack **root)
{
    fd = open("file.txt", O_WRONLY);
    // locking
    if (fcntl(fd, F_SETLKW, &fl) == -1)
    {
        perror("fcntl");
        exit(1);
    }
    while (*root)
    {
        Stack *temp = *root;
        *root = (*root)->next;
        free(temp->data);
        delete temp;
    }

    *root = NULL;
    // unlocking
    fl.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLKW, &fl) == -1)
    {
        perror("fcntl");
        exit(1);
    }
    close(fd);
    std::cout << "free all allocate" << std::endl;
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
    stack->data = strcpy((char *)malloc(BUFFSIZE), data);
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
    fd = open("file.txt", O_WRONLY);
    // locking
    if (fcntl(fd, F_SETLKW, &fl) == -1)
    {
        perror("fcntl");
        exit(1);
    }
    Stack *temp = *root;
    *root = (*root)->next;
    // unlocking
    fl.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLKW, &fl) == -1)
    {
        perror("fcntl");
        exit(1);
    }
    close(fd);
    size--;
    return temp;
}

void push(Stack **root, char *data)
{
    fd = open("file.txt", O_WRONLY);
    // locking
    if (fcntl(fd, F_SETLKW, &fl) == -1)
    {
        perror("fcntl");
        exit(1);
    }
    printf("im here");
    size++;
    Stack *Stack = newNode(data);
    Stack->next = *root;
    *root = Stack;
    // unlocking
    fl.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLKW, &fl) == -1)
    {
        perror("fcntl");
        exit(1);
    }
    printf("im here too ");
    close(fd);
}

char *top(Stack *root)
{
    char *s;
    if (isEmpty(root))
    {
        return NULL;
    }
    fd = open("file.txt", O_WRONLY);
    // locking
    if (fcntl(fd, F_SETLKW, &fl) == -1)
    {
        perror("fcntl");
        exit(1);
    }
    s = root->data;
    // unlocking
    fl.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLKW, &fl) == -1)
    {
        perror("fcntl");
        exit(1);
    }
    close(fd);
    return s;
}

int server(int argc, char *argv[])
{
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

    if (listen(listenFd, 100) == -1)
    {
        printf("\n listen has failed\n");
        return 0;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    red();
    welcome();
    reset();
    signal(SIGINT, sig_handler);
    signal(SIGQUIT, sig_handler);

    // need to fix
    // root = mmap(0, sizeof(my_stack), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED | MAP_ANON, -1, 0);

    if (!server(argc, argv))
        return 0;
    int i = 0;
    while (1)
    {
        std::cout << "Listening" << std::endl;
        addr_size = sizeof(serverStorage);
        // this is where client connects. svr will hang in this mode until client conn
        int connFd = accept(listenFd, (struct sockaddr *)&serverStorage, &addr_size);
        int pid_c = 0;
        if (connFd < 0)
        {
            std::cerr << "Cannot accept connection" << std::endl;
            return 0;
        }
        else
        {
            std::cout << "Connection successful" << std::endl;
        }
        pid_c = fork();
        if (pid_c)
        {
            printf("if inside\n");
            printf("if %d", getpid());
            socketThread(connFd);
            exit(0);
        }
        else
        {
            printf("else inside\n");
            printf("else %d", getpid());

            pid_thr[i++] = pid_c;
            if (i >= 49)
            {
                i = 0;
                while (i < 50)
                    waitpid(pid_thr[i++], NULL, 0);
                i = 0;
            }
        }
        pid_c++;
    }
    if (size != 0)
        free_stack(&my_stack);
}

void socketThread(int clientSocket)
{
    int sock = clientSocket;
    printf("inside\n");
    printf("socketThread %d", getpid());

    std::cout << &my_stack << std::endl;
    while (true)
    {
        char *writer = 0;
        char reader[BUFFSIZE] = {0};
        bzero(reader, BUFFSIZE);

        if (read(sock, reader, BUFFSIZE) == -1)
            puts("error");

        if (strncmp(reader, "PUSH", 4) == 0)
        {
            puts("Pushed");
            push(&my_stack, reader + 5);
            send(sock, "Pushed", 6, 0);
        }

        else if (strncmp(reader, "POP", 3) == 0)
        {
            Stack *temp = pop(&my_stack);
            write(sock, (temp != NULL) ? temp->data : "Empty", (temp != NULL) ? sizeof(temp->data) : em);
            if (temp != NULL)
            {
                free(temp->data);
                delete temp;
            }
        }
        else if (strncmp(reader, "COUNT", 5) == 0)
        {
            int number = size;
            char numberArray[10] = {0};
            if (size != 0)
            {
                for (int n = log10(size) + 1, i = n - 1; i >= 0; --i, number /= 10)
                {
                    numberArray[i] = (number % 10) + '0';
                }
                write(sock, numberArray, 10);
            }
            else
            {
                write(sock, "0", 1);
            }
        }
        else if (strncmp(reader, "CLEAN", 5) == 0)
        {
            if (size != 0)
            {
                free_stack(&my_stack);
            }
            write(sock, "Clean stack succeeded", 21);
        }

        else if (strncmp(reader, "TOP", 3) == 0)
        {
            writer = top(my_stack);
            write(sock, (writer != NULL) ? writer : "Empty", (writer != NULL) ? sizeof(writer) : em);
        }
        else if (strncmp(reader, "EXIT", 4) == 0)
        {
            write(sock, "success", 4);
            close(sock);
            std::cout << "\n Closing connection and the lock" << std::endl;
            break;
        }
        else
        {
            write(sock, "(-1)", 4);
        }
    }
    return;
}