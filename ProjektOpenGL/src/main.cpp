/*************************************************************************************/

// Troch� bardziej skomplikowany program wykorzystuj�cy funkcje biblioteki OpenGL

/*************************************************************************************/

#include <windows.h>

#include <gl/gl.h>

#include <glut.h>

#include <iostream>
/*************************************************************************************/


// Funkcaja okre�laj�ca, co ma by� rysowane
// (zawsze wywo�ywana, gdy trzeba przerysowa� scen�)

int WIDTH = 800;		
int HEIGHT = 800;

struct Point {//Struktura opisuj�ca punkty na osi X i Y
	int x;
	int y;
};

struct Axis {//Struktura przechowuj�ca informacje na temat
			 //warto�ci osi Re oraz Im
	float min;
	float max;
};

Axis x_axis = { -2.5, 1.5 };//Okre�lenie maks. i min. warto�ci osi Re 
Axis y_axis = { -1.25, 1.25 };//Im

int MAX_ITERATIONS = 40;//Ograniczenie iteracji

float map(float value, float in_min, float in_max, float out_min, float out_max)//Funkcja zwracaj�ca warto��
																				//danego punktu w zale�no�ci od
																				//danych ogranicze�
{
	return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void drawPixel(int x, int y, float bright) {//Funkcja rysuj�ca jedne piksel
	glBegin(GL_POINTS);
	glColor3f(bright, bright, bright); 
	glVertex2i(x, y);
	glEnd();
}

void Mandelbrot() {//Funkcja odpowiedzialna za rysowanie zbioru
	
	float Re_factor = (x_axis.max - x_axis.min) / (WIDTH - 1); //Obliczanie zmiennej wobec Re albo Im
	float Im_factor = (y_axis.max - y_axis.min) / (HEIGHT - 1);//wzgl�dem wysoko�ci i szeroko�ci okna

	for (int x = 0; x < WIDTH; x++) {//Rozpocz�cie p�tli przechodz�cej po ca�ej p�aszczy�nie

		float c_re = x_axis.min + x * Re_factor;//obliczanie cz�ci rzeczywistej liczby C

		for (int y = 0; y < HEIGHT; y++) {

			float c_im = y_axis.max - y * Im_factor;//Obliczenie cz�ci urojonej liczby C

			float Z_re = c_re, Z_im = c_im;

			int i = 0;

			for (; i < MAX_ITERATIONS; i++)
			{
				float Z_re2 = Z_re * Z_re, Z_im2 = Z_im * Z_im;

				if (Z_re2 + Z_im2 > 4)//Sprawdzenie czy liczba d��y do nieskonczono�ci
					break;

				Z_im = 2 * Z_re*Z_im + c_im;
				Z_re = Z_re2 - Z_im2 + c_re;
			}

			float bright = map(i, 0.0f, MAX_ITERATIONS, 0.0f, 1.0f);//Okre�lenie jasno�ci koloru wzgl�dem
																	//iteracji na pixelu
			if (i == MAX_ITERATIONS) {
				bright = 0.0f;
			}

			drawPixel(x, y, bright); 

		}
	}

}

void RenderScene(void){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Mandelbrot();

	glFlush();

	glutSwapBuffers();
}

/*************************************************************************************/

// Funkcja ustalaj�ca stan renderowania



void MyInit(void){
	//glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	

}

/*************************************************************************************/

// Funkcja s�u��ca do kontroli zachowania proporcji rysowanych obiekt�w
// niezale�nie od rozmiar�w okna graficznego



void ChangeSize(GLsizei horizontal, GLsizei vertical){
	
	GLfloat AspectRatio;

	WIDTH = horizontal;
	HEIGHT = vertical;

	if (vertical == 0)
		// Zabezpieczenie pzred dzieleniem przez 0

		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielko�ciokna okna urz�dzenia (Viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)

	glMatrixMode(GL_PROJECTION);
	

	glLoadIdentity();


	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	
	
	gluOrtho2D(0, horizontal, 0, vertical);

		
	glMatrixMode(GL_MODELVIEW);
	// Okre�lenie uk�adu wsp�rz�dnych     

	glLoadIdentity();
	
}


/*
	glViewport(0, 0, horizontal, vertical);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	WIDTH = horizontal;
	HEIGHT = vertical;

	gluOrtho2D(0, horizontal, 0, vertical);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

*/

/*************************************************************************************/

// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli



void main(void)
{

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	
	glutInitWindowSize(WIDTH, HEIGHT);

	glutCreateWindow("Mandelbrot");
	// Utworzenie okna i okre�lenie tre�ci napisu w nag��wku okna

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn� (callback)
	// Biblioteka GLUT b�dzie wywo�ywa�a t� funkcj� za ka�dym razem, gdy
	// trzeba b�dzie przerysowa� okno

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn� za
	// zmiany rozmiaru okna

	MyInit();
	// Funkcja MyInit (zdefiniowana powy�ej) wykonuje wszelkie 
	// inicjalizacje konieczneprzed przyst�pieniem do renderowania

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}

/*************************************************************************************/


