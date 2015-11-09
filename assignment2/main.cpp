#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <cmath>
#include <vector>
#include"Vector.h"
#include "Camera.h"

#include "rigidstate.h"
#include "rigidstateA.h"



#define automaticSpeed 20
#define SIZE 5
#define HalfSIZE 2.5

#define MASS 1

#define ErrorThr 0.001

float hStep = 0.01;

Rigidstate rigidState;
RigidstateA rigidStateA;
Rigidstate  rigidStateNew;

Matrix3x3 Io(MASS*1.0/12*(SIZE*SIZE*2),0,0, 0,MASS*1.0/12*(SIZE*SIZE*2),0, 0,0,MASS*1.0/12*(SIZE*SIZE*2));

#define VERTEXNUMBER 8
Vector3d bodyCenter(0,9,0);

Vector3d vertexForce[8]={Vector3d(0,-1,0),Vector3d(0,0,0),Vector3d(0,0,0),Vector3d(0,0,0),
                        Vector3d(0,0,0),Vector3d(0,0,0),Vector3d(0,0,0),Vector3d(0,0,0)};

Vector3d vertexPos[VERTEXNUMBER];

Vector3d Vpn(0,1,0);
double Cr = 0.5;

Vector3d bodyForce(0,-5,0);
//Vector3d totalTorque(0,0,0);

int WIDTH = 1000;
int HEIGHT = 800;

Camera *camera;

bool  resetSign = false;

bool  showGrid = false;

Vector3d vertexPosNew[VERTEXNUMBER];


void mouseEventHandler(int button, int state, int x, int y) {
    // let the camera handle some specific mouse events (similar to maya)
    camera->HandleMouseEvent(button, state, x, y);
    glutPostRedisplay();
}

void motionEventHandler(int x, int y) {
    // let the camera handle some mouse motions if the camera is to be moved
    camera->HandleMouseMotion(x, y);
    glutPostRedisplay();
}

// draws a simple grid
void makeGrid()
{
    //Setting  material
    GLfloat ball_mat_ambient[]  = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat ball_mat_diffuse[]  = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat ball_mat_specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat ball_mat_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat ball_mat_shininess  = 10.0f;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT,   ball_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   ball_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  ball_mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION,  ball_mat_emission);
    glMaterialf (GL_FRONT, GL_SHININESS, ball_mat_shininess);
    
    glLineWidth(1.0);
    
    for (float i=-12; i<12; i++) {
        for (float j=-12; j<12; j++) {
            glBegin(GL_LINES);
            glVertex3f(i, 0, j);
            glVertex3f(i, 0, j+1);
            glEnd();
            glBegin(GL_LINES);
            glVertex3f(i, 0, j);
            glVertex3f(i+1, 0, j);
            glEnd();
            
            if (j == 11){
                glBegin(GL_LINES);
                glVertex3f(i, 0, j+1);
                glVertex3f(i+1, 0, j+1);
                glEnd();
            }
            if (i == 11){
                glBegin(GL_LINES);
                glVertex3f(i+1, 0, j);
                glVertex3f(i+1, 0, j+1);
                glEnd();
            }
        }
    }
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex3f(-12, 0, 0);
    glVertex3f(12, 0, 0);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(0, 0, -12);
    glVertex3f(0, 0, 12);
    glEnd();
    glLineWidth(1.0);
}

