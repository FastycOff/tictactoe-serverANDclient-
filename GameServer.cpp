
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <string>
#pragma warning(disable:6385)

#pragma warning(disable: 4996) // отключение ошибки 
SOCKET newConnectionone;
SOCKET newConnectiontwo;

char matrix[9];
int move = 1;

void print()
{
	for (int i = 0; i < 9; ++i)
	{
		std::cout << matrix[i];
	}
	std::cout << std::endl;
}
//int test_win()
//{
	// 0 - still playing 1 - X win   2 - O win

	// X win
	//if (matrix[0] == 'X' and matrix[1] == 'X' and matrix[2] == 'X') { return 1; }				// [0][1][2]
	//else if (matrix[3] == 'X' and matrix[4] == 'X' and matrix[5] == 'X') { return 1; }			// [3][4][5]
	//else if (matrix[6] == 'X' and matrix[7] == 'X' and matrix[8] == 'X') { return 1; }			// [6][7][8]

	//else if (matrix[0] == 'X' and matrix[3] == 'X' and matrix[6] == 'X') { return 1; }
	//else if (matrix[0] == 'X' and matrix[3] == 'X' and matrix[6] == 'X') { return 1; }
	//else if (matrix[1] == 'X' and matrix[4] == 'X' and matrix[7] == 'X') { return 1; }

	//else if (matrix[0] == 'X' and matrix[4] == 'X' and matrix[8] == 'X') { return 1; }
	//else if (matrix[2] == 'X' and matrix[4] == 'X' and matrix[6] == 'X') { return 1; }
	// O win
	//else if (matrix[0] == 'O' and matrix[1] == 'O' and matrix[2] == 'O') { return 2; }          // [0][1][2]
	//else if (matrix[3] == 'O' and matrix[4] == 'O' and matrix[5] == 'O') { return 2; }			// [3][4][5]
	//else if (matrix[6] == 'O' and matrix[7] == 'O' and matrix[8] == 'O') { return 2; }			// [6][7][8]

	//else if (matrix[0] == 'O' and matrix[3] == 'O' and matrix[6] == 'O') { return 2; }
	//else if (matrix[0] == 'O' and matrix[3] == 'O' and matrix[6] == 'O') { return 2; }
	//else if (matrix[1] == 'O' and matrix[4] == 'O' and matrix[7] == 'O') { return 2; }

	//else if (matrix[0] == 'O' and matrix[4] == 'O' and matrix[8] == 'O') { return 2; }
	//else if (matrix[2] == 'O' and matrix[4] == 'O' and matrix[6] == 'O') { return 2; }
	// still playing
	//else { return 0; }

	
//}


void ClientHandler()
{
	char pointone[256];
	int point;
	char pointtwo[256];

	for (char& i : matrix)
	{
		i = ' ';
	}
	while (true)
	{
		std::cout << "move #" << move << std::endl;
		char movechr[1];
		movechr[0] = move;
		send(newConnectionone, movechr, sizeof(movechr), NULL);
		send(newConnectiontwo, movechr, sizeof(movechr), NULL);
		if (move == 1)
		{
			recv(newConnectionone, pointone, sizeof(pointone), NULL);
			point = pointone[0]-49;
			matrix[point] = 'O';
			move = 2;
			send(newConnectionone, matrix, sizeof(matrix), NULL);
			send(newConnectiontwo, matrix, sizeof(matrix), NULL);
			Sleep(30);
		}
		else if (move == 2)
		{
			recv(newConnectiontwo, pointtwo, sizeof(pointtwo), NULL);
			point = pointtwo[0]-49;
			matrix[point] = 'X';
			move = 1;
			send(newConnectionone, matrix, sizeof(matrix), NULL);
			send(newConnectiontwo, matrix, sizeof(matrix), NULL);
			Sleep(30);
		}
		
	}
}


int main()
{
	char clientmove[1];
	clientmove[0] = 1;
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);// указание версии
	if (WSAStartup(DLLVersion, &wsaData) != 0)// Если функция WSAStartup вернула не 0, то произошла ошибка, закрытие программы
	{
		std::cout << "Error #1. Startup Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr; // создание класса под названием addr для хранение адресса
	int sizeofaddr = sizeof(addr); // Переменная, содержащая размер
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Указание ip, на котором стоит  (localhost)
	addr.sin_port = htons(1111);// Указание порта
	addr.sin_family = AF_INET; // семейство протоколов для интернет протоколов константа "AF_INET"


	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);// создание сокета "sListen", хранит результат выполнения функции "socket"
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN); //Указание количества допустимых запросов в очереди


	std::cout << "\t\t\tServer has started!" << std::endl << std::endl;
	newConnectionone = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);//addr - информация об ip-addres того, кто подключился

	if (newConnectionone == 0)	//если функция возвращает 0, то клиент не смог подключиться
	{
		std::cout << "Error #2. Connection Error" << std::endl;
	}
	else
	{
		std::cout << "Client #1 connected successful" << std::endl;
		send(newConnectionone, clientmove, sizeof(clientmove), NULL);
		clientmove[0] = 2;
	}

	newConnectiontwo = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);//addr - информация об ip-addres того, кто подключился

	if (newConnectiontwo == 0)	//если функция возвращает 0, то клиент не смог подключиться
	{
		std::cout << "Error #2. Connection Error" << std::endl;
	}
	else
	{
		std::cout << "Client #2 connected successful" << std::endl;
		send(newConnectiontwo, clientmove, sizeof(clientmove), NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL); // создание нового потока в обработчиком клиентов.
	}
	SOCKET newConnectionfalse = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);
	return 0;
	
}
