#pragma comment(lib, "ws2_32.lib")
#include "stdafx.h"
#include <winsock2.h>
#include "ws2tcpip.h"

#include <iostream>
#include <string>

#define BUFFERS_LEN 1024
#define MAX_FILENAME_SIZE 64

#define DEBUG 0
#define LOCAL_PORT 4444

#define WINDOW_SIZE 4
#define MAX_SEQUENCE_NUMBER 7

#include <random>
#include <chrono>
#include <assert.h>

enum Operation {
	CLIENT = 1,
	SERVER = 2
};

typedef struct sequence_number_t {
private:
	uint32_t value = 0;
public:
	uint32_t get() const {
		return value;
	}

	void increment() {
		if (value == MAX_SEQUENCE_NUMBER) {
			value = 0;
		}
		else {
			value++;
		}
	}

	uint32_t distance_within_window(sequence_number_t other) const {
		int a = value;
		int	b = other.value;

		if (a < b) {
			int tmp = a;
			a = b;
			b = tmp;
		}

		if (a - b < WINDOW_SIZE) {
			return a - b;
		}
		else {
			return b + (MAX_SEQUENCE_NUMBER - a) + 1;
		}
	}

} sequence_number_t;

bool is_number(const std::string& s) {
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

bool is_ip_correct(std::string ip) {
	IN_ADDR tmp;
	if (inet_pton(AF_INET, ip.c_str(), &tmp) == 1) {
		return true;
	}
	std::cout << "IP is not correct!\n";
	return false;
}

bool get_int(int* in) {
	std::string input;
	std::getline(std::cin, input);
	if (is_number(input)) {
		*in = std::stoi(input);
		return true;
	}
	return false;
}

std::string get_target_ip() {
	std::string ip;
	do {
		std::cout << "Type receiver IP address (without port): ";
		std::getline(std::cin, ip);
	} while (!is_ip_correct(ip));
	return ip;
}

int get_port(std::string port_type) {
	int port = NULL;
	bool is_correct_port = false;
	do {
		std::printf("Type %s port: ", port_type.c_str());
		if (!get_int(&port)) {
			std::printf("Invalid port!\n");
		}
		is_correct_port = true;
	} while (!port);

	return port;
}

// https://stackoverflow.com/a/34597485
static void hash(std::size_t& result, const char* s, size_t n)
{
	const size_t prime = 31;
	for (size_t i = 0; i < n; i++) {
		result = s[i] + (result * prime);
	}
}

// https://lxp32.github.io/docs/a-simple-example-crc32-calculation/
uint32_t crc32_table[256];
void build_crc32_table(void) {
	for (uint32_t i = 0;i < 256;i++) {
		uint32_t ch = i;
		uint32_t crc = 0;
		for (size_t j = 0;j < 8;j++) {
			uint32_t b = (ch ^ crc) & 1;
			crc >>= 1;
			if (b) crc = crc ^ 0xEDB88320;
			ch >>= 1;
		}
		crc32_table[i] = crc;
	}
}

uint32_t crc32(const char* s, size_t n) {
	uint32_t crc = 0xFFFFFFFF;

	for (size_t i = 0;i < n;i++) {
		char ch = s[i];
		uint32_t t = (ch ^ crc) & 0xFF;
		crc = (crc >> 8) ^ crc32_table[t];
	}

	return ~crc;
}

// https://www.masterraghu.com/subjects/np/introduction/unix_network_programming_v1.3/ch14lev1sec2.html
bool recvfrom_timeout(int fd, int seconds, int microseconds) {
	fd_set rset;
	struct timeval tv;

	FD_ZERO(&rset);
	FD_SET(fd, &rset);

	tv.tv_sec = seconds;
	tv.tv_usec = microseconds;

	return select(fd + 1, &rset, NULL, NULL, &tv) <= 0;
}

typedef struct packet_t {
	sequence_number_t sequence_number;
	uint32_t crc;
	char data[BUFFERS_LEN - sizeof(sequence_number) - sizeof(crc)];
} packet_t;

typedef struct file_data_t {
	uint32_t stream_position;
	uint32_t size;
	char data[sizeof(packet_t().data) - sizeof(stream_position) - sizeof(size)];
} file_data_t;

bool read_file_into_packet(packet_t* packet_out, const sequence_number_t& sequence_number, uint32_t stream_position, FILE* f) {
	file_data_t* file_data_ptr = (file_data_t*)(packet_out->data);
	file_data_ptr->stream_position = stream_position;
	fseek(f, stream_position, SEEK_SET);
	file_data_ptr->size = fread(file_data_ptr->data, sizeof(char), sizeof(file_data_ptr->data), f);
	if (file_data_ptr->size == 0) {
		return false;
	}
	packet_out->sequence_number = sequence_number;
	packet_out->crc = crc32(file_data_ptr->data, file_data_ptr->size);
	return true;
}

bool send_packet(char* buffer_out, const sequence_number_t& sequence_number, const sequence_number_t& window_start, long int window_start_stream_position, int filesize, FILE* f, SOCKET& my_socket, sockaddr_in& target) {
	ZeroMemory(buffer_out, BUFFERS_LEN);

	// Populate packet with file data.
	packet_t* packet_out = (packet_t*)buffer_out;

	// Calculate stream position from sequence number.
	long int stream_position = sequence_number.distance_within_window(window_start) * sizeof(file_data_t().data) + window_start_stream_position;
	if (stream_position > filesize) {
		stream_position = filesize;
	}

	// Check for EOF.
	if (!read_file_into_packet(packet_out, sequence_number, stream_position, f)) {
		return false;
	}

	// Send packet.
	sendto(my_socket, (char*)packet_out, sizeof(packet_t), 0, (sockaddr*)&target, sizeof(sockaddr_in));

	return true;
}

// https://stackoverflow.com/a/21995693
template <
	class result_t = std::chrono::milliseconds,
	class clock_t = std::chrono::steady_clock,
	class duration_t = std::chrono::milliseconds
>
auto elapsed_since(std::chrono::time_point<clock_t, duration_t> const& start)
{
	return std::chrono::duration_cast<result_t>(clock_t::now() - start);
}

int main()
{
	// Initialize winsock.
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "Can't start Winsock" << std::endl;
		getchar();
		return 1;
	}

	// Create socket.
	SOCKET my_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (my_socket == SOCKET_ERROR) {
		std::cout << "Socket creation error" << std::endl;
		getchar();
		return 1;
	}

	char buffer_in[BUFFERS_LEN] = { 0 };
	char buffer_out[BUFFERS_LEN] = { 0 };

	int type;
	do {
		std::cout << "Choose server/client: \n Type '1' to be a CLIENT.\n Type '2' to be a SERVER.\n";
	} while (!get_int(&type) || type != CLIENT && type != SERVER);

#if DEBUG
	int local_port = LOCAL_PORT;
#else 
	int local_port = get_port("local");
#endif

	build_crc32_table();
	const std::string stop_signal = "STOP";
	int sockaddr_in_len = sizeof(sockaddr_in);

	if (type == CLIENT) {
		// Get target IP and port from stdin.
#if DEBUG
		std::string target_ip = "127.0.0.1";
		int target_port = LOCAL_PORT;
#else
		std::string target_ip = get_target_ip();
		int target_port = get_port("target");
#endif

		// Setup target address.
		sockaddr_in target;
		target.sin_family = AF_INET;
		target.sin_port = htons(target_port);
		inet_pton(AF_INET, target_ip.c_str(), &(target.sin_addr));

		while (true) {
			// Get filename from stdin.
			std::cout << "Enter filename: ";
			std::string fname;
			std::getline(std::cin, fname);

			// Open file.
			FILE* f;
			if ((f = fopen(fname.c_str(), "rb")) == NULL) {
				std::cout << "Error while opening file " << fname << std::endl;
				getchar();
				return 1;
			}

			// Get file size.
			fseek(f, 0, SEEK_END);
			int filesize = ftell(f);
			fseek(f, 0, SEEK_SET);

			// Send filename.		
			fname = fname.substr(fname.find_last_of("/\\") + 1); // remove path from filename
			sendto(my_socket, fname.c_str(), fname.size() + 1, 0, (sockaddr*)&target, sizeof(sockaddr));

			// Send file size.
			sendto(my_socket, (const char*)&filesize, sizeof(int), 0, (sockaddr*)&target, sizeof(sockaddr));

			// Read and send file.
			std::size_t file_hash = 1;
			const auto timeout_milliseconds = std::chrono::duration<double, std::milli>(50); // 0.05 seconds.
			bool eof_reached = false;
			std::chrono::steady_clock::time_point time_sent[WINDOW_SIZE];
			bool acks_mask[WINDOW_SIZE] = { false };
			bool sent_mask[WINDOW_SIZE] = { false };
			sequence_number_t window_start;
			long int window_start_stream_position = 0;
			int wait_for_acks_until_index;
			while (true) {
				// Send available packets within window.
				sequence_number_t window_sequence_number = window_start;

				for (size_t i = 0; i < WINDOW_SIZE; i++)
				{
					if (!sent_mask[i] || (!acks_mask[i] && elapsed_since(time_sent[i]) > timeout_milliseconds)) {
						// Send packet unless it's EOF.
						bool packet_sent = send_packet(buffer_out, window_sequence_number, window_start, window_start_stream_position, filesize, f, my_socket, target);
						if (!packet_sent && !eof_reached) {
							wait_for_acks_until_index = i;
#if DEBUG
							std::cout << "wait_for_acks_until_index " << wait_for_acks_until_index << std::endl;
#endif
						}
						eof_reached = eof_reached || !packet_sent;
						if (!packet_sent) {
							break;
						}

						// Update hash and sequence number on first/unique send.
						if (!sent_mask[i]) {
							// Update file hash.
							// TODO: hashing must be done strictly in order.
							// TODO: there should be a hash function impartial to order, like a set hash function.
							file_data_t* file_data_ptr = (file_data_t*)(((packet_t*)buffer_out)->data);
							hash(file_hash, file_data_ptr->data, file_data_ptr->size);
						}

#if DEBUG
						if (!sent_mask[i]) {
							printf("Sent packet %d\n", window_sequence_number.get());
						}
						else {
							printf("REsent packet %d\n", window_sequence_number.get());
						}
#endif

						// Set sliding window variables.
						sent_mask[i] = true;
						time_sent[i] = std::chrono::steady_clock::now();
					}
					window_sequence_number.increment();
				}

				// Non-blocking select to check for ACKs.
				if (!recvfrom_timeout(my_socket, 0, 0)) {
					// Write ACKs to mask.
					sequence_number_t reply_ack_number;
					recvfrom(my_socket, (char*)&reply_ack_number, sizeof(reply_ack_number), 0, (sockaddr*)&target, &sockaddr_in_len);
					int index = reply_ack_number.distance_within_window(window_start);
					acks_mask[index] = true;

#if DEBUG
					std::cout << std::endl;
					std::cout << "Got reply " << reply_ack_number.get() << "; window index = " << index << std::endl;
					printf("before: ");
					printf("%d [", window_start.get());
					for (int i = 0; i < WINDOW_SIZE; i++) {
						printf("%d ", acks_mask[i]);
					}
					printf("]\n");
#endif

					if (eof_reached) {
#if DEBUG
						std::cout << "EOF reached" << std::endl;
#endif
						bool all_acks_received = true;
						for (int i = 0; i < wait_for_acks_until_index; i++) {
							all_acks_received = all_acks_received && acks_mask[i];
						}

						if (all_acks_received) { // End transmission.
#if DEBUG
							std::cout << "End the transmission" << std::endl;
#endif
							break;
						}
						else { // Wait for other ACKs.
#if DEBUG
							std::cout << "Waiting for other ACKs..." << std::endl;
#endif
							continue;
						}
					}

					// Shift window.
					int shift_amount = 0;
					for (shift_amount = 0; shift_amount < WINDOW_SIZE; shift_amount++) {
						if (!acks_mask[shift_amount]) {
							break;
						}
					}
					for (int i = 0; i < WINDOW_SIZE - shift_amount; i++) {
						acks_mask[i] = acks_mask[i + shift_amount];
						sent_mask[i] = sent_mask[i + shift_amount];
						time_sent[i] = time_sent[i + shift_amount];
					}
					for (int i = WINDOW_SIZE - shift_amount; i < WINDOW_SIZE; i++) {
						acks_mask[i] = false;
						sent_mask[i] = false;
					}
					for (int i = 0; i < shift_amount; i++) {
						window_start.increment();
						window_start_stream_position += sizeof(file_data_t().data);
					}
#if DEBUG
					printf("after:  ");
					printf("%d [", window_start.get());
					for (int i = 0; i < WINDOW_SIZE; i++) {
						printf("%d ", acks_mask[i]);
					}
					printf("]\n");
#endif
				}
			}

			// Send stop signal.	
			sendto(my_socket, stop_signal.c_str(), stop_signal.size() + 1, 0, (sockaddr*)&target, sizeof(sockaddr));

			// Send file hash.
			sendto(my_socket, (const char*)&file_hash, sizeof(std::size_t), 0, (sockaddr*)&target, sizeof(sockaddr));

			// Close file.
			if (fclose(f) == EOF)
			{
				std::cout << "Error while closing file " << fname << std::endl;
				getchar();
				return 0;
			}
		}
	}
	else if (type == SERVER) {
		// Setup server address.
		struct sockaddr_in server;
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = INADDR_ANY;
		server.sin_port = htons(local_port);

		// Bind socket.
		if (bind(my_socket, (sockaddr*)&server, sizeof(server)) != 0) {
			printf("Binding error\n");
			getchar();
			return 1;
		}
#if DEBUG
		// For ACK testing.
		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_int_distribution<int> uni(0, 1000);
#endif

		while (true) {
			// Initialize client address memory to 0s.
			struct sockaddr_in client;
			ZeroMemory(&client, sizeof(client));

			// Receive filename.
			printf("Waiting for datagram ...\n");
			char fname[BUFFERS_LEN];
			recvfrom(my_socket, fname, BUFFERS_LEN, 0, (sockaddr*)&client, &sockaddr_in_len);

			// Receive filesize.
			int expected_filesize;
			recvfrom(my_socket, (char*)&expected_filesize, sizeof(expected_filesize), 0, (sockaddr*)&client, &sockaddr_in_len);

			// Open file.
			FILE* f;
			if ((f = fopen(fname, "wb")) == NULL)
			{
				fprintf(stderr, "Error while opening file %s\n", fname);
				getchar();
				return 1;
			}

			// Receive and write file.			
			int bytes_written_total = 0;
			std::size_t file_hash = 1;
			while (true) {
				// Clear memory.
				ZeroMemory(buffer_in, BUFFERS_LEN);

				// Receive packet.
				int received_bytes_this_packet = recvfrom(my_socket, buffer_in, sizeof(packet_t), 0, (sockaddr*)&client, &sockaddr_in_len);
				if (received_bytes_this_packet == 0) {
					break;
				}

				// Check for stop signal.
				if (strcmp(buffer_in, stop_signal.c_str()) == 0) {
					printf("Client ended the transmission\n");
					break;
				}

#if DEBUG
				if (uni(rng) < 200) // 20% chance of losing a packet.					
					continue;
#endif		

				// Get pointers to packet.
				packet_t* packet_in = (packet_t*)buffer_in;
				file_data_t* file_data_ptr = (file_data_t*)(packet_in->data);

				// Acknowledge received packet.
#if DEBUG
				if (uni(rng) < 900) // 10% chance of losing an acknowledgment.		
#endif					
					sendto(my_socket, (char*)&(packet_in->sequence_number), sizeof(packet_in->sequence_number), 0, (sockaddr*)&client, sockaddr_in_len);

				// TODO: Skip already written packets.
				// if (packet_in->sequence_number.get() < window_start) { // TODO: define < for circular numbers
				//	continue;
				// }

				// Compute and compare CRCs.
				uint32_t computed_crc = crc32(file_data_ptr->data, file_data_ptr->size);
				if (computed_crc != packet_in->crc) {
					// Drop packet if CRCs dont match.
					continue;
				}

				// Write bytes from file_data_ptr->data at file_data_ptr->stream_position.
				fseek(f, file_data_ptr->stream_position, SEEK_SET);
				bytes_written_total += fwrite(file_data_ptr->data, sizeof(char), file_data_ptr->size, f);
#if DEBUG
				printf("%.2f written\n", (float)bytes_written_total / (float)expected_filesize);
#endif

				// Update the hash value.
				hash(file_hash, file_data_ptr->data, file_data_ptr->size);
			}

			// Receive hash.
			std::size_t received_hash;
			recvfrom(my_socket, (char*)&received_hash, sizeof(received_hash), 0, (sockaddr*)&client, &sockaddr_in_len);

			// TODO: sizes will always mismatch because of the redundant writes.
			// Check filesize against received bytes.
			// if (expected_filesize != bytes_written_total) {
			// 	printf("%d bytes expected, %d bytes written total\n", expected_filesize, bytes_written_total);
			// }
			// 
			// TODO: hashes will most likely not match because packets came out of order.
			// Compare hashes
			// else if (file_hash != received_hash) {
			// 	printf("Hashes don't match!\n");
			// }
			// else {
			// 	printf("Successfully received file\n");
			// }

			// Close file.
			if (fclose(f) == EOF)
			{
				fprintf(stderr, "Error while closing file %s\n", fname);
				getchar();
				return 0;
			}
		}
	}

	closesocket(my_socket);
	getchar();
	return 0;
}
