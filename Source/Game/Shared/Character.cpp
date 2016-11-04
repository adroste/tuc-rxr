#include "Character.h"

Character::Character()
	: m_body(glm::vec3(40.0f, 10.0f, 0.0f))
{
	build();
	m_state = State::forwardLeft1;

}

Character::~Character()
{
}

void Character::update(float dt)
{
	//m_body.rotate(glm::rotate(0.002f, glm::vec3(0.0f, 1.0f, 0.0f)));

	/*if (GetPitch(*m_body.m_nodes[0]->m_nodes[0]->m_nodes[0]) > 1.5)
		m_state = State::forward;
	if (GetPitch(*m_body.m_nodes[0]->m_nodes[0]->m_nodes[0]) < 0.0)
		m_state = State::backward;
*/
	if (endTime < clock())
	{
		if (m_state == State::forwardLeft1)
			SetState(State::forwardLeft2);
		else if (m_state == State::forwardLeft2)
			SetState(State::forwardLeft1);
	}
	interpolate();

}

#ifdef _CLIENT
void Character::draw(Drawing& draw)
{


	m_body.draw(draw);
}
#endif
void Character::SetState(State state)
{
	leftArmPitchStart = GetPitch(*m_body.m_nodes[0]);
	rightArmPitchStart = GetPitch(*m_body.m_nodes[1]);
	leftArmRollStart = GetRoll(*m_body.m_nodes[0]->m_nodes[0]);
	rightArmRollStart = GetRoll(*m_body.m_nodes[1]->m_nodes[0]);
	leftArm2PitchStart = GetPitch(*m_body.m_nodes[0]->m_nodes[0]->m_nodes[0]);
	rightArm2PitchStart = GetPitch(*m_body.m_nodes[1]->m_nodes[0]->m_nodes[0]);

	leftLegPitchStart = GetPitch(*m_body.m_nodes[2]);
	rightLegPitchStart = GetPitch(*m_body.m_nodes[3]);
	leftLeg2PitchStart = GetPitch(*m_body.m_nodes[2]->m_nodes[0]);
	rightLeg2PitchStart = GetPitch(*m_body.m_nodes[3]->m_nodes[0]);

	m_state = state;
	switch (m_state)
	{
	case State::default:

	case State::stand:
		startTime = clock();
		endTime = clock() + CLOCKS_PER_SEC / 1;


		leftArmPitchEnd = 0.0f;
		rightArmPitchEnd = 0.0f;
		leftArmRollEnd = 0.0f;
		rightArmRollEnd = 0.0f;
		leftArm2PitchEnd = 0.0f;
		rightArm2PitchEnd = 0.0f;
		leftLegPitchEnd = 0.0f;
		rightLegPitchEnd = 0.0f;
		leftLeg2PitchEnd = 0.0f;
		rightLeg2PitchEnd = 0.0f;
		break;
	case State::forwardLeft1:
		startTime = clock();
		endTime = clock() + CLOCKS_PER_SEC / 0.5f;

		leftArmPitchEnd = 0.3f;
		rightArmPitchEnd = -0.1f;
		leftArmRollEnd = 0.0f;
		rightArmRollEnd = 0.0f;
		leftArm2PitchEnd = 0.5f;
		rightArm2PitchEnd = 0.1f;
		leftLegPitchEnd = -0.3f;
		rightLegPitchEnd = 0.5f;
		leftLeg2PitchEnd = 0.0f;
		rightLeg2PitchEnd = -0.1f;

		break;
	case State::forwardLeft2:
		startTime = clock();
		endTime = clock() + CLOCKS_PER_SEC / 0.5f;

		leftArmPitchEnd = -0.1f;
		rightArmPitchEnd = 0.0f;
		leftArmRollEnd = 0.0f;
		rightArmRollEnd = 0.0f;
		leftArm2PitchEnd = 0.0f;
		rightArm2PitchEnd = 0.3f;
		leftLegPitchEnd = 0.0f;
		rightLegPitchEnd = 0.1f;
		leftLeg2PitchEnd = 0.0f;
		rightLeg2PitchEnd = -0.3f;

		break;
	}
}

void Character::jump()
{

}

void Character::goForward()
{
}

void Character::goBackward()
{
}

void Character::run()
{
}

void Character::cower()
{
}

