# Pruebas de integración

## Prueba sobre archivo
Creacion, escritura(redireccion y sobre-escritura), lectura y eliminacion de archivos

```bash
> touch file.txt
> ls
file.txt
> echo "hola" > file.txt
> cat file.txt
hola
> echo "chau" >> file.txt
> cat file.txt
hola
chau
> echo "hola mundo" > file.txt
> cat file.txt
"hola mundo"
> rm file.txt
> ls
> echo "hola" > file2.txt
> ls
file2.txt
> cat file2.txt
hola
> unlink file2.txt
> ls

```

## Prueba sobre directorio
Creacion y eliminacion de directorios
```bash

> mkdir directorio
ls
directorio
> mkdir directorio2
> ls
directorio  directorio2
> rmdir directorio
> ls
directorio2
```

## Pruebas conjunta entre directorios y archivos

```bash
> touch file.txt
> mkdir directorio
> ls
directorio file.txt 
> touch directorio/file2.txt
> ls
directorio file.txt 
> ls directorio
file2.txt
> echo "hola" > directorio/file2.txt
> cat directorio/file2.txt
hola
> echo "chau" >> directorio/file2.txt
> cat directorio/file2.txt
hola
chau
> rmdir directorio
rmdir: failed to remove 'directorio': Directory not empty
> rm directorio/file2.txt
> ls directorio
> rmdir directorio
> ls
> file.txt
```
## Pruebas de persistencia de datos y stat

```bash
> touch file.txt
> mkdir directorio
> echo "hola" > directorio/file2.txt
> ls
directorio file.txt
> ls directorio
file2.txt
> stat file.txt
  File: file.txt
  Size: 0               Blocks: 8          IO Block: 4096   regular empty file
Device: 52h/82d Inode: 2           Links: 1
Access: (0644/-rw-r--r--)  Uid: (    0/    root)   Gid: (    0/    root)
Access: 2023-11-27 01:31:23.000000000 +0000
Modify: 2023-11-27 01:31:23.000000000 +0000
Change: 2023-11-27 01:31:23.000000000 +0000
 Birth: -
> umount prueba
```

Vuelvo a montar el file system

```bash
> ls
directorio file.txt
> ls directorio
file2.txt
> cat directorio/file2.txt
hola
> stat file.txt
  File: file.txt
  Size: 0               Blocks: 8          IO Block: 4096   regular empty file
Device: 52h/82d Inode: 2           Links: 1
Access: (0644/-rw-r--r--)  Uid: (    0/    root)   Gid: (    0/    root)
Access: 2023-11-27 01:31:23.000000000 +0000
Modify: 2023-11-27 01:31:23.000000000 +0000
Change: 2023-11-27 01:31:23.000000000 +0000
 Birth: -

```
