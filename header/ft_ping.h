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
# include <math.h>

# define PACKET_SIZE 64

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
    float min;
    float max;
    float avg;
    double mdev;
}   t_ping_stat;


typedef struct s_parsing
{
    t_option    option;
    t_infodest  infodest;
    t_ping_stat stat;
    int         sockfd;
    char        *dest;
    int         nb_seq;
    char        packet[PACKET_SIZE];
    struct      timeval start, end;

}   t_parsing;

extern t_parsing ping_parsing;


void parser(int, char**);
void loop();
double print_ip(struct icmphdr *recv_icmp_hdr, int seq, int ttl, double rtt, double variance);
double print_dns(struct icmphdr *recv_icmp_hdr, int seq, int ttl, double rtt, double variance);

#endif