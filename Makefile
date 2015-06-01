1337-617: main.c 1337-617.h 1337-617.c util.h
	gcc -o 1337-617.out main.c -I. -Wall

.PHONY: continuously
continuously:
	while true; do make 1>/dev/null; sleep 2; done