//-----------------------------------------------------------------------------
// File: Player.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//       such as player movement, some minor physics as well as rendering.
//
// Copyright (c) 1997-2005 GameInstitute.com. All rights reserved.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Player Specific Includes
//-----------------------------------------------------------------------------
#include "Player.h"
#include "Camera.h"

using namespace DirectX;

Player::Player()
{
	
}


Player::~Player()
{
	// Release any allocated memory
	if (m_pCamera) delete m_pCamera;

	// Clear required values
	m_pCamera = NULL;
}


//-----------------------------------------------------------------------------
// Name : SetCamOffset ()
// Desc : Set the offset, relative to the player, at which the camera will be 
//        positioned.
//-----------------------------------------------------------------------------
void Player::SetCamOffset(const XMFLOAT3& Offset)
{
	m_vecCamOffset = Offset;

	if (!m_pCamera) return;

	XMFLOAT3 pos;
	XMStoreFloat3(&pos, XMVectorAdd(XMLoadFloat3(&m_vecPos), XMLoadFloat3(&Offset)));

	m_pCamera->SetPosition(pos);
}

//-----------------------------------------------------------------------------
// Name : Update ()
// Desc : Update the players position based on the current velocity / gravity
//        settings. These will be scaled by the TimeScale factor passed in.
//-----------------------------------------------------------------------------
void Player::Update(float TimeScale)
{
	// Add on our gravity vector
	XMStoreFloat3(&m_vecVelocity, XMVectorMultiplyAdd(XMLoadFloat3(&m_vecGravity), XMVectorReplicate(TimeScale), XMLoadFloat3(&m_vecVelocity)));

	// Clamp the XZ velocity to our max velocity vector
	float Length = sqrtf(m_vecVelocity.x * m_vecVelocity.x + m_vecVelocity.z * m_vecVelocity.z);
	if (Length > m_fMaxVelocityXZ)
	{
		m_vecVelocity.x *= (m_fMaxVelocityXZ / Length);
		m_vecVelocity.z *= (m_fMaxVelocityXZ / Length);

	} // End if clamp XZ velocity

	  // Clamp the Y velocity to our max velocity vector
	Length = sqrtf(m_vecVelocity.y * m_vecVelocity.y);
	if (Length > m_fMaxVelocityY)
	{
		m_vecVelocity.y *= (m_fMaxVelocityY / Length);

	} // End if clamp Y velocity

	  // Move our player (will also move the camera if required)
	XMFLOAT3 moveVec;
	XMStoreFloat3(&moveVec, XMVectorMultiply(XMLoadFloat3(&m_vecVelocity), XMVectorReplicate(TimeScale)));
	Move(moveVec, false);

	  // Calculate the reverse of the velocity direction
	XMFLOAT3 vecDec = m_vecVelocity;
	
	XMStoreFloat3(&vecDec, XMVectorNegate(XMLoadFloat3(&m_vecVelocity)));
	XMStoreFloat3(&vecDec, XMVector3Normalize(XMLoadFloat3(&vecDec)));

	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, XMVector3Length(XMLoadFloat3(&m_vecVelocity)));
	// Retrieve the actual velocity length
	Length = tmp.x;

	// Calculate total deceleration based on friction values
	float Dec = (m_fFriction * TimeScale);
	if (Dec > Length) Dec = Length;

	// Apply the friction force
	XMStoreFloat3(&m_vecVelocity, XMVectorMultiplyAdd(XMLoadFloat3(&vecDec), XMVectorReplicate(Dec), XMLoadFloat3(&m_vecVelocity)));

}


//-----------------------------------------------------------------------------
// Name : SetCamera ()
// Desc : Sets the camera to view the player.
// Note : Only returns false on a fatal error of some type.
//-----------------------------------------------------------------------------
bool Player::SetCamera()
{
	// Check for a no-op
	if (m_pCamera) return true;

	if (!(m_pCamera = new Camera())) return false;

	// Attach the new camera to 'this' player object
	m_pCamera->AttachToPlayer(this);

	// Success!!
	return true;
}

void Player::SetPosition(const DirectX::XMFLOAT3& Position) {

	XMFLOAT3 pos;
	XMStoreFloat3(&pos, XMVectorAdd(XMLoadFloat3(&Position), XMVectorNegate(XMLoadFloat3(&m_vecPos))));

	Move(pos, false);
}

//-----------------------------------------------------------------------------
// Name : Move ()
// Desc : Move the camera in the specified direction for the specified distance
// Note : Specify 'true' to velocity if you wish the move to affect the cameras
//        velocity rather than it's absolute position
//-----------------------------------------------------------------------------
void Player::Move(ULONG Direction, float Distance, bool Velocity)
{
	XMVECTOR vecShift = { 0.0f, 0.0f, 0.0f, 0.0f };
	XMVECTOR d = XMVectorReplicate(Distance);

	XMVECTOR R = XMLoadFloat3(&m_vecRight);
	XMVECTOR U = XMLoadFloat3(&m_vecUp);
	XMVECTOR L = XMLoadFloat3(&m_vecLook);
	
	if (Direction & DIR_FORWARD) vecShift += L;
	if (Direction & DIR_BACKWARD) vecShift -= L;
	if (Direction & DIR_RIGHT) vecShift += R;
	if (Direction & DIR_LEFT) vecShift -= R;
	if (Direction & DIR_UP) vecShift += U;
	if (Direction & DIR_DOWN) vecShift -= U;

	vecShift = XMVector3NormalizeEst(vecShift) * Distance;

	XMFLOAT3 moveVec;
	XMStoreFloat3(&moveVec, vecShift);

	// Update camera vectors
	if (Direction) Move(moveVec, Velocity);
}

