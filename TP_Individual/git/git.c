cindy@cindy-VirtualBox:~/fork$ cd ..
cindy@cindy-VirtualBox:~$ git clone git@github.com:fiubatps/sisop_2023b_hsieh.git sisop
Clonando en 'sisop'...
remote: Enumerating objects: 2, done.
remote: Counting objects: 100% (2/2), done.
remote: Total 2 (delta 0), reused 2 (delta 0), pack-reused 0
Recibiendo objetos: 100% (2/2), listo.
cindy@cindy-VirtualBox:~$ cd sisop
cindy@cindy-VirtualBox:~/sisop$ git remote add https://github.com/fisop/fork
uso: git remote add [<opciones>] <nombre> <url>

    -f, --fetch           realizar fetch a las ramas remotas
    --tags                importar todos los tags y objetos asociados cuando realiza el fetch
                          o no realizar fetch a ningún tag (--no-tags)
    -t, --track <rama>    rama(s) para rastrear
    -m, --master <rama>   rama master
    --mirror[=(push|fetch)]
                          configurar remote como mirror al cual empujar o desde el cual realizar fetch

cindy@cindy-VirtualBox:~/sisop$ git remote add catedra https://github.com/fisop/fork
cindy@cindy-VirtualBox:~/sisop$ git checkout -b base_fork
Cambiado a nueva rama 'base_fork'
cindy@cindy-VirtualBox:~/sisop$ git branch
* base_fork
  main
cindy@cindy-VirtualBox:~/sisop$ git push -u origin base_fork
Total 0 (delta 0), reusado 0 (delta 0)
remote: 
remote: Create a pull request for 'base_fork' on GitHub by visiting:
remote:      https://github.com/fiubatps/sisop_2023b_hsieh/pull/new/base_fork
remote: 
To github.com:fiubatps/sisop_2023b_hsieh.git
 * [new branch]      base_fork -> base_fork
Rama 'base_fork' configurada para hacer seguimiento a la rama remota 'base_fork' de 'origin'.
cindy@cindy-VirtualBox:~/sisop$ git fetch --all
Extrayendo origin
Extrayendo catedra
warning: no hay commits comunes
remote: Enumerating objects: 63, done.
remote: Total 63 (delta 0), reused 0 (delta 0), pack-reused 63
Desempaquetando objetos: 100% (63/63), 14.57 KiB | 244.00 KiB/s, listo.
Desde https://github.com/fisop/fork
 * [nueva rama]      main         -> catedra/main
 * [nueva rama]      sundry_fixes -> catedra/sundry_fixes
 * [nueva rama]      systemd_run  -> catedra/systemd_run
cindy@cindy-VirtualBox:~/sisop$ git merge catedra/main --allow-unrelated-histories
ayuda: Esperando que tu editor cierre el archivo ... 

Use «fg» para volver a nano.

[1]+  Detenido                git merge catedra/main --allow-unrelated-histories
cindy@cindy-VirtualBox:~/sisop$ git merge catedra/main --allow-unrelated-histories
fatal: No has concluido la fusión (existe MERGE_HEAD).
Por favor, realiza un commit con los cambios antes de fusionar.
cindy@cindy-VirtualBox:~/sisop$ fg
git merge catedra/main --allow-unrelated-histories
Merge made by the 'recursive' strategy.
 .clang-files                            |   2 +
 .clang-format                           |  30 ++++++++++++
 .dockerignore                           |   8 ++++
 .github/workflows/pre-commit-check.yaml |  13 +++++
 .gitignore                              |   5 ++
 .pre-commit-config.yaml                 |  12 +++++
 Dockerfile                              |  11 +++++
 LICENSE                                 |  21 ++++++++
 Makefile                                |  21 ++++++++
 README.md                               |  21 ++++++++
 primes.c                                |   7 +++
 tests/argcounter.py                     |  10 ++++
 tests/primes-test.py                    | 149 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 tests/run                               |  12 +++++
 tests/test-fork                         |  70 +++++++++++++++++++++++++++
 tests/utils.py                          |  36 ++++++++++++++
 tests/xargs-test.py                     | 121 ++++++++++++++++++++++++++++++++++++++++++++++
 xargs.c                                 |  11 +++++
 18 files changed, 560 insertions(+)
 create mode 100644 .clang-files
 create mode 100644 .clang-format
 create mode 100644 .dockerignore
 create mode 100644 .github/workflows/pre-commit-check.yaml
 create mode 100644 .gitignore
 create mode 100644 .pre-commit-config.yaml
 create mode 100644 Dockerfile
 create mode 100644 LICENSE
 create mode 100644 Makefile
 create mode 100644 README.md
 create mode 100644 primes.c
 create mode 100755 tests/argcounter.py
 create mode 100755 tests/primes-test.py
 create mode 100755 tests/run
 create mode 100755 tests/test-fork
 create mode 100644 tests/utils.py
 create mode 100755 tests/xargs-test.py
 create mode 100644 xargs.c
