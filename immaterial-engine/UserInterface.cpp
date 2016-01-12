
#include "OpenGL.h"
#include "DEBUGGING.h"

#include "MathEngine.h"
#include "UserInterface.h"
#include "ShaderTypes.h"
#include "CameraManager.h"
#include "CameraObject.h"
#include "GraphicsManager.h"

// -------------------------------------------------------------------------
// Window has changed size, or has just been created. In either case, we need
// to use the window dimensions to set the viewport and the projection matrix.
void ChangeSize(int w, int h)
{
	UNUSED(w);
	UNUSED(h);

	out("ChangeSize()\n");
}

void KeyboardKeys( unsigned char key, int x, int y )
{
	UNUSED(x);
	UNUSED(y);

	// space bar
	if ( key == 0x20 )
	{
		// swap camera to next camera in group
		CameraMan::SetCurrCamera(CameraMan::NextCamera());
	}

	// ************************ DEBUGGING FUNCTIONS ******************************
	if (key == 0x43 || key == 0x63)
	{
		GraphicsObjMan::DebugSwitch();
	}

	// ************************ MOVEMENT FUNCTIONS *******************************
	CameraObject *myCamera = CameraMan::GetCurrCamera();
	Vect Pos, Right, Dir, Up, LookAt;
	myCamera->getPos( Pos );
	myCamera->getRight( Right );
	myCamera->getDir( Dir );
	myCamera->getUp( Up );
	myCamera->getLookAt( LookAt );
	float scale = 5.0f;
	
	// "W" key (move forward)
	if (key == 0x57 || key == 0x77)
	{
		Vect newPos = Pos - (scale * Dir);
		Vect newLook = LookAt - (scale * Dir);
		myCamera->setOrientAndPosition(Up, newLook, newPos);
	}

	// "A" key (strafe left)
	if (key == 0x41 || key == 0x61)
	{
		Vect newPos = Pos - (scale * Right);
		Vect newLook = LookAt - (scale * Right);
		myCamera->setOrientAndPosition(Up, newLook, newPos);
	}
	
	// "S" key (move backwards)
	if (key == 0x53 || key == 0x73)
	{
		Vect newPos = Pos + (scale * Dir);
		Vect newLook = LookAt + (scale * Dir);
		myCamera->setOrientAndPosition(Up, newLook, newPos);
	}

	// "D" key (strafe right)
	if (key == 0x44 || key == 0x64)
	{
		Vect newPos = Pos + (scale * Right);
		Vect newLook = LookAt + (scale * Right);
		myCamera->setOrientAndPosition(Up, newLook, newPos);
	}

	// "I" key (raise camera up)
	if (key == 0x49 || key == 0x69)
	{
		Vect newPos = Pos + Up;
		Vect newLook = LookAt + Up;
		myCamera->setOrientAndPosition(Up, newLook, newPos);
	}

	// "K" key (lower camera down)
	if (key == 0x4B || key == 0x6B)
	{
		Vect newPos = Pos - Up;
		Vect newLook = LookAt - Up;
		myCamera->setOrientAndPosition(Up, newLook, newPos);
	}

	// ************************ CAMERA FUNCTIONS *********************************

	// "F" key (add camera)
	if ( key == 0x46 || key == 0x66 )
	{
		// adds another camera to the camera manager, sets it as the current camera
		CameraObject *cam = new CameraObject ();

		cam->setViewport( 0, 0, GAME_WIDTH, GAME_HEIGHT );
		cam->setPerspective( 35.0f, float(GAME_WIDTH)/float(GAME_HEIGHT), 1.0f, 250.0f );
		cam->setOrientAndPosition( Vect(0.0f, 1.0f, 0.0f), Vect(0.0f, 0.0f, 0.0f), Vect(0.0f, 0.0f, 100.0f) );

		CameraMan::addCamera( cam );
		CameraMan::SetCurrCamera( cam );
	}

	// "R" key (remove camera)
	if (key == 0x52 || key == 0x72 )
	{
		CameraMan::removeCamera();
	}

	// "T" key (toggle camera movement mode)
	if (key == 0x54 || key == 0x74 )
	{
		CameraMan::switchState();
	}

	// "G" key (focus camera to a Game object)
	if (key == 0x47 || key == 0x67)
	{
		// camera takes its current lookat, finds an object in the game object manager with the same startPos as the lookat.
		CameraObject *cam = CameraMan::GetCurrCamera();
		Vect lookAt, upDir, camPos;
		cam->getLookAt(lookAt);
		cam->getUp(upDir);
		cam->getPos(camPos);

		GraphicsObject *p = 0;
		p = GraphicsObjMan::FindByLocation(lookAt);

		// if its already getting the first object
		if (p == GraphicsObjMan::GetFirstObj())
		{
			// it then goes to the next object in the graphicsManager and grabs its startPos, and multiplies it by the objects localToWorld matrix.
			if (p->getSibling() != 0)
			{
				p = (GraphicsObject *)p->getSibling();
			}
		}
		// if the camera is not looking at first object
		else 
		{
			if (p != 0 && (GraphicsObject *)p->getSibling() != 0)
			{
				p = (GraphicsObject *)p->getSibling();
			}
			else
			{
				p = GraphicsObjMan::GetFirstObj();
			}
		}

		// it then sets that Vect as the camera's current lookat. 
		cam->setOrientAndPosition( upDir, p->getStartPos(), camPos);
		printf("Camera focus change. Looking at new game object.\n");
	}
}

