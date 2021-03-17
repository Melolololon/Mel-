#include "MouseCursor.h"
#include"DirectInput.h"


MouseCursor::MouseCursor()
{
	

}

MouseCursor::~MouseCursor(){}

void MouseCursor::initialize()
{
	nearPos = 0;
	farPos = 0;
	position2D = 0;
}

void MouseCursor::update()
{
	position2D = DirectInput::getMousePosition();
	DirectInput::getMouse3DLine(nearPos, farPos);

}
