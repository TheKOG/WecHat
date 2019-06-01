#include<WINSOCK2.H>
#include<bits/stdc++.h>
#include<windows.h>
#define Width 640
#define Height 480
using namespace std;
#pragma comment(lib,"ws2_32.lib")
void Init(HWND hwnd);
void Run(HWND hwnd);
void Paint(HWND hwnd);
HWND Edit,Static,IP,Enter;
HDC WinMap,dWinMap;
HBITMAP dbm;
HBRUSH black,red,green,blue,white,grey,dark;
HPEN Black,White,Green,Red,Blue;
void copy(char *cD){
	int len=strlen(cD)+1;
	if(OpenClipboard(NULL)){
		HGLOBAL hmem=GlobalAlloc(GHND,len);
		char *pmem=(char*)GlobalLock(hmem);
		EmptyClipboard();
		memcpy(pmem,cD,len);
		SetClipboardData(CF_TEXT,hmem);
		CloseClipboard();
		GlobalFree(hmem);
	}
}
SOCKET makeSocket(char *ch,int port){
	WORD sockVersion=MAKEWORD(2,2);
	WSADATA data;
	if(WSAStartup(sockVersion,&data)!=0)return 0;
	SOCKET skt=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	sockaddr_in serAddr;
	serAddr.sin_family=AF_INET;
	serAddr.sin_port=htons(port);
	serAddr.sin_addr.S_un.S_addr=inet_addr(ch);
	if(connect(skt,(sockaddr*)&serAddr,sizeof(serAddr))==SOCKET_ERROR)return -1;
	return skt;
}
struct BUFFER{
	char ch[32767];
	int head[32767],sum,len[32767];
	void format(){head[0]=sum=0;}
	void ins(char *a){
		head[0]=0,len[sum]=strlen(a);
		for(int i=head[sum],j=0;j<len[sum];j++,i++)ch[i]=a[j];
		++sum,head[sum]=head[sum-1]+len[sum-1];
	}
	char* str(int num){return ch+head[num];}
}Data;
SOCKET sServer;
bool off,OFF;
void Main(PVOID pt){
	OFF=false;
	char* ch=(char*)pt;
	while((!OFF)&&(sServer=makeSocket(ch,8888))==-1);
	char buf[256];
	int len;
	off=false;
	while((len=recv(sServer,buf,256,0))>0){
		buf[len]=0x00;
		Data.ins(buf);
	}
	closesocket(sServer);
	WSACleanup();
	Data.format();
	off=true;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	HWND hwnd;
	MSG msg;
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc;
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); 
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); 
	if(!RegisterClassEx(&wc)) {MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);return 0;}
	hwnd=CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Caption",WS_VISIBLE|WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,Width+20,Height+43,NULL,NULL,hInstance,NULL);
	if(hwnd==NULL){MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);return 0;}
	Init(hwnd);
	_beginthread(Main,0,(char*)"192.168.2.18");
	while(msg.message!=WM_QUIT){
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else{
			Run(hwnd);
			Paint(hwnd);
		}
	}
	return msg.wParam;
}
void Run(HWND hwnd){
	
}
void Paint(HWND hwnd){
	Rectangle(dWinMap,-1,-1,Width+1,Height+1);
	RECT rct={0,Height-155,Width,Height-130};
	for(int i=Data.sum-1;i>=0;i--){
		DrawText(dWinMap,Data.str(i),Data.len[i],&rct,DT_SINGLELINE|DT_BOTTOM|DT_LEFT);
		rct.top-=25,rct.bottom-=25;
	}
	BitBlt(WinMap,0,20,Width,Height-120,dWinMap,0,0,SRCCOPY);
}
void Init(HWND hwnd){
    black=CreateSolidBrush(RGB(0,0,0));
    grey=CreateSolidBrush(RGB(200,200,200));
	red=CreateSolidBrush(RGB(255,0,0));
	green=CreateSolidBrush(RGB(0,255,0));
	blue=CreateSolidBrush(RGB(0,0,255));
	white=CreateSolidBrush(RGB(255,255,255));
	Black=CreatePen(PS_SOLID,0,RGB(0,0,0));
	White=CreatePen(PS_SOLID,0,RGB(255,255,255));
	Green=CreatePen(PS_SOLID,5,RGB(0,255,0));
	Red=CreatePen(PS_SOLID,5,RGB(255,0,0));
	Blue=CreatePen(PS_SOLID,5,RGB(0,0,255));
	WinMap=GetDC(hwnd);
	dWinMap=CreateCompatibleDC(WinMap);
	dbm=CreateCompatibleBitmap(WinMap,Width,Height);
	SelectObject(dWinMap,dbm);
	SelectObject(dWinMap,grey);
	SelectObject(dWinMap,White);
	Rectangle(dWinMap,-1,-1,Width+1,Height+1);
	SetBkMode(dWinMap,TRANSPARENT);
	HINSTANCE Hins;
	Edit=CreateWindow("edit","",WS_CHILD|WS_VISIBLE,0,Height-100,Width-35,100,hwnd,(HMENU)1,Hins,NULL);
	IP=CreateWindow("edit","192.168.2.18",WS_CHILD|WS_VISIBLE,0,0,Width-60,20,hwnd,(HMENU)2,Hins,NULL);
	Static=CreateWindow("Static","Send",WS_CHILD|WS_VISIBLE,Width-35,Height-20,35,20,hwnd,(HMENU)3,Hins,NULL);
	Enter=CreateWindow("Static","Connect",WS_CHILD|WS_VISIBLE,Width-60,0,60,20,hwnd,(HMENU)4,Hins,NULL);
}
char str[32767];
LRESULT CALLBACK WndProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam){
	switch(Message){
		case WM_DESTROY:{PostQuitMessage(0);break;}
		case WM_LBUTTONDOWN:{
			int x=LOWORD(lParam),y=HIWORD(lParam);
			if(x>=Width-35&&y>=Height-20){
				SendMessage(Edit,WM_GETTEXT,32767,(LPARAM)str);
				SendMessage(Edit,WM_SETTEXT,0,0);
	        	send(sServer,str,strlen(str),0);
			}
			if(x>=Width-60&&y<=20){
				OFF=true;
				closesocket(sServer);
				WSACleanup();
				Data.format();
				SendMessage(IP,WM_GETTEXT,32767,(LPARAM)str);
				while(!off);
				off=false;
				_beginthread(Main,0,str);
			}
			break;
		}
		default:return DefWindowProc(hwnd,Message,wParam,lParam);
	}
	return 0;
}

