/*************************************************************************************/

// Trochê bardziej skomplikowany program wykorzystuj¹cy funkcje biblioteki OpenGL

/*************************************************************************************/

#include <windows.h>

#include <gl/gl.h>

#include <glut.h>

#include <iostream>
/*************************************************************************************/


// Funkcaja okreœlaj¹ca, co ma byæ rysowane
// (zawsze wywo³ywana, gdy trzeba przerysowaæ scenê)

int WIDTH = 800;		
int HEIGHT = 800;

struct Point {//Struktura opisuj¹ca punkty na osi X i Y
	int x;
	int y;
};

struct Axis {//Struktura przechowuj¹ca informacje na temat
			 //wartoœci osi Re oraz Im
	float min;
	float max;
};

Axis x_axis = { -2.5, 1.5 };//Okreœlenie maks. i min. wartoœci osi Re 
Axis y_axis = { -1.25, 1.25 };//Im

int MAX_ITERATIONS = 40;//Ograniczenie iteracji

float map(float value, float in_min, float in_max, float out_min, float out_max)//Funkcja zwracaj¹ca wartoœæ
																				//danego punktu w zale¿noœci od
																				//danych ograniczeñ
{
	return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void drawPixel(int x, int y, float bright) {//Funkcja rysuj¹ca jedne piksel
	glBegin(GL_POINTS);
	glColor3f(bright, bright, bright); 
	glVertex2i(x, y);
	glEnd();
}

void Mandelbrot() {//Funkcja odpowiedzialna za rysowanie zbioru
	
	float Re_factor = (x_axis.max - x_axis.min) / (WIDTH - 1); //Obliczanie zmiennej wobec Re albo Im
	float Im_factor = (y_axis.max - y_axis.min) / (HEIGHT - 1);//wzglêdem wysokoœci i szerokoœci okna

	for (int x = 0; x < WIDTH; x++) {//Rozpoczêcie pêtli przechodz¹cej po ca³ej p³aszczyŸnie

		float c_re = x_axis.min + x * Re_factor;//obliczanie czêœci rzeczywistej liczby C

		for (int y = 0; y < HEIGHT; y++) {

			float c_im = y_axis.max - y * Im_factor;//Obliczenie czêœci urojonej liczby C

			float Z_re = c_re, Z_im = c_im;

			int i = 0;

			for (; i < MAX_ITERATIONS; i++)
			{
				float Z_re2 = Z_re * Z_re, Z_im2 = Z_im * Z_im;

				if (Z_re2 + Z_im2 > 4)//Sprawdzenie czy liczba d¹¿y do nieskonczonoœci
					break;

				Z_im = 2 * Z_re*Z_im + c_im;
				Z_re = Z_re2 - Z_im2 + c_re;
			}

			float bright = map(i, 0.0f, MAX_ITERATIONS, 0.0f, 1.0f);//Okreœlenie jasnoœci koloru wzglêdem
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

// Funkcja ustalaj¹ca stan renderowania



void MyInit(void){
	//glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	

}

/*************************************************************************************/

// Funkcja s³u¿¹ca do kontroli zachowania proporcji rysowanych obiektów
// niezale¿nie od rozmiarów okna graficznego



void ChangeSize(GLsizei horizontal, GLsizei vertical){
	
	GLfloat AspectRatio;

	WIDTH = horizontal;
	HEIGHT = vertical;

	if (vertical == 0)
		// Zabezpieczenie pzred dzieleniem przez 0

		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkoœciokna okna urz¹dzenia (Viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)

	glMatrixMode(GL_PROJECTION);
	

	glLoadIdentity();


	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	
	
	gluOrtho2D(0, horizontal, 0, vertical);

		
	glMatrixMode(GL_MODELVIEW);
	// Okreœlenie uk³adu wspó³rzêdnych     

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

// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli



void main(void)
{

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	
	glutInitWindowSize(WIDTH, HEIGHT);

	glutCreateWindow("Mandelbrot");
	// Utworzenie okna i okreœlenie treœci napisu w nag³ówku okna

	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹ (callback)
	// Biblioteka GLUT bêdzie wywo³ywa³a t¹ funkcjê za ka¿dym razem, gdy
	// trzeba bêdzie przerysowaæ okno

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹ za
	// zmiany rozmiaru okna

	MyInit();
	// Funkcja MyInit (zdefiniowana powy¿ej) wykonuje wszelkie 
	// inicjalizacje konieczneprzed przyst¹pieniem do renderowania

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}

/*************************************************************************************/


