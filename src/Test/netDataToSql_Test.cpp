/*
 * netDataToSql_Test.cpp
 *
 * Sniffer example of TCP/IP packet capture using libpcap.
 *
 ****************************************************************************
 *
 * Example compiler command-line for GCC:

 *   g++  -o netDataToSql_Test.o $(mysql_config --cflags) netDataToSql_Test.cpp -lpcap $(mysql_config --libs)
 ****************************************************************************
 *
 *
 ****************************************************************************
 *
 */

#define APP_NAME                "netDataToSql_Test.cpp"
#define APP_DESC                "Sniffer example using libpcap/Made by cjqhenry"

#include <pcap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
///----------
#include <mysql.h>  
#include <string>  
#include <sstream>  
///-----------
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
///
using namespace std;

#define ETH_NAME	"eth0"
#define dataBaseName "myIDS"

char  myip[16];

/* 默认捕获长度 (每个包捕获的最大长度)
   default snap length (maximum bytes per packet to capture) */
#define SNAP_LEN 1518

/* 以太网头部14个字节
   ethernet headers are always exactly 14 bytes [1] */
#define SIZE_ETHERNET 14

/* 以太网地址6个字节
   Ethernet addresses are 6 bytes */
#define ETHER_ADDR_LEN        6
//=========
#define PACKETS_NUM   2000
#define TCP_FLAG   0
#define UDP_FLAG   1

/* UDP header */
struct sniff_udp
{
    uint16_t sport;       /* source port */
    uint16_t dport;       /* destination port */
    uint16_t udp_length;
    uint16_t udp_sum;     /* checksum */
};
//=======

/* Ethernet header */
struct sniff_ethernet {
    u_char  ether_dhost[ETHER_ADDR_LEN];    /* destination host address */
    u_char  ether_shost[ETHER_ADDR_LEN];    /* source host address */
    u_short ether_type;                     /* IP? ARP? RARP? etc */
};

/* IP header */
struct sniff_ip {
    u_char  ip_vhl;                 /* version << 4 | header length >> 2 */
    u_char  ip_tos;                 /* type of service */
    u_short ip_len;                 /* total length */
    u_short ip_id;                  /* identification */
    u_short ip_off;                 /* fragment offset field */
#define IP_RF 0x8000            /* reserved fragment flag */
#define IP_DF 0x4000            /* dont fragment flag */
#define IP_MF 0x2000            /* more fragments flag */
#define IP_OFFMASK 0x1fff       /* mask for fragmenting bits */
    u_char  ip_ttl;                 /* time to live */
    u_char  ip_p;                   /* protocol */
    u_short ip_sum;                 /* checksum */
    struct  in_addr ip_src,ip_dst;  /* source and dest address */
};
#define IP_HL(ip)               (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)                (((ip)->ip_vhl) >> 4)

/* TCP header */
typedef unsigned long tcp_seq;

struct sniff_tcp {
    u_short th_sport;               /* source port */
    u_short th_dport;               /* destination port */
    tcp_seq th_seq;                 /* sequence number */
    tcp_seq th_ack;                 /* acknowledgement number */

    u_char  th_offx2;               /* data offset, rsvd */
#define TH_OFF(th)      (((th)->th_offx2 & 0xf0) >> 4)
    u_char  th_flags;
#define TH_FIN  0x01
#define TH_SYN  0x02
#define TH_RST  0x04
#define TH_PUSH 0x08
#define TH_ACK  0x10
#define TH_URG  0x20
#define TH_ECE  0x40
#define TH_CWR  0x80
#define TH_FLAGS        (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
    u_short th_win;                 /* window */
    u_short th_sum;                 /* checksum */
    u_short th_urp;                 /* urgent pointer */
};

///--------------


MYSQL *my_conn;
int tcp_num_count;  
int udp_num_count;  

string
string_to_hex(unsigned char * str, int len);

void
insert_tcp_hex_mysql(int, string, string, int, int,   unsigned char*, unsigned long, int, int);


void connectMysql();
///--------------
void
got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

void
print_payload(const u_char *payload, int len);

