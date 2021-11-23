#include<iostream>
#include <winsock2.h>
#include<conio.h>
#include<Windows.h>
#include<ctime>
#include <stdlib.h>
#include <process.h>

using namespace std;

#define PORT 23000 /*서버와 클라이언트간에 데이터를 주고 받을 포트번호*/
#define BUFFER_SIZE 100 

#pragma comment (lib , "ws2_32.lib")
#pragma warning(disable:4996)
HANDLE  mutex;
unsigned  threadID;


class SK {
private:
	WSADATA wsdata;
	sockaddr_in serverAddress;
	struct sockaddr_in clientAddress;
	int clientAddressSize = sizeof(clientAddress);
	int Sermap[10][10] = { 0, };
	int Coordinate1[5] = { 0, };
	int Coordinate2[5] = { 0, };
	int Coordinate3[5] = { 0, };
	int clientNumber = 0;
	int player[4] = { 0, };
	int allClientSocket[100];
	int we[1] = { 0, };
	SOCKET servsoc;
	SOCKET clisoc;
	int index[3] = { 0 , };
	int y, z, q;
public:
	SK();
	void Ser_open();
	void print();
	void Ser_gs();
	void con1();
	void con2();
	void con3();
	void trans();
	~SK();
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
		cout << "WS2_32.DLL 을 초기화 하는데 실패했습니다. " << endl;
		return;
	}

	servsoc = socket(AF_INET, SOCK_STREAM, 0);

	ZeroMemory(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET; /*소켓의 주소 체계를 설정.*/
	serverAddress.sin_port = htons(PORT); /*서버 소켓의 포트를 설정*/
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);/*서버의 IP를 설정 */

	if (bind(servsoc, (SOCKADDR*)&serverAddress,
		sizeof(serverAddress)) == SOCKET_ERROR)
	{
		cout << "serverSocket에 IP 와 PORT 를 부여 하는데 실패했습니다! " << endl;
		return;
	}
	if (listen(servsoc, 3) == -1) {
		cout << "listen 모드 실패!" << endl;
		return;
	}
	cout << "\n  탱크리스 서버가 만들어졌습니다.\n" << endl;
	while (1) {
		allClientSocket[clientNumber] = accept(servsoc, (struct sockaddr*)&clientAddress, &clientAddressSize);
		cout << "  " << clientNumber + 1 << "번째 플레이어가 접속 했습니다.  서버 인원 : [" << clientNumber + 1 << "/3]" << endl;
		player[0] = clientNumber + 1;
		if (clientNumber == 0) {
			player[1] = 8;
			player[2] = 0;
			player[3] = 2;
		}
		if (clientNumber == 1) {
			player[1] = 6;
			player[2] = 9;
			player[3] = 3;
		}
		if (clientNumber == 2) {
			player[1] = 0;
			player[2] = 5;
			player[3] = 5;
		}
		for (int i = 0; i < 4; i++) {
			player[i] = ntohl(player[i]);
		}
		send(allClientSocket[clientNumber], (char*)player, sizeof(player), 0);
		clientNumber++;
		if (clientNumber == 3) {
			cout << "  게임시작 인원이 충족되었습니다. " << endl;
			break;
		}
	}
	while(1) {
		cout << "  게임모드를 설정하시오. 1) 비안개모드 2) 안개모드 : ";
		cin >> index[0];
		if ((index[0] == 1 && index[0] != 2) || (index[0] != 1 && index[0] == 2)) {
			break;
		}
		else {
			cout << "  게임모드는 1) 과 2) 중에 다시 선택하세요. \n";
		}
	}
	while (1) {
		cout << "  탱크이동시간을 설정하시오 (500~1000) : ";
		cin >> index[1];
		if (index[1] >= 500 && index[1] <=1000) {
			break;
		}
		else {
			cout << "  탱크이동시간은 500~1000 사이 입니다. 다시 설정하세요. \n";
		}
	}
	system("cls");
	for (int i = 0; i < 2; i++) {
		index[i] = ntohl(index[i]);
	}
	for (int i = 0; i < 3; i++) {
		send(allClientSocket[i], (char*)index, sizeof(index), 0);
	}
	for (int i = 0; i < 2; i++) {
		index[i] = ntohl(index[i]);
	}
	cout << "\n  탱크리스를 시작하겠습니다. " << endl;
	Sleep(3000);
}
void SK::con1() {
	y =recv(allClientSocket[0], (char*)Coordinate1, sizeof(Coordinate1), 0);
	if (y == -1) return;
	trans();
	send(allClientSocket[0], (char*)we, sizeof(we), 0);
	send(allClientSocket[1], (char*)Coordinate1, sizeof(Coordinate1), 0);
	send(allClientSocket[2], (char*)Coordinate1, sizeof(Coordinate1), 0);
	for (int i = 0; i < 4; i++) {
		Coordinate1[i] = ntohl(Coordinate1[i]);
	}
	if (Coordinate1[3] == 1) {
		if (Coordinate1[2] == 2) {
			for (int i = 1; i < 4; i++) {
				if (Coordinate1[1] + i > 9 || Sermap[Coordinate1[0]][Coordinate1[1] + i] == 1) {
					break;
				}
				Sermap[Coordinate1[0]][Coordinate1[1] + i] = 6;
			}
		}
		if (Coordinate1[2] == 3) {
			for (int i = 1; i < 4; i++) {
				if (Coordinate1[1] - i < 0 || Sermap[Coordinate1[0]][Coordinate1[1] - i] == 1) {
					break;
				}
				Sermap[Coordinate1[0]][Coordinate1[1] - i] = 6;
			}
		}
		if (Coordinate1[2] == 4) {
			for (int i = 1; i < 4; i++) {
				if (Coordinate1[0] - i < 0 || Sermap[Coordinate1[0] - i][Coordinate1[1]] == 1) {
					break;
				}
				Sermap[Coordinate1[0] - i][Coordinate1[1]] = 7;
			}
		}
		if (Coordinate1[2] == 5) {
			for (int i = 1; i < 4; i++) {
				if (Coordinate1[0] + i > 9 || Sermap[Coordinate1[0] + i][Coordinate1[1]] == 1) {
					break;
				}
				Sermap[Coordinate1[0] + i][Coordinate1[1]] = 7;
			}
		}
	}
	Sermap[Coordinate1[0]][Coordinate1[1]] = Coordinate1[2];
}
void SK::con2() {
	z= recv(allClientSocket[1], (char*)Coordinate2, sizeof(Coordinate2), 0);
	if (z == -1) return;
	send(allClientSocket[0], (char*)Coordinate2, sizeof(Coordinate2), 0);
	send(allClientSocket[2], (char*)Coordinate2, sizeof(Coordinate2), 0);
	for (int i = 0; i < 4; i++) {
		Coordinate2[i] = ntohl(Coordinate2[i]);
	}

	if (Coordinate2[3] == 1) {
		if (Coordinate2[2] == 2) {
			for (int i = 1; i < 4; i++) {
				if (Coordinate2[1] + i > 9 || Sermap[Coordinate2[0]][Coordinate2[1] + i] == 1) {
					break;
				}
				Sermap[Coordinate2[0]][Coordinate2[1] + i] = 6;
			}
		}
		if (Coordinate2[2] == 3) {
			for (int i = 1; i < 4; i++) {
				if (Coordinate2[1] - i < 0 || Sermap[Coordinate2[0]][Coordinate2[1] - i] == 1) {
					break;
				}
				Sermap[Coordinate2[0]][Coordinate2[1] - i] = 6;
			}
		}
		if (Coordinate2[2] == 4) {
			for (int i = 1; i < 4; i++) {
				if (Coordinate2[0] - i < 0 || Sermap[Coordinate2[0] - i][Coordinate2[1]] == 1) {
					break;
				}
				Sermap[Coordinate2[0] - i][Coordinate2[1]] = 7;
			}
		}
		if (Coordinate2[2] == 5) {
			for (int i = 1; i < 4; i++) {
				if (Coordinate2[0] + i > 9 || Sermap[Coordinate2[0] + i][Coordinate2[1]] == 1) {
					break;
				}
				Sermap[Coordinate2[0] + i][Coordinate2[1]] = 7;
			}
		}
	}
	if (Sermap[Coordinate2[0]][Coordinate2[1]] == 6) {
		Sermap[Coordinate2[0]][Coordinate2[1]] = 6;
	}
	else if (Sermap[Coordinate2[0]][Coordinate2[1]] == 7) {
		Sermap[Coordinate2[0]][Coordinate2[1]] = 7;
	}
	else Sermap[Coordinate2[0]][Coordinate2[1]] = Coordinate2[2];
}
void SK::con3() {
	q= recv(allClientSocket[2], (char*)Coordinate3, sizeof(Coordinate3), 0);
	if (q == -1) return;
	send(allClientSocket[0], (char*)Coordinate3, sizeof(Coordinate3), 0);
	send(allClientSocket[1], (char*)Coordinate3, sizeof(Coordinate3), 0);
	for (int i = 0; i < 4; i++) {
		Coordinate3[i] = ntohl(Coordinate3[i]);
	}
	if (Coordinate3[3] == 1) {
		if (Coordinate3[2] == 2) {
			for (int i = 1; i < 4; i++) {
				if (Coordinate3[1] + i > 9 || Sermap[Coordinate3[0]][Coordinate3[1] + i] == 1) {
					break;
				}
				Sermap[Coordinate3[0]][Coordinate3[1] + i] = 6;
			}
		}
		if (Coordinate3[2] == 3) {
			for (int i = 1; i < 4; i++) {
				if (Coordinate3[1] - i < 0 || Sermap[Coordinate3[0]][Coordinate3[1] - i] == 1) {
					break;
				}
				Sermap[Coordinate3[0]][Coordinate3[1] - i] = 6;
			}
		}
		if (Coordinate3[2] == 4) {
			for (int i = 1; i < 4; i++) {
				if (Coordinate3[0] - i < 0 || Sermap[Coordinate3[0] - i][Coordinate3[1]] == 1) {
					break;
				}
				Sermap[Coordinate3[0] - i][Coordinate3[1]] = 7;
			}
		}
		if (Coordinate3[2] == 5) {
			for (int i = 1; i < 4; i++) {
				if (Coordinate3[0] + i > 9 || Sermap[Coordinate3[0] + i][Coordinate3[1]] == 1) {
					break;
				}
				Sermap[Coordinate3[0] + i][Coordinate3[1]] = 7;
			}
		}
	}
	if (Sermap[Coordinate3[0]][Coordinate3[1]] == 6) {
		Sermap[Coordinate3[0]][Coordinate3[1]] = 6;
	}
	else if (Sermap[Coordinate3[0]][Coordinate3[1]] == 7) {
		Sermap[Coordinate3[0]][Coordinate3[1]] = 7;
	}
	else Sermap[Coordinate3[0]][Coordinate3[1]] = Coordinate3[2];
}
void SK::trans() {
	if (kbhit()) {
		int tmp = _getch();
		if (tmp == 77 || tmp == 109) {
			if (index[0] == 1) {
				we[0] = ntohl(2);
				Coordinate1[4] = ntohl(2);
				index[0] = 2;
				tmp = 0;
			}
			else if (index[0] == 2) {
				we[0] = ntohl(1);
				Coordinate1[4] = ntohl(1);
				index[0] = 1;
				tmp = 0;
			}
		}
	}
}
void SK::Ser_gs() {
	Ser_open();
	while (1) {
		con1();
		con2();
		con3();
		print();
		if (y == -1 && z == -1 && q == -1) return;
	}
}
void SK::print() {
	system("cls");
	if (index[0] == 1) {
		cout << "\n    비안개 모드 \n" << endl;
	}
	if (index[0] == 2) {
		cout << "\n    안개 모드 \n" << endl;
	}
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (Sermap[i][j] == 0) cout << "□";
			else if (Sermap[i][j] == 1) cout << "■";
			else if (Sermap[i][j] == 2) cout << "▶";
			else if (Sermap[i][j] == 3) cout << "◀";
			else  if (Sermap[i][j] == 4) cout << "▲";
			else if (Sermap[i][j] == 5) cout << "▼";
			else if (Sermap[i][j] == 6) {
				cout << "ㅡ";
				Sermap[i][j] = 0;
			}
			else if (Sermap[i][j] == 7) {
				cout << "ㅣ";
				Sermap[i][j] = 0;
			}
		}
		cout << endl;
	}
	cout << "\n\n모드를 변경 할려면 m키를 누르시오. \n" << endl;
	Sermap[Coordinate2[0]][Coordinate2[1]] = 0;
	Sermap[Coordinate1[0]][Coordinate1[1]] = 0;
	Sermap[Coordinate3[0]][Coordinate3[1]] = 0;
}
SK ::~SK() {
	system("cls");
	if (Coordinate1[2] != 0) {
		cout << "\n\n  1번 플레이어가 우승했습니다. " << endl;
	}
	if (Coordinate2[2] != 0) {
		cout << "\n\n  2번 플레이어가 우승했습니다. " << endl;
	}
	if (Coordinate3[2] != 0) {
		cout << "\n\n  3번 플레이어가 우승했습니다. " << endl;
	}
	system("PAUSE");
}
int main() {
	SK b;

	b.Ser_gs();

}