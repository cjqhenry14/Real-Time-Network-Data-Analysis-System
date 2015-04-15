#ifndef SNIFFER_SQLTOCACHE_H
#define SNIFFER_SQLTOCACHE_H

#include "decoder_dataCache.h"

void* get_hex_data_from_sql(void *arg);

void* get_data_from_sql(void *arg);

void* get_data_from_pcap(void *arg);

#endif
