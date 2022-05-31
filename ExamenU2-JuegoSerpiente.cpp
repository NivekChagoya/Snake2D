//Librerias utilizadas
#include <deque>
#include <GL/glut.h>
#include <sstream>
#include <ctime>
#include "Funciones.h"

//Quitar advertencia de parametros no utilizados en las funciones 
#define UNUSED(param) (void)(param)

//Macros para direccion de la serpiente
#define ARRIBA     	1
#define ABAJO       2
#define IZQUIERDA   3
#define DERECHA     4

//Variable longitud borde
const double longitud = 30.0;
//Variable direccion inicializado en 1(arriba)
int direccion = ABAJO;
//Variable de movimiento 
bool movimiento = false;
//Variable de velocidad inicializado en 100ms
const int velocidad = 100;
//Cola doble enlazada de coordenadas del cuerpo de la serpiente
std::deque< std::deque<double> > cuerpoSnake;
//Variable para comida disponible
bool hayComida = false;
//Arreglo coordenadas de la comida
double coordComida[2];
//Variable de puntuacion
int puntuacion = 0;
//Variable de inicio de crecimiento
int inicioCrecimiento = 0;
//Constante de contador de crecimiento
const int  crecimiento= 1;

//Funcion de dibujar comida
void coordenadasComida(){
	//Si hay comida dibujada no dibuja
	if(hayComida){
        return;
    }
	//arreglo de coordenadas temporales de la comida
    int coord_temp_comida[2];
    //variable choque
    bool choqueCoordenadas;
	//do while
    do {	
        choqueCoordenadas = false;
        //Generamos nuestras coordenadas aleatorias temporales
        coord_temp_comida[0] = 2 * (rand() % ((int)(longitud + 1))) - (int)longitud;
        coord_temp_comida[1] = 2 * (rand() % ((int)(longitud + 1))) - (int)longitud;
        //coord_temp_comida[1] = 2 * (rand() % ((int)(longitud - 8))) - (int)longitud;

        // si las coordenadas de la comida generada es en un punto del cuerpo de la serpiente
        for(int a = 0; a < cuerpoSnake.size(); a++){
        	//comparacion de coordenas del cuerpo de la serpiente con la de la comida
            if(coord_temp_comida[0] == cuerpoSnake[a][0] &&
               coord_temp_comida[1] == cuerpoSnake[a][1]){
                choqueCoordenadas = true;
            }
        }
    } while(choqueCoordenadas);

	//guardamos las coordenadas temporales generadas en arreglo de coordendas comida
    coordComida[0] = coord_temp_comida[0];
    coordComida[1] = coord_temp_comida[1];

    hayComida = true;
}

//Funcion para dibujar la comida
void dibujarComida(){
	//funcion de traslacion
	glLoadIdentity();
    glTranslatef(coordComida[0], coordComida[1], -40.0);
    traslacion(coordComida[0], coordComida[1]);
	//traslacion(coordComida[0], coordComida[1]);
    glColor3f(0, 0, 1);//pintaremos en color rojo
    //generamos nuestra comida en forma de poligono para que aparezca rellenada
	glBegin(GL_POLYGON);
        glVertex2d( 1, 1);
        glVertex2d( 1, -1);
        glVertex2d(-1, -1);
        glVertex2d(-1, 1);
    glEnd();
}

//Mostrar texto en la ventana OpenGL
void drawBitmapText(char* texto, float x, float y, float z) {
    char* c;
    glLoadIdentity();
    glColor3f(0, 1, 0);//damos un color verde al texto
    //ubicamos el texto
    glRasterPos3f(x, y, z);
	//recorremos la cadena texto y mostramos caracter por caracter
    for(c = texto; *c != '\0'; c++){
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    }
}

