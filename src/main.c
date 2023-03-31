#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

#define IPV4 0x0800
#define IPV6 0x86DD
#define ARP 0x0806

#define BUFF_SIZE 2048

typedef uint64_t usize;

typedef struct __attribute__((packed)) Ethhdr
{
	uint8_t dstaddr[6];
	uint8_t srcaddr[6];
	uint16_t type;
	uint8_t data[1500];
	uint32_t crc;
} Ethhdr_t;

typedef struct __attribute__((packed)) Arphdr
{
	uint16_t htype;
	uint16_t ptype;
	uint8_t hlen, plen;
	uint16_t opcode;
	uint8_t data[80];
} Arphdr_t;

typedef struct __attribute__((packed)) Ipv4
{
	uint8_t v_hl;
	uint8_t tos;
	uint16_t len;
	uint16_t id;
	uint16_t frag;
	uint8_t ttl;
	uint8_t proto;
	uint16_t cksum;
	uint32_t srcip;
	uint32_t dstip;
	uint8_t data[512];
} Ipv4_t;

static char _intf_buff[100];
static int _active_sock = -1;

static void _setup_raw_sock()
{
	_active_sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(_active_sock < 0)
	{
		perror("socket function error!");	
		exit(1);
	}

	if(setsockopt(_active_sock, SOL_SOCKET, SO_BINDTODEVICE, _intf_buff, strlen(_intf_buff)) < 0)
	{
		perror("setsockopt error");
		exit(3);
	}
}

void listen_on_intf(char *intf)
{
    printf("Interface: %s\n", intf);
    if(strlen(intf) > 100)
    {
        perror("Interface's name length must be less than 100.");
        exit(5);
    }

    if(_active_sock >= 0)
        close(_active_sock);

    memset(_intf_buff, 0, 100);
    strncpy(_intf_buff, intf, strlen(intf));
    _setup_raw_sock();
}

void recv_ether_raw(char *buffer)
{
    // use thread to manage this function
    if(_active_sock < 0)
    {
        perror("Socket is not active");
        exit(4);
    }

    memset((char*) buffer, 0, BUFF_SIZE);
	while(1)
	{
		int32_t len;
		if((len = recvfrom(_active_sock, buffer, BUFF_SIZE, 0, NULL, NULL)) < 0)
		{
			perror("recvfrom error");
			break;
		}

		// Ethhdr_t hdr = *(Ethhdr_t*) buffer;
        // printf("Data length: %ld, EtherType: %d\n", strlen(buffer), hdr.type);
        /*
		if(ntohs(hdr.type) == IPV4)
		{
			Ipv4_t ip = *(Ipv4_t*) hdr.data;
			ip.dstip = ntohl(ip.dstip);
			printf(
                    "IPV4: %d.%d.%d.%d", 
                    (ip.srcip >> 24) & 0xFF, 
                    (ip.srcip >> 16) & 0xFF, 
                    (ip.srcip >> 8) & 0xFF, 
                    (ip.srcip & 0xFF)
                  );
			printf(
                    " -> %d.%d.%d.%d", 
                    (ip.dstip >> 24) & 0xFF, 
                    (ip.dstip >> 16) & 0xFF, 
                    (ip.dstip >> 8) & 0xFF, 
                    (ip.dstip & 0xFF)
                  );

			if(ip.proto == 6)
				printf(" (TCP)\n");
			else if(ip.proto == 17)
				printf(" (UDP)\n");
			else if(ip.proto == 1)
				printf(" (ICMP)\n");
		}
        */
	}
}

void free_pack(char* pack)
{
    free(pack);
}

char** list_ifs()
{
    char **buffer = malloc(sizeof *buffer * 20);
    for(int32_t i = 0; i < 20; i++)
        buffer[i] = NULL;

    struct ifaddrs *addrs;

    getifaddrs(&addrs);
    struct ifaddrs *tmp = addrs;

    int32_t idx = 0;
    while(tmp)
    {
        if(tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_PACKET)
        {
            long size = strlen(tmp->ifa_name);
            buffer[idx] = malloc(size + 1);
            memset(buffer[idx], 0, size + 1);
            strncat(buffer[idx], tmp->ifa_name, size);
        }

        tmp = tmp->ifa_next;
        idx++;
    }
    return buffer;
}

void free_ifs_array(char **arr)
{
    for(int32_t i = 0; i < 20; i++)
    {
        if(arr[i] != NULL)
            free(arr[i]);
    }
    free(arr);
}
