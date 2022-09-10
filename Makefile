all: jdate	


jdate: jdate.c
	gcc -std=gnu99 -Wall $< -o $@

clean:
	rm -f client server *.o


















