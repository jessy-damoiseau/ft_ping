#include "../header/ft_ping.h"


t_parsing ping_parsing;

void sigint_handler(__attribute__((unused))int sig){
    printf("\n--- %s ping statistics ---\n", ping_parsing.infodest.dns);
    printf("%d packets transmitted, %d received, %f%% packet loss, time %dms\n",
            ping_parsing.stat.transmitted, ping_parsing.stat.received, ping_parsing.stat.loss_pourcentage, 0);
    exit(EXIT_SUCCESS);
}

void init_struct(){
    ping_parsing.option.isVerbose = false;
    ping_parsing.nb_seq = 0;
}


int main(int ac, char **av){
    init_struct();
    signal(SIGINT, &sigint_handler);
    parser(ac, av);
    loop();
}