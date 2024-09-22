#include "./p2pnet.h"
#include <stdlib.h>

p2pnet_packet_t create_p2pnet_packet(uint8_t version, uint8_t traffic_class, uint32_t flow_label, uint8_t hop_limit, node_id_t source_id, node_id_t destination_id) {
    p2pnet_packet_t packet = (p2pnet_packet_t) malloc(HEADER_SIZE);
    packet[0] = version << 28 | traffic_class << 20 | flow_label;
    packet[1] = LAST_EXTENSION_HEADER << 8 | hop_limit; // Payload length is 0
    packet[2] = source_id << 16 | destination_id;
    return packet;
}

p2pnet_packet_t create_p2pnet_packet_with_payload(uint8_t version, uint8_t traffic_class, uint32_t flow_label, uint8_t hop_limit, node_id_t source_id, node_id_t destination_id, unsigned char* payload, uint16_t payload_length) {
    p2pnet_packet_t packet = (p2pnet_packet_t) malloc(HEADER_SIZE + payload_length);
    packet[0] = version << 28 | traffic_class << 20 | flow_label;
    packet[1] = payload_length << 16 | LAST_EXTENSION_HEADER << 8 | hop_limit;
    packet[2] = source_id << 16 | destination_id;
    memcpy(packet + 16, payload, payload_length);
    return packet;
}

void write_payload_to_p2pnet_packet(p2pnet_packet_t packet, unsigned char* payload, uint16_t payload_length) {
    packet = (p2pnet_packet_t) realloc(packet, HEADER_SIZE + payload_length);
    packet[1] = (packet[1] & 0x0000FFFF) | (payload_length << 16);
    memcpy(packet + 16, payload, payload_length);
}

void append_payload_to_p2pnet_packet(p2pnet_packet_t packet, unsigned char* payload, uint16_t payload_length) {
    packet = (p2pnet_packet_t) realloc(packet, HEADER_SIZE + get_p2pnet_payload_length(packet) + payload_length);
    memcpy(packet + 16 + get_p2pnet_payload_length(packet), payload, payload_length);
    packet[1] = (packet[1] & 0x0000FFFF) | ((get_p2pnet_payload_length(packet) + payload_length) << 16);
}

uint8_t get_p2pnet_version(p2pnet_packet_t packet) {
    return packet[0] >> 28;
}

uint8_t get_p2pnet_traffic_class(p2pnet_packet_t packet) {
    return (packet[0] >> 20) & 0xFF;
}

uint32_t get_p2pnet_flow_label(p2pnet_packet_t packet) {
    return packet[0] & 0xFFFFF;
}

uint16_t get_p2pnet_payload_length(p2pnet_packet_t packet) {
    return packet[1] >> 16;
}

uint8_t get_p2pnet_next_header(p2pnet_packet_t packet) {
    return (packet[1] >> 8) & 0xFF;
}

uint8_t get_p2pnet_hop_limit(p2pnet_packet_t packet) {
    return packet[1] & 0xFF;
}

node_id_t get_p2pnet_source(p2pnet_packet_t packet) {
    return packet[2] >> 16;
}

node_id_t get_p2pnet_destination(p2pnet_packet_t packet) {
    return packet[2] & 0xFFFF;
}

uint8_t* get_p2pnet_payload(p2pnet_packet_t packet) {
    return 
}

void copy_p2pnet_payload(p2pnet_packet_t packet, unsigned char* payload) {
    memcpy(payload, packet + 16, get_p2pnet_payload_length(packet));
}

void free_p2pnet_packet(p2pnet_packet_t* packet) {
    packet = NULL;
    free(*packet);
}