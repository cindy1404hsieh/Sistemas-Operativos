# Sistemas-Operativos
## Programa Sintetico
- Introducción a Sistemas Operativos
  - Sistemas Operativos
  - Una Computadora
  - ¿Qué es un Sistema Operativo?
  - Un Poco de Historia
  - Los Sistemas Operativos
  - La perspectiva del usuario
- El Kernel
  - Ejecución Directa
  - Limitar la Ejecución Directa
  - Modo Dual de Operaciones
  - Kernerl Land y User Land
  - Protección del Sistema
  - Visión General
  - Tareas específicas del Kernel
  - Modos de Transferencia
  - System Call
  - El Sistema Operativo Unix y sus Distintos Sabores
  - BSD
  - Tipos de Kernel
  - Iniciar al Sistema Operativo y el Kernel
- Introducción: x86
  - Ley de Moore
  - Bell’s Law
  - Los Registros de x86
  - Registros multipropósito
  - Registros de propósito especial
  - Registros de Segmento
  - Estructura del Stack Frame
  - Un ejemplo
- El Proceso
  - De Programa a Proceso
  - El Proceso
  - La Virtualización
  - El Proceso: por dentro
  - El API de Procesos
  - Desde el Kernel de Linux
- Scheduling o Planificación de Procesos
  - Time Sharing
  - Números y el Workload
  - Políticas Para Sistemas Mono-procesador
  - La Planificación en la vida real
  - Multi-Level Feedback Queue (MLFQ)
  - MLQF: Las reglas básicas
  - Segundo Approach
  - Resumen
  - Planificación: Proportional Share
  - Planificación Avanzada: Planificación multiprocesador
  - Afinidad de Cache
  - Multi-Queue Planificación
  - La Vida Real
  - unix xv6
- La Memoria
  - La Abstracción del Espacio de Direcciones: Introducción
  - El Espacio de Direcciones o Address Space
  - Conclusión
  - El API de Memoria
  - Address Translation
  - Un ejemplo
  - Dynamic Realocation o Memoria Segmentada [DAH]
  - Memoria Paginada
  - Hacia una eficiente Address Translation
  - La TLB
- Concurrencia
  - La Abstracción
  - El API de Threads
  - Estructura y Ciclo de Vida de un Thread
  - Threads y Linux
  - Sincronización
  - Una Mejor Solución Locks
  - Tipos de Datos Sincronizados
  - Errores comunes de concurrencia
- Sistema de Archivos (File System)
  - El Virtual File System
  - Archivos
  - Directorios o Directories
  - EL API
  - System Calls sobre Directorios
  - System Call sobre los Metadatos
  - umask()
  - Some File System Unix Commnds
  - Implementación de un File System
  - Organización general
- Shell
  - Los Shell Modernos
  - La perspectiva del usuario
  - Built-In
  - Los Comandos
- Shell Scripting
  - Varios
  - Variables
  - Estructuras de Control
  - Expresiones regulares
