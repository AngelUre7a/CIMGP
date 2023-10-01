#include <iostream>
#include <string>
#include "CImg.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <conio.h>
#define Pi 3.14159265

using namespace std;
using namespace cimg_library;


//colores
const unsigned char negro[] = { 0,0,0 };
const unsigned char blanco[] = { 255,255,255 };
const unsigned char rojo[] = { 255,0,0 };


//generar un numero aleatorio entre 0 y 1000
int minscreen = 0;
int maxscreen = 900;
int GenerarAleatorio(int minscreen, int maxscreen) {
	return 0 + rand() % (maxscreen - minscreen + 1);
}
struct Meteorito {
	int tamañosMeteoritos[2] = { 25,50 };
	int radio;
	int x;
	int y;
	CImg<unsigned char> meteorito_image;
	bool render = false;//imagen cargada en display

	Meteorito(const char* imagePath, int maxscreen, int _x) : meteorito_image(imagePath, maxscreen) {
		this->x = _x; //GenerarAleatorio(0, 1200);
		this->y = 20;
		
		//elige aleatoriamente el tamaño del meteoro
		int randomSizeIndex = rand() % 2;//0 o 1 para elegir entre 25 pixeles o 50pixeles
		int tamañoDelMeteorito = tamañosMeteoritos[randomSizeIndex];

		this->meteorito_image = imagePath;

		//asigan el tamaño del meteorito
		this->meteorito_image.resize(tamañoDelMeteorito, tamañoDelMeteorito);

		//calcula el radio como la mitad del tamaño del meteorito
		this->radio = tamañoDelMeteorito / 2;

	}

	void caer() {
		this->y += 1;
	}
	void borrarMeteorito() {
		this->meteorito_image.fill(0);

	}
};


struct Bala {
	double x;
	double y;
	double velocidadX;
	double velocidadY;
	double distanciaDeseada;
	double angulo;

	Bala(double _x, double _y, double _distanciaDeseada, double _angulo, double velocidadBala = 2.0) : x(_x), y(_y), distanciaDeseada(_distanciaDeseada), angulo(_angulo) {

		angulo = _angulo * Pi / 180.0;//aqui se convierte el angulo a radianes
		velocidadX = distanciaDeseada * cos(angulo) * velocidadBala;//X de la vel
		velocidadY = distanciaDeseada * sin(angulo) * velocidadBala;//Y de la vel
	}

	void mover() {
		x -= velocidadX;
		y -= velocidadY;
	}
};


//-----------------------------------------------V A R I A B L E S   P A R A   L O S   Á N G U L O S--------------------------------------------------------------------------------------------
float angulo = 90.0f; // Ángulo inicial DE LA NAVE
float intervaloDelAngulo = 5.0f; // cada presionada de la tecla suma o resta 0.5 al angulo
float anguloMaximo = 170.0f; // maximo del angulo
float anguloMinimo = 10.0f; // minimo del angulo


