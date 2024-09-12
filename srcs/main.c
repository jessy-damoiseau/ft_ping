#include "../header/ft_ping.h"


void init_struct(t_parsing *ping_parsing){
    ping_parsing->option.isVerbose = false;
}


int main(int ac, char **av){
    t_parsing ping_parsing;
    init_struct(&ping_parsing);
    parser(ac, av, &ping_parsing);
}