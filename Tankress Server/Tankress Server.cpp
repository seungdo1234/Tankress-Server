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
unsigned  threadID;
SOCKET servsoc;
SOCKET clisoc;
int clientNumber = 0;
int player[4] = { 0, };
int allClientSocket[100];
struct sockaddr_in clientAddress;
int clientAddressSize = sizeof(clientAddress);
class SK {
public:
	int Sermap[10][10] = { 0, };
	int Coordinate1[6] = { 0, };
	int Coordinate2[6] = { 0, };
	int Coordinate3[6] = { 0, };
	int we[1] = { 0, };
	int y = 4;
	int index[3] = { 0 , };
	virtual void  Ser_gs(void) = 0;
	SK();
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

class SK_C :public SK {
private:
	WSADATA wsdata;
	sockaddr_in serverAddress;
public:
	void Ser_open();
	void print();
	static unsigned __stdcall con(void* arg);
	void Ser_gs(void);
	~SK_C();
};
void SK_C::Ser_open() {
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
	if (listen(servsoc, 3) == -1) {
		cout << "listen ��� ����!" << endl;
		return;
	}
	cout << "\n  ��ũ���� ������ ����������ϴ�.\n" << endl;
	cout << "\n  ������ �����ҷ��� ENTER�� �����ÿ�.   (�ּ��ο� 1��) \n\n";
	unsigned long thread;
	thread = _beginthreadex(NULL, 0, con, (void*)Sermap, 0, &threadID);
	while (clientNumber == 0) {
	
	}
	while (1) {
		int tmp = _getch();
		if (tmp == 13) {
			break;
		}
	}
	while (1) {
		cout << "  ���Ӹ�带 �����Ͻÿ�. 1) ��Ȱ���� 2) �Ȱ���� : ";
		cin >> index[0];
		if ((index[0] == 1 && index[0] != 2) || (index[0] != 1 && index[0] == 2)) {
			break;
		}
		else {
			cout << "  ���Ӹ��� 1) �� 2) �߿� �ٽ� �����ϼ���. \n";
		}
	}
	while (1) {
		cout << "  ��ũ�̵��ð��� �����Ͻÿ� (300~500) : ";
		cin >> index[1];
		if (index[1] >= 300 && index[1] <= 500) {
			break;
		}
		else {
			cout << "  ��ũ�̵��ð��� 300~500 ���� �Դϴ�. �ٽ� �����ϼ���. \n";
		}
	}
	index[2] = clientNumber;
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
unsigned __stdcall SK_C::con(void* arg) {
	while (1) {
		allClientSocket[clientNumber] = accept(servsoc, (struct sockaddr*)&clientAddress, &clientAddressSize);
		cout << "  " << clientNumber + 1 << "��° �÷��̾ ���� �߽��ϴ�.  ���� �ο� : [" << clientNumber + 1 << "/3]" << endl;
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
	}
}
void SK_C::Ser_gs(void) {
	Ser_open();

	while (1) {
		y = recv(allClientSocket[0], (char*)Coordinate1, sizeof(Coordinate1), 0);
		if (y == -1) return;
		if (kbhit()) {
			int tmp = _getch();
			if (tmp == 77 || tmp == 109) {
				if (index[0] == 1) {
					index[0] = 2;
					tmp = 0;
				}
				else if (index[0] == 2) {
					index[0] = 1;
					tmp = 0;
				}
			}
		}
		if (index[2] > 1) {
			send(allClientSocket[1], (char*)Coordinate1, sizeof(Coordinate1), 0);
		}
		if (index[2] > 2) {
			send(allClientSocket[2], (char*)Coordinate1, sizeof(Coordinate1), 0);
		}
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
		if (Coordinate1[2] != 0) {
			Sermap[Coordinate1[0]][Coordinate1[1]] = Coordinate1[2];
		}
		if (index[2] > 1) {
			y = recv(allClientSocket[1], (char*)Coordinate2, sizeof(Coordinate2), 0);
			if (y == -1) return;
			send(allClientSocket[0], (char*)Coordinate2, sizeof(Coordinate2), 0);
		}
		if (index[2] > 2) {
			send(allClientSocket[2], (char*)Coordinate2, sizeof(Coordinate2), 0);
		}
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
		if (Coordinate2[2] != 0) {
			if (Sermap[Coordinate2[0]][Coordinate2[1]] == 6) {
				Sermap[Coordinate2[0]][Coordinate2[1]] = 6;
			}
			else if (Sermap[Coordinate2[0]][Coordinate2[1]] == 7) {
				Sermap[Coordinate2[0]][Coordinate2[1]] = 7;
			}
			else Sermap[Coordinate2[0]][Coordinate2[1]] = Coordinate2[2];
		}
		if (index[2] > 2) {
			y = recv(allClientSocket[2], (char*)Coordinate3, sizeof(Coordinate3), 0);
			if (y == -1) return;
			send(allClientSocket[0], (char*)Coordinate3, sizeof(Coordinate3), 0);
			send(allClientSocket[1], (char*)Coordinate3, sizeof(Coordinate3), 0);
		}
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
		if (Coordinate3[2] != 0) {
			if (Sermap[Coordinate3[0]][Coordinate3[1]] == 6) {
				Sermap[Coordinate3[0]][Coordinate3[1]] = 6;
			}
			else if (Sermap[Coordinate3[0]][Coordinate3[1]] == 7) {
				Sermap[Coordinate3[0]][Coordinate3[1]] = 7;
			}
			else Sermap[Coordinate3[0]][Coordinate3[1]] = Coordinate3[2];
		}
		print();
	}
}
void SK_C::print() {
	system("cls");
	if (index[0] == 1) {
		cout << "\n    ��Ȱ� ��� \n" << endl;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				if (Sermap[i][j] == 0) cout << "��";
				else if (Sermap[i][j] == 1) cout << "��";
				else if (Sermap[i][j] == 2) cout << "��";
				else if (Sermap[i][j] == 3) cout << "��";
				else  if (Sermap[i][j] == 4) cout << "��";
				else if (Sermap[i][j] == 5) cout << "��";
				else if (Sermap[i][j] == 6) {
					cout << "��";
					Sermap[i][j] = 0;
				}
				else if (Sermap[i][j] == 7) {
					cout << "��";
					Sermap[i][j] = 0;
				}
			}
			cout << endl;
		}
	}
	if (index[0] == 2) {
		cout << "\n    �Ȱ� ��� \n" << endl;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				if (Sermap[i][j] == 1) cout << "��";
				else cout << "��";
			}
			cout << endl;
		}
	}
	cout << "\n\n��带 ���� �ҷ��� mŰ�� �����ÿ�. \n" << endl;
	Sermap[Coordinate2[0]][Coordinate2[1]] = 0;
	if (index[2] > 1) {
		Sermap[Coordinate1[0]][Coordinate1[1]] = 0;
	}
	if (index[2] > 2) {
		Sermap[Coordinate3[0]][Coordinate3[1]] = 0;
	}
}
SK_C ::~SK_C() {
	system("cls");
	if (Coordinate1[2] != 0) {
		cout << "\n\n  1�� �÷��̾ ����߽��ϴ�. " << endl;
	}
	else if (Coordinate2[2] != 0) {
		cout << "\n\n  2�� �÷��̾ ����߽��ϴ�. " << endl;
	}
	else if (Coordinate3[2] != 0) {
		cout << "\n\n  3�� �÷��̾ ����߽��ϴ�. " << endl;
	}
	cout << "\n  ������ �����ϴ�......... \n\n";

	Sleep(3000);
}
int main() {
	SK_C b;

	b.Ser_gs();

}