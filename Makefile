compile:
	i686-w64-mingw32-gcc -o malware.exe backdoor.c -lwininet -lwsock32
	gcc -o server server.c
clean:
	rm server malware.exe