void Character::build()
{
	float armLength1 = 3.5f; // oberarm
	float armLength2 = 3.0f; // unterarm
	float legLength1 = 5.0f; // oberschnekel
	float legLength2 = 5.0f; // unterschenkel
	float bodyLength = 6.0f; // abstand schulter hüfte

	float armWidth = 1.0f;
	float legWidth = 1.5f;
	float bodyWidth = 4.0f;

	auto leftShoulder = std::unique_ptr<CharNode>(new CharNode(glm::vec3(0.0f, 0.0f, (bodyWidth + armWidth) / 2)));
	auto rightShoulder = std::unique_ptr<CharNode>(new CharNode(glm::vec3(0.0f, 0.0f, -(bodyWidth + armWidth) / 2)));
	auto leftArm1 = std::unique_ptr<CharNode>(new CharNode(glm::vec3(0.0f, 0.0f, 0.0f)));
	auto rightArm1 = std::unique_ptr<CharNode>(new CharNode(glm::vec3(0.0f, 0.0f, 0.0f)));
	auto leftArm2 = std::unique_ptr<CharNode>(new CharNode(glm::vec3(0.0f, armLength1, 0.0f)));
	auto rightArm2 = std::unique_ptr<CharNode>(new CharNode(glm::vec3(0.0f, armLength1, 0.0f)));
	auto leftLeg1 = std::unique_ptr<CharNode>(new CharNode(glm::vec3(0.0f, bodyLength, bodyWidth / 2 - legWidth / 2)));
	auto rightLeg1 = std::unique_ptr<CharNode>(new CharNode(glm::vec3(0.0f, bodyLength, - bodyWidth / 2 + legWidth / 2)));
	auto leftLeg2 = std::unique_ptr<CharNode>(new CharNode(glm::vec3(0.0f, legLength1, 0.0f)));
	auto rightLeg2 = std::unique_ptr<CharNode>(new CharNode(glm::vec3(0.0f, legLength1, 0.0f)));

	std::vector<std::unique_ptr<Cube>> body;
	std::vector<std::unique_ptr<Cube>> ls;
	std::vector<std::unique_ptr<Cube>> rs;
	std::vector<std::unique_ptr<Cube>> la1;
	std::vector<std::unique_ptr<Cube>> ra1;
	std::vector<std::unique_ptr<Cube>> la2;
	std::vector<std::unique_ptr<Cube>> ra2;
	std::vector<std::unique_ptr<Cube>> ll1;
	std::vector<std::unique_ptr<Cube>> rl1;
	std::vector<std::unique_ptr<Cube>> ll2;
	std::vector<std::unique_ptr<Cube>> rl2;

	ls.push_back(std::unique_ptr<Cube>(new Cube(CubeDesc(Color::Blue().toDWORD()), glm::vec3(0.0f, 0.0f, 0.0f), false, armWidth)));
	rs.push_back(std::unique_ptr<Cube>(new Cube(CubeDesc(Color::Blue().toDWORD()), glm::vec3(0.0f, 0.0f, 0.0f), false, armWidth)));

	for (float i = 0; i < armLength1 - armWidth / 2; i += armWidth)
		la1.push_back(std::unique_ptr<Cube>(new Cube(CubeDesc(Color::Yellow().toDWORD()), glm::vec3(0.0f, i, 0.0f),false, armWidth)));
	la1.push_back(std::unique_ptr<Cube>(new Cube(CubeDesc(Color::Yellow().toDWORD()), glm::vec3(0.0f, armLength1 - armWidth / 2, 0.0f),false, armWidth)));

	for (float i = 0; i < armLength1 - armWidth / 2; i += armWidth)
		ra1.push_back(std::unique_ptr<Cube>(new Cube(CubeDesc(Color::Yellow().toDWORD()), glm::vec3(0.0f, i, 0.0f), false,armWidth)));
	ra1.push_back(std::unique_ptr<Cube>(new Cube(CubeDesc(Color::Yellow().toDWORD()), glm::vec3(0.0f, armLength1 - armWidth / 2, 0.0f),false, armWidth)));

	for (float i = 0; i < armLength2 - armWidth / 2; i += armWidth)
		la2.push_back(std::unique_ptr<Cube>(new Cube(CubeDesc(Color::White().toDWORD()), glm::vec3(0.0f, i, 0.0f),false, armWidth)));
	la2.push_back(std::unique_ptr<Cube>(new Cube(CubeDesc(Color::White().toDWORD()), glm::vec3(0.0f, armLength2 - armWidth / 2, 0.0f),false, armWidth)));

	for (float i = 0; i < armLength2 - armWidth / 2; i += armWidth)
		ra2.push_back(std::unique_ptr<Cube>(new Cube(CubeDesc(Color::White().toDWORD()), glm::vec3(0.0f, i, 0.0f), false, armWidth)));
	ra2.push_back(std::unique_ptr<Cube>(new Cube(CubeDesc(Color::White().toDWORD()), glm::vec3(0.0f, armLength2 - armWidth / 2, 0.0f), false, armWidth)));


	for (float i = 0; i < legLength1 - legWidth / 2; i += legWidth)
		ll1.push_back(std::unique_ptr<Cube>(new Cube(CubeDesc(Color::Red().toDWORD()), glm::vec3(0.0f, i, 0.0f),false, legWidth)));
	ll1.push_back(std::unique_ptr<Cube>(new Cube(CubeDesc(Color::Red().toDWORD()), glm::vec3(0.0f, legLength1 - legWidth / 2, 0.0f),false, legWidth)));

	for (float i = 0; i < legLength1 - legWidth / 2; i += legWidth)
		rl1.push_back(std::unique_ptr<Cube>(new Cube(CubeDesc(Color::Red().toDWORD()), glm::vec3(0.0f, i, 0.0f), false, legWidth)));
	rl1.push_back(std::unique_ptr<Cube>(new Cube(CubeDesc(Color::Red().toDWORD()), glm::vec3(0.0f, legLength1 - legWidth / 2, 0.0f),false, legWidth)));

	for (float i = 0; i < legLength2 - legWidth / 2; i += legWidth)
		ll2.push_back(std::unique_ptr<Cube>(new Cube(CubeDesc(Color::Cyan().toDWORD()), glm::vec3(0.0f, i, 0.0f),false, legWidth)));
	ll2.push_back(std::unique_ptr<Cube>(new Cube(CubeDesc(Color::Cyan().toDWORD()), glm::vec3(0.0f, legLength2 - legWidth / 2, 0.0f), false, legWidth)));

	for (float i = 0; i < legLength2 - legWidth / 2; i += legWidth)
		rl2.push_back(std::unique_ptr<Cube>(new Cube(CubeDesc(Color::Cyan().toDWORD()), glm::vec3(0.0f, i, 0.0f), false, legWidth)));
	rl2.push_back(std::unique_ptr<Cube>(new Cube(CubeDesc(Color::Cyan().toDWORD()), glm::vec3(0.0f, legLength2 - legWidth / 2, 0.0f), false, legWidth)));

	body.push_back(std::unique_ptr<Cube>(new Cube(CubeDesc(Color::Green().toDWORD()), glm::vec3(0.0f, 0.0f, 0.0f), false, bodyWidth)));
	body.push_back(std::unique_ptr<Cube>(new Cube(CubeDesc(Color::Gray().toDWORD()), glm::vec3(bodyWidth/2, 0.0f, 0.0f), false, bodyWidth/3)));

	m_body.setCubes(std::move(body));
	leftShoulder->setCubes(std::move(ls));
	rightShoulder->setCubes(std::move(rs));
	leftArm1->setCubes(std::move(la1));
	leftArm2->setCubes(std::move(la2));
	rightArm1->setCubes(std::move(ra1));
	rightArm2->setCubes(std::move(ra2));
	leftLeg1->setCubes(std::move(ll1));
	leftLeg2->setCubes(std::move(ll2));
	rightLeg1->setCubes(std::move(rl1));
	rightLeg2->setCubes(std::move(rl2));

	leftArm1->addNode(std::move(leftArm2));
	rightArm1->addNode(std::move(rightArm2));
	leftLeg1->addNode(std::move(leftLeg2));
	rightLeg1->addNode(std::move(rightLeg2));
	leftShoulder->addNode(std::move(leftArm1));
	rightShoulder->addNode(std::move(rightArm1));

	m_body.addNode(std::move(leftShoulder));
	m_body.addNode(std::move(rightShoulder));
	m_body.addNode(std::move(leftLeg1));
	m_body.addNode(std::move(rightLeg1));


}

