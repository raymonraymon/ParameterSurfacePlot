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




//圆周率宏
#define GL_PI 3.1415f
//获取屏幕的宽度
GLint SCREEN_WIDTH = 0;
GLint SCREEN_HEIGHT = 0;
//设置程序的窗口大小
GLint windowWidth = 400;
GLint windowHeight = 300;
GLfloat roate = 0.0;// set rote of roate ying yu bu hao  bu zhuang le 设置旋转速率
GLfloat rote = 0.0;//shezhi旋转角度
GLfloat anglex = 0.0;//X 轴旋转
GLfloat angley = 0.0;//Y 轴旋转
GLfloat anglez = 0.0;//Z 轴旋转
GLfloat oldx;//当左键按下时记录鼠标坐标  
GLfloat oldy;
//绕x轴旋转角度
GLfloat xRotAngle = 0.0f;
//绕y轴旋转角度
GLfloat yRotAngle = 0.0f;

//是否打开不显示背面功能
GLint cullFace = 0;
//是否打开深度测试功能
GLint depthTest = 0;
//背面是采用线条描外框还是使用单色填充　
GLint outLine = 0;
//菜单回调函数
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
	//重新绘制
	glutPostRedisplay();
}
#define MAX_CHAR        128
void drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) { // 如果是第一次调用，执行初始化
						 // 为每一个ASCII字符产生一个显示列表
		isFirstCall = 0;

		// 申请MAX_CHAR个连续的显示列表编号
		lists = glGenLists(MAX_CHAR);

		// 把每个字符的绘制命令都装到对应的显示列表中
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// 调用每个字符对应的显示列表，绘制每个字符
	for (; *str != '\0'; ++str)
			glCallList(lists + *str);
}

