#include "../header/ft_ping.h"


volatile struct s_parsing *ping_parsing;

void init_struct(){
    ping_parsing->option.isVerbose = false;
    ping_parsing->nb_seq = 0;
}


int main(int ac, char **av){
    printf("check");
    init_struct();
    printf("check0");
    parser(ac, av);
    loop();
}