cindy@cindy-VirtualBox:~/sisop$ git push origin base_fork
Enumerando objetos: 48, listo.
Contando objetos: 100% (48/48), listo.
Compresión delta usando hasta 2 hilos
Comprimiendo objetos: 100% (41/41), listo.
Escribiendo objetos: 100% (47/47), 9.97 KiB | 2.49 MiB/s, listo.
Total 47 (delta 10), reusado 0 (delta 0)
remote: Resolving deltas: 100% (10/10), done.
To github.com:fiubatps/sisop_2023b_hsieh.git
   d8e5f41..2bbd5d7  base_fork -> base_fork
cindy@cindy-VirtualBox:~/sisop$ git checkout -b entrega_fork
Cambiado a nueva rama 'entrega_fork'
cindy@cindy-VirtualBox:~/sisop$ git push -u origin entrega_fork
Total 0 (delta 0), reusado 0 (delta 0)
remote: 
remote: Create a pull request for 'entrega_fork' on GitHub by visiting:
remote:      https://github.com/fiubatps/sisop_2023b_hsieh/pull/new/entrega_fork
remote: 
To github.com:fiubatps/sisop_2023b_hsieh.git
 * [new branch]      entrega_fork -> entrega_fork
Rama 'entrega_fork' configurada para hacer seguimiento a la rama remota 'entrega_fork' de 'origin'.
cindy@cindy-VirtualBox:~/sisop$ git status
En la rama entrega_fork
Tu rama está actualizada con 'origin/entrega_fork'.

Cambios no rastreados para el commit:
  (usa "git add <archivo>..." para actualizar lo que será confirmado)
  (usa "git restore <archivo>..." para descartar los cambios en el directorio de trabajo)
	modificados:     primes.c
	modificados:     xargs.c

sin cambios agregados al commit (usa "git add" y/o "git commit -a")
cindy@cindy-VirtualBox:~/sisop$ make test
./tests/run /home/cindy/sisop
DEPRECATED: The legacy builder is deprecated and will be removed in a future release.
            Install the buildx component to build images with BuildKit:
            https://docs.docker.com/go/buildx/

Sending build context to Docker daemon  204.3kB
Step 1/7 : FROM python:3.10-alpine
 ---> 532546851701
Step 2/7 : RUN apk update && apk add --no-cache make gcc build-base valgrind
 ---> Using cache
 ---> 3d9593da2937
Step 3/7 : RUN adduser -D fisop
 ---> Using cache
 ---> 475f080be2dd
Step 4/7 : USER fisop
 ---> Using cache
 ---> 5304d446ceaf
Step 5/7 : ENV PATH="/home/fisop/.local/bin:${PATH}"
 ---> Using cache
 ---> 681de85a3714
Step 6/7 : WORKDIR /fork
 ---> Using cache
 ---> aab456086a5f
