#include "Clone.h"


Clone::Clone(Ped original)
{
	ped = PED::CLONE_PED(original, 0.0f, FALSE, TRUE);

	//Move Clone to original
	ENTITY_X::SET_ENTITY_COORDS(ped, ENTITY_X::GET_ENTITY_COORDS(original) + Vector3f(0.0f, 0.0f, -0.45f));
	ENTITY_X::SET_ENTITY_QUATERNION(ped, ENTITY_X::GET_ENTITY_QUATERNION(original));
	ENTITY::SET_ENTITY_HEADING(ped, ENTITY::GET_ENTITY_HEADING(original));

	//Blip
	blip = UI::ADD_BLIP_FOR_ENTITY(ped);
	UI::SET_BLIP_SPRITE(blip, 197); //Yoga

	//p_rc_handset 2553089994
	//SKEL_R_Hand 0xdead
	objController = OBJECT::CREATE_OBJECT(2553089994, 0.0f, 0.0f, 0.0f, FALSE, TRUE, FALSE);
	ENTITY::ATTACH_ENTITY_TO_ENTITY(objController, ped, PED::GET_PED_BONE_INDEX(ped, 0xdead), 0.16f, 0.07f, -0.12f, 180.0f, -105.0f, 5.0f, FALSE, FALSE, FALSE, TRUE, 0, TRUE);

	cam1 = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", true);
	CAM::SET_CAM_NEAR_CLIP(cam1, 0.6f);
	//Head 0x796e //INDEV
	CAM_X::ATTACH_CAM_TO_ENTITY(cam1, ped, Vector3f(0.0f, 0.0f, 0.8f), true);

	playAnimations();
	refreshSettingsDynamic();
}

Clone::~Clone()
{
	//Delete Blip
	UI::REMOVE_BLIP(&blip);

	//Delete Controller
	ENTITY::DELETE_ENTITY(&objController);

	//Delete Ped
	ENTITY::SET_ENTITY_COLLISION(ped, FALSE, FALSE);
	ENTITY::DELETE_ENTITY(&ped);

	

	//Delete cam
	CAM::DESTROY_CAM(cam1, true);
}

Vector3f Clone::getPosition()
{
	return ENTITY_X::GET_ENTITY_COORDS(ped);
}

Quaternionf Clone::getQuaternion()
{
	return ENTITY_X::GET_ENTITY_QUATERNION(ped);
}

void Clone::setPedFreeze(bool freeze)
{
	ENTITY::FREEZE_ENTITY_POSITION(ped, freeze?TRUE:FALSE);
}

void Clone::refreshSettingsDynamic()
{

}

void Clone::refreshCamMode()
{
	switch (Settings::camMode.get())
	{
	case camModeC1: CAM::SET_CAM_ACTIVE(cam1, true); CAM::RENDER_SCRIPT_CAMS(1, 0, 3000, false, false); break;
	default: break;
	}
}

void Clone::update(Vector3f lookAt)
{
	if (CAM::IS_CAM_RENDERING(cam1))
	{
		CAM_X::POINT_CAM_AT_COORD(cam1, lookAt);

		Vector3f position = Convert(CAM::GET_CAM_COORD(cam1));

		CAM::SET_CAM_FOV(cam1, calculateFOV((lookAt - position).norm()));

		Vector3 rot = CAM::GET_CAM_ROT(cam1, 0);
		Quaternionf rotation = ENTITY_X::GET_ENTITY_QUATERNION(ped);

		//See updateCamera
		AudioHandler::setListener(position, Vector3f(0.0f, 0.0f, 0.0f), rotation); //INDEV camera turns
	}
}

void Clone::playAnimations()
{
	//{"jimmy_playingvideogame_loop_d", "missarmenian3"},
	//{"ld_2g_sit_idle", "mini@strip_club@lap_dance_2g@ld_2g_reach"},

	//Request animation dicationary
	STREAMING::REQUEST_ANIM_DICT("missarmenian3");
	//STREAMING::REQUEST_ANIM_DICT("mini@strip_club@lap_dance_2g@ld_2g_reach");

	DWORD maxWait = GetTickCount() + 2000;
	while (!STREAMING::HAS_ANIM_DICT_LOADED("missarmenian3"))
	{
		WAIT(0);
		if (GetTickCount() > maxWait)
		{
			//showSubtitle("Didn't load weapon@w_sp_jerrycan. Time out.", 1000);
			return;
		}
	}

	//Intro
	//AI::TASK_PLAY_ANIM(ped, "mini@strip_club@lap_dance_2g@ld_2g_reach", "ld_2g_sit_idle", 8.0, 0.0, -1, 1, 0.0f, false, false, false);
	AI::TASK_PLAY_ANIM(ped, "missarmenian3", "jimmy_playingvideogame_loop_d", 8.0, 0.0, -1, 17, 0.0f, false, false, false);

	/*
	float speed > normal speed is 8.0f
	----------------------

	float speedMultiplier > multiply the playback speed
	----------------------

	int duration: time in millisecond
	----------------------
	-1 _ _ _ _ _ _ _> Default (see flag)
	0 _ _ _ _ _ _ _ > Not play at all
	Small value _ _ > Slow down animation speed
	Other _ _ _ _ _ > freeze player control until specific time (ms) has
	_ _ _ _ _ _ _ _ _ passed. (No effect if flag is set to be
	_ _ _ _ _ _ _ _ _ controllable.)

	int flag:
	----------------------
	enum eAnimationFlags
	{
	ANIM_FLAG_NORMAL = 0,
	ANIM_FLAG_REPEAT = 1,
	ANIM_FLAG_STOP_LAST_FRAME = 2,
	ANIM_FLAG_UPPERBODY = 16,
	ANIM_FLAG_ENABLE_PLAYER_CONTROL = 32,
	ANIM_FLAG_CANCELABLE = 120,
	};
	Odd number : loop infinitely
	Even number : Freeze at last frame
	Multiple of 4: Freeze at last frame but controllable

	01 to 15 > Full body
	10 to 31 > Upper body
	32 to 47 > Full body > Controllable
	48 to 63 > Upper body > Controllable
	...
	001 to 255 > Normal
	256 to 511 > Garbled
	...

	playbackRate:

	values are between 0.0 and 1.0


	lockX:

	0 in most cases 1 for rcmepsilonism8 and rcmpaparazzo_3
	> 1 for mini@sprunk


	lockY:

	0 in most cases
	1 for missfam5_yoga, missfra1mcs_2_crew_react


	lockZ:

	0 for single player
	Can be 1 but only for MP 
	*/
}

float Clone::calculateFOV(float distance)
{
	static const float closeDist = 2.0f;	//Close Fov @ 2 meters distance

	/*
	Constant Object Size: c
	Close Distance: dist
	Close Field of View: alpha
	Arbitrary Distance: dist'
	Arbitrary Field of View: alpha'

	c = dist*tan(alpha/2) = dist'*tan(alpha'/2)
	->
	alpha' = 2*arctan(dist/dist'*tan(alpha/2))

	infFOV is (lower) hardlimit for FOV
	*/

	return RadToDeg( 2.0f*atan(2.0f/distance*tanf(DegToRad(float(Settings::camClone1CloseFOV.get() - Settings::camClone1InfFOV.get()))/2.0f)) ) + float(Settings::camClone1InfFOV.get());
}