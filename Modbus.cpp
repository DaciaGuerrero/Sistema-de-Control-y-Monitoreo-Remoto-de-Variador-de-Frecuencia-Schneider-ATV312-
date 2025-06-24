/*-------------------------------------------------------------------------------------------
 * 
 * Descarga de la biblioteca Modbus:
 * https://github.com/smarmengol/Modbus-Master-Slave-for-Arduino/blob/master/ModbusRtu.h
 * 
 ---------------------------------------------------------------------------------------------*/
#include <ModbusRtu.h>  
#include "Modbus.h"

const int RS485_DE_RE = 2;

/*-------------------------------------------------------------------------------------------
Modbus master(ID, Puerto, pinTxRx)
ID = 0 para modo maestro, o de 1 a 247 para modo esclavo
Puerto: puerto de comunicación 
pinTxRx: pin de control Tx/Rx de la placa. Si no lo hay (control automático), usar cero
---------------------------------------------------------------------------------------------*/
Modbus master(0, 1, RS485_DE_RE); // Modo maestro, puerto Serial1, pin 2 para control Tx/Rx
modbus_t telegram[1];             // Estructura para el telegrama Modbus

uint16_t au16data[16];           // Arreglo de datos para uso en la red Modbus

/*---------------------------------------------------------
    Inicializa el módulo Modbus 
----------------------------------------------------------*/
void StartModbus(void)
{
   pinMode(RS485_DE_RE, OUTPUT);          // Configura el pin como salida
   master.begin(19200);                   // Dirección del puerto serial y velocidad en baudios
   Serial1.begin(19200, SERIAL_8E1);      // 19200bps, paridad par, 1 bit de parada. Valor por defecto del variador
   master.setTimeOut(1000);               // Tiempo máximo de espera para respuesta (en milisegundos)
}

/*---------------------------------------------------------
    Envía comando de marcha/parada y referencia de velocidad 
----------------------------------------------------------*/
uint16_t EnviaComando(uint16_t cmd, uint16_t referencia)
{
   uint16_t erroresPrevios = master.getErrCnt();  // Guarda el contador de errores actual

   au16data[0] = cmd;             // Comando de marcha/parada
   au16data[1] = referencia;      // Referencia de velocidad, entre LSP y HSP
          
   telegram[0].u8id = 1;          // Dirección del esclavo
   telegram[0].u8fct = 16;        // Código de función: escribir múltiples registros
   telegram[0].u16RegAdd = 8501;  // Dirección inicial de escritura
   telegram[0].u16CoilsNo = 2;    // Cantidad de registros a escribir
   telegram[0].au16reg = au16data;  // Puntero al arreglo de datos a escribir
   master.query(telegram[0]);       // Envía el telegrama al esclavo

   do
   {
      master.poll();                      // Espera la respuesta
   } while(master.getState() == COM_WAITING); // Mientras la red espera respuesta, queda en bucle
   
   return (master.getErrCnt() - erroresPrevios); // Devuelve la diferencia de errores. Si es cero, todo salió bien.
}

/*---------------------------------------------------------
    Lee registros del variador (estado, frecuencia, corriente)
----------------------------------------------------------*/
uint16_t LeRegistradores(void)
{
   uint16_t erroresPrevios = master.getErrCnt();  // Guarda el contador de errores actual
   
   telegram[0].u8id = 1;            // Dirección del esclavo
   telegram[0].u8fct = 3;           // Código de función: lectura de registros
   telegram[0].u16RegAdd = 3201;    // Dirección inicial de lectura
   telegram[0].u16CoilsNo = 4;      // Cantidad de registros a leer
   telegram[0].au16reg = au16data;  // Puntero al arreglo de almacenamiento
   master.query(telegram[0]);       // Envía la solicitud al esclavo
  
   do
   {
      master.poll();                      // Espera la respuesta
   } while(master.getState() == COM_WAITING); // Mientras la red espera respuesta, queda en bucle

   return (master.getErrCnt() - erroresPrevios); // Devuelve la diferencia de errores. Si es cero, la lectura fue exitosa.
}