int main()
{
	const int ancho_ventana = 1280;
	const int altura_ventana = 720;
	bool botonInicioPresionado = false;
	bool lobby = true;

	//balas
	vector<Bala>balas;//vector para las balas

	CImgDisplay ventana(ancho_ventana, altura_ventana, "Space X");//creacion de la ventana
	//ventana.draw_rectangle(457, 393, 618, 455,red);
	CImg<unsigned char> inicio("inicio2.png");//cargar una imagen del equipo
	CImg<unsigned char> juego("juego.jpg");
	CImg<unsigned char> juegoNuevo("juego.jpg");
	CImg<unsigned char> play("PLAY.jpg");
	CImg<unsigned char> vida("VIDA.jpg");

	CImg<unsigned char> nave("nave.png");
	nave.resize(70, 70);
	inicio.resize(ancho_ventana, altura_ventana);//escalar el fondo con la ventana
	juego.resize(ancho_ventana, altura_ventana);//escalar el fondo con la ventana
	juegoNuevo.resize(ancho_ventana, altura_ventana);//escalar el fondo con la ventana
	ventana.paint();

	int vidasRestantes = 10;
	vector<CImg<unsigned char>> vidas(vidasRestantes, vida);

	for (int i = 0; i < vidasRestantes; i++) {
		vidas[i].resize(20, 20);
	}

	vector<Meteorito>meteoritos;

	srand(static_cast<unsigned int>(time(nullptr)));

	//se crea una variable para el tiempo inicial de cada meteorito
	auto tiempoInicial = chrono::high_resolution_clock::now();//chrono almacena el tiempo actual en la que se da PLAY
	int tiempoMinimo = 5000; // 5 segundos
	int tiempoMaximo = 10000;// 10 segundos
	int tiempoAleatorio = tiempoMinimo + rand() % (tiempoMaximo - tiempoMinimo + 1);

	int numRocks = meteoritos.size();

	//TEMPORIZADOR
	chrono::steady_clock::time_point startTime;


	while (!ventana.is_closed())
	{

		ventana.display(botonInicioPresionado ? juego : inicio);//cargar imagen en la ventana

		if (lobby) {
			if (ventana.button() && ventana.mouse_x() >= 550 && ventana.mouse_x() <= 725 && ventana.mouse_y() >= 470 && ventana.mouse_y() <= 525)//BOTON jugar
			{
				botonInicioPresionado = !botonInicioPresionado;
				lobby = false;
				startTime = chrono::steady_clock::now();

			}

			if (ventana.button() && ventana.mouse_x() >= 570 && ventana.mouse_x() <= 705 && ventana.mouse_y() >= 544 && ventana.mouse_y() <= 583)//BOTON salir
			{
				lobby = false;
				return 0;
			}
		}
		else {

			//-------------------------------------------------------G E N E R A C I O N   D E   M E T E O R I T O S-----------------------------------------------------------------------------------------------------------------------

			auto tiempoActualPlay = chrono::high_resolution_clock::now();
			auto tiempoTranscurrido = chrono::duration_cast<chrono::milliseconds>(tiempoActualPlay - tiempoInicial).count();

			if (tiempoTranscurrido >= tiempoAleatorio) {//generar un nuevo meteorito
				const char* imagePath = "meteorito.png";
				//el ancho de la ventana es 1200
				Meteorito miMeteorito(imagePath, ancho_ventana, GenerarAleatorio(0, ancho_ventana - 10));
				// Verificar si el meteorito ya se agregó antes
				if (!miMeteorito.render) {
					meteoritos.push_back(miMeteorito);
					miMeteorito.render = true; // Marcar como agregado
				}

				//reiniciar el temp.
				tiempoInicial = tiempoActualPlay;
				tiempoAleatorio = tiempoMinimo + rand() % (tiempoMaximo - tiempoMinimo + 1);

			}
			else if (tiempoTranscurrido < tiempoMinimo) {

			}

			//------------------------------------------------------F I N   D E   G E N E R A C I O N   D E   M E T E O R I T O S------------------------------------------------------------------------------------------------------------------------

			juego.fill(juegoNuevo);

			juego.draw_rectangle(10, altura_ventana - 50, ancho_ventana - 10, altura_ventana - 10, blanco);//dibuja el rectangle blanco de abajo


			//COORDENADAS DE LA NAVE
			int naveY = altura_ventana - 100;
			int naveX = (ancho_ventana / 2 - 25);

			//-------------------------------------------------------------------------------------------------------------------------------

			auto tiempoActual = chrono::high_resolution_clock::now();

			//----------------------------------------------------T E C L A D O--------------------------------------------------------

			char tecla = ventana.key();

			if (tecla == ventana.keycode("ARROWRIGHT")) {//"ARROWRIGHT" verifica si se pulso la tecla de la flecha derecha, segun la libreria CIMG
				// Rotar hacia la izquierda
				angulo += intervaloDelAngulo; // Incrementa el ángulo
				// Limitar el ángulo máximo
				angulo = min(angulo, anguloMaximo);
			}
			else if (tecla == ventana.keycode("ARROWLEFT")) {//"ARROWLEFT" verifica si se pulso la tecla de la flecha izquierda, segun la libreria CIMG
				// Rotar hacia la derecha
				angulo -= intervaloDelAngulo; // Decrementa el ángulo
				// Limitar el ángulo mínimo
				angulo = max(angulo, anguloMinimo);
			}
			else if (tecla == ventana.keycode("ESC")) { //"ESC" verifica si se pulso la tecla ESC, segun la libreria CIMG
				break; // Salir del bucle si se presiona Esc
			}
			// Dibujar la imagen de la nave con el ángulo actual
			CImg<unsigned char> nave_rotada = nave.get_rotate(angulo);
			juego.draw_image(naveX, naveY, nave_rotada);
			//----------------------------------------------------T E C L A D O   F I N -----------------------------------------------------------------



//----------------------------------------------------B A L A S---------------------------------------------------------------------------------------
		
			if (tecla == ventana.keycode("ARROWUP")) {

				// la bala sale desde el centro de la imagen de la nave
				int balaX = naveX + (nave_rotada.width() / 2);
				int balaY = naveY + (nave_rotada.width() / 2);

				double distanciaDeseada = 100.0;

				Bala nuevaBala(balaX, balaY, distanciaDeseada, angulo, 0.2);
				balas.push_back(nuevaBala);

			}
			for (int i = 0; i < balas.size(); i++) {
				Bala& bala = balas[i];
				bala.mover();
				juego.draw_circle(bala.x, bala.y, 5, blanco);//bala de 5 de radio, osea 10 pixeles
				if (bala.y < 0 || bala.x<0 || bala.x>ancho_ventana || bala.y>altura_ventana) {
					balas.erase(balas.begin() + i);
				}
			}

			//-----------------------------------D E T E C C I O N   D E   C O L I S I O N   E N T R E   B A L A S   Y   M E T E O R I T O S-------------------------------------------------------


						// Colisión de meteoritos y balas
			for (int i = 0; i < meteoritos.size(); i++) {
				Meteorito& meteorito = meteoritos[i];

				// Si el meteorito está dentro de los límites del área de la bala
				for (int j = 0; j < balas.size(); j++) {
					Bala& bala = balas[j];

					//calcular la distancia entre el centro de la bala y el centro del meteorito
					double distancia = sqrt(pow(bala.x - meteorito.x, 2) + pow(bala.y - meteorito.y, 2));

					// Verificar si la distancia es menos o igual al radio del meteorito
					if (distancia<=meteorito.radio) {

						// Colisión detectada, eliminar bala y meteorito
						balas.erase(balas.begin() + j);
						meteoritos.erase(meteoritos.begin() + i);
						break;  // Importante para salir del bucle interno
					}
				}
			}
			//--------------------------------------------------------------------F I N   D E   C O L I S I O N ------------------------------------------------------------------------------------

			int vidaX = 10; // Coordenada x inicial de las vidas
			int vidaY = altura_ventana - 30; // Coordenada y fija de las vidas

			juego.draw_text(vidaX, altura_ventana - 53, ("VIDA"), negro, 50, 1, 25);
			for (const auto& vidaImage : vidas) {
				juego.draw_image(vidaX, vidaY, vidaImage);
				vidaX += 20; // Ajustar la posición para la siguiente vida
			}
			string anguloActual = "Angulo de  disparo: " + to_string(angulo);
			juego.draw_text(ancho_ventana / 2 + 75, altura_ventana - 30, anguloActual.c_str(), angulo <= 30 ? rojo : negro, 100, 1, 25);


			chrono::steady_clock::time_point currentTime = chrono::steady_clock::now();
			int tiempoTrans = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
			// Calcula el tiempo restante en segundos
			int tiempoRestante = max(0, 120 - tiempoTrans);
			string tiempoRestanteStr = to_string(tiempoRestante);

			// Dibuja el tiempo restante en la ventana del juego
			juego.draw_text(ancho_ventana - 50, altura_ventana - 30, +tiempoRestanteStr.c_str(), tiempoRestante <= 30 ? rojo : negro, 100, 1, 25);

			if (tiempoRestante <= 0) {
				CImg<unsigned char>gameover(ancho_ventana, altura_ventana, 1, 3);
				gameover.draw_text(ancho_ventana / 3, altura_ventana / 2, "You Win!!", negro, 50);
				gameover.draw_text(ancho_ventana / 3, altura_ventana / 2 + 30, "Lograste sobrevivir la lluvia de meteoritos :)", negro, 50);
				gameover.display(ventana);
				this_thread::sleep_for(chrono::seconds(3));
				ventana.wait();
				return 0;
			}

			//------------------------------------------------R E N D E R I Z A C I O N   D E   M E T E O R I T O S-------------------------------------------------------------------------------------
			
						//genera los meteoritos
			for (Meteorito& meteorito : meteoritos) {
				//cout << "meteorito generado en la pos x: " << meteorito.x << "." << endl << endl << endl << endl << endl;
				if (!meteorito.render) {
					juego.draw_circle(meteorito.x, meteorito.y, meteorito.radio, rojo);
					meteorito.render = true;
				}
				else {
					juego.draw_circle(meteorito.x, meteorito.y, meteorito.radio, rojo);
					meteorito.caer();

					//------------------------------------------------F I N  D E   R E N D E R I Z A C I O N ---------------------------------------------------------------------------------------------





					//----------------------------------------------------M A N E J O   D E   V I D A S---------------------------------------------------------------------------------------------------

					if (meteorito.y >= altura_ventana) {//borra el meteorito que ya llego abajo
						meteorito.borrarMeteorito();
						meteoritos.erase(meteoritos.begin());
					}
					for (int i = 0; i < meteoritos.size(); i++) {
						Meteorito& meteorito = meteoritos[i];
						//el meteorito ya llego abajo //  altura_ventana - 55 = EL FINAL DE ABAJO - 55 PIXELES
						if (meteorito.y >= altura_ventana - 55) {
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
					if (vidasRestantes == 0) {//pantalla de cuando pierde por llegar a 0 vidas
						CImg<unsigned char>gameover(ancho_ventana, altura_ventana, 1, 3);
						gameover.draw_text(ancho_ventana / 3, altura_ventana / 2, "Game Over", negro, 50);
						gameover.draw_text(ancho_ventana / 3, altura_ventana / 2 + 30, "Los meteoritos destruyeron tu base :(", negro, 50);
						gameover.display(ventana);
						this_thread::sleep_for(chrono::seconds(3));
						ventana.wait();
						return 0;
					}

				}//else
			}//for

		}//else
		cimg::wait(50);
	}//while
	return 0;
}//main