void rigidBodyInit()
{
    
    vertexPos[0] = Vector3d(-HalfSIZE,-HalfSIZE,HalfSIZE)+bodyCenter;
    vertexPos[1] = Vector3d(HalfSIZE,-HalfSIZE,HalfSIZE)+bodyCenter;
    vertexPos[2] = Vector3d(HalfSIZE,-HalfSIZE,-HalfSIZE)+bodyCenter;
    vertexPos[3] = Vector3d(-HalfSIZE,-HalfSIZE,-HalfSIZE)+bodyCenter;
    vertexPos[4] = Vector3d(-HalfSIZE,HalfSIZE,HalfSIZE)+bodyCenter;
    vertexPos[5] = Vector3d(HalfSIZE,HalfSIZE,HalfSIZE)+bodyCenter;
    vertexPos[6] = Vector3d(HalfSIZE,HalfSIZE,-HalfSIZE)+bodyCenter;
    vertexPos[7] = Vector3d(-HalfSIZE,HalfSIZE,-HalfSIZE)+bodyCenter;
    
    
    
    Matrix3x3 m(1,0,0, 0,1,0, 0,0,1);
    rigidState.xposition = bodyCenter;
    rigidState.quater = Quaternion(m);
    rigidState.pfmom = Vector3d(0,0,0);
    rigidState.lamom = Vector3d(0,0,0);
    
    Vector3d w(0,0,0);
    rigidStateA.velocity = Vector3d(0,0,0);
    rigidStateA.quaterA = 0.5*w*(rigidState.quater);
    rigidStateA.force = Vector3d(0,0,0);
    rigidStateA.torque = Vector3d(0,0,0);
    
    
    
}


void myDisplay(void)
{
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    // draw the camera created in perspective
    camera->PerspectiveDisplay(WIDTH, HEIGHT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glTranslatef(0,0,0);
    glPushMatrix();
    if (showGrid)
        makeGrid();
    glPopMatrix();
    
     
    //Setting ball material
    GLfloat ball_mat_ambient[]  = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat ball_mat_diffuse[]  = {0.5f, 0.3f, 0.1f, 1.0f};
    GLfloat ball_mat_specular[] = {0.5f, 0.3f, 0.1f, 1.0f};
    GLfloat ball_mat_emission[] = {0.5f, 0.3f, 0.1f, 1.0f};
    GLfloat ball_mat_shininess  = 10.0f;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT,   ball_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   ball_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  ball_mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION,  ball_mat_emission);
    glMaterialf (GL_FRONT, GL_SHININESS, ball_mat_shininess);
    
    
    glPushMatrix();
    Matrix3x3 m = rigidState.quater.rotation();
    GLdouble RM[16] = {m[0][0],m[1][0],m[2][0],0,  m[0][1],m[1][1],m[2][1],0,  m[0][2],m[1][2],m[2][2],0, rigidState.xposition.x, rigidState.xposition.y, rigidState.xposition.z,1};
    glMultMatrixd(RM);
    //glTranslated(rigidState.xposition.x, rigidState.xposition.y, rigidState.xposition.z);
    glutSolidCube(SIZE);
    glPopMatrix();
    
    
    GLfloat ball1_mat_ambient[]  = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat ball1_mat_diffuse[]  = {0.5f, 0.0f, 0.1f, 1.0f};
    GLfloat ball1_mat_specular[] = {0.5f, 0.0f, 0.1f, 1.0f};
    GLfloat ball1_mat_emission[] = {0.5f, 0.0f, 0.1f, 1.0f};
    GLfloat ball1_mat_shininess  = 10.0f;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT,   ball1_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   ball1_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  ball1_mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION,  ball1_mat_emission);
    glMaterialf (GL_FRONT, GL_SHININESS, ball1_mat_shininess);
    
    glPushMatrix();
    for (int i=0; i<8; i++) {
        glPointSize(5);
        glBegin(GL_POINTS);
        glVertex3f(vertexPos[i].x,vertexPos[i].y, vertexPos[i].z);
        glEnd();
    }
    glPopMatrix();
    
    
    
    glFlush();
    glutSwapBuffers();
}







