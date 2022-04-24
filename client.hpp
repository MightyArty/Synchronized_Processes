#include <string.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>
#define BUFFSIZE 1024
using namespace std;
char wirter[BUFFSIZE];
char reader[BUFFSIZE];
int sockFd;
/**
 * @brief 
 *
 * @return none.
 */
void blue();
/**
 * @brief 
 *
 * @return none.
 */
void red();
/**
 * @brief 
 *
 * @return none.
 */
void yellow();
/**
 * @brief 
 *
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
int client(int argc, char *argv[]);
void welcom();