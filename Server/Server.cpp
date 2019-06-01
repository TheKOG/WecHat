#include<bits/stdc++.h>
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
set<SOCKET>lst;
int ClientSum;
void Send(char *buf){
	for(set<SOCKET>::iterator i=lst.begin();i!=lst.end();i++)
	    send(*i,buf,strlen(buf),0);
}
struct DATA{SOCKET skt;sockaddr_in addr;};
void Recv(PVOID pt){
	DATA dt=*(DATA*)pt;
	SOCKET sClient=dt.skt;
	sockaddr_in Addr=dt.addr;
	lst.insert(sClient);
	printf("A new online user:%s,the number of users online:%d\n",inet_ntoa(Addr.sin_addr),++ClientSum);
	char buf[32767],ch[32767+16];
	int len;
	while((len=recv(sClient,buf,32767,0))>0){
		buf[len]='\0';
		wsprintf(ch,"%s:%s%c\n",inet_ntoa(Addr.sin_addr),buf,'\0');
		Send(ch);
	}
	printf("One user goes offline:%s,the number of users online:%d\n",inet_ntoa(Addr.sin_addr),--ClientSum);
	lst.erase(sClient);
	closesocket(sClient);
}
int main(int argc,char* argv[]){
	WORD sockVersion=MAKEWORD(2,2);
	WSADATA wsaData; 
	if(WSAStartup(sockVersion,&wsaData)!=0)return 0;
	SOCKET slisten=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(slisten==INVALID_SOCKET){
		printf("socket error!");
		return 0;
	}
	sockaddr_in sin;
	sin.sin_family=AF_INET;
	sin.sin_port=htons(8888);
	sin.sin_addr.S_un.S_addr=INADDR_ANY; 
	sockaddr_in remoteAddr;
	if(bind(slisten,(LPSOCKADDR)&sin,sizeof(sin))==SOCKET_ERROR)printf("bind error!");
	if(listen(slisten,5)==SOCKET_ERROR)return printf("listen error!"),0;
	puts("server start success...");
	SOCKET sClient;
	int nAddrlen=sizeof(remoteAddr);
	char revData[255];
	while(true){
		sClient=accept(slisten,(SOCKADDR*)&remoteAddr,&nAddrlen);
		if(sClient==INVALID_SOCKET){
			printf("accept error!");
			break;
		}
		DATA dt={sClient,remoteAddr};
		_beginthread(Recv,0,&dt);
	}
	closesocket(slisten);
	WSACleanup();
	return 0;
} 
