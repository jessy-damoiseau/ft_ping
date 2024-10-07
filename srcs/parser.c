#include "../header/ft_ping.h"

void help(){
    printf("\nUsage\n  ft_ping [options] <destination>\n");
    printf("\nOptions:\n");
    printf("  <destination>  DNS name or IP address\n");
    printf("  -v             Verbose output\n");
    printf("  -h             Print help\n");
}

void resolve_dns(){
    struct hostent *host;
    struct in_addr **addr_list;

    host = gethostbyname(ping_parsing.dest);
    if (host == NULL){
        printf("ping: connect: Network is unreachable");
        exit(EXIT_FAILURE);
    }
    // printf("%s\n", host->h_aliases);
    addr_list = (struct in_addr **)host->h_addr_list;
    ping_parsing.infodest.ip = inet_ntoa(*addr_list[0]);
    ping_parsing.infodest.dns = ping_parsing.dest;
}

void parser(int ac, char **av){
    int opt;
    while ((opt = getopt(ac, av, "vh")) != -1){
        switch (opt){
            case 'v':
                ping_parsing.option.isVerbose = true;
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
    ping_parsing.dest = av[optind];
    resolve_dns();
}