void SpecialKeys(int key, int _x, int _y)
{
	UNUSED(_x);
	UNUSED(_y);

	// ************************** CAMERA ROTATION FUNCTIONS ********************
	CameraObject *myCamera = CameraMan::GetCurrCamera();

	float vert_angle = 0.0f;
	float horz_angle = 0.0f;
	float pan_angle = 0.0f;
	float tilt_angle = 0.0f;

	if(key == GLUT_KEY_UP)
	{
		if(CameraMan::getState() == ORBIT)
		{
			vert_angle = 0.1f;
		}
		else
		{
			tilt_angle = 0.01f;
		}
	}
	if(key == GLUT_KEY_DOWN)
	{
		if (CameraMan::getState() == ORBIT)
		{
			vert_angle = -0.1f;
		}
		else
		{
			tilt_angle = -0.01f;
		}
	}
	
	if(key == GLUT_KEY_LEFT)
	{
		if (CameraMan::getState() == ORBIT)
		{
			horz_angle = 0.1f;
		}
		else
		{
			pan_angle = 0.01f;
		}
	}
    
	if(key == GLUT_KEY_RIGHT)
	{
		if (CameraMan::getState() == ORBIT)
		{
			horz_angle = -0.1f;
		}
		else
		{
			pan_angle = -0.01f;
		}
	}

	Vect Tar;
	Vect Pos;
	Vect Up;
	Vect Dir;
	Vect Right;

	myCamera->getLookAt( Tar );
	myCamera->getPos( Pos );
	myCamera->getUp( Up );
	myCamera->getDir( Dir );
	myCamera->getRight( Right );

	Matrix T_neg(TRANS,-Tar);
	Matrix T(TRANS,Tar);

	// orbiting
	// up is a vect, make it a point
	Vect Up_pt = Pos + Up;

	Quat q_vert_axis(ROT_AXIS_ANGLE, Right, vert_angle );
	Quat q_horz_axis(ROT_AXIS_ANGLE, Up, horz_angle );

	// Keenan comments. yes.
	// Head screw:  First translate to origin, ROT around axis, translate back
	//    Look mom - mixed quaternions and matrices
	Matrix M = T_neg * q_vert_axis * q_horz_axis * T;

	// Move my key points around this new transforms
	//     Pos and Up_pt
	Vect Pos_new = Pos * M;
	Vect Up_pt_new = Up_pt * M;

	// convert up_pt to a vector again
	Vect Up_new = Up_pt_new - Pos_new;
	
	// Upate the camera
	myCamera->setOrientAndPosition( Up_new, Tar, Pos_new );


	// panning/tilting
	// get an updated up for this as well. (you never know)
	Vect cUp;
    myCamera->getUp(cUp);

	Vect cRight;
	myCamera->getRight(cRight);

    Matrix Pan(ROT_AXIS_ANGLE, cUp, pan_angle);
	Matrix Tilt(ROT_AXIS_ANGLE, cRight, tilt_angle);

    Vect cPos;
    myCamera->getPos(cPos);
    Vect cLook;
    myCamera->getLookAt( cLook );

    Matrix MM;
    Matrix MM0(TRANS, -cPos);
    Matrix MM1(TRANS, cPos);

    MM = MM0 * Pan * Tilt * MM1;

    cLook *= MM;

    // Upate the camera
    myCamera->setOrientAndPosition(cUp, cLook, cPos );

	glutPostRedisplay();
}
