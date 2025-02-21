#include "../header/ft_ping.h"

double print_ip(struct icmphdr *recv_icmp_hdr, int seq, int ttl, double rtt, double variance){
    printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%.1f ms\n", 
                   sizeof(ping_parsing.packet) + sizeof(*recv_icmp_hdr), ping_parsing.infodest.ip,
                   recv_icmp_hdr->un.echo.sequence, ttl, (float)rtt);
            if (ping_parsing.stat.min == 0 || rtt < ping_parsing.stat.min)
                ping_parsing.stat.min = rtt;
            if (rtt > ping_parsing.stat.max)
                ping_parsing.stat.max = rtt;
            ping_parsing.stat.avg = (ping_parsing.stat.avg * (seq - 1) + rtt) / seq;
            variance = (variance * (seq - 1) + (rtt - ping_parsing.stat.avg) * (rtt - (ping_parsing.stat.avg - (rtt / seq)))) / seq;
            ping_parsing.stat.mdev = sqrt(variance);
            return variance;
}


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

void setup_packet(int seq, pid_t packet_id) {
    struct icmphdr *icmp_hdr;
    memset(ping_parsing.packet, 0, sizeof(ping_parsing.packet));
    icmp_hdr = (struct icmphdr *)ping_parsing.packet;
    icmp_hdr->type = ICMP_ECHO;
    icmp_hdr->code = 0;
    icmp_hdr->un.echo.id = packet_id;
    icmp_hdr->un.echo.sequence = seq;
    icmp_hdr->checksum = checksum(ping_parsing.packet, sizeof(ping_parsing.packet));
}

void init_socket() {
    int ttl_value = 64;
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    ping_parsing.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (ping_parsing.sockfd < 0) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(ping_parsing.sockfd, IPPROTO_IP, IP_TTL, &ttl_value, sizeof(ttl_value)) < 0) {
        perror("setsockopt error");
        close(ping_parsing.sockfd);
        exit(EXIT_FAILURE);
    }
    if (setsockopt(ping_parsing.sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout)) < 0) {
        perror("setsockopt error");
        close(ping_parsing.sockfd);
        exit(EXIT_FAILURE);
    }
}

void loop() {
    double variance = 0;
    struct sockaddr_in addr;
    struct timeval start, end;
    pid_t packet_id;
    
    init_socket();
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ping_parsing.infodest.ip);
    packet_id = getpid() & 0xFFFF;
    
    printf("FT_PING %s (%s) %ld data bytes", 
    ping_parsing.infodest.dns, ping_parsing.infodest.ip, sizeof(ping_parsing.packet));
    if (ping_parsing.option.isVerbose)
        printf(", id 0x%x = %d\n", packet_id, packet_id);
    else
        printf("\n");
    gettimeofday(&ping_parsing.start, NULL);
    
    while (true) {
        setup_packet(ping_parsing.nb_seq, packet_id);
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
            variance = print_ip(recv_icmp_hdr, ++ping_parsing.nb_seq, ttl, rtt, variance);  
        }
        sleep(1);
    }
}