#include "../header/ft_ping.h"

void help(){
    printf("\nUsage\n  ft_ping [OPTION...] HOST ...\n");
    printf("\nSend ICMP ECHO_REQUEST packets to network hosts.\n");
    printf("\nOptions:\n");
    printf("  <destination>  DNS name or IP address\n");
    printf("  -v             verbose output\n");
    printf("  -h             give this help list\n");
    printf("  -u             give a short usage message\n");
}

void usage(){
    printf("Usage: ft_ping [-huv] HOST ...\n");
    printf("Try 'ft_ping -h' for more information.\n");
}

void resolve_dns(){
    struct hostent *host;
    struct in_addr **addr_list;

    host = gethostbyname(ping_parsing.dest);
    if (host == NULL){
        printf("ping: unknown host");
        exit(EXIT_FAILURE);
    }
    addr_list = (struct in_addr **)host->h_addr_list;
    ping_parsing.infodest.ip = inet_ntoa(*addr_list[0]);
    ping_parsing.infodest.dns = ping_parsing.dest;
}

void parser(int ac, char **av){
    int opt;
    while ((opt = getopt(ac, av, "vhu")) != -1){
        switch (opt){
            case 'v':
                ping_parsing.option.isVerbose = true;
                break;
            case 'h':
                help();
                exit(EXIT_SUCCESS);
            case 'u':
                usage();
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