void
print_hex_ascii_line(const u_char *payload, int len, int offset);

void
print_app_banner(void);

void
print_app_usage(void);

//------------------------------
char* getMyip()
{
    int sock;
    struct sockaddr_in sin;
    struct ifreq ifr;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        perror("socket");
        return NULL;
    }

    strncpy(ifr.ifr_name, ETH_NAME, IFNAMSIZ);
    ifr.ifr_name[IFNAMSIZ - 1] = 0;

    if (ioctl(sock, SIOCGIFADDR, &ifr) < 0)
    {
        perror("ioctl");
        return NULL;
    }

    memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
    //printf("eth0: %s\n", inet_ntoa(sin.sin_addr));
    char * ipaddr = (char *) malloc (sizeof(char)*16);
    strcpy(ipaddr,inet_ntoa(sin.sin_addr));

    return ipaddr;
}

//------------------------------

/*
 * app name/banner
 */
void
print_app_banner(void)
{

    printf("%s - %s\n", APP_NAME, APP_DESC);
    printf("\n");

    return;
}

/*
 * print help text
 */
void
print_app_usage(void)
{

    printf("Usage: %s [interface]\n", APP_NAME);
    printf("\n");
    printf("Options:\n");
    printf("    interface    Listen on <interface> for packets.\n");
    printf("\n");

    return;
}

/*
 * print data in rows of 16 bytes: offset   hex   ascii
 *
 * 00000   47 45 54 20 2f 20 48 54  54 50 2f 31 2e 31 0d 0a   GET / HTTP/1.1..
 */
void
print_hex_ascii_line(const u_char *payload, int len, int offset)
{

    int i;
    int gap;
    const u_char *ch;

    /* offset */
    printf("%05d   ", offset);

    /* hex */
    ch = payload;
    for(i = 0; i < len; i++) {
        printf("%02x ", *ch);
        ch++;
        /* print extra space after 8th byte for visual aid */
        if (i == 7)
            printf(" ");
    }
    /* print space to handle line less than 8 bytes */
    if (len < 8)
        printf(" ");

    /* fill hex gap with spaces if not full line */
    if (len < 16) {
        gap = 16 - len;
        for (i = 0; i < gap; i++) {
            printf("   ");
        }
    }
    printf("   ");

    /* ascii (if printable) */
    ch = payload;
    for(i = 0; i < len; i++) {
        if (isprint(*ch))
            printf("%c", *ch);
        else
            printf(".");
        ch++;
    }

    printf("\n");

    return;
}

/*
 * 打印包的有效载荷数据（避免打印二进制数据）
 * print packet payload data (avoid printing binary data)
 */
void
print_payload(const u_char *payload, int len)
{

    int len_rem = len;
    int line_width = 16;                        /* 每行的字节数 | number of bytes per line */
    int line_len;
    int offset = 0;                                        /* 从0开始的偏移计数器 | zero-based offset counter */
    const u_char *ch = payload;

    if (len <= 0)
        return;

    /* data fits on one line */
    if (len <= line_width) {
        print_hex_ascii_line(ch, len, offset);
        return;
    }

    /* 数据跨越多行 data spans multiple lines */
    for ( ;; ) {
        /* 计算当前行的长度 | compute current line length */
        line_len = line_width % len_rem;

        /* 显示分割线 | print line */
        print_hex_ascii_line(ch, line_len, offset);

        /* 计算总剩余 | compute total remaining */
        len_rem = len_rem - line_len;

        /* 转移到打印的剩余字节的指针
                   shift pointer to remaining bytes to print */
        ch = ch + line_len;

        /* 添加偏移 | add offset */
        offset = offset + line_width;

        /* 检查是否有线宽字符或更少
                   check if we have line width chars or less */
        if (len_rem <= line_width) {
            /* print last line and get out */
            print_hex_ascii_line(ch, len_rem, offset);
            break;
        }
    }

    return;
}