float Character::GetTimeProgress()
{
	float c = clock();
	if (c - endTime > 0)
		return 1.0f;
	if (c - startTime < 0)
		return 0.0f;

	return (c - startTime) / (endTime - startTime);
}

void Character::interpolate()
{
	float progress = GetTimeProgress();

	float leftArmPitch = leftArmPitchStart + (leftArmPitchEnd - leftArmPitchStart) * progress;
	float rightArmPitch = rightArmPitchStart + (rightArmPitchEnd - rightArmPitchStart) * progress;
	float leftArmRoll = leftArmRollStart + (leftArmRollEnd - leftArmRollStart) * progress;
	float rightArmRoll = rightArmRollStart + (rightArmRollEnd - rightArmRollStart) * progress;
	float leftArm2Pitch = leftArm2PitchStart + (leftArm2PitchEnd - leftArm2PitchStart) * progress;
	float rightArm2Pitch = rightArm2PitchStart + (rightArm2PitchEnd - rightArm2PitchStart) * progress;

	float leftLegPitch = leftLegPitchStart + (leftLegPitchEnd - leftLegPitchStart) * progress;
	float rightLegPitch = rightLegPitchStart + (rightLegPitchEnd - rightLegPitchStart) * progress;
	float leftLeg2Pitch = leftLeg2PitchStart + (leftLeg2PitchEnd - leftLeg2PitchStart) * progress;
	float rightLeg2Pitch = rightLeg2PitchStart + (rightLeg2PitchEnd - rightLeg2PitchStart) * progress;

	leftArmPitch = leftArmPitch - GetPitch(*m_body.m_nodes[0]);
	rightArmPitch = rightArmPitch - GetPitch(*m_body.m_nodes[1]);
	leftArmRoll = leftArmRoll - GetRoll(*m_body.m_nodes[0]->m_nodes[0]);
	rightArmRoll = rightArmRoll - GetRoll(*m_body.m_nodes[1]->m_nodes[0]);
	leftArm2Pitch = leftArm2Pitch - GetPitch(*m_body.m_nodes[0]->m_nodes[0]->m_nodes[0]);
	rightArm2Pitch = rightArm2Pitch - GetPitch(*m_body.m_nodes[1]->m_nodes[0]->m_nodes[0]);

	leftLegPitch = leftLegPitch - GetPitch(*m_body.m_nodes[2]);
	rightLegPitch = rightLegPitch - GetPitch(*m_body.m_nodes[3]);
	leftLeg2Pitch = leftLeg2Pitch - GetPitch(*m_body.m_nodes[2]->m_nodes[0]);
	rightLeg2Pitch = rightLeg2Pitch - GetPitch(*m_body.m_nodes[3]->m_nodes[0]);

	Character::leftArmPitch(leftArmPitch);
	Character::rightArmPitch(rightArmPitch);
	Character::leftArmRoll(leftArmRoll);
	Character::rightArmRoll(rightArmRoll);
	Character::leftArm2Pitch(leftArm2Pitch);
	Character::rightArm2Pitch(rightArm2Pitch);

	Character::leftLegPitch(leftLegPitch);
	Character::rightLegPitch(rightLegPitch);
	Character::leftLeg2Pitch(leftLeg2Pitch);
	Character::rightLeg2Pitch(rightLeg2Pitch);
}

