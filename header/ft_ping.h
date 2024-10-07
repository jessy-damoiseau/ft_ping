# ifndef FT_PING_H

# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>
# include <getopt.h>
# include <stdlib.h>
# include <arpa/inet.h>
# include <netinet/ip_icmp.h>
# include <netdb.h>
# include <string.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <signal.h>

# define PKT_SIZE 84
# define DATA_PKT_SIZE (PKT_SIZE - sizeof(struct icmp) - sizeof(time_t))

typedef struct s_option
{
    bool isVerbose;
}   t_option;

typedef struct s_infodest
{
    char    *ip;
    char    *dns;
}   t_infodest;

typedef struct s_ping_stat{
    int transmitted;
    int received;
    float loss_pourcentage;
    float min;
    float max;
    float avg;
    float mdev;
}   t_ping_stat;


typedef struct s_parsing
{
    t_option    option;
    t_infodest  infodest;
    t_ping_stat stat;
    char        *dest;
    int         nb_seq;


}   t_parsing;

extern t_parsing ping_parsing;

typedef struct s_ping_pkt{
    struct icmp hdr;
    time_t timestamp;
    char data[DATA_PKT_SIZE];
}  t_ping_pkt;


void parser(int, char**);
void loop();

#endif