void
insert_tcp_hex_mysql(int type, string src_ip, string dst_ip, int src_port, int dst_port,   unsigned char *payload, unsigned long seq, int len,int fin)
{

    int i;
    std::stringstream   stream;
    string  stype, ssrc_port, sdst_port, sseq,slen ,sack;
    string insert_sql;
    string sfin;

    stream << type;
    stype=stream.str();
    stream.str("");

    stream << src_port;
    ssrc_port=stream.str();
    stream.str("");

    stream << dst_port;
    sdst_port=stream.str();
    stream.str("");

    stream << seq;
    sseq=stream.str();
    stream.str("");

    stream << len;
    slen=stream.str();
    stream.str("");

    stream << fin;
    sfin=stream.str();
    stream.str("");

    unsigned char *ch;
    string strHexContent;

    strHexContent=string_to_hex(payload,len);
    /* hex */
//    ch = payload;
//    for(i = 0; i < len; i++) {
//        if (isprint(*ch))
//        {
//            //printf("%c", *ch);
//            if(*ch!=34&&*ch!=39)
//                str.push_back(*ch);
//        }

//        ch++;
//    }

    //IMPORTANT: in the mysql, ch is isprintable and no ' or ".

    insert_sql ="INSERT INTO pcapData VALUES( NULL, '"+ stype +"', '"+  src_ip +"', '"+  dst_ip +"', '"+  ssrc_port +"', '"+ sdst_port +"', '"+  strHexContent +"', '"+ sseq +"','"+ slen +"','"+ sfin +"');";

    int res =mysql_real_query( my_conn , insert_sql.c_str() , insert_sql.length() );

    if (!res)
    {
        tcp_num_count++;
        printf("Inserted %lu rows\n", (unsigned long)mysql_affected_rows(my_conn));
    }
    else
    {
        fprintf(stderr, "Insert error %d: %s\n", mysql_errno(my_conn),
                mysql_error(my_conn));
    }


    return;
}
/*
 * 解析/显示 包
 * dissect/print packet
 */
