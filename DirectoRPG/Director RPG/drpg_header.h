#ifndef _DROG_HEADER_
#define _DROG_HEADER_

// 使用drpg engine需要額外安裝以下函式庫
// freeglut(OpenGL Utility Toolkit), http://freeglut.sourceforge.net/
// glm(OpenGL Mathematics), http://glm.g-truc.net/0.9.4/index.html
// glee(OpenGL Easy Extension library), http://slackbuilds.org/repository/13.1/libraries/GLee/
// ilut(Devil), http://openil.sourceforge.net/

#include "drpg_vector3d.h"

#include <GL\glm\glm.hpp>
#include <GL\glm\gtc\matrix_transform.hpp>
#include <GL\glm\gtx\transform.hpp>
#include <GL\glm\gtc\type_ptr.hpp>

#include <string>
#include <tChar.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include <algorithm>
#include <mscoree.h>
#include <Windows.h>

#include <GL\GLee.h>
#include <GL\freeglut.h>

#define ILUT_USE_OPENGL
#include <IL\il.h>
#include <IL\ilu.h>
#include <IL\ilut.h>

#include <IK\irrKlang.h>

#define PI 3.14159265			// 圓周率

struct Button{
	bool Checked;
	bool Used;

	drpg::vector3 Site;
	drpg::vector3 Size;
};

#endif