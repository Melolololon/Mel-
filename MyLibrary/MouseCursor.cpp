#include "MouseCursor.h"
#include"DirectInput.h"


MouseCursor::MouseCursor()
{
	

}

MouseCursor::~MouseCursor(){}

void MouseCursor::Initialize()
{
	nearPos = 0;
	farPos = 0;
	position2D = 0;
}

void MouseCursor::Update()
{
	position2D = DirectInput::GetMousePosition();
	DirectInput::GetMouse3DLine(nearPos, farPos);

}