//-----------------------------------------------------------------------------
// Name : Move ()
// Desc : Move the camera by the specified amount based on the vector passed.
//-----------------------------------------------------------------------------
void Player::Move(const XMFLOAT3& vecShift, bool Velocity)
{
	//m_vecVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// Update velocity or actual position ?
	if (Velocity)
	{
		XMStoreFloat3(&m_vecVelocity, XMVectorAdd(XMLoadFloat3(&m_vecVelocity), XMLoadFloat3(&vecShift)));
		
	} // End if Velocity
	else
	{
		XMStoreFloat3(&m_vecPos, XMVectorAdd(XMLoadFloat3(&m_vecPos), XMLoadFloat3(&vecShift)));
		m_pCamera->Move(vecShift);

	} // End if !Velocity

}

//-----------------------------------------------------------------------------
// Name : Rotate ()
// Desc : Rotate the player around the players local axis.
//-----------------------------------------------------------------------------
void Player::Rotate(float x, float y, float z)
{

	// Validate requirements
	if (!m_pCamera) return;

	// Update & Clamp pitch / roll / Yaw values
	if (x)
	{
		float pbRad = XMConvertToRadians(89.0f);

		// Make sure we don't overstep our pitch boundaries
		m_fPitch += x;
		if (m_fPitch >  pbRad) { x -= (m_fPitch - pbRad); m_fPitch = pbRad; }
		if (m_fPitch < -pbRad) { x -= (m_fPitch + pbRad); m_fPitch = -pbRad; }

	} // End if any Pitch

	if (y)
	{
		// Ensure yaw (in degrees) wraps around between 0 and 2PI
		m_fYaw += y;
		if (m_fYaw >  XM_2PI) m_fYaw -= XM_2PI;
		if (m_fYaw <  0.0f) m_fYaw += XM_2PI;
		
	} // End if any yaw

	  // Roll is purely a statistical value, no player rotation actually occurs
	if (z)
	{
		float rbRad = XMConvertToRadians(20.0f);

		// Make sure we don't overstep our roll boundaries
		m_fRoll += z;
		if (m_fRoll >  rbRad) { z -= (m_fRoll - rbRad); m_fRoll = rbRad; }
		if (m_fRoll < -rbRad) { z -= (m_fRoll + rbRad); m_fRoll = -rbRad; }

	} // End if any roll

	  // Allow camera to rotate prior to updating our axis
	m_pCamera->Rotate(x, y, z);

	// Now rotate our axis
	if (y)
	{
		// Build rotation matrix
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_vecUp), y);

		// Update our vectors
		XMStoreFloat3(&m_vecLook, XMVector3TransformNormal(XMLoadFloat3(&m_vecLook), mtxRotate));
		XMStoreFloat3(&m_vecRight, XMVector3TransformNormal(XMLoadFloat3(&m_vecRight), mtxRotate));

	} // End if any yaw

	  // Because many rotations will cause floating point errors, the axis will eventually become
	  // non-perpendicular to one other causing all hell to break loose. Therefore, we must
	  // perform base vector regeneration to ensure that all vectors remain unit length and
	  // perpendicular to one another. This need not be done on EVERY call to rotate (i.e. you
	  // could do this once every 50 calls for instance).
	XMStoreFloat3(&m_vecLook, XMVector3Normalize(XMLoadFloat3(&m_vecLook)));
	XMStoreFloat3(&m_vecRight,XMVector3Cross(XMLoadFloat3(&m_vecUp), XMLoadFloat3(&m_vecLook)));
	XMStoreFloat3(&m_vecRight, XMVector3Normalize(XMLoadFloat3(&m_vecRight)));
	XMStoreFloat3(&m_vecUp, XMVector3Cross(XMLoadFloat3(&m_vecLook), XMLoadFloat3(&m_vecRight)));
	XMStoreFloat3(&m_vecUp, XMVector3Normalize(XMLoadFloat3(&m_vecUp)));
}

//-----------------------------------------------------------------------------
// Name : SetVolumeInfo ()
// Desc : Set the players collision volume information
//-----------------------------------------------------------------------------
void Player::SetVolumeInfo(const VOLUME_INFO& Volume)
{
	m_Volume = Volume;
}

//-----------------------------------------------------------------------------
// Name : GetVolumeInfo ()
// Desc : Retrieve the players collision volume information
//-----------------------------------------------------------------------------
const VOLUME_INFO& Player::GetVolumeInfo() const
{
	return m_Volume;
}


