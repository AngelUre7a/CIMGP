#include <iostream>
#include "CImg.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <conio.h>
using namespace std;
using namespace cimg_library;


//colores
const unsigned char negro[] = { 0,0,0 };
const unsigned char blanco[] = { 255,255,255 };
const unsigned char rojo[] = { 255,0,0 };


//generar un numero aleatorio entre 0 y 1720
int minscreen = 0;
int maxscreen = 1000;
int GenerarAleatorio(int minscreen, int maxscreen) {
	return 0 + rand() % (maxscreen - minscreen + 1);
}
struct Meteorito {
	int x;
	int y;
	CImg<unsigned char> meteorito_image;
	bool render = false;//imagen cargada en display

	Meteorito(const char* imagePath, int maxscreen, int _x) : meteorito_image(imagePath, maxscreen) {
		this->x = _x; //GenerarAleatorio(0, 1200);
		this->y = 20;
		this->meteorito_image = imagePath;
		this->meteorito_image.resize(50, 50);

	}

	void caer() {
		this->y += 1;
	}
	void borrarMeteorito() {
		this->meteorito_image.fill(0);

	}
};
struct Bala {
	int x;
	int y;
	int velocidad;

	Bala(int _x, int _y, int _velocidad) : x(_x), y(_y), velocidad(_velocidad) {
		this->x = 600;
		this->y = 700;
	}
	void disparo() {
		this->y -= 2;
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

	//balas
	vector<Bala>balas;
	bool dispararBala = false;
	clock_t tiempoUltimoDisparo = 0;
	const int tiempoDeRetrasoEntreDisparos = 100;
	//chrono::steady_clock::time_point tiempoUltimoDisparo;


	CImgDisplay ventana(ancho_ventana, altura_ventana, "Space X");//creacion de la ventana
	//ventana.draw_rectangle(457, 393, 618, 455,red);
	CImg<unsigned char> inicio("inicio2.png");//cargar una imagen del equipo
	CImg<unsigned char> juego("juego.jpg");
	CImg<unsigned char> juegoNuevo("juego.jpg");
	CImg<unsigned char> play("PLAY.jpg");
	CImg<unsigned char> vida("VIDA.jpg");
	int radio = 25;
	inicio.resize(ancho_ventana, altura_ventana);//escalar el fondo con la ventana
	ventana.paint();

	int vidasRestantes = 10;
	vector<CImg<unsigned char>> vidas(10, vida);

	vector<Meteorito>meteoritos;

	srand(static_cast<unsigned int>(time(nullptr)));

	//se crea una variable para el tiempo inicial de cada meteorito
	auto tiempoInicial = chrono::high_resolution_clock::now();//chrono almacena el tiempo actual en la que ejecuta el programa
	int tiempoMinimo = 5000; // 5 segundos
	int tiempoMaximo = 10000;// 10 segundos


	int numRocks = meteoritos.size();



	while (!ventana.is_closed())
	{
		int x = ventana.mouse_x();
		int y = ventana.mouse_y();

		auto tiempoActual = chrono::high_resolution_clock::now();
		auto tiempoTranscurrido = chrono::duration_cast<chrono::milliseconds>(tiempoActual - tiempoInicial).count();

		if (tiempoTranscurrido >= tiempoMinimo) {//generar un nuevo meteorito
			const char* imagePath = "meteorito.png";
			int ancho_ventana = 600; // Supongamos que el ancho de la ventana es 600
			Meteorito miMeteorito(imagePath, ancho_ventana, GenerarAleatorio(0, 1200));
			// Verificar si el meteorito ya se agreg� antes
			if (!miMeteorito.render) {
				meteoritos.push_back(miMeteorito);
				miMeteorito.render = true; // Marcar como agregado
			}

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

			//ventana.display(juego);

			juego.fill(juegoNuevo);

			//------------------------------------------------------------------

			clock_t tiempoActual = clock();
			if ((tiempoActual - tiempoUltimoDisparo) * 1000 / CLOCKS_PER_SEC >= tiempoDeRetrasoEntreDisparos) {
				//si espacio esta presionado
				if (_kbhit() && _getch() == ' ') {        //kbhit verifica si una tecla esta disponible
					balas.push_back(Bala(600, 700, 2));	  //getch se usa para leer la tecla que se presiona
					tiempoUltimoDisparo = tiempoActual;
				}
			}


			for (Bala& bala : balas) {
				bala.disparo();
				juego.draw_circle(bala.x, bala.y, 3, negro);
			}

			int vidaX = 10; // Coordenada x inicial de las vidas
			int vidaY = 10; // Coordenada y fija de las vidas
			for (const auto& vidaImage : vidas) {
				juego.draw_image(vidaX, vidaY, vidaImage);
				vidaX += 30; // Ajustar la posici�n para la siguiente vida
			}


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
				break;

			}

			//genera los meteoritos
			for (Meteorito& meteorito : meteoritos) {
				cout << "meteorito generado en la pos x: " << meteorito.x << "." << endl << endl << endl << endl << endl;
				if (!meteorito.render) {
					juego.draw_circle(meteorito.x, meteorito.y, 10, rojo);
					meteorito.render = true;
				}
				else {
					//meteorito.meteorito_image.fill(0);
					juego.draw_circle(meteorito.x, meteorito.y, 10, rojo);
					meteorito.caer();

					//juego.fill()
					//meteorito.meteorito_image.set_linear_atX(meteorito);
						//juego.set
					//juego.draw_circle(meteorito.x, meteorito.y, 10, rojo);

					if (meteorito.y >= altura_ventana) {
						meteorito.borrarMeteorito();
						meteoritos.erase(meteoritos.begin());
					}

					//meteoritos.erase(meteoritos.begin()+1);
					for (int i = 0; i < meteoritos.size(); i++) {
						Meteorito& meteorito = meteoritos[i];
						//el meteorito ya llego abajo
						if (meteorito.y >= 300) {
							if (vidasRestantes > 0) {//si el vector aun no esta vacido
								vidasRestantes--;//restar una vida(eliminar un vector)
								vidas.pop_back();
								//vida.resize(10-i, 10);
							}
							//borrar meteorito
							meteoritos.erase(meteoritos.begin() + i);

							continue;
						}

					}
					if (vidasRestantes == 0) {
						CImg<unsigned char>gameover(ancho_ventana, altura_ventana, 1, 3);
						gameover.draw_text(ancho_ventana / 3, altura_ventana / 2, "Game Over", negro, 50);
						gameover.display(ventana);
						this_thread::sleep_for(chrono::seconds(3));
						ventana.display(inicio);
						lobby = true;
						break;
					}

				}//else
			}//for

		}//else
		cimg::wait(50);
	}//while
	return 0;
}//main