void update()
{
    rigidStateA.velocity = (1.0/MASS)*rigidState.pfmom;
    Matrix3x3 R = rigidState.quater.rotation();
    Matrix3x3 Iinverse = R * (Io.inv()) * (R.inv());
    Vector3d w = Iinverse * rigidState.lamom;
    
    
    rigidStateA.quaterA = 0.5 * w * rigidState.quater;

    Vector3d totalforce(0,0,0);
    for (int i=0; i<VERTEXNUMBER; i++) {
        totalforce = totalforce + vertexForce[i];
    }
    totalforce = totalforce + bodyForce;
    
    rigidStateA.force = totalforce ;
    
    Vector3d totalTorque(0,0,0);
    Vector3d torques[8];
    for (int i=0; i<VERTEXNUMBER; i++) {
        torques[i] = (vertexPos[i] - bodyCenter) % vertexForce[i];
        totalTorque = totalTorque + torques[i];
    }
    rigidStateA.torque = totalTorque;
    
    
    rigidStateNew.xposition = rigidState.xposition + (rigidStateA.velocity * hStep);
    rigidStateNew.quater = rigidState.quater + (rigidStateA.quaterA * hStep);
    rigidStateNew.pfmom = rigidState.pfmom + (rigidStateA.force * hStep);
    rigidStateNew.lamom = rigidState.lamom + (rigidStateA.torque * hStep);
  
    bodyCenter = rigidStateNew.xposition;

    vertexPosNew[0] = R*(Vector3d(-HalfSIZE,-HalfSIZE,HalfSIZE))+bodyCenter;
    vertexPosNew[1] = R*(Vector3d(HalfSIZE,-HalfSIZE,HalfSIZE))+bodyCenter;
    vertexPosNew[2] = R*(Vector3d(HalfSIZE,-HalfSIZE,-HalfSIZE))+bodyCenter;
    vertexPosNew[3] = R*(Vector3d(-HalfSIZE,-HalfSIZE,-HalfSIZE))+bodyCenter;
    vertexPosNew[4] = R*(Vector3d(-HalfSIZE,HalfSIZE,HalfSIZE))+bodyCenter;
    vertexPosNew[5] = R*(Vector3d(HalfSIZE,HalfSIZE,HalfSIZE))+bodyCenter;
    vertexPosNew[6] = R*(Vector3d(HalfSIZE,HalfSIZE,-HalfSIZE))+bodyCenter;
    vertexPosNew[7] = R*(Vector3d(-HalfSIZE,HalfSIZE,-HalfSIZE))+bodyCenter;
 
    
    for (int i=0; i<VERTEXNUMBER; i++) {
        if ((vertexPosNew[i].y-0)*(vertexPos[i].y-0)<0) {
            
            
            if (rigidStateA.velocity.norm()< 0.1) {
                resetSign = true;
            }
            
            Vector3d r = vertexPos[i] - bodyCenter;
            double Vn =  Vpn * (rigidStateA.velocity + w%r);
            double j = -1*(1+Cr)*Vn / ( 1.0/MASS + Vpn* (Iinverse*(r%Vpn)%r) );
            Vector3d J = j * Vpn;
            rigidState.pfmom = rigidState.pfmom + J;
            rigidState.lamom = rigidState.lamom + r%J;
            
            
            
            rigidStateA.velocity = (1.0/MASS)*rigidState.pfmom;
            Matrix3x3 R = rigidState.quater.rotation();
            Matrix3x3 Iinverse = R * (Io.inv()) * (R.inv());
            Vector3d w = Iinverse * rigidState.lamom;
            
            
            rigidStateA.quaterA = 0.5 * w * rigidState.quater;
            
            Vector3d totalforce(0,0,0);
            for (int i=0; i<VERTEXNUMBER; i++) {
                totalforce = totalforce + vertexForce[i];
            }
            totalforce = totalforce + bodyForce;
            
            rigidStateA.force = totalforce ;
            
            Vector3d totalTorque(0,0,0);
            Vector3d torques[8];
            for (int i=0; i<VERTEXNUMBER; i++) {
                torques[i] = (vertexPos[i] - bodyCenter) % vertexForce[i];
                totalTorque = totalTorque + torques[i];
            }
            rigidStateA.torque = totalTorque;
            
            
            rigidStateNew.xposition = rigidState.xposition + (rigidStateA.velocity * hStep);
            rigidStateNew.quater = rigidState.quater + (rigidStateA.quaterA * hStep);
            rigidStateNew.pfmom = rigidState.pfmom + (rigidStateA.force * hStep);
            rigidStateNew.lamom = rigidState.lamom + (rigidStateA.torque * hStep);
            
            bodyCenter = rigidStateNew.xposition;
            vertexPosNew[0] = R*(Vector3d(-HalfSIZE,-HalfSIZE,HalfSIZE))+bodyCenter;
            vertexPosNew[1] = R*(Vector3d(HalfSIZE,-HalfSIZE,HalfSIZE))+bodyCenter;
            vertexPosNew[2] = R*(Vector3d(HalfSIZE,-HalfSIZE,-HalfSIZE))+bodyCenter;
            vertexPosNew[3] = R*(Vector3d(-HalfSIZE,-HalfSIZE,-HalfSIZE))+bodyCenter;
            vertexPosNew[4] = R*(Vector3d(-HalfSIZE,HalfSIZE,HalfSIZE))+bodyCenter;
            vertexPosNew[5] = R*(Vector3d(HalfSIZE,HalfSIZE,HalfSIZE))+bodyCenter;
            vertexPosNew[6] = R*(Vector3d(HalfSIZE,HalfSIZE,-HalfSIZE))+bodyCenter;
            vertexPosNew[7] = R*(Vector3d(-HalfSIZE,HalfSIZE,-HalfSIZE))+bodyCenter;
            
            
        }
    }
    
    
    
    
    
    rigidState.xposition = rigidStateNew.xposition;
    rigidState.quater = rigidStateNew.quater;
    rigidState.pfmom = rigidStateNew.pfmom;
    rigidState.lamom = rigidStateNew.lamom;
    
    for (int i=0; i<VERTEXNUMBER; i++) {
        vertexPos[i] = vertexPosNew[i];
    }
    
    

    
}



