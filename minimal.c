#include <windows.h>
#include <gl/glut.h>
#include <math.h>
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "paulslib.h"

//http://paulbourke.net/geometry/verrill/

//The source code creates polygonal facets in the "geom" format.Most 
//of the complexity comes from deriving colours and normals at each vertex, 
//of course this is necessary in order to get a smooth surface
//with a limited polygonal representation.
XYZ Eval(double,double);
#define N 151
//int main(int argc,char **argv)
int gen_Data()
{
   int i,j,k;
   double u,v,u1,v1,delta=0.001;
   XYZ p[4],n[4],p1,p2;
   COLOUR colour[4];
   COLOUR black = {0.0,0.0,0.0};
   glBegin(GL_QUADS);
   for (i=0;i<N;i++) {
	   for (j = 0; j < N; j++) {

		   u = i * TWOPI / N;
		   u1 = (i + 1) * TWOPI / N;
		   v = 0.4 + j / N;
		   v1 = 0.4 + (j + 1) / N;

		   p[0] = Eval(u, v);
		   p1 = Eval(u + delta, v);
		   p2 = Eval(u, v + delta);
		   n[0] = CalcNormal(p[0], p1, p2);

		   p[1] = Eval(u1, v);
		   p1 = Eval(u1 + delta, v);
		   p2 = Eval(u1, v + delta);
		   n[1] = CalcNormal(p[1], p1, p2);

		   p[2] = Eval(u1, v1);
		   p1 = Eval(u1 + delta, v1);
		   p2 = Eval(u1, v1 + delta);
		   n[2] = CalcNormal(p[2], p1, p2);

		   p[3] = Eval(u, v1);
		   p1 = Eval(u + delta, v1);
		   p2 = Eval(u, v1 + delta);
		   n[3] = CalcNormal(p[3], p1, p2);

		   colour[0] = GetColour(u, 0.0, TWOPI, 4);
		   colour[1] = GetColour(u1, 0.0, TWOPI, 4);
		   colour[2] = GetColour(u1, 0.0, TWOPI, 4);
		   colour[3] = GetColour(u, 0.0, TWOPI, 4);

		   //printf("f4nc ");
		   //for (k = 0; k < 4; k++)
			  // printf("%g %g %g ", p[k].x, p[k].y, p[k].z);
		   //for (k = 0; k < 4; k++)
			  // printf("%g %g %g ", n[k].x, n[k].y, n[k].z);
		   //for (k = 0; k < 4; k++)
			  // printf("%g %g %g ", colour[k].r, colour[k].g, colour[k].b);
		   //printf("\n");

		   for (k = 0; k < 4; k++)
		   {
			   glColor3f(colour[k].r, colour[k].g, colour[k].b);
			   glNormal3f(n[k].x, n[k].y, n[k].z);
			   glVertex3f(p[k].x, p[k].y, p[k].z);
			}	
      }
   }
   glEnd();
   return 0;
}

XYZ Eval(double u,double v)
{
   XYZ p;

   p.x = -20 * v * cos(u) + (20 * cos(u))/v - (20 * v*v*v * cos(3*u))/3; 
   p.y =  60 * v * sin(u) - (20 * sin(u))/v - (20 * v*v*v * sin(3*u))/3;
   p.z =  40 * log(v);

   return(p);
}




//Բ���ʺ�
#define GL_PI 3.1415f
//��ȡ��Ļ�Ŀ��
GLint SCREEN_WIDTH = 0;
GLint SCREEN_HEIGHT = 0;
//���ó���Ĵ��ڴ�С
GLint windowWidth = 400;
GLint windowHeight = 300;
GLfloat roate = 0.0;// set rote of roate ying yu bu hao  bu zhuang le ������ת����
GLfloat rote = 0.0;//shezhi��ת�Ƕ�
GLfloat anglex = 0.0;//X ����ת
GLfloat angley = 0.0;//Y ����ת
GLfloat anglez = 0.0;//Z ����ת
GLfloat oldx;//���������ʱ��¼�������  
GLfloat oldy;
//��x����ת�Ƕ�
GLfloat xRotAngle = 0.0f;
//��y����ת�Ƕ�
GLfloat yRotAngle = 0.0f;

