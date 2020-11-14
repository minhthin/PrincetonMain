all: ft

ft: dynarray.o node.o checker_ft.o ft.o ft_client.c
	gcc217 -g dynarray.o node.o checker_ft.o ft.o ft_client.c -o ft 

checker_ft.o: checker_ft.c dynarray.h checker_ft.h node.h a4def.h
	gcc217 -g -c checker_ft.c 

dynarray.o: dynarray.c dynarray.h
	gcc217 -g -c dynarray.c

ft.o: ft.c dynarray.h ft.h a4def.h node.h checker_ft.h
	gcc217 -g -c ft.c

node.o: node.c dynarray.h node.h a4def.h
	gcc217 -g -c node.c