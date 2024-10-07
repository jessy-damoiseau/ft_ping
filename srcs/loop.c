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

void setup_packet(t_ping_pkt *packet, time_t timestamp) {
    packet->timestamp = timestamp;
    packet->hdr.icmp_type = ICMP_ECHO;
    packet->hdr.icmp_code = 0;
    packet->hdr.icmp_id = getpid();
    packet->hdr.icmp_seq = 0;
    packet->hdr.icmp_cksum = 0;
    packet->hdr.icmp_cksum = checksum(packet, sizeof(struct icmp) + DATA_PKT_SIZE);
}

void loop(){
    struct sockaddr_in addr;
    t_ping_pkt packet;
    struct timeval timestamp;
    int sockfd;
    

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        perror("Erreur de création de la socket");
        return ;
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ping_parsing.infodest.ip);

    printf("FT_PING %s (%s) %zu(%ld) bytes of data.\n",
            ping_parsing.infodest.dns, ping_parsing.infodest.ip,
            DATA_PKT_SIZE + sizeof(struct ip),
            DATA_PKT_SIZE
    );
    while (true)
    {
        if (gettimeofday(&timestamp, NULL) < 0) {
            perror("Erreur de récupération du temps");
            return ;
        }
        setup_packet(&packet, timestamp.tv_sec);
        if (sendto(sockfd, &packet, sizeof(struct icmp) + DATA_PKT_SIZE, 0, (struct sockaddr *)&addr, sizeof(addr)) <= 0) {
            perror("Erreur d'envoi du paquet");
        } else {
            gettimeofday(&timestamp, NULL);
            printf("%ld bytes from %s: icmp_seq=%d ttl=64\n",
                    DATA_PKT_SIZE + sizeof(struct ip),
                    ping_parsing.infodest.ip,
                    ping_parsing.nb_seq);
            ping_parsing.stat.transmitted++;
            ping_parsing.stat.received++;
            // printf("Envoi du paquet à %s\n", inet_ntoa(addr.sin_addr));
        }
        sleep(1);
        ping_parsing.nb_seq++;
    }

}