#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"") //程序后台跑

#include <stdio.h>
#include <WinSock2.h>
#include <sys\stat.h>
#include <cstdio>
#include <ctime>
#include <time.h>

int main()
{
	WSADATA  wsaData;//是指向WSADATA结构的结构指针	
	WORD    wVersionRequested = MAKEWORD(1, 1);// word is unsigned short    wVersionRequested用于制定加载winsock库的版本，MAKEWORD(a,b)a是高位字节，b是地位字节	
	int err = WSAStartup(wVersionRequested, &wsaData);//用于初始化网络资源，并建立一个套接字库	
	if (err != 0) return 0;

	char *pUserName;
	char caUserName[100];
	char caTemp[100];
	char caMTime[50]; //文件最后修改时间
	char caBuffer[1024];

	//----------------------给服务器发送HTTP请求--------------------//
	int m_socket;
	struct sockaddr_in address;
	char sendBuf[10024];
	char recvBuf[10024];

	FILE *fp;
	fp = fopen("1.jpg", "wb");//要下载什么文件，就open什么文件

	memset(recvBuf, 0, sizeof(recvBuf));
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == -1)
	{
		memset(caBuffer, 0, sizeof(caBuffer));
		strcpy(caBuffer, "Create socket Faile !!");
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("58.57.27.129");//换成服务器IP
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(80);//换成服务器端口

	if (connect(m_socket, (struct sockaddr *)&addrSrv, sizeof(addrSrv)) < 0)
	{
		FILE *fp2;
		fp2 = fopen("c:\\ErrLog.log", "w");

		memset(caBuffer, 0, sizeof(caBuffer));
		strcpy(caBuffer, "Connect Server Faile !!");
		fwrite(caBuffer, strlen(caBuffer), 1, fp2);

		fclose(fp2);
	}

	memset(sendBuf, 0, sizeof(sendBuf));

	strcpy(sendBuf, "GET http://img.jiaodong.net/pic/0/10/68/36/10683662_739110.jpg HTTP/1.1\r\nHost:img.jiaodong.net\r\nConnection: Close\r\n\r\n");
	

	int temp1 = send(m_socket, sendBuf, strlen(sendBuf), 0);

	while (1)
	{
		memset(recvBuf, 0, sizeof(recvBuf));
		int iRecv = recv(m_socket, recvBuf, 10023, 0);
		if (iRecv <= 0)
		{
			break;
		}
		recvBuf[iRecv] = '\0';

		char *p;
		p = strstr(recvBuf, "\r\n\r\n");
		if (p == NULL)
		{
			p = &recvBuf[0];
		}
		else
		{
			p = p + 4;
		}
		fwrite(p, sizeof(char), recvBuf + iRecv - p, fp);
	}

	fclose(fp);
	closesocket(m_socket);
	WSACleanup();
}
