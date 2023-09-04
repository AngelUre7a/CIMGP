#include <iostream>
#include <Magick++.h>
#include "CImg.h"

using namespace std;
using namespace cimg_library;

/* render(CImgDisplay* ventana) {
	CImg<unsigned char> play("inicio.jpg");
	ventana->flush(play);

}
*/
int main()
{
	//temporizador
	//const int tiempoMinimo = 5000;//5ms
	//const int tiempoMaximo = 10000;//10ms
	//int tiempoAleatorio = GenerarAleatorio(tiempoMinimo, tiempoMaximo);
	const int ancho_ventana = 1280;
	const int altura_ventana = 720;
	bool botonInicioPresionado = false;
	bool lobby = true;

	CImgDisplay ventana(ancho_ventana, altura_ventana, "Space X");//creacion de la ventana
		//ventana.draw_rectangle(457, 393, 618, 455,red);
	CImg<unsigned char> inicio("inicio2.png");//cargar una imagen del equipo
	CImg<unsigned char> juego("juego.jpg");
	CImg<unsigned char> play("PLAY.jpg");
	inicio.resize(ancho_ventana, altura_ventana);//escalar el fondo con la ventana
	ventana.paint();

	while (!ventana.is_closed())
	{
		int x = ventana.mouse_x();
		int y = ventana.mouse_y();
		ventana.display(botonInicioPresionado ? juego : inicio);//cargar imagen en la ventana
		//ventana.wait();//cargar ventana hasta que el usuario se salga

		//if (lobby) {
		

		if (ventana.button() && ventana.mouse_x() >= 550 && ventana.mouse_x() <= 725 && ventana.mouse_y() >= 470 && ventana.mouse_y() <= 525)
		{
			botonInicioPresionado = !botonInicioPresionado;
			lobby = false;

		}

			if (ventana.button() && ventana.mouse_x() >= 570 && ventana.mouse_x() <= 705 && ventana.mouse_y() >= 544 && ventana.mouse_y() <= 583)
			{
			lobby = false;
			return 0;
			}
		//}
		
	}
	return 0;
}