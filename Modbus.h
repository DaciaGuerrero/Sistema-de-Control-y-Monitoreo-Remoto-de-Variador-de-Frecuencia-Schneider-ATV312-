
void StartModbus(void);                                        // Inicializa el módulo Modbus
uint16_t EnviaComando(uint16_t cmd, uint16_t referencia);      // Envía referencia de velocidad
uint16_t LeRegistradores(void);                                // Lee registros del variador

extern uint16_t au16data[16];                                  // Arreglo de datos Modbus

/***************************************************************
 * Proyecto desarrollado por:
 * Dacia / Sofía
 * Materia: Accionamientos Eléctricos y Electrónica Industrial
 * Año: 2025
 ***************************************************************/