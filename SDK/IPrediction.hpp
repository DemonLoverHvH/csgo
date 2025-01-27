#pragma once
#include "IClientEntityList.hpp"
#include "CUserCmd.hpp"
#include "math/Vector.hpp"
#include "../utilities/vfunc.hpp"
#include "../utilities/pad.hpp"

class IMoveHelper
{
public:
	VFUNC(void, setHost, 1, (Player_t* host), (this, host));
};

class CMoveData
{
public:
	bool m_firstRunOfFunctions;
	bool m_gameCodeMovedPlayer;
	int m_playerHandle;
	int m_impulseCommand;
	Vector m_viewAngles;
	Vector m_absViewAngles;
	int m_buttons; 
	int m_oldButtons;
	float m_forwardMove;
	float m_sideMove;
	float m_upMove;
	float m_maxSpeed;
	float m_clientMaxSpeed;
	Vector m_velocity;
	Vector m_angles;
	Vector m_oldAngles;
	float m_outStepHeight;
	Vector m_outWishVel;
	Vector m_outJumpVel;
	Vector m_constraintCenter;
	float m_constraintRadius;
	float m_constraintWidth;
	float m_constraintSpeedFactor;
	PAD(20);
	Vector m_absOrigin;
};

class IPrediction
{
public:
	VFUNC(void, setLocalViewangles, 13, (Vector& angle), (this, std::ref(angle)));
	VFUNC(bool, inPrediction, 14, (), (this));
	VFUNC(void, setupMove, 20, (Player_t* localPlayer, CUserCmd* cmd, IMoveHelper* moveHelper, CMoveData* moveData), (this, localPlayer, cmd, moveHelper, moveData))
	VFUNC(void, finishMove, 21, (Player_t* localPlayer, CUserCmd* cmd, CMoveData* moveData), (this, localPlayer, cmd, moveData))
};
