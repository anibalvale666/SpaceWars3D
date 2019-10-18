#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include <bits/stdc++.h>
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1
#define ECHAP 27

void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);
//function called on each frame
GLvoid window_idle();

//Delta time variables
int passed_time = 0;
int timebase = 0;
int reload_time = 0;
float delta_time = 1;

//Save when clic a key in keyboard
bool up_pressed = false;
bool down_pressed = false;
bool left_pressed = false;
bool right_pressed = false;
bool fire_pressed = false;
bool upload_score = false;

//Mathematic functions
float normal_vector(float x, float y)
{
	return sqrt( pow(x,2) + pow(y,2) );
}
//End mathematic functions

int current_score = 100; 

void show_score()
{
	glRasterPos3f(-240, 240,0);
	
	string header_score_str = "Score: ";

	stringstream ss;
	ss << current_score;
	string str = ss.str();
	
	string current_score_str = header_score_str + str;
	
	char *current_score_char = new char[current_score_str.length() + 1];
	strcpy(current_score_char, current_score_str.c_str());
	
	for (current_score_char; *current_score_char != '\0'; current_score_char++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *current_score_char);
	}
}

//Class Gun
class Gun
{
public:
	pair<double, double> position;
	float speed_move;
	pair<double, double> direction;
	int colision_radio;

	Gun(double x, double y);
	void move();
	void draw();
};

Gun::Gun(double x, double y)
{
	position.first = x;
	position.second = y;
	direction = make_pair(0,0);
	colision_radio = 1;
    speed_move = 150;
}

void Gun::move()
{
	position.second += speed_move * delta_time;
}

void Gun::draw()
{
	double x = position.first;
	double y = position.second;
	
	glBegin(GL_QUADS);
	
	glColor3f(1.0f,1.0f,1.0f);
	
	glVertex2f(x + colision_radio, y - colision_radio);
	glVertex2f(x + colision_radio, y + colision_radio);
	glVertex2f(x - colision_radio, y + colision_radio);
	glVertex2f(x - colision_radio, y - colision_radio);
	glEnd();
}
//End class Gun

//Class GameMap, contains info about the screen size and limits
class GameMap
{
private:
	double screen_width;
	double screen_height;
	pair<double, double> screen_limits_x;
	
	double size_gaming_zone;
	double size_player_zone;
public:
	pair<double, double> screen_limits_y;
	GameMap(double screen_w, double screen_h);
	double get_screen_width();
	double get_screen_height();
	double get_size_gaming_zone();
	double get_size_player_zone();
	pair<double, double> get_screen_limits_x();
	pair<double, double> get_screen_limits_y();
	
	bool is_in_player_zone(double pos_x, double pos_y);
};

GameMap::GameMap(double screen_w, double screen_h)
{
	size_player_zone = 210;
	
	screen_width = screen_w;
	screen_height = screen_h;
	
	screen_limits_x = make_pair((screen_width/2)*(-1) , (screen_width/2));
	screen_limits_y = make_pair((screen_height/2)*(-1) , (screen_height/2));
	
	size_gaming_zone = screen_height - size_player_zone; 
	
}

double GameMap::get_screen_width() { return screen_width; }
double GameMap::get_screen_height() { return screen_height; }
double GameMap::get_size_gaming_zone() { return size_gaming_zone; }
double GameMap::get_size_player_zone() { return size_player_zone; }
pair<double, double> GameMap::get_screen_limits_x() { return screen_limits_x; }
pair<double, double> GameMap::get_screen_limits_y() { return screen_limits_y; }

bool GameMap::is_in_player_zone(double pos_x, double pos_y)
{
	double max_y = (screen_height / 2) - size_gaming_zone;
	
	if ((pos_y < max_y) and (pos_y > screen_limits_y.first) and (pos_x < screen_limits_x.second) and (pos_x > screen_limits_x.first))
		return true;
	
	return false;
}
//End class GameMap

GameMap my_map(490, 490);

//Class Player
class Player
{
public:
	pair<double, double> position; 
	float speed_move;
	vector<Gun> balls;
	
	Player(double initial_pos_x, double initial_pos_y);
	void draw();
	void move();
	void fire();

	pair<double, double> get_position();
};

Player::Player(double initial_pos_x, double initial_pos_y)
{
	position.first = initial_pos_x;
	position.second = initial_pos_y;
	speed_move = 100;
}

void Player::move()
{
	double new_pos = 0;

	if (up_pressed)
	{
		new_pos = position.second + (speed_move * delta_time);
		
		if (my_map.is_in_player_zone( position.first, new_pos ))
			position.second = new_pos;
	}

	if (down_pressed)
	{
		new_pos = position.second - (speed_move * delta_time);
		
		if (my_map.is_in_player_zone( position.first, new_pos ))
			position.second = new_pos;
	}

	if (left_pressed)
	{
		new_pos =  position.first - (speed_move * delta_time);
		
		if (my_map.is_in_player_zone( new_pos, position.second ))
			position.first = new_pos;
	}

	if (right_pressed)
	{
		new_pos =  position.first + (speed_move * delta_time);
		
		if (my_map.is_in_player_zone( new_pos, position.second ))
			position.first = new_pos;
	}
}

