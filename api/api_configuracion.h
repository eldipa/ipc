#ifndef _API_CONFIGURACION_H_
#define _API_CONFIGURACION_H_

#include "iqueue_manager.h"
#include "ipc_queue_manager.h"
#include "broker_queue_manager.h"
#include "genericerror.h"

#include "iniparser.h"
#include <string>

class ApiConfiguracion {
private:
   static std::string wkdir;
   static std::string torre_ip;
   static std::string torre_wkdir;

public:
   
   static yasper::ptr<IQueueManager> get_queue_manager(const char* directorio_de_trabajo, const char* config_file, bool create = false) {
      //config_file = config_file;
      directorio_de_trabajo = directorio_de_trabajo + 1 -1;
      create = !!create;

      dictionary * ini = iniparser_load(config_file);

      if (ini==NULL) {
         Log::crit("cannot parse the config file: %s\n", config_file);
         throw GenericError("cannot parse the config file %s", config_file);
      }

      char* working_dir;
      char* ip;
      char* port;
      int use_ipc;

      working_dir = iniparser_getstring(ini, "AEROPUERTO:working_dir", NULL);
      ip = iniparser_getstring(ini, "MESSAGE_BROKER:ip", NULL);
      port = iniparser_getstring(ini, "MESSAGE_BROKER:port", NULL);
      use_ipc = iniparser_getboolean(ini, "MESSAGE_BROKER:use_ipc", -1);

      if( !use_ipc ) {
         return new BrokerQueueManager( new MessageBrokerStub(ip, port) );
      } else {
         return new IpcQueueManager(working_dir);
      }

      iniparser_freedict(ini);

      // Funciona con colas del so pero accedidas a travez de un broker en shared memory (para debug)
      // if( create )
      //    return new BrokerQueueManager( new MessageBroker(directorio_de_trabajo, create) );
      // else
      //    return new BrokerQueueManager( new MessageBroker(directorio_de_trabajo) );

   }
   
    static std::string get_wkdir(const char* archivo_configuracion) {
       if(ApiConfiguracion::wkdir == "") {
          ApiConfiguracion::parse_config_file(archivo_configuracion);
       } 
       return ApiConfiguracion::wkdir;
    }

   static std::string get_torre_ip(const char* config_file) {
      if(ApiConfiguracion::torre_ip == "") {
         dictionary * ini = iniparser_load(config_file);

         if (ini==NULL) {
            Log::crit("cannot parse the config file: %s\n", config_file);
            throw GenericError("cannot parse the config file %s", config_file);
         }

         ApiConfiguracion::torre_ip = iniparser_getstring(ini, "TORRE_DE_CONTROL:ip", NULL);
         Log::info("ApiConfiguracion: ok al leer ip %s", torre_ip.c_str());
         iniparser_freedict(ini);
      }

      return ApiConfiguracion::torre_ip;
   }

   static std::string get_torre_wkdir(const char* config_file) {
      if(ApiConfiguracion::torre_wkdir == "") {
         dictionary * ini = iniparser_load(config_file);

         if (ini==NULL) {
            Log::crit("cannot parse the config file: %s\n", config_file);
            throw GenericError("cannot parse the config file %s", config_file);
         }

         ApiConfiguracion::wkdir = iniparser_getstring(ini, "TORRE_DE_CONTROL:working_dir", NULL);
         Log::info("ApiConfiguracion: ok al leer TORRE_DE_CONTROL:working_dir %s", ApiConfiguracion::torre_wkdir.c_str());
         iniparser_freedict(ini);
      }

      return ApiConfiguracion::torre_wkdir;
   }

private:

    static void parse_config_file(const char * config_file) {
       dictionary * ini = iniparser_load(config_file);

       if (ini==NULL) {
          Log::crit("cannot parse the config file: %s\n", config_file);
          throw GenericError("cannot parse the config file %s", config_file);
       }

       ApiConfiguracion::wkdir = iniparser_getstring(ini, "AEROPUERTO:working_dir", NULL);
       Log::info("ApiConfiguracion: ok al leer wkdir %s", wkdir.c_str());
       iniparser_freedict(ini);
    }

};


#endif