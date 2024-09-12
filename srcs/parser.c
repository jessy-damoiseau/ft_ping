#include "../header/ft_ping.h"

void help(){
    printf("\nUsage\n  ft_ping [options] <destination>\n");
    printf("\nOptions:\n");
    printf("  <destination>  DNS name or IP address\n");
    printf("  -v             Verbose output\n");
    printf("  -h             Print help\n");
}

void parser(int ac, char **av, t_parsing *ping_parsing){
    int opt;

    while ((opt = getopt(ac, av, "vh")) != -1){
        switch (opt){
            case 'v':
                ping_parsing->option.isVerbose = true;
                break;
            case 'h':
                help();
                exit(EXIT_SUCCESS);
            default:
                help();
                exit(EXIT_FAILURE);
                break;
        }
    }

    if (ac - optind != 1){
        help();
        exit(EXIT_FAILURE);
    }
    ping_parsing->dest = av[optind];
}