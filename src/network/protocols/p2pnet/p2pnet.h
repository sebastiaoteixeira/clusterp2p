/**
 * An IPv6 inspired protocol for communication between peers in a mesh network.
 * The protocol is designed to be lightweight and efficient.
 * It can be implemented on top of any transport layer protocol (e.g. UDP, TCP) or directly on top of the link layer.
 * It is not dependent of any specific physical layer technology (e.g. WiFi, LoRa, Ethernet).
 * Here, we define the packet format and the API for creating and parsing packets.
 */

#ifndef P2PNET_H
#define P2PNET_H

#include <stdint.h>
#include "../../identity.h"

typedef struct {
    uint32_t* packet;

    // The next atributtes are not part of the packet itself,
    // but are used to keep track of the packet's state
    uint32_t* seek;
    uint8_t seek_type; // 0 = main header, 1 = extension header, 2 = payload
} p2pnet_packet_t;

#define P2Pv1 1

#define HEADER_SIZE 12

#define LAST_EXTENSION_HEADER 59

p2pnet_packet_t create_p2pnet_packet(uint8_t version, uint8_t traffic_class, uint32_t flow_label, uint8_t hop_limit, node_id_t source_id, node_id_t destination_id);
p2pnet_packet_t create_p2pnet_packet_with_payload(uint8_t version, uint8_t traffic_class, uint32_t flow_label, uint8_t hop_limit, node_id_t source_id, node_id_t destination_id, unsigned char* payload, uint16_t payload_length);


// Header Extensions
void add_header_extension(p2pnet_packet_t packet, uint8_t next_header);
p2pnet_packet_t seek_next_header(p2pnet_packet_t* packet);

// Setters
void write_payload_to_p2pnet_packet(p2pnet_packet_t packet, unsigned char* payload, uint16_t payload_length);
void append_payload_to_p2pnet_packet(p2pnet_packet_t packet, unsigned char* payload, uint16_t payload_length);

// Getters
uint8_t get_p2pnet_version(p2pnet_packet_t packet);
uint8_t get_p2pnet_traffic_class(p2pnet_packet_t packet);
uint32_t get_p2pnet_flow_label(p2pnet_packet_t packet);
uint16_t get_p2pnet_payload_length(p2pnet_packet_t packet);
uint8_t get_p2pnet_next_header(p2pnet_packet_t packet);
uint8_t get_p2pnet_hop_limit(p2pnet_packet_t packet);
node_id_t get_p2pnet_source(p2pnet_packet_t packet);
node_id_t get_p2pnet_destination(p2pnet_packet_t packet);
uint8_t* get_p2pnet_payload(p2pnet_packet_t packet);
void copy_p2pnet_payload(p2pnet_packet_t packet, unsigned char* payload);


void free_p2pnet_packet(p2pnet_packet_t* packet);




#endif // P2PNET_H