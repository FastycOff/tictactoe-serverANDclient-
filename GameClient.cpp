
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#pragma warning(disable: 4996) // отключение ошибки 
SOCKET Connection;

char toprint[9];

void matrix_print()
{
	std::cout << "\n[1][2][3]\n[4][5][6]\n[7][8][9]\n\n";
}
void print()
{
	for (int counter = 0, y = 0; y < 3; ++y)
	{
		std::cout << '|';
		for (int x = 0; x < 3; ++x)
		{
			std::cout << toprint[counter] << '|';
			++counter;
		}
		std::cout << std::endl;
	}
}

void ClientHandler()
{
	matrix_print();
	char point[256];
	char movechar[1];
	recv(Connection, movechar, sizeof(movechar), NULL);
	const int mymove = movechar[0];
	int move;

	while (true)
	{
		recv(Connection, movechar, sizeof(movechar), NULL);
		move = movechar[0];
		if (move == mymove)
		{
			std::cout << "Enter x: ";
			std::cin >> point;
			send(Connection, point, sizeof(point), NULL);
			recv(Connection, toprint, sizeof(toprint), NULL);
			print();
		}
		else
		{
			std::cout << "waiting..." << std::endl;
			recv(Connection, toprint, sizeof(toprint), NULL);
			print();
		}
	}
}

int main()
{
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

	Connection = socket(AF_INET, SOCK_STREAM, NULL); // создание сокета для подключения клиента к серверу.
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) // Если подключение успешно, то функция вернет 0
	{
		std::cout << "Fail #1. Failed to connect to the server." << std::endl;
		return 1;
	}
	std::cout << "\t\t\tConnected successful\n" << std::endl;
	ClientHandler();

	return 0;
}