//Funcion para la puntuacion
void puntuacionSnake(){
    const char* msgPuntuacion = (char*) "Puntuacion del Jugador 1: ";
    const char* msgLongitud = (char*) "Longitud de la serpiente: ";
    const char* nombres = (char*) "Kevin Chagoya y Arturo Hernandez";
    //tratamos los string como un stream
    std::stringstream nombres_string;
	std::stringstream puntuacion_string;
    std::stringstream longitud_string;
    nombres_string << nombres;
    puntuacion_string << msgPuntuacion << puntuacion;
    longitud_string << msgLongitud << cuerpoSnake.size();
	//Mostramos los mensajes en la ventana
	drawBitmapText((char*) nombres_string.str().c_str(), -longitud + 15.0f, longitud - 2.0f, -39.5f);
    drawBitmapText((char*) puntuacion_string.str().c_str(), -longitud + 15.0f, longitud - 4.0f, -39.5f);
    drawBitmapText((char*) longitud_string.str().c_str(), -longitud + 15.0f, longitud  - 6.0f, -39.5f);
}

//Funcion de dibujar el cuerpo de la Serpiente
void dibujarSnake(){
     // pintamos las coordenadas del cuerpo de la serpiente
    glLoadIdentity();
    glTranslated(cuerpoSnake[0][0], cuerpoSnake[0][1], -40.0);
    glColor3f(1, 1, 0);//color verde
        //Damos forma de cuadro
    glBegin(GL_POLYGON);
        glVertex2d( 1,  1);
        glVertex2d( 1, -1);
        glVertex2d(-1, -1);
        glVertex2d(-1,  1);
    glEnd();
    for(int a = 1; a < cuerpoSnake.size(); a++){
        glLoadIdentity();
        glTranslated(cuerpoSnake[a][0], cuerpoSnake[a][1], -40.0);
        glColor3f(0, 1, 1);//color verde
        glBegin(GL_POLYGON);
            glVertex2d( 1,  1);
            glVertex2d( 1, -1);
            glVertex2d(-1, -1);
            glVertex2d(-1,  1);
        glEnd();
    }
}

//Funcion para mostrar todo en la ventana
void display(){
	//Limpiamos la superficie de dibujo
    glClear(GL_COLOR_BUFFER_BIT);
    //Matriz de cámara
    glMatrixMode(GL_MODELVIEW);
	//llamada a la funcion de dibujar serpiente
    dibujarSnake();
	//llamada a la funcion de generar y dibujar comida
    coordenadasComida();
    dibujarComida();
	//llamada a la funcion de mostrar puntuaciones
    puntuacionSnake();
	//intercambiamos los búferes de la ventana 
    glutSwapBuffers();
}

//Funcion para los movimientos de la serpiente 
void movimientoSnake(int d){
    direccion = d;
    int parte_siguiente = cuerpoSnake.size() - 1;
    std::deque<double> cabezaSnake = cuerpoSnake[parte_siguiente];

    double dx = 0.0;
    double dy = 0.0;
    double borde = 0.0;
    int ejeSnake = 0;
	//Saber que direccion ir
    switch (direccion){
        case ARRIBA:{
            dy = 2.0;
            borde = longitud;//restar 10 no encimar
            ejeSnake= 1;
            break;
        }

        case ABAJO:{
            dy = -2.0;
            borde = -longitud;
            ejeSnake = 1;
            break;
        }

        case DERECHA:{
            dx = 2.0;
            borde = longitud;
            ejeSnake = 0;
            break;
        }

        case IZQUIERDA:{
            dx = -2.0;
            borde = -longitud;
            ejeSnake = 0;
            break;
        }
    }
	
	//Para saber si choca con su mismo cuerpo
    // Si nos movemos dentro de una coordenada del cuerpo de la serpiente
    for(int a = 0; a < cuerpoSnake.size(); a++){
    	//comparamos coordenadas para saber si son iguales
        if(cuerpoSnake[0][0] + dx == cuerpoSnake[a][0] &&
           cuerpoSnake[0][1] + dy == cuerpoSnake[a][1]){
           	MessageBox(0,"GAME OVER","Perdiste",MB_OK);
            exit(0);//salimos
        }
    }

    // si chocamos con la pared del area de juego
    if(cuerpoSnake[0][ejeSnake] == borde){
    	MessageBox(0,"GAME OVER","Perdiste",MB_OK);
		exit(0);	
        /**const char* fin = (char*) "GAME OVER"	;
	    std::stringstream fin_string;
	    fin_string << fin;
		drawBitmapText((char*) fin_string.str().c_str(), -longitud + 20.0f, longitud - 7.0f, -39.5f);**/
    }

    // Si la serpiente ingiere la comida la puntuacion y la longitud de nuestros contadores incrementara
    if(cuerpoSnake[0][0] + dx == coordComida[0] &&
       cuerpoSnake[0][1] + dy == coordComida[1]){
        puntuacion += 10;//aumentamos el contador de puntuacion en 10
        inicioCrecimiento++;//aumentamos el contador de longitud en 1
        hayComida = false;//cambiamos el estado de la comida de que no hay 
    }

	//ubicamos la nueva cabeza de la serpiente
    cabezaSnake[0] = cuerpoSnake[0][0] + dx;
    cabezaSnake[1] = cuerpoSnake[0][1] + dy;
	
	//agregamos la cabeza al inicio
    cuerpoSnake.push_front(cabezaSnake);

    if(!inicioCrecimiento){
        cuerpoSnake.pop_back();
    } else if(inicioCrecimiento == crecimiento){
        inicioCrecimiento = 0;
    } else {
        inicioCrecimiento++;
    }
    glutPostRedisplay();
}

