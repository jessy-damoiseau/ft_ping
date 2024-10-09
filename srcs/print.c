#include "../header/ft_ping.h"

double print_ip(struct icmphdr *recv_icmp_hdr, int seq, int ttl, double rtt, double variance){
    printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%.1f ms\n", 
                   sizeof(ping_parsing.packet), ping_parsing.infodest.ip,
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

double print_dns(struct icmphdr *recv_icmp_hdr, int seq, int ttl, double rtt, double variance){
    printf("%ld bytes from %s (%s): icmp_seq=%d ttl=%d time=%.1f ms\n", 
                   sizeof(ping_parsing.packet), ping_parsing.infodest.dns, ping_parsing.infodest.ip,
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