void
got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{

    static int count = 1;                   /* 包计数器                | packet counter */


    /* declare pointers to packet headers */
    struct sniff_ethernet *ethernet;  /* 以太网头部        | The ethernet header [1] */
    struct sniff_ip *ip;              /* IP 头部                | The IP header */
    struct sniff_tcp *tcp;            /* TCP 头部                | The TCP header */

    unsigned char *payload;                    /* Packet payload */

    int size_ip;
    int size_tcp;
    int size_payload;
    int proto_flag=2;// 0=TCP_FLAG; 1=UDP_FLAG
    struct sniff_udp *udp;            /* UDP 头部                | The UDP header */
    int size_udp;

    //====
    /* 显示包总数 */
    printf("\nPacket number %d:\n", count);
    count++;

    /* 定义以太网头部
           define ethernet header */
    ethernet = (struct sniff_ethernet*)(packet);

    /* 定义/计算 IP 头部偏移
           define/compute ip header offset */
    ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
    size_ip = IP_HL(ip)*4;
    if (size_ip < 20) {
        printf("   * Invalid IP header length: %u bytes\n", size_ip);
        return;
    }

    /* 显示源IP和目的IP
           print source and destination IP addresses */

    ///only print internet->me infoermation

    if(strcmp(inet_ntoa(ip->ip_src),myip)==0)
        return;


    /* 确定协议
           determine protocol */
    switch(ip->ip_p) {
    case IPPROTO_TCP://useful
        printf("   Protocol: TCP\n");
        proto_flag=0;
        break;

    case IPPROTO_UDP://useful
        printf("   Protocol: UDP\n");
        proto_flag=1;
        break;

    case IPPROTO_ICMP://useless
        printf("   Protocol: ICMP\n");
        return;

    case IPPROTO_IP: //useless
        printf("   Protocol: IP\n");
        return;

    default:
        printf("   Protocol: unknown\n");
        return;
    }

    /*
   *  -------------------      This packet is TCP. -------------------
   */
    if (proto_flag == 0)
    {
        /* define/compute tcp header offset */
        tcp = (struct sniff_tcp *) (packet + SIZE_ETHERNET + size_ip);
        size_tcp = TH_OFF (tcp) * 4;
        if (size_tcp < 20)
        {
            printf ("   * Invalid TCP header length: %u bytes\n", size_tcp);
            return;
        }

        printf("       From: %s\n", inet_ntoa(ip->ip_src));
        printf("         To: %s\n", inet_ntoa(ip->ip_dst));
        printf ("   Src port  : %d\n", ntohs (tcp->th_sport));
        printf ("   Dst port  : %d\n", ntohs (tcp->th_dport));
        printf ("   Seq number: %d\n", ntohl (tcp->th_seq));
        int fin=0;
        if(tcp->th_flags & TH_FIN)
            fin=1;
        printf ("   FIN       : %d\n", fin);



        /* define/compute tcp payload (segment) offset */
        payload = (unsigned char *) (packet + SIZE_ETHERNET + size_ip + size_tcp);

        /* compute tcp payload (segment) size */
        size_payload = ntohs (ip->ip_len) - (size_ip + size_tcp);
        printf ("  TCP size_payload: %d\n", size_payload);

        if (size_payload > 0)
        {
            //printf ("   Payload (%d bytes):\n", size_payload);
            insert_tcp_hex_mysql(0, inet_ntoa(ip->ip_src), myip, ntohs (tcp->th_sport),
                                 ntohs (tcp->th_dport), payload, ntohl (tcp->th_seq), size_payload,fin );
            //  print_payload(payload, size_payload);

        }

    }               //end tcp


    //=====================================================================================

    /*
   *   -------------------    This packet is UDP. -------------------
   */
    else if (proto_flag == 1)
    {
        /* define/compute udp header offset */
        udp = (struct sniff_udp *) (packet + SIZE_ETHERNET + size_ip);
        printf("       From: %s\n", inet_ntoa(ip->ip_src));
        printf("         To: %s\n", inet_ntoa(ip->ip_dst));
        printf ("   Src port: %d\n", ntohs (udp->sport));
        printf ("   Dst port: %d\n", ntohs (udp->dport));

        /* define/compute udp payload (segment) offset */
        payload = (unsigned char *) (packet + SIZE_ETHERNET + size_ip + 8);
        size_payload = ntohs (ip->ip_len) - (size_ip + 8);
        printf ("  UDP size_payload: %d\n", size_payload);
        /*
       * Print payload data; it might be binary, so don't just
       * treat it as a string.
       */
        if (size_payload > 0)
        {
            print_payload(payload, size_payload);
            int i;
            std::stringstream   stream;
            string  ssrc_port, sdst_port, slen;
            string insert_sql;

            string stype="1";
            string sseq="0";

            stream << ntohs (udp->sport);
            ssrc_port=stream.str();
            stream.str("");

            stream << ntohs (udp->dport);
            sdst_port=stream.str();
            stream.str("");

            stream << size_payload;
            slen=stream.str();
            stream.str("");

            unsigned char *ch;

            string strHexContent;

            strHexContent=string_to_hex(payload,size_payload);

//            ch = payload;
//            for(i = 0; i < size_payload; i++) {
//                if (isprint(*ch))
//                {
//                    //printf("%c", *ch);
//                    str.push_back(*ch);
//                }

//                ch++;
//            }

            insert_sql ="INSERT INTO pcapData VALUES( NULL, '"+ stype +"', '"+ inet_ntoa(ip->ip_src) +"', '"+ myip +"', '"+  ssrc_port +"', '"+ sdst_port +"', '"+  strHexContent +"', '"+ sseq +"', '"+ slen +"','"+ sseq +"');";

            int res =mysql_real_query( my_conn , insert_sql.c_str() , insert_sql.length() );

            if (!res)
            {
                udp_num_count++;
                printf("Inserted %lu rows\n", (unsigned long)mysql_affected_rows(my_conn));
            }
            else
            {
                fprintf(stderr, "Insert error %d: %s\n", mysql_errno(my_conn),
                        mysql_error(my_conn));
            }

        }//end if payload > 0

    }//end udp


    return;
}

void connectMysql()
{

    my_conn=mysql_init(NULL);

    if(!mysql_real_connect(my_conn,"localhost","root","",dataBaseName,0,NULL,0)) //连接test数据库
    {
        printf("Connect Error!n");
        exit(1);
    }

}


