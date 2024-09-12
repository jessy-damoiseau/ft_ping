# ifndef FT_PING_H

# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>
# include <getopt.h>
# include <stdlib.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <string.h>


typedef struct s_option
{
    bool isVerbose;
}   t_option;

typedef struct s_infodest
{
    char    *ip;
    char    *dns;
}   t_infodest;



typedef struct s_parsing
{
    t_option    option;
    t_infodest  infodest;
    char        *dest;


}   t_parsing;


void parser(int, char**, t_parsing*);

#endif