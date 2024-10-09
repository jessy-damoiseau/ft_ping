#include "../header/ft_ping.h"


unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

void setup_packet(int seq) {
    struct icmphdr *icmp_hdr;
    memset(ping_parsing.packet, 0, sizeof(ping_parsing.packet));
    icmp_hdr = (struct icmphdr *)ping_parsing.packet;
    icmp_hdr->type = ICMP_ECHO;
    icmp_hdr->code = 0;
    icmp_hdr->un.echo.id = getpid();
    icmp_hdr->un.echo.sequence = seq;
    icmp_hdr->checksum = checksum(ping_parsing.packet, sizeof(ping_parsing.packet));
}

void init_socket() {
    int TTL_VALUE = 64;
    ping_parsing.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (ping_parsing.sockfd < 0) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }
    setsockopt(ping_parsing.sockfd, IPPROTO_IP, IP_TTL, &TTL_VALUE, sizeof(TTL_VALUE));
    if (ping_parsing.option.isVerbose)
        printf("ft_ping: sock4.fd: %d (socktype: SOCK_RAW)\n", ping_parsing.sockfd);
}

void loop() {
    int seq = 0;
    double variance = 0;
    struct sockaddr_in addr;
    struct timeval start, end;

    init_socket();
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ping_parsing.infodest.ip);

    printf("FT_PING %s (%s) %d(%d) bytes of data.\n", 
    ping_parsing.infodest.dns, ping_parsing.infodest.ip, PACKET_SIZE,
    PACKET_SIZE + (int)sizeof(struct iphdr));
    gettimeofday(&ping_parsing.start, NULL);
    while (true) {
        setup_packet(seq++);
        gettimeofday(&start, NULL);
        if (sendto(ping_parsing.sockfd, ping_parsing.packet, sizeof(ping_parsing.packet), 0, (struct sockaddr *)&addr, sizeof(addr)) <= 0) {
            perror("sendto error");
        }
        ping_parsing.stat.transmitted++;
        struct sockaddr_in r_addr;
        socklen_t addr_len = sizeof(r_addr);
        int ttl;
        char recv_packet[1024];
        struct icmphdr *recv_icmp_hdr;
        if (recvfrom(ping_parsing.sockfd, recv_packet, sizeof(recv_packet), 0, (struct sockaddr *)&r_addr, &addr_len) > 0) {
            ping_parsing.stat.received++;
            gettimeofday(&end, NULL);
            double rtt = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
            recv_icmp_hdr = (struct icmphdr *)(recv_packet + (sizeof(struct iphdr)));
            struct iphdr *ip_hdr = (struct iphdr *)recv_packet;
            ttl = ip_hdr->ttl;
            if (!strcmp(ping_parsing.infodest.dns, ping_parsing.infodest.ip))
                variance = print_ip(recv_icmp_hdr, seq, ttl, rtt, variance);
            else
                variance = print_dns(recv_icmp_hdr, seq, ttl, rtt, variance);   
        }
        sleep(1);
    }
}