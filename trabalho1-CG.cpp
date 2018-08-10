#include <iostream>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <math.h>
#include <list>


using namespace std;

int memsafe = -10;
class Ponto {
    public:
        float x;
        float y;
    Ponto(float givenX, float givenY){
        this->x = givenX;
        this->y = givenY;
    }
};

list<Ponto> pontos;

Ponto pontoMedio(Ponto ponto1, Ponto ponto2){
    return Ponto((ponto1.x + ponto2.x)/2, (ponto1.y + ponto2.y)/2);
}

void smoothPoints(){
    list<Ponto> result;
    
    list<Ponto>::iterator aux = pontos.begin();
    result.push_back(*(pontos.begin()));

    for (list<Ponto>::iterator it = pontos.begin();
         it != pontos.end(); ++it)
    {    
    
        list<Ponto>::iterator first, second, third;
        first = second = third = it;
        advance(second, 1);
        
        if(second == pontos.end()){
            break;
        }
        advance(third, 2);

        if(third == pontos.end()){
            break;
        }
        result.push_back(pontoMedio(pontoMedio(*first, *second), *second));
        result.push_back(pontoMedio(pontoMedio(*second, *third), *second));
    }
    advance(aux, 1);
    if(aux != pontos.end()){
        aux = pontos.end();
        --aux;
        result.push_back(*aux);
    }

    pontos = result;
}

void mouse (int button, int state, int givenX, int givenY)
{
    // Respond to mouse button presses.
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        pontos.push_back(Ponto((2.*givenX)/500 - 1., -(2.*givenY)/500 + 1.));
        glutPostRedisplay();
    }
}

void keyboard_cb(unsigned char key, int X, int Y)
{
    switch (key)
    {
        case 27:             // ESCAPE key
            exit (0);
            break;
            
        case 's':

            if(memsafe < 6){
                smoothPoints();
            }
            memsafe++;
            glutPostRedisplay();
            break;
    }
}


void display() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0);
    glPointSize(8.0);
    
    glLineWidth(2);
    

    glBegin(GL_LINE_STRIP);

    glColor3f(1.0, 0.0, 0.0);

    for (list<Ponto>::iterator it = pontos.begin();
         it != pontos.end(); ++it)
    {      
        glColor3f(1.0, 0.0, 0.0);
        float px = it->x;
        float py = it->y;
        cout << "X: " << px << " Y: " << py << endl; 
        glVertex2f(px,py);
    }
    glEnd();    
    glFlush();
    glutSwapBuffers();
}


int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize (500, 500);
    glutCreateWindow("Pontos");
    glutKeyboardFunc(keyboard_cb);
    glutMouseFunc(mouse);
    
    glutDisplayFunc(display);
    glutMainLoop();
    
    return EXIT_SUCCESS ;

}
