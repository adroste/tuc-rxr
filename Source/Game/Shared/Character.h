#pragma once
#include <math.h>
#include <time.h>
#include "../../System/Input.h"
#include "CharNode.h"

class Character : public Input::IReceiver
{
public:
	enum State
	{
		forwardLeft1,
		forwardLeft2,
		forwardRight1,
		forwardRight2,
		backwardLeft1,
		backwardLeft2,
		backwardRight1,
		backwardRight2,
		stand,
		default
	};
	Character();
	virtual ~Character();
	virtual void update(float dt);
#ifdef _CLIENT
	virtual void draw(Drawing& draw);
#endif
	virtual void SetState(State state);
	virtual void jump();
	virtual void goForward();
	virtual void goBackward();
	virtual void run();
	virtual void cower();
private:
	void build();
	float GetTimeProgress();
	void interpolate();

	void leftArmPitch(float angle);
	void rightArmPitch(float angle);
	void leftArmRoll(float angle);
	void rightArmRoll(float angle);
	void leftArm2Pitch(float angle);
	void rightArm2Pitch(float angle);

	void leftLegPitch(float angle);
	void rightLegPitch(float angle);
	void leftLeg2Pitch(float angle);
	void rightLeg2Pitch(float angle);

	float GetPitch(CharNode& node);
	float GetRoll(CharNode& node);
	float GetYaw(CharNode& node);


	float leftArmPitchStart = 0.0f;
	float leftArmPitchEnd = 0.0f;
	float rightArmPitchStart = 0.0f;
	float rightArmPitchEnd = 0.0f;
	float leftArmRollStart = 0.0f;
	float leftArmRollEnd = 0.0f;
	float rightArmRollStart = 0.0f;
	float rightArmRollEnd = 0.0f;
	float leftArm2PitchStart = 0.0f;
	float leftArm2PitchEnd = 0.0f;
	float rightArm2PitchStart = 0.0f;
	float rightArm2PitchEnd = 0.0f;
	float leftLegPitchStart = 0.0f;
	float leftLegPitchEnd = 0.0f;
	float rightLegPitchStart = 0.0f;
	float rightLegPitchEnd = 0.0f;
	float leftLeg2PitchStart = 0.0f;
	float leftLeg2PitchEnd = 0.0f;
	float rightLeg2PitchStart = 0.0f;
	float rightLeg2PitchEnd = 0.0f;

	clock_t startTime;
	clock_t endTime;

	CharNode m_body;
	State m_state;

};