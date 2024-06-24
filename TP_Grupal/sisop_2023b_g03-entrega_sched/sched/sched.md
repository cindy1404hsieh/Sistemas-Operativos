# sched

Lugar para respuestas en prosa, seguimientos con GDB y documentación del TP.

## Documentacion selección de scheduler

Agregamos una variable de entorno temporal para que, al momento de compilar, selecionar con qué scheduler ejecutamos tanto los test como algún proceso en particular.

Esto lo hacemos seteando la variable de entorno SCHED_PR en 1.

En caso de no agregar ninguna variable temporal, entonces por defecto se compilará JOS con scheduler con Round Robin.

Algunos ejemplos:

```
make grade SCHED_PR=1
```

Ejecuta las pruebas con scheduler con prioridades.

```
make grade
```

Ejecuta las pruebas con scheduler Round Robin.

```
make run-hello-nox SCHED_PR=1
```

Ejecuta hello.c con scheduler con prioridades.

```
make run-hello-nox
```

Ejecuta hello.c con scheduler Round Robin.


## Seguimiento GDB

A continuación un seguimiento a través de imágenes sobre lo que ocurre al momento de realizar un context switch en JOS.

1. **Corremos qemu en modo debug sin entorno gráfico**.

    ![make-qemu-nox-gdb](https://drive.google.com/uc?id=1v7Hbo3LIf2tmvU9lPwxb28vQF81I-Z5g)

2. **Corremos ```make gdb``` para comenzar el proceso de debug.**

    ![make-gdb](https://drive.google.com/uc?id=1CI1dmZfaIfS4-c3RWtQybxG8jTn_Ial2)

### Registros antes de context switch

3. **Establezco como punto de interrupción antes de la ejecución de la función `context_switch`.**

    ![esp-eip-cs-antes](https://drive.google.com/uc?id=1BXAOkxLv23mtCJIyPkwjUBQHbCA0yw5V)

    Vemos que los registros del stack (%esp) y el instruction pointer (%eip) tienen direcciones de memoria altas, lo que estaría indicando que antes de que se ejecute el context switch se encuentra en modo kernel. Esto también lo podemos ver reflejado al ver los últimos 2 bits de ```p/t $cs``` donde el 00 indica que estamos en el anillo 0 que corresponde al kernel.

4. **Info registers antes de context switch.**

    A continuación mostramos como se encontraban los registros restantes antes de la ejecución del context_switch.

    ![info-registers-antes](https://drive.google.com/uc?id=136AZPD6Fo9c7qyPLCZqSmOqgaQ34BQEK)

    Podemos notar por ejemplo que para los registros de propósito general, tenemos posiciones de memoria altas (ej: 0xf01223c0) lo que nos da otro indicio sobre que estamos trabajando en modo kernel.

### Stack Instruccion a instrucción

5. **Stack luego de realizarse el `add $4,%esp`.**  

    ![stack-01](https://drive.google.com/uc?id=1VGADewv4XKi5DtrZO06PWVRxK1DxRaiL)

    Con esta instrucción se apunta el puntero al struct TrapFrame al stack actual, que previamente apuntaba al instruccion pointer

6. **Stack luego de realizarse el `mov (%esp),%esp`.** 

    ![stack-02](https://drive.google.com/uc?id=1XEJVtShoQ3LS1z319dAnV2RyHJzN3ZGY)
    
    Con esta instrucción apuntamos al puntero al primer elemento del struct TrapFrame en este caso el struct
    `struct PushRegs tf_regs`.

7. **Stack luego de realizar `popal`.**

    ![stack-03](https://drive.google.com/uc?id=1tUIYJnhF_C8QYim4ReVaNLHDd-s9w-R4)

    Con esta instrucción "cargamos" todos los registros del struct PushRegs en una única instrucción.

8. **Cargo el registro %es (extra segment).**

    ![stack-04](https://drive.google.com/uc?id=1CLrs1A4OiBw5cblSC7GYDKYCR2_ccTOu)

9. **Cargo el registro %ds (data segment).**

    ![stack-05](https://drive.google.com/uc?id=1oAoqwv_SNEK4kkWGqeuURYh5eMlZnel1)

10. **Salteo tanto el trapno (`tf_trapno`) err(`tf_err`) del struct TrapFrame.**

    ![stack-06](https://drive.google.com/uc?id=1CI6btM6-DciFkVC86_DQH-hlig7w1Bp4)

    Esto para luego poder realizar el iret y que el mismo se encargue de configurar los registros restantes correctamente.

### Registros antes y después de iret

11. **Antes de `iret`.**

    ![antes-iret-registros-principales](https://drive.google.com/uc?id=1xZbVKjEEwQofT7IlKToyjTvxuHRT9nZ1)

    Como vimos en 3, vemos que los registros del stack (%esp) y el instruction pointer (%eip) siguen con direcciones de memoria altas, lo que estaría indicando que antes de que se ejecute el iret del context switch se encuentra en modo kernel. el registro code segment (`%cs`) también nos indica que seguimos en modo kernel.

12. **Después de `iret`.**

    ![despues-iret-registros-principales](https://drive.google.com/uc?id=1iMFRSrQ5IPLUfN0L9manwWNUXh0ndpdS)

    Luego a aplicar iret, vemos que los registros principales han cambiado de dirección de memoria por direcciones más bajas, similares a las que indica el archivo `memlayout.h` como direcciones correspondientes al stack de usuario para el caso del stack de usuario %esp.
    Además también vemos que el anillo paso del ring 0 al ring 3, que corresponde al modo usuario (el print de %cs en formato binario termina en con los bits 11).

13. **Algunos registros generales después de `iret`.**

    ![despues-info-registers](https://drive.google.com/uc?id=1yhOtPpXFCOjKy-HB_lW1p-TRs-anpFth)

    Vemos que iret reestablece los registros en general, la mayoría sin una dirección de memoria en específico, a exepción de los más importantes (%esp, %eip, %cs, ect.), que vimos anteriormente que corresponden a direcciones de memoria correspondientes al modo usuario.

---

## Scheduler con prioridades

Definimos a la prioridad como un entero que va de 20 a 0, siendo 20 la menor prioridad (`MIN_PRIORITY`) y 0 la mayor prioridad posible para un environment (`MAX_PRIORITY`).

Para construirlo, partimos de la base de Round Robin, pero tomando la prioridad como otra variable de ejecución de procesos.

La heurística consiste en partir de la base de que todos los environments arrancan en prioridad máxima (prioridad 0) y se ejecutan ellos a medida que tengan prioridad <= MIN_PRIORITY.

Para que un environment se ejecute deben ocurrir 2 cosas:

- El proceso debe estar en estado `ENV_RUNNABLE`, es decir listo para ser ejecutado.

- la prioridad del environment debe ser menor ó igual que la prioridad "general" que tiene el scheduler.

Esto hace que se ejecuten los environments (procesos) y se reduzca(aumenta numéricamente) la prioridad del environment en 1.

Para evitar que procesos se mantengan en la última prioridad (los llamo PU) y otros siempre se mantengan en las prioridades más altas abriendo la posibilidad a que no se lleguen a ejecutar nunca los procesos PU, es por ello que cada ciertos "ciclos" de llamados al scheduler reseateamos la prioridad de los procesos con la función `priority_reset(void)`.

Establecimos como la cantidad de ciclos máxima antes de resetar prioridades al valor 15 (`CYCLES_FOR_PRIORITY_RESET=15`).

## Ejemplos

### pruebas.c

Máxima prioridad: 0 Mínima prioridad: en nuestro caso es 20
Partimos de la base de Round Robin, vamos recorriendo los procesos de envs, y nos vamos fijando las prioridades. Se ejecutarán aquellos procesos que tengan la prioridad mayor o igual a la prioridad actual. Ejemplo:

Prioridad_actual = 3

Se ejecutarán todos los procesos de envs que estén en estado ready to run y que tengan prioridad <= 3:

PA con prioridad 1: se va a ejecutar PB con prioridad 3: se va a ejecutar PC con prioridad 4: no se va a ejecutar PD con prioridad 0: se va a ejecutar PE con prioridad 8: no se va a ejecutar

Luego de recorrer el array envs, disminuirá la prioridad actual y repite el proceso con la nueva prioridad, en el ejemplo sería 4. De esta forma, evitaremos que los procesos con mayor prioridad esperen un ciclo para ejecutarse.

En el caso de hacer un fork, como se muestra en user/pruebas.c, el proceso hijo tendrá menor prioridad que su padre. Esto evita que fork se transforme en una forma de aumentar la prioridad de un proceso.
