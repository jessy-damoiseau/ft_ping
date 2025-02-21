#include "../header/ft_ping.h"

t_parsing ping_parsing;

void sigint_handler(__attribute__((unused))int sig){
    gettimeofday(&ping_parsing.end, NULL);
    printf("\n--- %s ping statistics ---\n", ping_parsing.infodest.dns);
    printf("%d packets transmitted, %d received, %.1f%% packet loss\n",
            ping_parsing.stat.transmitted, ping_parsing.stat.received, 
            ((ping_parsing.stat.transmitted - ping_parsing.stat.received) /
            (float)ping_parsing.stat.transmitted) * 100.0);
    if (ping_parsing.nb_seq != 0)
        printf("round-trip min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
        ping_parsing.stat.min, ping_parsing.stat.avg,
        ping_parsing.stat.max, ping_parsing.stat.mdev);

    close(ping_parsing.sockfd);
    exit(EXIT_SUCCESS);
}

void init_struct(){
    ping_parsing.option.isVerbose = false;
    ping_parsing.nb_seq = 0;
    ping_parsing.stat.transmitted = 0;
    ping_parsing.stat.received = 0;
    ping_parsing.stat.min = 0;
    ping_parsing.stat.max = 0;
    ping_parsing.stat.avg = 0;
    ping_parsing.stat.mdev = 0;
}


int main(int ac, char **av){
    init_struct();
    signal(SIGINT, &sigint_handler);
    parser(ac, av);
    loop();
}