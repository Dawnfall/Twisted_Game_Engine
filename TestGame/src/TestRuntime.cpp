#include "TestRuntime.h"
#include "Debug/Logger.h"

#include <vector>
#include <string>
#include <iostream>

#include "gtx/string_cast.hpp"
#include <iostream>


void TestRuntime::OnInit(Twisted::Application& app)
{
	app.Game.Systems.AddSystem<Twisted::RenderSystem>();

	app.WindowManager.CreateNewWindow(window1Title, monitorWidth, monitorHeight);
	app.WindowManager.closeWindowEvent.AddListener([&app]() { app.Stop(); });
}

void TestRuntime::OnBeforeRun(Twisted::Application& app)
{
	TestTransform(app.Game);
}

void TestRuntime::OnRun(Twisted::Application& app) {}

void TestRuntime::TestTransform(Twisted::GameCore& game)
{
	Twisted::Vec3f t1Pos = { 0.0f,0.0f,0.0f };
	Twisted::Vec3f t2Pos = { 1.0f,0.0f,0.0f };

	Twisted::Vec3f t1RotEulerDeg = { 0.0f,0.0f,90.0f };
	Twisted::Vec3f t2RotEulerDeg = { 0.0f,0.0f,0.0f };

	Twisted::Vec3f t1Scale = { 1.0f, 1.0f, 1.0f };
	Twisted::Vec3f t2Scale = { 1.0f, 1.0f, 1.0f };

	//Dawn::Quat t1RotQuat(0.9559025f, 0.1577649f, 0.1458546f, 0.2002177f);
	//Dawn::Quat t2RotQuat = { 1.0f,0.0f,0.0f,0.0f };

	entt::entity t1 = game.CreateEntity();
	Twisted::CTransform& trans1 = game.AddComponent<Twisted::CTransform>(t1);
	trans1.SetLocalPosition(t1Pos);
	trans1.SetLocalRotation(Twisted::Quat(glm::radians(t1RotEulerDeg)));
	trans1.SetLocalScale(t1Scale);

	entt::entity t2 = game.CreateEntity();
	Twisted::CTransform& trans2 = game.AddComponent<Twisted::CTransform>(t2);
	trans2.SetParent(t1, game);
	trans2.SetLocalPosition(t2Pos);
	trans2.SetLocalRotation(Twisted::Quat(glm::radians(t2RotEulerDeg)));
	trans2.SetLocalScale(t2Scale);

	Twisted::Vec3f t2WorldRotResult = glm::degrees(glm::eulerAngles(trans2.GetWorldRotation(game)));
	Twisted::Vec3f t2WorldPosResult = trans2.GetWorldPosition(game);
	int a = 2;

	//Dawn::Quat q(0.9559025f, 0.1577649f, 0.1458546f, 0.2002177f);
	//Dawn::Vec3f anglesRad = glm::eulerAngles(glm::normalize(q));
	//Dawn::Vec3f anglesDeg = glm::degrees(anglesRad);
	//Dawn::Mat4x4f mat = glm::toMat4(q);
	//Dawn::Quat q2 = Dawn::Quat(anglesRad);

	//std::cout << "q1:" <<"\n" << glm::to_string(q)<<"\n";
	//std::cout << "angles Rad:" << "\n" << glm::to_string(anglesRad) << "\n";
	//std::cout <<"angles Deg:" <<"\n" << glm::to_string(anglesDeg) << "\n";
	//std::cout << "q2:" <<"\n" << glm::to_string(q2) << "\n";
	//std::cout << "Mat4:" << "\n" << glm::to_string(mat) << "\n";

	//int a = 1;

	//Dawn::Vec3f t1Scale = { 2.0f,2.0f,1.0f };
	//Dawn::Vec3f t1EulerRad = { 0.3776081f, 0.2173794f, 0.4546341f };

	//Dawn::Mat4x4f t1LocalModelMat() = {
	//	0.8772786f, -0.3367557f,  0.3420202f,
	//	0.4287986f,  0.8700462f, -0.2432104f,
	//	-0.2156709f,  0.3600211f,  0.9076734f
	//};


	//Dawn::Vec3f t2Scale = { 1.0f,1.0f,1.0f };

	//entt::entity t1 = game.CreateEntity();
	//Dawn::CTransform& trans1 = game.AddComponent<Dawn::CTransform>(t1);

	//trans1.SetLocalPosition(t1Pos);
	//trans1.SetLocalRotation(t1RotRad);
	//trans1.SetLocalScale(t1Scale);

	//auto model = trans1.GetLocalModelMatrix();


	//entt::entity t2 = game.CreateEntity();
	//Dawn::CTransform& trans2 = game.AddComponent<Dawn::CTransform>(t2);
	//trans2.SetParent(t1, game);
	//trans2.SetWorldPosition(t2Pos, game);
	//trans2.SetLocalRotation(t2RotRad);
	//trans2.SetLocalScale(t2Scale);

	////test
	//auto t1ResEulerLocal = trans1.GetLocalRotationEulerRad();
	//if (glm::distance(resT1EulerLocal, t1EulerRad) > 0.001f)
	//{
	//	std::cout << "failed euler comparison" << std::endl;
	//}

	//auto t1ResLocalRotMatrix=trans1.getrota
	//auto t1ResLocalModelMatrix = trans1.GetLocalModelMatrix();





}
