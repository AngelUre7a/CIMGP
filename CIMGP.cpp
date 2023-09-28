#include <iostream>
#include "CImg.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>
using namespace std;
using namespace cimg_library;


//colores
const unsigned char negro[] = { 0,0,0 };
const unsigned char blanco[] = { 255,255,255 };


//generar un numero aleatorio entre 0 y 1720
int minscreen = 0;
int maxscreen = 1000;
int GenerarAleatorio(int minscreen, int maxscreen) {
	return 0 + rand() % (maxscreen - minscreen + 1);
}
struct Meteorito {
	int x;
	int y;
	int velocidadCaida;
	CImg<unsigned char> meteorito_image;
	bool render = false;//imagen cargada en display

	Meteorito(const char* imagePath, int maxscreen) : meteorito_image(imagePath, maxscreen) {
		this->x = GenerarAleatorio(0, 1670);
		this->y = 20;
		this->meteorito_image = imagePath;
		meteorito_image.resize(50, 50);

	}

	void caer() {
		this->y += 1;
	}
	void borrarMeteorito() {
		meteorito_image.fill(0);

	}
};


//TEMPORIZADOR
chrono::steady_clock::time_point startTime = chrono::steady_clock::now();
int tiempoLimiteSegundos = 120;// 2 minutos

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
			int ancho_ventana = 600; // Supongamos que el ancho de la ventana es 600
			Meteorito miMeteorito(imagePath, ancho_ventana);
			meteoritos.push_back(miMeteorito);

			//reiniciar el temp.
			tiempoInicial = tiempoActual;
			tiempoMinimo = GenerarAleatorio(5000, 10000);

		}
		else if (tiempoTranscurrido < tiempoMinimo) {

		}
		/*for (Meteorito& meteorito : meteoritos)
		{
			meteorito.y += 1;
		}*/

		ventana.display(botonInicioPresionado ? juego : inicio);//cargar imagen en la ventana
		//ventana.wait();

		if (lobby) {
			if (ventana.button() && ventana.mouse_x() >= 550 && ventana.mouse_x() <= 725 && ventana.mouse_y() >= 470 && ventana.mouse_y() <= 525)//BOTON jugar
			{
				botonInicioPresionado = !botonInicioPresionado;
				lobby = false;

			}

			if (ventana.button() && ventana.mouse_x() >= 570 && ventana.mouse_x() <= 705 && ventana.mouse_y() >= 544 && ventana.mouse_y() <= 583)//BOTON salir
			{
				lobby = false;
				return 0;
			}
		}
		else {

			chrono::steady_clock::time_point currentTime = chrono::steady_clock::now();
			int tiempoTrans = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
			tiempoLimiteSegundos = max(0, 120 - tiempoTrans);
			if (tiempoLimiteSegundos <= 0) {
				CImg<unsigned char>finaljuego(ancho_ventana, altura_ventana, 1, 3);
				finaljuego.draw_text(ancho_ventana / 3, altura_ventana / 2, "Ganaste!!", negro, 50);
				finaljuego.display(ventana);
				this_thread::sleep_for(chrono::seconds(6));
				ventana.display(inicio);
				lobby = true;

			}

			for (Meteorito& meteorito : meteoritos) {
				cout << "meteorito generado en la pos x: " << meteorito.x << "." << endl << endl << endl << endl << endl;
				if (!meteorito.render) {
					juego.draw_image(meteorito.x, meteorito.y, meteorito.meteorito_image);
					meteorito.render = true;
				}
				else {
					// Dibujar el meteorito en su nueva posición
					juego.draw_image(meteorito.x, meteorito.y, meteorito.meteorito_image);
					meteorito.caer();


				}//else
			}//for
		}//else
		cimg::wait(50);
	}//while
	return 0;
}//main