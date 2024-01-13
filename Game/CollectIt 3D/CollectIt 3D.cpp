// Snake3D.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include<math.h>
#include <stdlib.h>  
#include <ctime> 
#include <sstream>

using namespace tle;
bool flag1 = true;
bool flag2 = true;
bool flag3 = true;
void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );

	/**** Set up your scene here ****/

	// loading fonts
	IFont* titleFont = myEngine->LoadFont("stencil std", 80);
	IFont* gameOverFont = myEngine->LoadFont("Times New Roman", 50);
	IFont* scoreFont = myEngine->LoadFont("Comic Sans MS", 36);
	IFont* instructionsFont = myEngine->LoadFont("stencil std", 15);
	IFont* timeFont = myEngine->LoadFont("stencil std", 36);

	// loading and creating floor model
	IMesh* floorMesh;
	IModel* floor;
	floorMesh = myEngine->LoadMesh("floor.x");
	floor = floorMesh->CreateModel();
	floor->Scale(-1);
	floor->SetSkin("Grass1.jpg");

	// loading and creating yellow orb
	IMesh* sphereMesh;
	IModel* orbs[1000];
	sphereMesh = myEngine->LoadMesh("Sphere.x");
	orbs[0] = sphereMesh->CreateModel(0,30,-500);
	orbs[0]->Scale(5);
	orbs[0]->SetSkin("YellowBall.jpg");

	// loading and creating redOrb
	IModel* redOrb;
	redOrb = sphereMesh->CreateModel(-100, 20, -400);
	redOrb->SetSkin("RedBall.jpg");
	redOrb->Scale(2.5);

	// loading and blueOrb
	IModel* blueOrb;
	blueOrb = sphereMesh->CreateModel(-300, 20, -400);
	blueOrb->SetSkin("CueTip.jpg");
	blueOrb->Scale(2.5);

	//loading white spheres
	IModel* whiteSphere1;
	whiteSphere1 = sphereMesh->CreateModel(0, 30, -700);
	whiteSphere1->SetSkin("WhiteBall.jpg");
	whiteSphere1->Scale(5);

	IModel* whiteSphere2;
	whiteSphere2 = sphereMesh->CreateModel(0, 30, 700);
	whiteSphere2->SetSkin("WhiteBall.jpg");
	whiteSphere2->Scale(5);

	IModel* whiteSphere3;
	whiteSphere3 = sphereMesh->CreateModel(0, 30, 100);
	whiteSphere3->SetSkin("WhiteBall.jpg");
	whiteSphere3->Scale(5);


	//loading and creating camera
	ICamera* myCamera;
	myCamera = myEngine->CreateCamera(kManual);
	myCamera->SetPosition(0, 1000, -1800);
	myCamera->LookAt(floor);

	//initializing variables
	int i = 0;
	int score = 0;
	float collX, collY, collZ, collDist;
	float blueOrbCollX, blueOrbCollY, blueOrbCollZ, blueOrbCollDist;
	float obst1CollX, obst1CollY, obst1CollZ, obst1Dist;
	float obst2CollX, obst2CollY, obst2CollZ, obst2Dist;
	float obst3CollX, obst3CollY, obst3CollZ, obst3Dist;
	int yellowOrbRadius = 50;
	float sphereSpeed = 1.5;
	float moveSpeed = 0.5;

	//initialize random seed
	srand(time(NULL));

	//store scores
	
	//test
	double testx=0, testz=0;

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();
		float time = clock() / 1000;
		//print time
		stringstream outTime;
		outTime << "Time (s): " << time;
		timeFont->Draw(outTime.str(), 1100, 600, kRed, kCentre);
		outTime.str("");

		//print title
		titleFont->Draw("  CollectIt! 3D", 600, 40, kMagenta, kCentre);

		//print score
		stringstream outScore;
		outScore << "Score: " << score;
		scoreFont->Draw(outScore.str(), 600, 0, kRed, kCentre);
		outScore.str("");

		//print instructions
		instructionsFont->Draw("Collect the RED orbs using the YELLOW orb to score.", 10, 0, kBlack, kLeft);
		instructionsFont->Draw("Toucing the BLUE orb with the YELLOW orb will",10, 30, kBlack, kLeft);
		instructionsFont->Draw("reduce your score.", 10, 60, kBlack, kLeft);
		instructionsFont->Draw("Be careful! No hitting the WHITE orb and no crossing", 10, 90, kBlack, kLeft);
		instructionsFont->Draw("the boundries with the YELLOW orb or else- GAME OVER!", 10, 120, kBlack, kLeft);

		instructionsFont->Draw("Controls-", 1250, 0, kBlack, kRight);
		instructionsFont->Draw("W,A,S,D for navigation", 1250, 30, kBlack, kRight);
		instructionsFont->Draw("Alt to pause",1250, 60, kBlack, kRight);
		instructionsFont->Draw("Esc to quit game", 1250, 90, kBlack, kRight);

		//Generate random x and z position for redOrb
		int randomX, randomZ;
		randomX = rand() % 1801 + (-900);
		randomZ = rand() % 1801 + (-900);

		//Generate random x and z position for blueOrb
		int blueOrbX, blueOrbZ;
		blueOrbX = rand() % 1801 + (-900);
		blueOrbZ = rand() % 1801 + (-900);

		//check if snake is out of bounds
		float xPosition = orbs[0]->GetX();
		float zPosition = orbs[0]->GetZ();

		if (xPosition > 1000||xPosition<-1000)
		{
			gameOverFont->Draw("Game over!", 600, 300, kRed, kCentre);
			gameOverFont->Draw("Press Esc to quit", 600, 400, kRed, kCentre);
			moveSpeed = 0;
			sphereSpeed = 0;
		}
		if (zPosition > 1000 || zPosition<-1000)
		{
			gameOverFont->Draw("Game over!", 600, 300, kRed, kCentre);
			gameOverFont->Draw("Press Esc to quit", 600, 400, kRed, kCentre);
			moveSpeed = 0;
			sphereSpeed = 0;
		}


		//movements
		if (myEngine->KeyHeld(Key_D))
		{
			orbs[0]->MoveX(moveSpeed);
			
		}

		if (myEngine->KeyHeld(Key_A))
		{
			orbs[0]->MoveX(-moveSpeed);
			
		}
		
		if (myEngine->KeyHeld(Key_W))
		{
			orbs[0]->MoveZ(moveSpeed);
		}

		if (myEngine->KeyHeld(Key_S))
		{
			orbs[0]->MoveZ(-moveSpeed);
		}

		//collision detection calculation
		collX = redOrb->GetX() - orbs[0]->GetX();
		collY = redOrb->GetY() - orbs[0]->GetY();
		collZ = redOrb->GetZ() - orbs[0]->GetZ();
		collDist = sqrt(collX*collX + collY*collY + collZ*collZ);

		//collision detection
		if (collDist < yellowOrbRadius)
		{
			i++;
			orbs[i] = sphereMesh->CreateModel(0, 0, 10);
			orbs[i]->AttachToParent(orbs[i - 1]);
			orbs[i]->SetSkin("RedBall.jpg");
			score += 10;
			
			redOrb->SetPosition(randomX, 20, randomZ);
			blueOrb->SetPosition(blueOrbX, 20, blueOrbZ);
		}

		//blueOrb collision detection calculation
		blueOrbCollX = blueOrb->GetX() - orbs[0]->GetX();
		blueOrbCollY = blueOrb->GetY() - orbs[0]->GetY();
		blueOrbCollZ = blueOrb->GetZ() - orbs[0]->GetZ();
		blueOrbCollDist = sqrt(blueOrbCollX*blueOrbCollX + blueOrbCollY*blueOrbCollY + blueOrbCollZ*blueOrbCollZ);

		//blueOrb collision detection
		if (blueOrbCollDist < yellowOrbRadius)
		{
			score = score- 10;
			redOrb->SetPosition(randomX, 20, randomZ);
			blueOrb->SetPosition(blueOrbX, 20, blueOrbZ);
		}

		//whitesphere1 movement
		float xCoordinate1;
		xCoordinate1 = whiteSphere1->GetX();
		
		if (flag1 == true)
		{
			whiteSphere1->RotateX(0.5);
			whiteSphere1->RotateY(0.5);
			whiteSphere1->MoveX(sphereSpeed);
			if (xCoordinate1 >= 1000)
			{
				flag1 = false;
			}
			
		}
		if (flag1 == false)
		{
			whiteSphere1->RotateX(0.5);
			whiteSphere1->RotateY(0.5);
			whiteSphere1->MoveX(-sphereSpeed);
			if (xCoordinate1 <= -1000)
			{
				flag1 = true;
			}
		}
		
		//whitesphere2 movement
		float xCoordinate2;
		xCoordinate2 = whiteSphere2->GetX();

		if (flag2 == true)
		{
			whiteSphere2->RotateX(0.5);
			whiteSphere2->RotateY(0.5);
			whiteSphere2->MoveX(sphereSpeed);
			if (xCoordinate2 >= 1000)
			{
				flag2 = false;
			}

		}
		if (flag2 == false)
		{
			whiteSphere2->RotateX(0.5);
			whiteSphere2->RotateY(0.5);
			whiteSphere2->MoveX(-sphereSpeed);
			if (xCoordinate2 <= -1000)
			{
				flag2 = true;
			}
		}

		//whitesphere3 movement
		float xCoordinate3;
		xCoordinate3 = whiteSphere3->GetX();

		if (flag3 == true)
		{
			whiteSphere3->RotateX(0.5);
			whiteSphere3->RotateY(0.5);
			whiteSphere3->MoveX(-sphereSpeed);
			if (xCoordinate3 <= -1000)
			{
				flag3 = false;
			}

		}
		if (flag3 == false)
		{
			whiteSphere3->RotateX(0.5);
			whiteSphere3->RotateY(0.5);
			whiteSphere3->MoveX(sphereSpeed);
			if (xCoordinate3 >= 1000)
			{
				flag3 = true;
			}
		}

		// obstical collisions

		//obstical1 detection calculation
		for (int j = 0; j <= i; j++)
		{
			obst1CollX = whiteSphere1->GetX() - orbs[j]->GetX();
			obst1CollY = whiteSphere1->GetY() - orbs[j]->GetY();
			obst1CollZ = whiteSphere1->GetZ() - orbs[j]->GetZ();
		}
		
		obst1Dist = sqrt(obst1CollX*obst1CollX + obst1CollY*obst1CollY + obst1CollZ*obst1CollZ);

		//obstical1  detection
		if (obst1Dist < yellowOrbRadius)
		{
			gameOverFont->Draw("Game over!", 600, 300, kRed, kCentre);
			gameOverFont->Draw("Press Esc to quit", 600, 400, kRed, kCentre);
			sphereSpeed = 0;
			moveSpeed = 0;
		}

		//obstical2 detection calculation
		for (int j = 0; j<=i; j++)
		{
			obst2CollX = whiteSphere2->GetX() - orbs[j]->GetX();
			obst2CollY = whiteSphere2->GetY() - orbs[j]->GetY();
			obst2CollZ = whiteSphere2->GetZ() - orbs[j]->GetZ();
		}
		
		obst2Dist = sqrt(obst2CollX*obst2CollX + obst2CollY*obst2CollY + obst2CollZ*obst2CollZ);

		//obstical2  detection
		if (obst2Dist < yellowOrbRadius)
		{
			gameOverFont->Draw("Game over!", 600, 300, kRed, kCentre);
			gameOverFont->Draw("Press Esc to quit", 600, 400, kRed, kCentre);
			sphereSpeed = 0;
			moveSpeed = 0;
		}

		//obstical3 detection calculation
		for (int j = 0; j <= i; j++)
		{
			obst3CollX = whiteSphere3->GetX() - orbs[j]->GetX();
			obst3CollY = whiteSphere3->GetY() - orbs[j]->GetY();
			obst3CollZ = whiteSphere3->GetZ() - orbs[j]->GetZ();
		}

		obst3Dist = sqrt(obst3CollX*obst3CollX + obst3CollY*obst3CollY + obst3CollZ*obst3CollZ);

		//obstical3  detection
		if (obst3Dist < yellowOrbRadius)
		{
			gameOverFont->Draw("Game over!", 600, 300, kRed, kCentre);
			gameOverFont->Draw("Press Esc to quit", 600, 400, kRed, kCentre);
			sphereSpeed = 0;
			moveSpeed = 0;
		}
		
		//Quit game
		if (myEngine->KeyHeld(Key_Escape))
		{
			myEngine->Stop();
		}

		/**** Update your scene each frame here ****/

	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
	
	
}