void Player::draw()
{
	double x = position.first;
	double y = position.second;
	
	glPointSize(8.0);
	
	glBegin(GL_POINTS);//front shooter
	glColor3f(1,1,1);//111
	glVertex2f(x, y + 24);
	glEnd();
	
	glBegin(GL_QUADS);
	glColor3f(0.137255,0.419608,0.556863);//middle body
	glVertex2f(x - 10, y + 10);
	glVertex2f(x - 10, y - 10);
	glVertex2f(x + 10, y - 10);
	glVertex2f(x + 10, y + 10);
	glEnd();
	
	glBegin(GL_POINTS);//design on middle
	glColor3f(0.90,0.91,0.98);
	glVertex2f(x-10,y-5);
	glVertex2f(x+10,y-5);
	glEnd();
	
	glBegin(GL_POINTS);//thrusters
	glColor3f(1,1,1);
	glVertex2f(x-7,y-12);
	glVertex2f(x+7,y-12);
	glEnd();
	
	glBegin(GL_TRIANGLES);//right wing
	glColor3f(0.196078,0.8,0.196078);
	glVertex2f(x+10,y+10);
	glVertex2f(x+10,y-10);
	glVertex2f(x+25,y-15);
	glEnd();
	
	glBegin(GL_TRIANGLES);//left wing
	glColor3f(0.196078,0.8,0.196078);
	glVertex2f(x-10,y+10);
	glVertex2f(x-10,y-10);
	glVertex2f(x-25,y-15);
	glEnd();
	
	
	glBegin(GL_QUADS);//up body
	glColor3f(0.99609, 0.83984, 0);
	glVertex2f(x - 10, y + 10);
	glVertex2f(x - 18, y + 22);
	glVertex2f(x + 18, y + 22);
	glVertex2f(x + 10, y + 10);
	glEnd();
}

void Player::fire()
{
	balls.push_back(Gun(position.first, position.second));
}

pair<double, double> Player::get_position() { return position; }
//End class Player

Player *my_player;

void mover_proyectiles();
void dibujar_proyectiles();

GLvoid callback_special(int key, int x, int y)
{
	glMatrixMode(GL_PROJECTION);
	if ( key == GLUT_KEY_UP )
	{
		up_pressed = true;
		glutPostRedisplay();			
	}
	if ( key == GLUT_KEY_DOWN )
	{
		down_pressed = true;
		glutPostRedisplay();
	}
	if ( key == GLUT_KEY_LEFT )
	{
		left_pressed = true;
		glutPostRedisplay();
	}
	if ( key == GLUT_KEY_RIGHT )
	{
		right_pressed = true;
		glutPostRedisplay();
	}
}

GLvoid callback_specialUp(int key, int x, int y)
{
	glMatrixMode(GL_PROJECTION);
	if ( key == GLUT_KEY_UP  )
	{
		up_pressed = false;
		glutPostRedisplay();
	}
	if ( key == GLUT_KEY_DOWN )
	{
		down_pressed = false;
		glutPostRedisplay();
	}
	if ( key == GLUT_KEY_LEFT )
	{
		left_pressed =false;
		glutPostRedisplay();
	}
	if ( key == GLUT_KEY_RIGHT )
	{
		right_pressed = false;
		glutPostRedisplay();
	}
}

GLvoid window_key(unsigned char key, int x, int y)
{
	if( key == 'z')
		fire_pressed = true;
	

	else if (key == 't')
	{
		current_score -= 10;
		upload_score = true;
	}
}

GLvoid window_keyUp(unsigned char key, int x, int y)
{
	if( key == 'z')
		fire_pressed = false;
}

GLvoid callback_mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		
	}
}

GLvoid callback_motion(int x, int y)
{
	glutPostRedisplay(); 
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Game");
	
	my_player = new Player(0,-55);

	initGL();
	init_scene();
	
	
	glutDisplayFunc(&window_display);
	
	glutKeyboardFunc(&window_key);
	glutKeyboardUpFunc(&window_keyUp);
	
	glutSpecialFunc(&callback_special);
	glutSpecialUpFunc(&callback_specialUp);
	glutMouseFunc(&callback_mouse);
	glutMotionFunc(&callback_motion);
	
	glutIdleFunc(&window_idle);
	
	glutMainLoop();
	
	return 1;
}

GLvoid initGL()
{
	GLfloat position[] = { 0.0f, 5.0f, 10.0f, 0.0 };
	
	//enable light : try without it
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHTING);
	//light 0 "on": try without it
	glEnable(GL_LIGHT0);
	
	//shading model : try GL_FLAT
	glShadeModel(GL_SMOOTH);
	
	glEnable(GL_DEPTH_TEST);
	
	//enable material : try without it
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	glClearColor(RED, GREEN, BLUE, ALPHA);
}

GLvoid window_display()
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20.0f, 20.0f, -20.0f, 20.0f, -20.0f, 20.0f);
	gluPerspective(45.0, 1.0, 1.0, 100.0);
	glTranslatef(0.0f, 0.0f, -30.0f);

	passed_time = glutGet(GLUT_ELAPSED_TIME); 
	delta_time = float(passed_time -timebase)/1000.0;
	timebase = passed_time;
	
	show_score();
	
	my_player->move();
	my_player->draw();

	
	//Dibujar_proyectiles
	dibujar_proyectiles();
	glutSwapBuffers();
	
	glFlush();
}



GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);
	glMatrixMode(GL_MODELVIEW);
}

void init_scene()
{
	
}

//Function called on each frame
GLvoid window_idle()
{
	//Para disparar, se requiere una recarga de 10 ciclos
	if( fire_pressed == true)
	{
		if(reload_time == 0)
		{
			my_player->fire();
			reload_time = 100;
		}
		else
		{
			reload_time--;
		}
	}

	mover_proyectiles();
	glutPostRedisplay();
}

void mover_proyectiles()
{
	for (int i = 0; i < my_player->balls.size();)
	{
		my_player->balls[i].move();
		if( abs(my_player->balls[i].position.second) >= my_map.screen_limits_y.second)
		{
			my_player->balls.erase(my_player->balls.begin() + i);
		}
		else
		{
			++i;
		}
	}
}

void dibujar_proyectiles()
{
	for (int i = 0; i < my_player->balls.size(); i++) {
		my_player->balls[i].draw();
	}
}
