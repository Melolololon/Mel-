#include "MouseCursor.h"
#include"DirectInput.h"


float fNum = 40;
MouseCursor::MouseCursor()
{
	nearPos = 0;
	farPos = 0;
	position2D = 0;

}

MouseCursor::~MouseCursor(){}


void MouseCursor::update()
{
	position2D = DirectInput::getMousePosition();
	DirectInput::getMouse3DLine(nearPos, farPos);

}
