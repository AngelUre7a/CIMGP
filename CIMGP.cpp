#include <iostream>
#include <Magick++.h>
#include "CImg.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace cimg_library;


//generar un numero aleatorio entre 0 y 1720
int min = 1;
int max = 1280;
int GenerarAleatorio(int min, int max) {
	return 0 + rand() % (max - min + 1);
}

struct Meteorito {
	int x;
	int y;
	int velocidadCaida;
	CImg<unsigned char> meteorito_image;
	bool render = false;//imagen cargada en display

	Meteorito(const char* imagePath, int ancho_ventana) : meteorito_image(imagePath, ancho_ventana) {
		this->x = GenerarAleatorio(0, ancho_ventana);
		this->y = 0;
		this->velocidadCaida = GenerarAleatorio(1, 5);//velocidad del meteorito
		this->meteorito_image = imagePath;
		meteorito_image.resize(50, 50);

	}
	
	void caer(){
		this->y += 1;
	}

};

int main()
{
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

	vector<Meteorito>meteoritos;

	srand(static_cast<unsigned int>(time(nullptr)));

	//se crea una variable para el tiempo inicial de cada meteorito
	auto tiempoInicial = chrono::high_resolution_clock::now();//chrono almacena el tiempo actual en la que ejecuta el programa
	int tiempoMinimo = 5000; // 5 segundos
	int tiempoMaximo = 10000;// 10 segundos

	while (!ventana.is_closed())
	{
		int x = ventana.mouse_x();
		int y = ventana.mouse_y();

		auto tiempoActual = chrono::high_resolution_clock::now();
		auto tiempoTranscurrido = chrono::duration_cast<chrono::milliseconds>(tiempoActual - tiempoInicial).count();

		if (tiempoTranscurrido >= tiempoMinimo) {//generar un nuevo meteorito
			const char* imagePath = "meteorito.png";
			int ancho_ventana = 800; // Supongamos que el ancho de la ventana es 800
			Meteorito miMeteorito(imagePath, ancho_ventana);
			meteoritos.push_back(miMeteorito);

			//reiniciar el temp.
			tiempoInicial = tiempoActual;
			tiempoMinimo = GenerarAleatorio(5000, 10000);
		}

		/*for (Meteorito& meteorito : meteoritos)
		{
			meteorito.y += 1;
		}*/

		ventana.display(botonInicioPresionado ? juego : inicio);//cargar imagen en la ventana
		//ventana.wait();//cargar ventana hasta que el usuario se salga

		if (lobby) {




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
		}
		else {
			for (Meteorito& meteorito : meteoritos) {
				if (!meteorito.render) {
					juego.draw_image(meteorito.x, meteorito.y, meteorito.meteorito_image);
					meteorito.render = true;
				}
				else {
					//Borrar el dibujo anterior del meteorito (dibujar un rectángulo blanco en su ubicación anterior)
				   //juego.draw_rectangle(meteorito.x, meteorito.y, meteorito.x + 50, meteorito.y + 50, cimg_library::CImg<unsigned char>::background(255, 255, 255));

					meteorito.caer();

					// Dibujar el meteorito en su nueva posición
					juego.draw_image(meteorito.x, meteorito.y, meteorito.meteorito_image);

				}
			}
		}

		cimg::wait(50);
	}
		return 0;
}