Step 7/7 : COPY . .
 ---> f61c64cca557
Successfully built f61c64cca557
Successfully tagged test-fork-runner:latest
rm -f primes xargs *.o core vgcore.*
cc -ggdb3 -O2 -Wall -Wextra -std=c11 -Wvla -D_DEFAULT_SOURCE  -c -o primes.o primes.c
cc   primes.o   -o primes
cc -ggdb3 -O2 -Wall -Wextra -std=c11 -Wvla -D_DEFAULT_SOURCE  -c -o xargs.o xargs.c
xargs.c: In function 'main':
xargs.c:11:14: warning: unused parameter 'argc' [-Wunused-parameter]
   11 | int main(int argc, char *argv[]) {
      |          ~~~~^~~~
cc   xargs.o   -o xargs
COMMAND: xargs
packaging arguments [ARGS IN PACKAGE: 4]
  [ARGS SENT] - less: OK
  [ARGS SENT] - same: OK
  [ARGS SENT] - more: OK
  [ARGS SENT] - twice: OK
  [ARGS SENT] - above twice: OK
5/5 passed

COMMAND: primes
  correct primes up to 10: OK
  correct primes up to 100: OK
  correct primes up to 1000: OK
  correct primes up to 10000: FAIL

Prime numbers missing:
---------------------
{9767, 9769, 9781, 9787, 9791, 9803, 9811, 9817, 9829, 9833, 9839, 9851, 9857, 9859, 9871, 9883, 9887, 9901, 9907, 9923, 9929, 9931, 9941, 9949, 9967, 9973}
            
3/4 passed
rm -f primes xargs *.o core vgcore.*
cindy@cindy-VirtualBox:~/sisop$ make format
xargs -r clang-format -i <.clang-files
xargs: clang-format: No existe el archivo o el directorio
make: *** [Makefile:16: format] Error 127
cindy@cindy-VirtualBox:~/sisop$ git add .
cindy@cindy-VirtualBox:~/sisop$ git status
En la rama entrega_fork
Tu rama está actualizada con 'origin/entrega_fork'.

Cambios a ser confirmados:
  (usa "git restore --staged <archivo>..." para sacar del área de stage)
	modificados:     primes.c
	modificados:     xargs.c

cindy@cindy-VirtualBox:~/sisop$ git commit -m "entrega fork"
[entrega_fork 3559725] entrega fork
 2 files changed, 174 insertions(+), 18 deletions(-)
 rewrite primes.c (98%)
 rewrite xargs.c (61%)
cindy@cindy-VirtualBox:~/sisop$ git push
Enumerando objetos: 7, listo.
Contando objetos: 100% (7/7), listo.
Compresión delta usando hasta 2 hilos
Comprimiendo objetos: 100% (4/4), listo.
Escribiendo objetos: 100% (4/4), 1.35 KiB | 1.35 MiB/s, listo.
Total 4 (delta 1), reusado 0 (delta 0)
remote: Resolving deltas: 100% (1/1), completed with 1 local object.
To github.com:fiubatps/sisop_2023b_hsieh.git
   2bbd5d7..3559725  entrega_fork -> entrega_fork
cindy@cindy-VirtualBox:~/sisop$ sudo apt install clang-format
[sudo] contraseña para cindy:       
Leyendo lista de paquetes... Hecho
Creando árbol de dependencias       
Leyendo la información de estado... Hecho
Se instalarán los siguientes paquetes adicionales:
  clang-format-10 libclang-cpp10 libllvm10
Se instalarán los siguientes paquetes NUEVOS:
  clang-format clang-format-10 libclang-cpp10 libllvm10
0 actualizados, 4 nuevos se instalarán, 0 para eliminar y 19 no actualizados.
Se necesita descargar 25,3 MB de archivos.
Se utilizarán 119 MB de espacio de disco adicional después de esta operación.
¿Desea continuar? [S/n] s
Des:1 http://archive.ubuntu.com/ubuntu focal/main amd64 libllvm10 amd64 1:10.0.0-4ubuntu1 [15,3 MB]
Des:2 http://archive.ubuntu.com/ubuntu focal/universe amd64 libclang-cpp10 amd64 1:10.0.0-4ubuntu1 [9.944 kB]
Des:3 http://archive.ubuntu.com/ubuntu focal/universe amd64 clang-format-10 amd64 1:10.0.0-4ubuntu1 [40,9 kB]
Des:4 http://archive.ubuntu.com/ubuntu focal/universe amd64 clang-format amd64 1:10.0-50~exp1 [3.272 B]
Descargados 25,3 MB en 5s (4.666 kB/s)   
Seleccionando el paquete libllvm10:amd64 previamente no seleccionado.
(Leyendo la base de datos ... 334218 ficheros o directorios instalados actualmente.)
Preparando para desempaquetar .../libllvm10_1%3a10.0.0-4ubuntu1_amd64.deb ...
Desempaquetando libllvm10:amd64 (1:10.0.0-4ubuntu1) ...
Seleccionando el paquete libclang-cpp10 previamente no seleccionado.
Preparando para desempaquetar .../libclang-cpp10_1%3a10.0.0-4ubuntu1_amd64.deb ...
Desempaquetando libclang-cpp10 (1:10.0.0-4ubuntu1) ...
Seleccionando el paquete clang-format-10 previamente no seleccionado.
Preparando para desempaquetar .../clang-format-10_1%3a10.0.0-4ubuntu1_amd64.deb ...
Desempaquetando clang-format-10 (1:10.0.0-4ubuntu1) ...
Seleccionando el paquete clang-format previamente no seleccionado.
Preparando para desempaquetar .../clang-format_1%3a10.0-50~exp1_amd64.deb ...
Desempaquetando clang-format (1:10.0-50~exp1) ...
Configurando libllvm10:amd64 (1:10.0.0-4ubuntu1) ...
Configurando libclang-cpp10 (1:10.0.0-4ubuntu1) ...
Configurando clang-format-10 (1:10.0.0-4ubuntu1) ...
Configurando clang-format (1:10.0-50~exp1) ...
Procesando disparadores para man-db (2.9.1-1) ...
Procesando disparadores para libc-bin (2.31-0ubuntu9.9) ...
cindy@cindy-VirtualBox:~/sisop$ make format
xargs -r clang-format -i <.clang-files
cindy@cindy-VirtualBox:~/sisop$ git status
En la rama entrega_fork
Tu rama está actualizada con 'origin/entrega_fork'.

Cambios no rastreados para el commit:
  (usa "git add <archivo>..." para actualizar lo que será confirmado)
  (usa "git restore <archivo>..." para descartar los cambios en el directorio de trabajo)
	modificados:     primes.c
	modificados:     xargs.c

sin cambios agregados al commit (usa "git add" y/o "git commit -a")
cindy@cindy-VirtualBox:~/sisop$ git add .
cindy@cindy-VirtualBox:~/sisop$ git commit -m "format files"
[entrega_fork afe7e97] format files
 2 files changed, 175 insertions(+), 174 deletions(-)
 rewrite primes.c (93%)
 rewrite xargs.c (89%)
cindy@cindy-VirtualBox:~/sisop$ git push
Enumerando objetos: 7, listo.
Contando objetos: 100% (7/7), listo.
Compresión delta usando hasta 2 hilos
Comprimiendo objetos: 100% (4/4), listo.
Escribiendo objetos: 100% (4/4), 1.29 KiB | 1.29 MiB/s, listo.
Total 4 (delta 1), reusado 0 (delta 0)
remote: Resolving deltas: 100% (1/1), completed with 1 local object.
To github.com:fiubatps/sisop_2023b_hsieh.git
   3559725..afe7e97  entrega_fork -> entrega_fork
cindy@cindy-VirtualBox:~/sisop$ 

