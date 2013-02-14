#include "messagequeue.h"
#include <cstring>
#include <cstdio>
#include <iostream>

typedef struct {
	long mtype;
	char msg[10];
} mensaje_t;

int main() {
	MessageQueue outbound("/tmp/test", char(128));
	MessageQueue inbound("/tmp/test", char(0));
	mensaje_t mensaje;

	mensaje.mtype = 1;

	for (int i = 0; i < 10; i++) {
		sprintf(mensaje.msg, "hola%d", i);

		std::cout << "enviado:" << mensaje.msg << std::endl;

		outbound.push(&mensaje, sizeof(mensaje_t) - sizeof(long));
	}
	return -1;
}