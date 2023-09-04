#include <iostream>
#include <Magick++.h>
#include "CImg.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace cimg_library;

struct Meteorito {
	int x;
	int y;
	int velocidadCaida;
};
//generar un numero aleatorio entre 0 y 1720
int min = 1;
int max = 1280;
int GenerarAleatorio(int min,int max) {
	return 0 + rand() % (max - min + 1);
}

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

	CImg<unsigned char>meteorito_image("meteorito.png");
	
	while (!ventana.is_closed())
	{
		int x = ventana.mouse_x();
		int y = ventana.mouse_y();

		auto tiempoActual = chrono::high_resolution_clock::now();
		auto tiempoTranscurrido = chrono::duration_cast<chrono::milliseconds>(tiempoActual - tiempoInicial).count();

		if (tiempoTranscurrido >= tiempoMinimo) {//generar un nuevo meteorito
			Meteorito nuevoMeteorito;
			nuevoMeteorito.x = GenerarAleatorio(0, ancho_ventana);
			nuevoMeteorito.y = 0;
			nuevoMeteorito.velocidadCaida = GenerarAleatorio(1, 5);//velocidad del meteorito
			meteoritos.push_back(nuevoMeteorito);

			//reiniciar el temp.
			tiempoInicial = tiempoActual;
			tiempoMinimo = GenerarAleatorio(5000,10000);
		}

		for (Meteorito& meteorito : meteoritos)
		{
			meteorito.y += meteorito.velocidadCaida;
		}

			ventana.display(botonInicioPresionado ? juego : inicio);//cargar imagen en la ventana
		//ventana.wait();//cargar ventana hasta que el usuario se salga

		if (lobby) {


		
			for (const Meteorito& meteorito : meteoritos) {
				juego.draw_image(meteorito.x, meteorito.y, meteorito_image);
				cout << "meteorito en la posicion x: " << meteorito.x << " y en la posicion y: " << meteorito.y;
			}

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
		
	}
	return 0;
}