//显示回调函数
void renderScreen(void) {

	drawString("");//这句话删除后,XYZ不会显示
	 
	GLfloat x, y, z;
	//把整个窗口清理为当前清理颜色：黑色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//将当前Matrix状态入栈
	glPushMatrix();
	glRotatef(rote, 0.0f, 1.0f, 0.0f);
	glRotatef(anglex, 1.0, 0.0, 0.0);
	glRotatef(angley, 0.0, 1.0, 0.0);
	glRotatef(anglez, 0.0, 0.0, 1.0);

	//坐标系绕x轴旋转xRotAngle
	glRotatef(xRotAngle, 1.0f, 0.0f, 0.0f);
	//坐标系绕y轴旋转yRotAngle
	glRotatef(yRotAngle, 0.0f, 1.0f, 0.0f);
	if (depthTest)
		glEnable(GL_DEPTH_TEST);//打开深度测试功能
	else
		glDisable(GL_DEPTH_TEST);//关闭深度测试功能
	if (cullFace)
		glEnable(GL_CULL_FACE);//打开不显示背面功能
	else
		glDisable(GL_CULL_FACE);//关闭不显示背面功能
	if (outLine)
		glPolygonMode(GL_BACK, GL_LINE);//多边形背部使用线条描框
	else
		glPolygonMode(GL_BACK, GL_FILL);//多边形背部使用单色填充
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	//进行平滑处理　
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	//绘制坐标系
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
	glColor3f(1.0f, 1.0f, 1.0f);//置为白色字体失效
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

	////开始绘制两个四边形
	//glBegin(GL_QUADS);
	////红色绘制第一个四边形
	//glColor3f(1.0f, 0.0f, 0.0f);
	//glVertex3f(0.0f, 0.0f, 0.0f);

	//glVertex3f(25.0f, 0.0f, 0.0f);
	//glVertex3f(30.0f, 30.0f, 0.0f);
	//glVertex3f(0.0f, 25.0f, 0.0f);

	////红色绘制第二个四边形
	//glColor3f(0.0f, 1.0f, 0.0f);
	//glVertex3f(0.0f, 0.0f, 0.0f);
	//glVertex3f(-25.0f, 0.0f, 0.0f);
	//glVertex3f(-30.0f, -30.0f, 0.0f);
	//glVertex3f(0.0f, -25.0f, 0.0f);
	////结束绘四边形
	//glEnd();
	gen_Data();
	rote += roate;
	//恢复压入栈的Matrix
	glPopMatrix();
	//交换两个缓冲区的指针
	glutSwapBuffers();
}
//设置Redering State 
void setupRederingState(void) {
	//设置清理颜色为黑色
	glClearColor(0.0f, 0.0, 0.0, 1.0f);
	//设置绘画颜色为绿色
	glColor3f(0.0f, 1.0f, 0.0f);
}
//窗口大小变化回调函数
void changSize(GLint w, GLint h) {
	//横宽比率
	GLfloat ratio;
	//设置坐标系为x(-100.0f,100.0f)、y(-100.0f,100.0f)、z(-100.0f,100.0f)
	GLfloat coordinatesize = 100.0f;
	//窗口宽高为零直接返回
	if ((w == 0) || (h == 0))
		return;
	//设置视口和窗口大小一致
	glViewport(0, 0, w, h);
	//对投影矩阵应用随后的矩阵操作
	glMatrixMode(GL_PROJECTION);
	//重置当前指定的矩阵为单位矩阵　
	glLoadIdentity();
	ratio = (GLfloat)w / (GLfloat)h;
	//正交投影
	if (w < h)
		glOrtho(-coordinatesize, coordinatesize, -coordinatesize / ratio, coordinatesize / ratio, -coordinatesize, coordinatesize);
	else
		glOrtho(-coordinatesize * ratio, coordinatesize*ratio, -coordinatesize, coordinatesize, -coordinatesize, coordinatesize);
	//对模型视图矩阵堆栈应用随后的矩阵操作
	glMatrixMode(GL_MODELVIEW);
	//重置当前指定的矩阵为单位矩阵　
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
			oldx = x;//当左键按下时记录鼠标坐标  
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

void onMouseMove(int x, int y)   //处理鼠标拖动
{
	GLint deltax = oldx - x;
	GLint deltay = oldy - y;
	anglex += 360 * (GLfloat)deltax / (GLfloat)SCREEN_WIDTH;//根据屏幕上鼠标滑动的距离来设置旋转的角度  
	angley += 360 * (GLfloat)deltay / (GLfloat)SCREEN_HEIGHT;
	anglez += 360 * (GLfloat)deltay / (GLfloat)SCREEN_HEIGHT;
	oldx = x;//记录此时的鼠标坐标，更新鼠标坐标  
	oldy = y;//若是没有这两句语句，滑动是旋转会变得不可控  
	glutPostRedisplay();
	glutPostRedisplay();
}



//按键输入处理回调函数
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
	//重新绘制
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	//初始化glut 
	glutInit(&argc, argv);
	//使用双缓冲区模式
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	//获取系统的宽像素
	SCREEN_WIDTH = glutGet(GLUT_SCREEN_WIDTH);
	//获取系统的高像素
	SCREEN_HEIGHT = glutGet(GLUT_SCREEN_HEIGHT);
	//创建窗口，窗口名字为OpenGL TriAngle Demo
	glutCreateWindow("OpenGL Quads Demo");
	//菜单回调函数
	glutCreateMenu(processMenu);
	//添加菜单
	glutAddMenuEntry("Toggle depth test", 1);
	glutAddMenuEntry("Toggle cull face", 2);
	glutAddMenuEntry("Toggle back outline", 3);
	//将菜单榜定到鼠标右键上
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	//设置窗口大小
	glutReshapeWindow(windowWidth, windowHeight);
	//窗口居中显示
	glutPositionWindow((SCREEN_WIDTH - windowWidth) / 2, (SCREEN_HEIGHT - windowHeight) / 2);
	//窗口大小变化时的处理函数
	glutReshapeFunc(changSize);

	glutMouseFunc(Mouse);
	glutMotionFunc(onMouseMove);

	//设置显示回调函数 
	glutDisplayFunc(renderScreen);
	//设置按键输入处理回调函数
	glutSpecialFunc(specialKey);
	//设置全局渲染参数
	setupRederingState();
	glutMainLoop();
	return 0;
}
//https ://blog.csdn.net/yearafteryear/article/details/9090269