//Funcion para 
void moverSnake(int value){
    if(!movimiento){
        UNUSED(value);//por si no utilizamos nuestro parametro
        movimientoSnake(direccion);
    } else {
        movimiento = false;
    }
	//temporizador en ms
    glutTimerFunc(velocidad, moverSnake, 0);
}

//iniciamos glut
void initGL(){
    glMatrixMode(GL_PROJECTION);//proyeccion
    gluPerspective(76.0f, 1.0f, 0.0f, 40.0f);//perpectiva
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//borramos los buffers de color
}

//Funcion para cambiar los sentidos de la serpiente(arriba,abajo,izquierda y derecha)
void teclasOprimidas(int key, int x, int y){
    UNUSED(x);
    UNUSED(y);
	//saber que tecla fue oprimida
    switch(key){
    	//tecla arriba
        case GLUT_KEY_UP:{
            if(direccion == IZQUIERDA || direccion == DERECHA){
                movimiento = true;
                movimientoSnake(ARRIBA);
            }
            break;
        }
		//tecla abajo
        case GLUT_KEY_DOWN:{
            if(direccion == IZQUIERDA || direccion == DERECHA){
                movimiento = true;
                movimientoSnake(ABAJO);
            }
            break;
        }
		//tecla izquierda
        case GLUT_KEY_LEFT:{
            if(direccion == ARRIBA || direccion == ABAJO){
                movimiento = true;
                movimientoSnake(IZQUIERDA);
            }
            break;
        }
		//tecla derecha
        case GLUT_KEY_RIGHT:{
            if(direccion == ARRIBA || direccion == ABAJO){
                movimiento = true;
                movimientoSnake(DERECHA);
            }
        }
    }
}

//Funcion para establecer el ancho y la altura de la proyección
void reshape(GLsizei width, GLsizei height){
    UNUSED(width);
    UNUSED(height);
    //bloqueamos el tamaño de la ventana
    glutReshapeWindow(600, 600);
}

//Funcion Principal
int main(int argc, char** argv){
    glutInit(&argc, argv);//inicimos OpenGL
    glutInitWindowSize(600, 600);//tamaño de ventana
    glutInitWindowPosition(450, 80);
    //Titulo de la ventana
	const char titulo[] = "Examen Unidad II: Juego de la Serpiente";
    glutCreateWindow(titulo);//pasamos el titulo de nuestra ventana
    glutDisplayFunc(display);//visualización de las funciones para la ventana
    glutReshapeFunc(reshape);//remodelación para la ventana
    glutSpecialFunc(teclasOprimidas);//llamada del teclado para la ventana 
    glutTimerFunc(velocidad, moverSnake, 0);//llamada del temporizador que se activará en ms
	//Tamaño inicial de nuestra serpiente
    const int tamanoInicialSnake = 3;
    // Especificamos las coordenadas de cada parte de la serpiente.
    for(int a = 1; a <= tamanoInicialSnake; a++){
        std::deque<double> fila;
        fila.push_back(0.0);
        fila.push_back((longitud + 2.0 + (tamanoInicialSnake * 2)) - (a * 2));
        cuerpoSnake.push_front(fila);
    }
	srand(time(NULL));
    initGL();
    glutMainLoop();
    return 0;
}
