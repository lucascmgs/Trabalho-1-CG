#include <iostream>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <list>
using namespace std;

const int WINDOWSIZE = 500;

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

//Calcula o ponto na metade da distância entre dois pontos
Ponto pontoMedio(Ponto ponto1, Ponto ponto2){
    return Ponto((ponto1.x + ponto2.x)/2, (ponto1.y + ponto2.y)/2);
}

void smoothPoints(){
    list<Ponto> result;
    
    list<Ponto>::iterator aux = pontos.begin();
    //Acrescenta na lista o primeiro ponto, que é fixo
    result.push_back(*(pontos.begin()));

    //Itera sobre a lista de pontos
    for (list<Ponto>::iterator it = pontos.begin();
         it != pontos.end(); ++it)
    {    
    
        list<Ponto>::iterator first, second, third;

        //Pegamos referências para três pontos, que representam uma "quina" das retas
        first = second = third = it;
        advance(second, 1);
        
        if(second == pontos.end()){
            break;
        }
        advance(third, 2);

        if(third == pontos.end()){
            break;
        }

        //Acrescenta os pontos necessários
        result.push_back(pontoMedio(pontoMedio(*first, *second), *second));
        result.push_back(pontoMedio(pontoMedio(*second, *third), *second));
    }

    advance(aux, 1);
    //Checa se a lista possui apenas um elemento, acrescentando o último ponto da lista, que também é fixo, caso contrário
    if(aux != pontos.end()){
        aux = pontos.end();
        --aux;
        result.push_back(*aux);
    }
    //Limpa a lista e atualiza com o resultado
    pontos.clear();
    pontos = move(result);
}

void resetPoints(){
    //Limpa a lista
    pontos.clear();
}

void mouse (int button, int state, int givenX, int givenY)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        pontos.push_back(Ponto((2.*givenX)/WINDOWSIZE - 1., -(2.*givenY)/WINDOWSIZE + 1.));
        glutPostRedisplay();
    }
}

void keyboard_cb(unsigned char key, int X, int Y)
{
    switch (key)
    {
        case 27:             
            exit (0);
            break;

        //Tecla para chamar a suaviação das retas
        case 's':
            smoothPoints();
            glutPostRedisplay();
            break;
        //Tecla para limpar os pontos
        case 'r':
            resetPoints();
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

   

    for (list<Ponto>::iterator it = pontos.begin();
         it != pontos.end(); ++it)
    {      
        glColor3f(0.0, 0.0, 1.0);
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
    glutInitWindowSize (WINDOWSIZE, WINDOWSIZE);
    glutCreateWindow("Pontos");
    glutKeyboardFunc(keyboard_cb);
    glutMouseFunc(mouse);
    
    glutDisplayFunc(display);
    glutMainLoop();
    
    return EXIT_SUCCESS ;

}