//�Ƿ�򿪲���ʾ���湦��
GLint cullFace = 0;
//�Ƿ����Ȳ��Թ���
GLint depthTest = 0;
//�����ǲ��������������ʹ�õ�ɫ��䡡
GLint outLine = 0;
//�˵��ص�����
void processMenu(int value) {
	switch (value) {
	case 1:
		depthTest = !depthTest;
		break;
	case 2:
		cullFace = !cullFace;
	case 3:
		outLine = !outLine;
		break;
	default:
		break;
	}
	//���»���
	glutPostRedisplay();
}
#define MAX_CHAR        128
void drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) { // ����ǵ�һ�ε��ã�ִ�г�ʼ��
						 // Ϊÿһ��ASCII�ַ�����һ����ʾ�б�
		isFirstCall = 0;

		// ����MAX_CHAR����������ʾ�б���
		lists = glGenLists(MAX_CHAR);

		// ��ÿ���ַ��Ļ������װ����Ӧ����ʾ�б���
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// ����ÿ���ַ���Ӧ����ʾ�б�����ÿ���ַ�
	for (; *str != '\0'; ++str)
			glCallList(lists + *str);
}

//��ʾ�ص�����
void renderScreen(void) {

	drawString("");//��仰ɾ����,XYZ������ʾ
	 
	GLfloat x, y, z;
	//��������������Ϊ��ǰ������ɫ����ɫ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//����ǰMatrix״̬��ջ
	glPushMatrix();
	glRotatef(rote, 0.0f, 1.0f, 0.0f);
	glRotatef(anglex, 1.0, 0.0, 0.0);
	glRotatef(angley, 0.0, 1.0, 0.0);
	glRotatef(anglez, 0.0, 0.0, 1.0);

	//����ϵ��x����תxRotAngle
	glRotatef(xRotAngle, 1.0f, 0.0f, 0.0f);
	//����ϵ��y����תyRotAngle
	glRotatef(yRotAngle, 0.0f, 1.0f, 0.0f);
	if (depthTest)
		glEnable(GL_DEPTH_TEST);//����Ȳ��Թ���
	else
		glDisable(GL_DEPTH_TEST);//�ر���Ȳ��Թ���
	if (cullFace)
		glEnable(GL_CULL_FACE);//�򿪲���ʾ���湦��
	else
		glDisable(GL_CULL_FACE);//�رղ���ʾ���湦��
	if (outLine)
		glPolygonMode(GL_BACK, GL_LINE);//����α���ʹ���������
	else
		glPolygonMode(GL_BACK, GL_FILL);//����α���ʹ�õ�ɫ���
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	//����ƽ������
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	//��������ϵ
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(-80.0f, 0.0f, 0.0f);
	glVertex3f(80.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -80.0f, 0.0f);
	glVertex3f(0.0f, 80.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -80.0f);
	glVertex3f(0.0f, 0.0f, 80.0f);
	glEnd();

	glPushMatrix();
	glTranslatef(80.0f, 0.0f, 0.0f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glutWireCone(3, 6, 10, 10);	
	glRasterPos3f(10.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);//��Ϊ��ɫ����ʧЧ
	drawString("X");
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 80.0f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glutWireCone(3, 6, 10, 10);	
	glRasterPos3f(0.0f, 10.0f, 0.0f);
	drawString("Y");
	
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 80.0f);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glutWireCone(3, 6, 10, 10);	
	glRasterPos3f(0.0f, 0.0f, 10.0f);
	drawString("Z");
	
	glPopMatrix();

	////��ʼ���������ı���
	//glBegin(GL_QUADS);
	////��ɫ���Ƶ�һ���ı���
	//glColor3f(1.0f, 0.0f, 0.0f);
	//glVertex3f(0.0f, 0.0f, 0.0f);

	//glVertex3f(25.0f, 0.0f, 0.0f);
	//glVertex3f(30.0f, 30.0f, 0.0f);
	//glVertex3f(0.0f, 25.0f, 0.0f);

	////��ɫ���Ƶڶ����ı���
	//glColor3f(0.0f, 1.0f, 0.0f);
	//glVertex3f(0.0f, 0.0f, 0.0f);
	//glVertex3f(-25.0f, 0.0f, 0.0f);
	//glVertex3f(-30.0f, -30.0f, 0.0f);
	//glVertex3f(0.0f, -25.0f, 0.0f);
	////�������ı���
	//glEnd();
	gen_Data();
	rote += roate;
	//�ָ�ѹ��ջ��Matrix
	glPopMatrix();
	//����������������ָ��
	glutSwapBuffers();
}
//����Redering State 
void setupRederingState(void) {
	//����������ɫΪ��ɫ
	glClearColor(0.0f, 0.0, 0.0, 1.0f);
	//���û滭��ɫΪ��ɫ
	glColor3f(0.0f, 1.0f, 0.0f);
}
//���ڴ�С�仯�ص�����
void changSize(GLint w, GLint h) {
	//������
	GLfloat ratio;
	//��������ϵΪx(-100.0f,100.0f)��y(-100.0f,100.0f)��z(-100.0f,100.0f)
	GLfloat coordinatesize = 100.0f;
	//���ڿ��Ϊ��ֱ�ӷ���
	if ((w == 0) || (h == 0))
		return;
	//�����ӿںʹ��ڴ�Сһ��
	glViewport(0, 0, w, h);
	//��ͶӰ����Ӧ�����ľ������
	glMatrixMode(GL_PROJECTION);
	//���õ�ǰָ���ľ���Ϊ��λ����
	glLoadIdentity();
	ratio = (GLfloat)w / (GLfloat)h;
	//����ͶӰ
	if (w < h)
		glOrtho(-coordinatesize, coordinatesize, -coordinatesize / ratio, coordinatesize / ratio, -coordinatesize, coordinatesize);
	else
		glOrtho(-coordinatesize * ratio, coordinatesize*ratio, -coordinatesize, coordinatesize, -coordinatesize, coordinatesize);
	//��ģ����ͼ�����ջӦ�����ľ������
	glMatrixMode(GL_MODELVIEW);
	//���õ�ǰָ���ľ���Ϊ��λ����
	glLoadIdentity();
}
//https ://blog.csdn.net/u013470545/article/details/59704087
void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			roate = 0;
			rote = 0;
			oldx = x;//���������ʱ��¼�������  
			oldy = y;
			//printf("left");
		}

	}
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			roate += 1.0f;
			//printf("right");
		}

	}
