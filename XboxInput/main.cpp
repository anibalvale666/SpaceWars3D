#include "CXBOXController.h"
#include <iostream>

using namespace std;

#define XINPUT_GAMEPAD_DPAD_UP          0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN        0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT        0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT       0x0008
#define XINPUT_GAMEPAD_START            16
#define XINPUT_GAMEPAD_BACK             32
#define XINPUT_GAMEPAD_LEFT_THUMB       0x00000040
#define XINPUT_GAMEPAD_RIGHT_THUMB      0x00000080
#define XINPUT_GAMEPAD_LEFT_SHOULDER    256
#define XINPUT_GAMEPAD_RIGHT_SHOULDER   512
#define XINPUT_GAMEPAD_A                4096
#define XINPUT_GAMEPAD_B                8192
#define XINPUT_GAMEPAD_X                16384
#define XINPUT_GAMEPAD_Y                32768



CXBOXController* Player1;
int main(int argc, char* argv[])
{
	Player1 = new CXBOXController(1);

	while(true)
	{
		if(Player1->IsConnected())
		{
			//cout << "Player1->GetState().Gamepad.wButtons: " << Player1->GetState().Gamepad.wButtons << endl;
			if(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
			{
				std::cout << "A" << std::endl;
				//Player1->Vibrate(65535, 0);
			}

			if(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
			{
				std::cout << "B" << std::endl;
				//Player1->Vibrate(0, 65535);
			}

			if(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X)
			{
				std::cout << "X" << std::endl;
				//Player1->Vibrate(65535, 65535);
			}

			if(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y)
			{
				std::cout << "Y" << std::endl;
				//Player1->Vibrate();
			}

			if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
			{
				std::cout << "Boton_del_Joystick_Izquierdo" << std::endl;
				//break;
			}

			if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
			{
				std::cout << "Boton_del_Joystick_Derecho" << std::endl;
				//break;
			}

			if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
			{
				std::cout << "LB" << std::endl;
				//break;
			}

			if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{
				std::cout << "RB" << std::endl;
				//break;
			}

			if(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
			{
				std::cout << "Fecha_Arriba" << std::endl;
				//break;
			}

			if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
			{
				std::cout << "Fecha_Abajo" << std::endl;
				//break;
			}

			if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
			{
				std::cout << "Fecha_Izquierda" << std::endl;
				//break;
			}

			if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
			{
				std::cout << "Fecha_Derecha" << std::endl;
				//break;
			}

		}
		else
		{
			std::cout << "\n\tERROR! PLAYER 1 - XBOX 360 Controller Not Found!\n";
			std::cout << "Press Any Key To Exit.";
			std::cin.get();
			break;
		}
	}

	delete(Player1);

	return( 0 );
}
