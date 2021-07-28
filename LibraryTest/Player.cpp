#include "Player.h"
#include"LibMath.h"
#include"PhysicsTestObject.h"
#include"GameObjectManager.h"

Player::Player()
{
	position = Vector3(0, 0, -120);
	velocity = 0;
	calcPhysics = false;

	Texture* crossTex = Texture::Get("cross");
	crossSpr = std::make_unique<Sprite2D>(crossTex);
	Vector2 crossSize = crossTex->GetTextureSize();
	//crossSpr->SetPosition(Vector2(Library::GetWindowWidth() / 2 /*- crossSize.x / 2*/, Library::GetWindowHeight()/* - crossSize.y /2*/));
	crossSpr->SetPosition(Vector2(0,0));
}

void Player::Update()
{
	const float CAMERA_SPEED = 3.0f;
	if (Input::KeyState(DIK_UP))angle.x += CAMERA_SPEED;
	if (Input::KeyState(DIK_DOWN))angle.x -= CAMERA_SPEED;
	if (Input::KeyState(DIK_LEFT))angle.y += CAMERA_SPEED;
	if (Input::KeyState(DIK_RIGHT))angle.y -= CAMERA_SPEED;

	Vector3 moveDir = 0;
	if (Input::KeyState(DIK_W))moveDir.z++;
	if (Input::KeyState(DIK_S))moveDir.z--;
	if (Input::KeyState(DIK_A))moveDir.x--;
	if (Input::KeyState(DIK_D))moveDir.x++;
	moveDir = LibMath::RotateVector3(moveDir, Vector3(0, 1, 0), angle.y);
	
	const float MOVE_SPEED = 2.0f;
	velocity = moveDir * MOVE_SPEED;
	position += velocity;

	Camera::Get()->SetAngle(angle);
	Camera::Get()->SetRotateCriteriaPosition( position);

	if(Input::KeyTrigger(DIK_SPACE))
	{
		Vector3 bulletVel = Vector3::Normalize(Camera::Get()->GetTargetPosition() - position);
		float BULLET_SPEED = 3.0f;
		GameObjectManager::GetInstance()->AddObject(std::make_shared<PhysicsTestObject>(position, bulletVel * BULLET_SPEED));
	}

	
}

void Player::Draw()
{
}

void Player::DrawCross()
{
	crossSpr->Draw();
}
