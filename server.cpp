#include "server.hpp"
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
/**
 * @brief The functions welcome,red,yellow,blue,green and reset are just for fun
 * We want you to enjoy the proccess :)
 */
void welcome()
{
    red();
    printf("    $$      $$$$$  $$$$$$$$$ $     $         ");
    blue();
    printf(" $$$$$         $$     $$$$$       $$      $  $    \n");
    red();
    printf("   $  $     $   $      $      $   $          ");
    blue();
    printf(" $   $        $  $    $   $      $  $     $ $    \n");
    red();
    printf("  $ -- $    $$$$$      $        $    ");
    yellow();
    printf(" @@@@@@ ");
    blue();
    printf(" $$$$$$$     $ -- $   $$$$$     $ -- $    $$        \n");
    red();
    printf(" $      $   $    $     $        $            ");
    blue();
    printf(" $     $    $      $  $    $   $      $   $ $         \n");
    red();
    printf("$        $  $     $    $        $            ");
    blue();
    printf(" $$$$$$$   $        $ $     $ $        $  $  $       \n");
}

void free_stack(struct info_Stack **info_mmap)
{

    while ((*info_mmap)->head_address != NULL)
    {
        Stack *temp = (*info_mmap)->head_address;
        (*info_mmap)->head_address = (*info_mmap)->head_address->next;
        munmap(temp, sizeof(Stack *));
    }
    munmap((*info_mmap)->size, sizeof(int *));
    munmap(*info_mmap, sizeof(struct info_Stack *));

    std::cout << "free all allocate" << std::endl;
}
void sig_handler(int signum)
{
    free_stack(&global_info);
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
        kill(getppid(), SIGTSTP);
        exit(1);
    }
}
int isEmpty(struct info_Stack **info_mmap)
{
    return (*info_mmap)->head_address == NULL;
}
Stack *pop(info_Stack **mmap_info)
{
    // fd = open("file.txt", O_WRONLY);
    fl.l_type = F_WRLCK;
    if (fcntl(fd, F_SETLKW, &fl) == -1)
    {
        perror("fcntl");
        exit(1);
    }
    if ((*mmap_info)->head_address == NULL)
    {
        return NULL;
    }

    Stack *temp = (*mmap_info)->head_address;
    (*mmap_info)->head_address = (*mmap_info)->head_address->next;
    (*(*mmap_info)->size)--;
    fl.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLKW, &fl) == -1)
    {
        perror("fcntl");
        exit(1);
    }
    close(fd);
    return temp;
}
void push(info_Stack **mmap_info, char *data)
{
    // fd = open("file.txt", O_WRONLY);
    fl.l_type = F_WRLCK;
    if (fcntl(fd, F_SETLKW, &fl) == -1)
    {
        perror("fcntl");
        exit(1);
    }
    if ((*mmap_info)->head_address == NULL)
    {
        (*mmap_info)->head_address = (*mmap_info)->mmap_address;
        strcpy((*mmap_info)->head_address->data, data);
        (*mmap_info)->head_address->next = NULL;
    }
    else
    {

        Stack *temp = (*mmap_info)->head_address + 1;
        strcpy(temp->data, data);
        temp->next = (*mmap_info)->head_address;
        (*mmap_info)->head_address = temp;
    }

    (*(*mmap_info)->size) += 1;
    fl.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLKW, &fl) == -1)
    {
        perror("fcntl");
        exit(1);
    }
    close(fd);
}
char *top(struct info_Stack **info)
{
    // fd = open("file.txt", O_WRONLY);
    fl.l_type = F_WRLCK;
    if (fcntl(fd, F_SETLKW, &fl) == -1)
    {
        perror("fcntl");
        exit(1);
    }
    if ((*(info))->head_address == NULL)
    {
        return NULL;
    }
    struct Stack *temp = (*info)->head_address;
    return temp->data;
    fl.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLKW, &fl) == -1)
    {
        perror("fcntl");
        exit(1);
    }
    close(fd);
}
int server(int argc, char *argv[])
{

    if (argc >= ARG_N)
    {
        try
        {
            portNo = atoi(argv[1]);
            if ((portNo > MAX_PORT) || (portNo < MIN_PORT))
            {
                throw std::invalid_argument("Please enter a port number between 2000 - 65535");
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            portNo = htons(PORT);
            std::cout << "Port :" << portNo << std::endl;
        }
    }
    else
    {
        portNo = htons(PORT);
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

    if (listen(listenFd, MAX_CLIENT) == -1)
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
    signal(SIGTSTP, sig_handler);
    signal(SIGQUIT, sig_handler);
    if (!server(argc, argv))
    {
        return 0;
    }

    struct info_Stack *mmap_info = (struct info_Stack *)mmap(NULL, sizeof(struct info_Stack *), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    mmap_info->mmap_address = (struct Stack *)mmap(NULL, MAX_STACK * sizeof(struct Stack) + 1, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    mmap_info->size = (int *)mmap(NULL, sizeof(int *), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    (*mmap_info->size) = 0;
    global_info = mmap_info;

    int i = 0;
    pid_t pid[50];
    int pid_c = 0;
    int count = 0;
    char ch;
    while (1)
    {
        count++;
        if (count == 4)
        {
            break;
        }
        std::cout << "Listening" << std::endl;
        len = sizeof(clntAdd);
        // this is where client connects. svr will hang in this mode until client conn
        int connFd = accept(listenFd, (struct sockaddr *)&clntAdd, &len);

        if (connFd < 0)
        {
            std::cerr << "Cannot accept connection" << std::endl;
        }
        else
        {
            std::cout << "Connection successful" << std::endl;
        }
        if ((pid_c = fork()) != 0)
        {
            task1(connFd, getpid(), &mmap_info);
        }
        else
        {
            pid[i++] = pid_c;
            if (i >= MAX_CLIENT - 1)
            {
                i = 0;
                while (i < MAX_CLIENT)
                    waitpid(pid[i++], NULL, 0);
                i = 0;
            }
        }
    }
    while (i < MAX_CLIENT)
        waitpid(pid[i++], NULL, 0);
    for (int i = 0; i < MAX_CLIENT; i++)
    {
        kill(pid[i], 0);
    }
    if ((*mmap_info->size) != 0)
    {
        free_stack(&mmap_info);
    }
    puts("finish");
    exit(1);
    return 1;
}
void task1(int sock, pid_t process_pid, struct info_Stack **front)
{
    while (true)
    {
        fd = open("file.txt", O_WRONLY | O_CREAT);
        if (fd == -1)
        {
            printf("error in opening file\n");
            perror("file");
        }

        char *writer = 0;
        char reader[BUFFSIZE] = {0};
        bzero(reader, BUFFSIZE);
        if (read(sock, reader, BUFFSIZE) == -1)
        {
            puts("error");
        }
        if (strncmp(reader, "PUSH", 4) == 0)
        {
            puts("Pushed");
            push(front, reader + 5);
            send(sock, "Pushed", 6, 0);
        }
        else if (strncmp(reader, "POP", 3) == 0)
        {
            Stack *temp = pop(front);
            write(sock, (temp != NULL) ? temp->data : "Empty", (temp != NULL) ? sizeof(temp->data) : em);
        }
        else if (strncmp(reader, "TOP", 3) == 0)
        {
            writer = top(front);
            write(sock, (writer != NULL) ? writer : "Empty", (writer != NULL) ? sizeof(writer) : em);
        }
        else if (strncmp(reader, "COUNT", 5) == 0)
        {
            // fd = open("file.txt", O_WRONLY);
            fl.l_type = F_WRLCK;
            if (fcntl(fd, F_SETLKW, &fl) == -1)
            {
                perror("fcntl");
                exit(1);
            }
            int number = *(*front)->size;
            std::cout << number << std::endl;
            char numberArray[10] = {0};
            if (number != 0)
            {
                for (int n = log10(number) + 1, i = n - 1; i >= 0; --i, number /= 10)
                {
                    numberArray[i] = (number % 10) + '0';
                }
                write(sock, numberArray, 10);
            }
            else
            {
                write(sock, "0", 1);
            }
            fl.l_type = F_UNLCK;
            if (fcntl(fd, F_SETLKW, &fl) == -1)
            {
                perror("fcntl");
                exit(1);
            }
            close(fd);
        }
        else if (strncmp(reader, "CLEAN", 5) == 0)
        {
            (*front)->head_address = NULL;
            write(sock, "Clean stack succeeded", 21);
        }
        else if (strncmp(reader, "EXIT", 4) == 0)
        {
            write(sock, "succ", 4);
            close(sock);
            std::cout << "\nClosing thread and connection" << std::endl;
            break;
        }
        else if (strncmp(reader, "FRONT", 4) == 0)
        {
            std::cout << "Front " << (*(front))->head_address << std::endl;
            write(sock, "front", 5);
        }
        else
        {
            write(sock, "(-1)", 4);
        }
    }
    exit(1);
    return;
}
