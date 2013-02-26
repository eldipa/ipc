#include "api_trasbordo.h"
#include "api_constants.h"
#include <exception>
#include <stdexcept>
#include <string>
#include <vector>
#include "api_common.h"

#include "api_configuracion.h"

using namespace std;

ApiTrasbordo::ApiTrasbordo(const char* directorio_de_trabajo, const char* config_file, int numero_de_vuelo)
	: queue_manager(ApiConfiguracion::get_queue_manager(directorio_de_trabajo, config_file)),
		cola_cargadores_equipaje(queue_manager->get_queue(PATH_COLA_ROBOTS_INTERCARGO, 0))
{

	semaforos = new SemaphoreSet(string(directorio_de_trabajo).append(PATH_IPC_ROBOTS_INTERCARGO).c_str(), 0, 0, 0);
	memoria_zonas = new SharedMemory(string(directorio_de_trabajo).append(PATH_IPC_ROBOTS_INTERCARGO).c_str(), 1, 0, 0,
		false, false);
	cinta = new CintaCentral(std::string("robot_intercargo").append(intToString(numero_de_vuelo)).c_str(),
		directorio_de_trabajo, -1, -2);
	id_productor = -1;

	zonas_asignadas = static_cast<int *>(memoria_zonas->memory_pointer());
	vuelos_esperando = zonas_asignadas + MAX_ZONAS;
}

ApiTrasbordo::~ApiTrasbordo() {
	delete semaforos;
	delete memoria_zonas;
	if (cinta) {
		delete cinta;
	}
}

void ApiTrasbordo::poner_en_cinta_principal(const Equipaje& equipaje) {
	cinta->colocar_elemento(&equipaje, id_productor);
}

int ApiTrasbordo::esperar_vuelo_entrante(int numero_vuelo_destino) {
	mensaje.mtype = 0;
	cola_cargadores_equipaje->pull(&this->mensaje, sizeof(MENSAJE_VUELO_ENTRANTE), numero_vuelo_destino);
	return mensaje.vuelo_entrante;
}

int ApiTrasbordo::esperar_zona_asignada(int numero_vuelo) {
	int zona_asignada = -1;
	int i;

	while (zona_asignada == -1) {
		semaforos->wait_on(0);

		for (i = 0; i < MAX_ZONAS ; i++) {
			if (zonas_asignadas [i] == numero_vuelo) {
				zona_asignada = i + 1;
				break;
			}
		}

		if (zona_asignada == -1) {
			for (i = 0; i < MAX_ROBOTS_INTERCARGO_ESPERANDO_POR_ZONAS ; i++) {
				if (vuelos_esperando [i] == 0) {
					vuelos_esperando [i] = numero_vuelo;
					semaforos->signalize(0);
					semaforos->wait_on(i + 1);
					break;
				}
			}
		} else {
			semaforos->signalize(0);
		}
	}
	id_productor = zona_asignada + MAX_SCANNERS + 1;
	return zona_asignada;
}