//https ://blog.csdn.net/qq_33519317/article/details/98250689
}

void onMouseMove(int x, int y)   //��������϶�
{
	GLint deltax = oldx - x;
	GLint deltay = oldy - y;
	anglex += 360 * (GLfloat)deltax / (GLfloat)SCREEN_WIDTH;//������Ļ����껬���ľ�����������ת�ĽǶ�  
	angley += 360 * (GLfloat)deltay / (GLfloat)SCREEN_HEIGHT;
	anglez += 360 * (GLfloat)deltay / (GLfloat)SCREEN_HEIGHT;
	oldx = x;//��¼��ʱ��������꣬�����������  
	oldy = y;//����û����������䣬��������ת���ò��ɿ�  
	glutPostRedisplay();
	glutPostRedisplay();
}



//�������봦��ص�����
void specialKey(int key, int x, int y) {

	if (key == GLUT_KEY_UP) {
		xRotAngle -= 5.0f;
	}
	else if (key == GLUT_KEY_DOWN) {
		xRotAngle += 5.0f;
	}
	else if (key == GLUT_KEY_LEFT) {
		yRotAngle -= 5.0f;
	}
	else if (key == GLUT_KEY_RIGHT) {
		yRotAngle += 5.0f;
	}
	//���»���
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	//��ʼ��glut 
	glutInit(&argc, argv);
	//ʹ��˫������ģʽ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	//��ȡϵͳ�Ŀ�����
	SCREEN_WIDTH = glutGet(GLUT_SCREEN_WIDTH);
	//��ȡϵͳ�ĸ�����
	SCREEN_HEIGHT = glutGet(GLUT_SCREEN_HEIGHT);
	//�������ڣ���������ΪOpenGL TriAngle Demo
	glutCreateWindow("OpenGL Quads Demo");
	//�˵��ص�����
	glutCreateMenu(processMenu);
	//��Ӳ˵�
	glutAddMenuEntry("Toggle depth test", 1);
	glutAddMenuEntry("Toggle cull face", 2);
	glutAddMenuEntry("Toggle back outline", 3);
	//���˵��񶨵�����Ҽ���
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	//���ô��ڴ�С
	glutReshapeWindow(windowWidth, windowHeight);
	//���ھ�����ʾ
	glutPositionWindow((SCREEN_WIDTH - windowWidth) / 2, (SCREEN_HEIGHT - windowHeight) / 2);
	//���ڴ�С�仯ʱ�Ĵ�����
	glutReshapeFunc(changSize);

	glutMouseFunc(Mouse);
	glutMotionFunc(onMouseMove);

	//������ʾ�ص����� 
	glutDisplayFunc(renderScreen);
	//���ð������봦��ص�����
	glutSpecialFunc(specialKey);
	//����ȫ����Ⱦ����
	setupRederingState();
	glutMainLoop();
	return 0;
}
//https ://blog.csdn.net/yearafteryear/article/details/9090269