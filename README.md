# Proyecto 1 de Sistemas Operativos 

#### Integrantes:
- Maria José Castillo Acuña.
- Francy Pilar Jelvez Jen.
- Pedro Ignacio Palacios Rossi.
- Diego Venegas Anabalón.

##### Compilación y Ejecución:
Para compilar la Shell utilizar: g++ -o Shellcito Shell.cpp \
Para ejecutar la Shell utilizar: ./Shell

##### Comandos internos de Shellcito:
- exit: Cierra la shell, no requiere ningún argumento.
- fav 'arg1' 'args': Almacena los comando utilizados.
arg1 puede ser:
    - "crear", donde args es la ruta de un archivo en formato "ruta/misfavoritos.txt". Crea archivo donde se almacenan los comandos favoritos
    dada la ruta y nombre de archivo.
    - "eliminar", donde args es "n1,n2", donde n1 y n2 cn numeros.  Elimina comandos asociados a los números entregados.
    - "mostrar", no tiene args. Despliega la lista comandos existentes en la lista con su respectivo número.
    - "buscar", args una palabra. Busca comandos que contengan substring args en la lista de favoritos y los despliega en pantalla junto con su número asociado.
    - "borrar", no tiene args. Borra todos los comandos en la lista de favoritos.
    - "num", args=ejecutar. Ejecuta el comando, cuyo número en la lista es num.
    - "cargar", no tiene args. Lee comandos de archivo de favoritos, los mantiene en memoria y los despliega en pantalla.
    - "guardar", no tiene args. Guarda comandos agregados en sesión de shell actual. 
- set recordatorio 'n' 'mensaje':
Permite programar un recordatorio que se realizara en n segundos, además, todo lo que le siga a 'n' formara parte del mensaje del recordatorio.

##### Comandos externos:
Shellcito permite uso de comandos como lo son: ls, ps, sort, head, etc. Además, también permite el uso de pipes "|".
