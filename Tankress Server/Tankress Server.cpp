#include<iostream>
#include <winsock2.h>
#include<conio.h>
#include<Windows.h>
#include<ctime>
#include <stdlib.h>
#include <process.h>

using namespace std;

#define PORT 23000 /*������ Ŭ���̾�Ʈ���� �����͸� �ְ� ���� ��Ʈ��ȣ*/
#define BUFFER_SIZE 100 

#pragma comment (lib , "ws2_32.lib")
#pragma warning(disable:4996)
HANDLE  mutex;
int clientNumber = 0;
unsigned  threadID;
int allClientSocket[100];

class SK {
private:
	WSADATA wsdata;
	sockaddr_in serverAddress;
	struct sockaddr_in clientAddress;
	int clientAddressSize = sizeof(clientAddress);
	int Sermap[10][10] = { 0, };
	int Coordinate1[3] = { 0, };
	int Coordinate2[3] = { 0, };
	int Coordinate3[3] = { 0, };
	SOCKET servsoc;
	SOCKET clisoc;
	int index[3] = { 0 , };
public:
	SK();
	void Ser_open();
	void print();
	void Ser_gs();
	void con1();
	void con2();
	void con3();
};
SK::SK() {
	Sermap[1][3] = 1;
	Sermap[1][4] = 1;
	Sermap[2][3] = 1;
	Sermap[2][4] = 1;
	Sermap[4][0] = 1;
	Sermap[4][1] = 1;
	Sermap[5][0] = 1;
	Sermap[5][1] = 1;
	Sermap[3][7] = 1;
	Sermap[3][8] = 1;
	Sermap[4][7] = 1;
	Sermap[4][8] = 1;
	Sermap[6][3] = 1;
	Sermap[6][4] = 1;
	Sermap[7][3] = 1;
	Sermap[7][4] = 1;
	Sermap[7][7] = 1;
	Sermap[7][8] = 1;
	Sermap[8][7] = 1;
	Sermap[8][8] = 1;
}
void SK::Ser_open() {
	if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0) {
		cout << "WS2_32.DLL �� �ʱ�ȭ �ϴµ� �����߽��ϴ�. " << endl;
		return;
	}

	servsoc = socket(AF_INET, SOCK_STREAM, 0);

	ZeroMemory(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET; /*������ �ּ� ü�踦 ����.*/
	serverAddress.sin_port = htons(PORT); /*���� ������ ��Ʈ�� ����*/
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);/*������ IP�� ���� */

	if (bind(servsoc, (SOCKADDR*)&serverAddress,
		sizeof(serverAddress)) == SOCKET_ERROR)
	{
		cout << "serverSocket�� IP �� PORT �� �ο� �ϴµ� �����߽��ϴ�! " << endl;
		return;
	}
	if (listen(servsoc, 5) == -1) {
		cout << "listen ��� ����!" << endl;
		return;
	}
	cout << "\n  ��ũ���� ������ ����������ϴ�.\n" << endl;
	while (1) {
		allClientSocket[clientNumber] = accept(servsoc, (struct sockaddr*)&clientAddress, &clientAddressSize);
		cout << "  " << clientNumber + 1 << "��° �÷��̾ ���� �߽��ϴ�.  ���� �ο� : [" << clientNumber + 1 << "/3]" << endl;
		clientNumber++;
		if (clientNumber == 3) {
			cout << "  ���ӽ��� �ο��� �����Ǿ����ϴ�. " << endl;
			break;
		}
	}
	cout << "  ���Ӹ�带 �����Ͻÿ�. 1) ��Ȱ���� 2) �Ȱ���� : ";
	cin >> index[1];
	cout << "\n  ��ũ�̵��ð��� �����Ͻÿ� (500~1000) : ";
	cin >> index[2];
	system("cls");
	for (int i = 0; i < 3; i++) {
		index[i] = ntohl(index[i]);
	}
	for (int i = 0; i < 3; i++) {
		send(allClientSocket[i], (char*)index, sizeof(index), 0);
	}
	for (int i = 0; i < 3; i++) {
		index[i] = ntohl(index[i]);
	}
	cout << "\n  ��ũ������ �����ϰڽ��ϴ�. " << endl;
	Sleep(3000);
}
void SK::con1() {
	recv(allClientSocket[0], (char*)Coordinate1, sizeof(Coordinate1), 0);
	send(allClientSocket[1], (char*)Coordinate1, sizeof(Coordinate1), 0);
	send(allClientSocket[2], (char*)Coordinate1, sizeof(Coordinate1), 0);
	for (int i = 0; i < 3; i++) {
		Coordinate1[i] = ntohl(Coordinate1[i]);
	}
	Sermap[Coordinate1[0]][Coordinate1[1]] = Coordinate1[2];
}
void SK::con2() {
	recv(allClientSocket[1], (char*)Coordinate2, sizeof(Coordinate2), 0);
	send(allClientSocket[0], (char*)Coordinate2, sizeof(Coordinate2), 0);
	send(allClientSocket[2], (char*)Coordinate2, sizeof(Coordinate2), 0);
	for (int i = 0; i < 3; i++) {
		Coordinate2[i] = ntohl(Coordinate2[i]);
	}
	Sermap[Coordinate2[0]][Coordinate2[1]] = Coordinate2[2];
}
void SK::con3() {
	recv(allClientSocket[2], (char*)Coordinate3, sizeof(Coordinate3), 0);
	send(allClientSocket[0], (char*)Coordinate3, sizeof(Coordinate3), 0);
	send(allClientSocket[1], (char*)Coordinate3, sizeof(Coordinate3), 0);
	for (int i = 0; i < 3; i++) {
		Coordinate3[i] = ntohl(Coordinate3[i]);
	}
	Sermap[Coordinate3[0]][Coordinate3[1]] = Coordinate3[2];
}
void SK::Ser_gs() {
	Ser_open();
	while (1) {
		con1();
		con2();
		con3();
		print();
	}
}

void SK::print() {
	system("cls");
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (Sermap[i][j] == 0) cout << "��";
			if (Sermap[i][j] == 1) cout << "��";
			if (Sermap[i][j] == 2) cout << "��";
			if (Sermap[i][j] == 3) cout << "��";
			if (Sermap[i][j] == 4) cout << "��";
			if (Sermap[i][j] == 5) cout << "��";
		}
		cout << endl;
	}
	Sermap[Coordinate2[0]][Coordinate2[1]] = 0;
	Sermap[Coordinate1[0]][Coordinate1[1]] = 0;
	Sermap[Coordinate3[0]][Coordinate3[1]] = 0;
}
int main() {
	SK b;

	b.Ser_gs();

}