int main(int argc, char **argv)
{
    connectMysql();

    strcpy(myip,getMyip());
    printf("%s\n",myip);


    tcp_num_count = 0;
    udp_num_count = 0;

    char *dev = NULL;                                        /* 捕获设备的名称 | capture device name */
    char errbuf[PCAP_ERRBUF_SIZE];                /* 错误的缓冲区   | error buffer */
    pcap_t *handle;                                                /* 数据包捕获句柄 | packet capture handle */

    char filter_exp[] = "ip";                        /* 过滤表达示          | filter expression [3] */
    struct bpf_program fp;                                /* 编译过滤表达示 | compiled filter program (expression) */
    bpf_u_int32 mask;                                        /* 子网掩码                  | subnet mask */
    bpf_u_int32 net;                                        /* IP 地址                  | ip */
    int num_packets = PACKETS_NUM;                                /* 捕获的数据包数量 | number of packets to capture */

    /* 显示程序版本信息 */
    print_app_banner();

    /* 检查来自命令行参数需要捕获设备的名称
           check for capture device name on command-line */
    if (argc == 2) {
        dev = argv[1];
    }
    else if (argc > 2) {
        fprintf(stderr, "error: unrecognized command-line options\n\n");
        print_app_usage();
        exit(EXIT_FAILURE);
    }
    else {
        /* 如果命令行参数没有指定, 则自动找到一个设备
                   find a capture device if not specified on command-line */
        dev = pcap_lookupdev(errbuf);
        if (dev == NULL) {
            fprintf(stderr, "Couldn't find default device: %s\n",
                    errbuf);
            exit(EXIT_FAILURE);
        }
    }

    /* 获得捕获设备的网络号和掩码
           get network number and mask associated with capture device */
    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n",
                dev, errbuf);
        net = 0;
        mask = 0;
    }

    /* 显示捕获设备信息
           print capture info */
    printf("Device: %s\n", dev);
    // printf("Number of packets: %d\n", num_packets);
    printf("Filter expression: %s\n", filter_exp);

    /* 打开捕获设备
           @1        捕获的设备
           @2        每次捕获数据的最大长度
           @3        1 启用混杂模式
           @4        捕获时间, 单位ms
           @5        错误缓冲区
           open capture device */
    handle = pcap_open_live(dev, SNAP_LEN, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        exit(EXIT_FAILURE);
    }

    /*        pcap_datalink();
                        返回数据链路层类型，例如DLT_EN10MB;

           确保我们对以太网设备捕获
           make sure we're capturing on an Ethernet device [2] */
    if (pcap_datalink(handle) != DLT_EN10MB) {
        fprintf(stderr, "%s is not an Ethernet\n", dev);
        exit(EXIT_FAILURE);
    }

    /* 编译过滤表达式
           compile the filter expression */
    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
        fprintf(stderr, "Couldn't parse filter %s: %s\n",
                filter_exp, pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }

    /* 应用过滤规则
           apply the compiled filter */
    if (pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "Couldn't install filter %s: %s\n",
                filter_exp, pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }

    /* 设置回高函数并开始捕获包
           now we can set our callback function */
    ///pcap_loop(handle, num_packets, got_packet, NULL);
    pcap_loop(handle, num_packets, got_packet, NULL);

    /* cleanup */
    pcap_freecode(&fp);
    pcap_close(handle);

    printf("\nCapture complete.\n");
    printf ("   %d TCP PACTETS are completed :\n", tcp_num_count);
    printf ("   %d UDP PACTETS are completed :\n", udp_num_count);


    mysql_close(my_conn);

    return 0;
}


string string_to_hex(unsigned char * str, int len)
{
   string hex_content;

   int i;
   for(i=0;i<len;i++)
   {
        char val[2] = {'0'};
        sprintf(val, "%2X", str[i]); //小写则为%x
        if(str[i]>=0 && str[i]<16)
        {
            val[0]='0';
        }
        hex_content.append(string(val));
  }

  return hex_content;
}
