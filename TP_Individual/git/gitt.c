cindy@cindy-VirtualBox:~$ git clone git@github.com:fiubatps/sisop_2023b_g03.git
Clonando en 'sisop_2023b_g03'...
remote: Enumerating objects: 233, done.
remote: Counting objects: 100% (233/233), done.
remote: Compressing objects: 100% (83/83), done.
remote: Total 233 (delta 117), reused 233 (delta 117), pack-reused 0
Recibiendo objetos: 100% (233/233), 35.14 KiB | 8.79 MiB/s, listo.
Resolviendo deltas: 100% (117/117), listo.
cindy@cindy-VirtualBox:~$ git status
fatal: no es un repositorio git (ni ninguno de los directorios superiores): .git
cindy@cindy-VirtualBox:~$ ls
Descargas  Documentos  Escritorio  Imágenes  Música  Plantillas  primes.c  Público  README.md  sisop  sisop_2023b_g03  Vídeos  xargs.c
cindy@cindy-VirtualBox:~$ cd sisop_2023b_g03/
cindy@cindy-VirtualBox:~/sisop_2023b_g03$ git status
En la rama main
Tu rama está actualizada con 'origin/main'.

nada para hacer commit, el árbol de trabajo está limpio
cindy@cindy-VirtualBox:~/sisop_2023b_g03$ git checkout entrega_shell 
Rama 'entrega_shell' configurada para hacer seguimiento a la rama remota 'entrega_shell' de 'origin'.
Cambiado a nueva rama 'entrega_shell'
cindy@cindy-VirtualBox:~/sisop_2023b_g03$ git status
En la rama entrega_shell
Tu rama está actualizada con 'origin/entrega_shell'.

Cambios no rastreados para el commit:
  (usa "git add <archivo>..." para actualizar lo que será confirmado)
  (usa "git restore <archivo>..." para descartar los cambios en el directorio de trabajo)
	modificados:     shell/builtin.c

sin cambios agregados al commit (usa "git add" y/o "git commit -a")
cindy@cindy-VirtualBox:~/sisop_2023b_g03$ git add
Nada especificado, nada agregado.
Tal vez quiso decir 'git add .'?
cindy@cindy-VirtualBox:~/sisop_2023b_g03$ git add .
cindy@cindy-VirtualBox:~/sisop_2023b_g03$ git add builtin.c
fatal: ruta especificada 'builtin.c' no concordó con ningún archivo
cindy@cindy-VirtualBox:~/sisop_2023b_g03$ git status
En la rama entrega_shell
Tu rama está actualizada con 'origin/entrega_shell'.

Cambios a ser confirmados:
  (usa "git restore --staged <archivo>..." para sacar del área de stage)
	modificados:     shell/builtin.c

Cambios no rastreados para el commit:
  (usa "git add <archivo>..." para actualizar lo que será confirmado)
  (usa "git restore <archivo>..." para descartar los cambios en el directorio de trabajo)
	modificados:     shell/builtin.c

cindy@cindy-VirtualBox:~/sisop_2023b_g03$ git restore .
cindy@cindy-VirtualBox:~/sisop_2023b_g03$ git status
En la rama entrega_shell
Tu rama está actualizada con 'origin/entrega_shell'.

Cambios a ser confirmados:
  (usa "git restore --staged <archivo>..." para sacar del área de stage)
	modificados:     shell/builtin.c

cindy@cindy-VirtualBox:~/sisop_2023b_g03$ git restore --staged shell/builtin.c
cindy@cindy-VirtualBox:~/sisop_2023b_g03$ 