void Character::leftArmPitch(float angle)
{
	m_body.m_nodes[0]->rotate(0.0f, angle);
}

void Character::rightArmPitch(float angle)
{
	m_body.m_nodes[1]->rotate(0.0f, angle);
}

void Character::leftArmRoll(float angle)
{
	m_body.m_nodes[0]->rotate(angle, 0.0f);
}

void Character::rightArmRoll(float angle)
{
	m_body.m_nodes[1]->rotate(angle, 0.0f);
}

void Character::leftArm2Pitch(float angle)
{
	m_body.m_nodes[0]->m_nodes[0]->m_nodes[0]->rotate(0.0f, angle);
}

void Character::rightArm2Pitch(float angle)
{
	m_body.m_nodes[1]->m_nodes[0]->m_nodes[0]->rotate(0.0f, angle);
}

void Character::leftLegPitch(float angle)
{
	m_body.m_nodes[2]->rotate(0.0f, angle);
}

void Character::rightLegPitch(float angle)
{
	m_body.m_nodes[3]->rotate(0.0f, angle);
}

void Character::leftLeg2Pitch(float angle)
{
	m_body.m_nodes[3]->m_nodes[0]->rotate(0.0f, angle);
}

void Character::rightLeg2Pitch(float angle)
{
	m_body.m_nodes[2]->m_nodes[0]->rotate(0.0f, angle);
}

// angle functions kinda not correct yet
// should work witch correct functions

float Character::GetPitch(CharNode & node)
{
	//return atan2(-node.m_rot[3][0], sqrt(node.m_rot[2][1]*node.m_rot[2][1]+ node.m_rot[2][2]*node.m_rot[2][2]));
	return asin(node.m_rot[1][0]);
}

float Character::GetRoll(CharNode & node)
{
	return atan2(-node.m_rot[1][2], node.m_rot[1][1]);
}

float Character::GetYaw(CharNode & node)
{
	return atan2(-node.m_rot[2][1], node.m_rot[0][0]);
}




