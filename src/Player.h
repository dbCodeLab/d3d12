#pragma once

//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//       such as player movement, some minor physics as well as rendering.
//
// Copyright (c) 1997-2002 Adam Hoult & Gary Simmons. All rights reserved.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CPlayer Specific Includes
//-----------------------------------------------------------------------------
#include "d3dUtil.h"

//-----------------------------------------------------------------------------
// Forward Declarations
//-----------------------------------------------------------------------------
class Camera;


//-----------------------------------------------------------------------------
// structures and Enumerators
//-----------------------------------------------------------------------------
typedef struct _VOLUME_INFO     // Stores information about our object volume
{
	DirectX::XMFLOAT3 Min;            // Minimum object space extents of the volume
	DirectX::XMFLOAT3 Max;            // Maximum object space extents of the volume

} VOLUME_INFO;


//-----------------------------------------------------------------------------
// Main Class Definitions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : Player (Class)
// Desc : Player class handles all player manipulation, update and management.
//-----------------------------------------------------------------------------
class Player
{
public:
	//-------------------------------------------------------------------------
	// Enumerators
	//-------------------------------------------------------------------------
	enum DIRECTION {
		DIR_FORWARD = 1,
		DIR_BACKWARD = 2,
		DIR_LEFT = 4,
		DIR_RIGHT = 8,
		DIR_UP = 16,
		DIR_DOWN = 32,

		DIR_FORCE_32BIT = 0x7FFFFFFF
	};

	//-------------------------------------------------------------------------
	// Constructors & Destructors for This Class.
	//-------------------------------------------------------------------------
	Player();
	virtual ~Player();

	//-------------------------------------------------------------------------
	// Public Functions for This Class.
	//-------------------------------------------------------------------------
	bool                SetCamera();
	void                Update(float TimeScale);

	void                SetFriction(float Friction) { m_fFriction = Friction; }
	void                SetGravity(const DirectX::XMFLOAT3& Gravity) { m_vecGravity = Gravity; }
	void                SetMaxVelocityXZ(float MaxVelocity) { m_fMaxVelocityXZ = MaxVelocity; }
	void                SetMaxVelocityY(float MaxVelocity) { m_fMaxVelocityY = MaxVelocity; }
	void                SetVelocity(const DirectX::XMFLOAT3& Velocity) { m_vecVelocity = Velocity; }
	void                SetCamLag(float CamLag) { m_fCameraLag = CamLag; }
	void                SetCamOffset(const DirectX::XMFLOAT3& Offset);
	void                SetVolumeInfo(const VOLUME_INFO& Volume);
	const VOLUME_INFO&  GetVolumeInfo() const;

	Camera           * GetCamera() const { return m_pCamera; }
	const DirectX::XMFLOAT3 & GetVelocity() const { return m_vecVelocity; }
	const DirectX::XMFLOAT3 & GetCamOffset() const { return m_vecCamOffset; }

	const DirectX::XMFLOAT3 & GetPosition() const { return m_vecPos; }
	const DirectX::XMFLOAT3 & GetLook() const { return m_vecLook; }
	const DirectX::XMFLOAT3 & GetUp() const { return m_vecUp; }
	const DirectX::XMFLOAT3 & GetRight() const { return m_vecRight; }

	float               GetYaw() const { return m_fYaw; }
	float               GetPitch() const { return m_fPitch; }
	float               GetRoll() const { return m_fRoll; }

	void                SetPosition(const DirectX::XMFLOAT3& Position);

	void                Move(ULONG Direction, float Distance, bool Velocity = false);
	void                Move(const DirectX::XMFLOAT3& vecShift, bool Velocity = false);
	void                Rotate(float x, float y, float z);

private:
	//-------------------------------------------------------------------------
	// Private Variables for This Class.
	//-------------------------------------------------------------------------
	Camera       * m_pCamera = nullptr;              // Our current camera object
	VOLUME_INFO     m_Volume;               // Stores information about players collision volume
											// Players position and orientation values
	DirectX::XMFLOAT3     m_vecPos = { 0.0f, 0.0f, 0.0f };  // Player Position
	DirectX::XMFLOAT3     m_vecUp = { 0.0f, 1.0f, 0.0f };   // Player Up Vector
	DirectX::XMFLOAT3     m_vecRight = { 1.0f, 0.0f, 0.0f }; // Player Right Vector
	DirectX::XMFLOAT3     m_vecLook = { 0.0f, 0.0f, 1.0f };  // Player Look Vector
	DirectX::XMFLOAT3     m_vecCamOffset = { 0.0f, 0.0f, 0.0f };         // Camera offset
	float           m_fPitch = 0.0f;  // Player pitch
	float           m_fRoll = 0.0f;   // Player roll
	float           m_fYaw = 0.0f;   // Player yaw

											// Force / Player Update Variables
	DirectX::XMFLOAT3     m_vecVelocity = { 0.0f, 0.0f, 0.0f };          // Movement velocity vector
	DirectX::XMFLOAT3     m_vecGravity = { 0.0f, 0.0f, 0.0f };           // Gravity vector
	float           m_fMaxVelocityXZ = 125.0f;       // Maximum camera velocity on XZ plane
	float           m_fMaxVelocityY = 125.0f;        // Maximum camera velocity on Y Axis
	float           m_fFriction = 250.0f;            // The amount of friction causing the camera to slow
	float           m_fCameraLag = 0.0f;           // Amount of camera lag in seconds (0 to disable)

};