void timeProc(int id)
{
    if (id == 1) {
        //claculate
        update();
        glutPostRedisplay();
        if (resetSign == false) {
            glutTimerFunc(automaticSpeed, timeProc, 1);
        }
    }
    
}

void handleKey(unsigned char key, int x, int y){
    switch(key){
        case 'a':
        case 'A':
            resetSign = false;
            glutTimerFunc(automaticSpeed, timeProc, 1);
            break;
        
        case 'g':
        case 'G':
            showGrid = !showGrid;
            break;
        
        case 's':
        case 'S':
            resetSign = true;
            break;
            

            
        case 'q':       // q - quit
        case 'Q':
        case 27:        // esc - quit
            exit(0);
            
        default:        // not a valid key -- just ignore it
            return;
    }
}


void init() {
    //LoadParameters(ParamFilename);
    // set up camera
    // parameters are eye point, aim point, up vector
    camera = new Camera(Vector3d(0, 0, 1), Vector3d(0, 0, 0), Vector3d(0, 1, 0));
    
    // black background for window
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    //glDepthRange(0, 1);
    
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
    
    //Setting lights
    
    GLfloat light0_position[] = {30.0f, 30.0f, 30.0f, 1.0f}; //spot light
    GLfloat light1_position[] = {-30.0f, -30.0f, -30.0f, 1.0f};
    GLfloat light0_ambient[]  = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat light0_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light0_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    
    
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT1, GL_AMBIENT,  light0_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light0_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light0_specular);
    
    
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
    
    
    rigidBodyInit();
    
    //std::cout<<rigidStateA.quaterA<<"\n";
    
}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Particle");
    
    init();
    
    glutDisplayFunc(myDisplay);
    glutMouseFunc(mouseEventHandler);
    glutMotionFunc(motionEventHandler);
    glutKeyboardFunc(handleKey);
    
    glutMainLoop();
    return 0;
}