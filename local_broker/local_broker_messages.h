/*
 * local_broker_messages.h
 *
 *  Created on: 06/02/2013
 *      Author: gonzalo
 */

#ifndef LOCAL_BROKER_MESSAGES_H_
#define LOCAL_BROKER_MESSAGES_H_

#include "api_constants.h"

typedef struct {
	long mtype;
	char recurso [MAX_NOMBRE_RECURSO];
} traspaso_token_t;

typedef struct {
	long mtype;
	long id_secuencia_vista;
	char vista_grupo [MAX_NOMBRE_RECURSO];
} traspaso_vista_t;

#endif /* LOCAL_BROKER_CONSTANTS_H_ */