/*
	Name: Shaun Knapp
	Class IST 460
	Date: 4/24/2012

	Function: Space Invaders with boss fight
*/
 
// MyGame.cpp 
#include "MyDirectX.h" 
#include <cmath>
//#include "MyDirectX.cpp"
using namespace std; 
const string APPTITLE = "SPACE INVADERS"; 
const int SCREENW = 1024; 
const int SCREENH = 768; 
long counter = 0; 
SPRITE background, spaceShip, invaderShip[60], spaceshipBullet[3], invaderBullet[5],
	invaderExplosion, Lives, Kills, Boss, Info, GameStart, GameEndLose, GameEndWin,
	bb, bbDiff, invaderBoss, bossBullet; 

string messageLives;
char messageBoss[255];
char messageKills[255];
string messageGameInfo;
string messageGameStart;
string messageGameEndLose;
string messageGameEndWin;
string messageDifficulty;

int delay = 200; 
int currentMessage = 0; 
int currentMessagePosition = 0; 
int currentTicks; 
int shipLife = 4;
int bossLife = 25;
int invaderKilled = 0;
int offset = 16;
int explosionTicCount = 0;
double increaseSpeed = 0.3;
bool bossFight = false;
char s[255];

int spaceshipBulletCounter = 0;

int invaderAttacker = 0;
int invaderAttackerCounter = 2;

// boolean game mode
bool gameMenu = true;
bool gameStart = false;
bool gameEndLose = false;
bool gameEndWin = false;

bool easyGame = true;
bool normalGame = false;

//dsound‐>Create(&wave, “explosion.wav”);

// declare images
LPDIRECT3DTEXTURE9 imgSheet = NULL;
LPDIRECT3DTEXTURE9 imginvaderShip = NULL;
LPDIRECT3DTEXTURE9 imginvaderBoss = NULL;
LPDIRECT3DTEXTURE9 imginvaderExplosion = NULL;
LPDIRECT3DTEXTURE9 imgspaceShip = NULL; 
LPDIRECT3DTEXTURE9 imgBB = NULL; 
LPDIRECT3DTEXTURE9 imgbossBullet = NULL;
LPDIRECT3DTEXTURE9 imgBackground = NULL;

//declare some font objects 
LPD3DXFONT fontTimesNewRoman40 = NULL; 
int frame = 0;
int collisions = 0; 
int backgroundChange = 0;

//the wave sound
CSound *sound_bounce = NULL;
CSound *sound_electric = NULL;

bool Game_Init(HWND window) 
{
	//initialize Direct3D 
	Direct3D_Init(window, SCREENW, SCREENH, false); 
	//initialize DirectInput 
	DirectInput_Init(window); 
	//initialize DirectSound
	if (!DirectSound_Init(window))
	{
		MessageBox(window,"Error initializing DirectSound",
		APPTITLE.c_str(),0);
		return false;
	}
	//create some fonts 
	fontTimesNewRoman40 = MakeFont("Times New Roman", 20); 

	//load the sprite textures 
	imgBackground = LoadTexture("background.bmp");
	if (!imgBackground) return false;
	imgspaceShip = LoadTexture("spaceShip.bmp"); 
	if (!imgspaceShip) return false;
	imgSheet = LoadTexture("spriteSheet.bmp"); 
	if (!imgSheet) return false; 
	imginvaderShip = LoadTexture("invaderShip.bmp");
	if (!imginvaderShip) return false;
	imginvaderBoss = LoadTexture("invaderBoss.bmp");
	if (!imginvaderBoss) return false;
	imginvaderExplosion = LoadTexture("invaderExplosion.bmp");
	if (!imginvaderExplosion) return false;
	imgbossBullet = LoadTexture("bossBullet.bmp");
	if (!imgbossBullet) return false;
	imgBB = LoadTexture("bb.png"); 
	
	// bounding box for invaders
	bb.height = 66;
	bb.width = 440;
	bb.x = 0;
	bb.y = 0;
	bb.velx = 0.3f;
	bb.vely = 8.0f;

	// display game difficulty
	bbDiff.height = 1;
	bbDiff.width = 1;
	bbDiff.x = 400;
	bbDiff.y = 50;

	// game info
	Info.height = 1;
	Info.width = 1;
	Info.x = 200;
	Info.y = 200;

	// display lives sprite
	Lives.x = 10;
	Lives.y = 10;
	Lives.width = 36;
	Lives.height = 26;
	Lives.frame = 0;
	Lives.columns = 1;
	Lives.startframe = 1;
	Lives.endframe = 1;

	// display kills sprite
	Kills.x = 600;
	Kills.y = 10;
	Kills.width = 22;
	Kills.height = 24.5;
	Kills.frame = 0;
	Kills.columns = 1;
	Kills.startframe = 1;
	Kills.endframe = 1;

	// display boss Life
	Boss.x = 800;
	Boss.y = 2;
	Boss.width = 71;
	Boss.height = 65;
	Boss.frame = 0;
	Boss.columns = 1;
	Boss.startframe = 1;
	Boss.endframe = 1;

	// display GameStart menu
	GameStart.x = 300;
	GameStart.y = 300;
	GameStart.width = 1;
	GameStart.height = 1;
	GameStart.frame = 0;
	GameStart.columns = 1;
	GameStart.startframe = 1;
	GameStart.endframe = 1;

	// background
	background.x = 0;
	background.y = 0;
	background.width = 1024;
	background.height = 768;
	background.frame = 0;
	background.columns = 1;

	// player's ship
	spaceShip.x = 400; 
	spaceShip.y = 710; 
	spaceShip.width = 36;
	spaceShip.height = 26;
	spaceShip.frame = 0;
	spaceShip.columns = 1;
	spaceShip.startframe = 1; 
	spaceShip.endframe = 1; 
	spaceShip.velx = -0.75f; 
	spaceShip.vely = -1.50f; 
	
	// array of invaders
	for (int z = 0; z < 60; z++) 
	{
		invaderShip[z].alive = true; // <----- SET TRUE WHEN USING INVADERS ----->
		if (z < 20)
		invaderShip[z].x = 0 + z * 22;
		if (z >= 20 && z < 40)
		invaderShip[z].x = 0 + (z - 20) * 22;
		if (z >= 40 && z < 60)
		invaderShip[z].x = 0 + (z - 40) * 22;
		if (z < 20)
		invaderShip[z].y = 40;
		if (z >= 20 && z < 40)
		invaderShip[z].y = 65;
		if (z >= 40 && z < 60)
		invaderShip[z].y = 90;
		invaderShip[z].width = 22;
		invaderShip[z].height = 24.5; 
		invaderShip[z].frame = 1;
		invaderShip[z].columns = 1; 
		invaderShip[z].startframe = 1;
		invaderShip[z].endframe = 3; 
		invaderShip[z].velx = 0.3f; 
		invaderShip[z].vely = 8.0f; 
		invaderShip[z].maxVelx = 0.4f;
		invaderShip[z].maxVely = 0.4f;
		invaderShip[z].scaling = 2;
		invaderShip[z].delay = 150;
		invaderShip[z].mass = 1;
	} 

	// invader Boss
	invaderBoss.alive = false;
	invaderBoss.x = 150; 
	invaderBoss.y = -75; 
	invaderBoss.width = 71;
	invaderBoss.height = 65;
	invaderBoss.mass = 5;
	invaderBoss.frame = 0;
	invaderBoss.columns = 1;
	invaderBoss.startframe = 0; 
	invaderBoss.endframe = 1; 
	invaderBoss.velx = -0.005f; 
	invaderBoss.vely = -0.005f; 
	invaderBoss.maxVelx = 0.1f;
	invaderBoss.maxVely = 0.1f;
	invaderBoss.delay = 100;

	// player's bullet
	for (int x = 0; x < 3; x++)
	{
		spaceshipBullet[x].alive = false;
		spaceshipBullet[x].x = 1100; 
		spaceshipBullet[x].y = 800; 
		spaceshipBullet[x].width = spaceshipBullet[x].height = 22.5;
		spaceshipBullet[x].frame = 213;
		spaceshipBullet[x].columns = 20;
		spaceshipBullet[x].startframe = 20; 
		spaceshipBullet[x].endframe = 188; 
		spaceshipBullet[x].velx = 1.25f; 
		spaceshipBullet[x].vely = 2.0f; 
	}
	
	// array of invader bullets
	for (int x = 0; x < 5; x++)
	{
	invaderBullet[x].alive = false;
	invaderBullet[x].x = 0; 
	invaderBullet[x].y = 0; 
	invaderBullet[x].width = invaderBullet[x].height = 22.5;
	invaderBullet[x].frame = 213;
	invaderBullet[x].columns = 20;
	invaderBullet[x].startframe = 20; 
	invaderBullet[x].endframe = 188; 
	invaderBullet[x].velx = -0.75f; 
	invaderBullet[x].vely = 1.25f; 
	}

	bossBullet.alive = false;
	bossBullet.x = 1100;
	bossBullet.y = 800;
	bossBullet.width = 20;
	bossBullet.height = 24;
	bossBullet.frame = 0;
	bossBullet.columns = 1;
	bossBullet.startframe = 0;
	bossBullet.endframe = 1;
	bossBullet.velx = -0.75f;
	bossBullet.vely = 1.75f;
	bossBullet.delay = 75;

	// invader explosion
	invaderExplosion.alive = false;
	invaderExplosion.x = 1100; 
	invaderExplosion.y = 800; 
	invaderExplosion.width = 33;
	invaderExplosion.height = 37;
	invaderExplosion.frame = 1;
	invaderExplosion.columns = 1;
	invaderExplosion.startframe = 1; 
	invaderExplosion.endframe = 2; 
	invaderExplosion.velx = 1.25f; 
	invaderExplosion.vely = 1.75f; 
	invaderExplosion.delay = 300;

	// initiate lives in string
	messageLives = "Lives: X X X";

	// initiate menu for GameStart
	messageGameInfo = "Welcome to Space Invaders!";
	messageGameInfo += "\n\nThe evil aliens are headed toward Earth, you must stop them before they reach";
	messageGameInfo += "\n the planet!";
	messageGameInfo += "\nUse the arrow keys to move your ship and 'spacebar' to fire";
	messageGameStart = "Press S to begin the game.";

	// intiate menu for GameEndLose
	messageGameEndLose = "You have been defeated. Please try again. Would you like to play again? Y / N";

	// intiate menu for GameEndWin
	messageGameEndWin = "You defeated all of the invaders! Would you like to play again? Y / N";

	//load bounce wave file
	sound_bounce = LoadSound("explode.wav");
	if (!sound_bounce)
	{
		MessageBox(window,"Error loading step.wav",APPTITLE.c_str(),0);
		return false;
	}
	

} 

// ARRIVAL FUNCTION BUILT FOR FINAL BOSS
//
//
void Arrival()
{
	double targVecOffX =0, targVecOffY = 0, targVecOffZ = 0;
	double normX = 0, normY = 0, normZ = 0;
	double desiredVelX = 0, desiredVelY = 0, desiredVelZ = 0;
	double steerX = 0, steerY = 0, steerZ = 0;
	double accelX = 0, accelY = 0, accelZ = 0;
	double distance = 0;
	double rampedSpeed = 0;
	double clippedSpeed = 0;

	double slowingDistance = 50;

	targVecOffX = spaceShip.x - invaderBoss.x;
	//targVecOffY = spaceShip.y - invaderBoss.y;

	distance = sqrt(pow(targVecOffX, 2) + pow(targVecOffY, 2));

	rampedSpeed = invaderBoss.maxVelx * (distance/slowingDistance);

	clippedSpeed = min(rampedSpeed, invaderBoss.maxVelx);

	desiredVelX = (clippedSpeed/distance) * targVecOffX;
	//desiredVelY = (clippedSpeed/distance) * targVecOffY;

	steerX = desiredVelX - invaderBoss.velx;
	//steerY = desiredVelY - invaderBoss.vely;

	accelX = steerX/invaderBoss.mass;
	//accelY = steerY/invaderBoss.mass;

	invaderBoss.velx += accelX;
	//invaderBoss.vely += accelY;

	invaderBoss.x += invaderBoss.velx;
	//invaderBoss.y += invaderBoss.vely;
}

void Game_Run(HWND window) 
{ 
	if (!d3ddev) return; 
	DirectInput_Update();
	// color the background
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);

	// display a difficulty setting
	if (easyGame == true)
	{
		messageDifficulty = "(currently the game is set to EASY, press 'W' to chance to NORMAL)";
	}
	if (normalGame == true)
	{
		messageDifficulty = "(currently the game is set to NORMAL, press 'E' to change to EASY)";
	}

	// start game keypress
	if (Key_Down(DIK_S))
	{
		gameStart = true;
		gameMenu = false;
	}
	if (gameEndLose == true | gameEndWin == true)
	{
		if (Key_Down(DIK_Y))
		{
			Game_Init;
			gameStart = true;
			gameMenu = false;
		}
		if (Key_Down(DIK_N))
		{
			gameover = true;
			gameMenu = false;
		}
	}
	// end game keypress
	if (Key_Down(DIK_ESCAPE))
	{
		gameover = true;
	}

	// change game difficulty
	if (gameMenu == true)
	{
		if (Key_Down(DIK_W))
		{
			normalGame = true;
			easyGame = false;
		}
		if (Key_Down(DIK_E))
		{
			easyGame = true;
			normalGame = false;
		}
	}

	if (gameStart == true) // START GAME
	{
	// allow control of player's ship
	if (Key_Down(DIK_LEFT) && spaceShip.x > 0) spaceShip.x -= 1.5f;
	if (Key_Down(DIK_RIGHT) && spaceShip.x < SCREENW-spaceShip.width) spaceShip.x += 1.5f;
	if (Key_Down(DIK_SPACE))
		{
			if (easyGame == true)
			{
				for (int x = 0; x < 2; x++)
				{
					if (spaceshipBullet[x].alive != true)
					{		
						spaceshipBullet[x].x = spaceShip.x + 5;
						spaceshipBullet[x].y = spaceShip.y;
						spaceshipBullet[x].alive = true;			
					}
				}
			}
			if (normalGame == true)
			{
				if (spaceshipBullet[0].alive != true)
				{		
					spaceshipBullet[0].x = spaceShip.x + 5;
					spaceshipBullet[0].y = spaceShip.y;
					spaceshipBullet[0].alive = true;			
				}
			}
			/*
			if (spaceshipBullet[0].y > 500)
			{
				spaceshipBullet[1].x = spaceShip.x + 5;
				spaceshipBullet[1].y = spaceShip.y;
				spaceshipBullet[1].alive = true;
			}
			*/
				
		}
	
	// detect controller and allow user to control player's ship
	if (XInput_Controller_Found())
    {
		if (controllers[0].sThumbRX)
		{
			for (int x = 0; x < 5; x++)
			{
				if (spaceshipBullet[x].alive != true)
				{
					spaceshipBullet[x].x = spaceShip.x + 5;
					spaceshipBullet[x].y = spaceShip.y;
					spaceshipBullet[x].alive = true;
				}
			}
		}

        //left analog thumb stick
        if (controllers[0].sThumbLX < -5000) 
            spaceShip.x -= 1.5f;
        else if (controllers[0].sThumbLX > 5000) 
            spaceShip.x += 1.5f;

        //left and right triggers
        if (controllers[0].bLeftTrigger > 128) 
            spaceShip.x -= 1.5f;
        else if (controllers[0].bRightTrigger > 128) 
            spaceShip.x += 1.5f;

		//left and right D-PAD
        if (controllers[0].wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) 
            spaceShip.x -= 1.5f;
        else if (controllers[0].wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
            spaceShip.x += 1.5f;

        //left and right shoulders
        if (controllers[0].wButtons & XINPUT_GAMEPAD_DPAD_LEFT) 
            spaceShip.x -= 1.5f;
        else if (controllers[0].wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
            spaceShip.x += 1.5f;
    }
	
	// KEY STATEMENT FOR INVADERS FIRING ON PLAYER
	//
	// generate a random number, detect alive invaders in rows, fire bullets
	for (int x = 0; x < 60; x++)
	{
		for (int c = 40; c < 60; c++)
		{
			if (invaderShip[c].alive == true)
			{
				for (int y = 0; y < 5; y++)
				{
					int z = rand()%20;
					if (invaderBullet[y].alive == false)
					{
						
					invaderBullet[y].x = invaderShip[z+40].x + 11;
					invaderBullet[y].y = invaderShip[z+40].y + 24.5;	
					invaderBullet[y].alive = true;
					}
				}
			}
		}
		for (int a = 0; a < 20; a++)
		{
			if (invaderShip[a].alive == true)
			{
				for (int y = 0; y < 5; y++)
				{
					int z = rand()%20;
					if (invaderBullet[y].alive == false)
					{
						invaderBullet[y].x = invaderShip[z].x + 11;
						invaderBullet[y].y = invaderShip[z].y + 24.5;
						invaderBullet[y].alive = true;
					}
				}
			}
		}
		for (int b = 20; b < 40; b++)
		{
			if (invaderShip[b].alive == true)
			{
				for (int y = 0; y < 5; y++)
				{
					int z = rand()%20;
					if (invaderBullet[y].alive == false)
					{
						invaderBullet[y].x = invaderShip[z+20].x + 11;
						invaderBullet[y].y = invaderShip[z+20].y + 24.5;
						invaderBullet[y].alive = true;
					}
				}
			}
					
		}
	}

	// fire boss bullet
	if (invaderBoss.alive == true)
	{
		if (invaderBoss.y == 75)
		{
			if (bossBullet.alive == false)
			{
				bossBullet.x = invaderBoss.x + 30;
				bossBullet.y = invaderBoss.y + 65;
				bossBullet.alive = true;
			}
		}
	}

	// check to see if player's bullet is on screen or not
	for (int x = 0; x < 3; x++)
	{
		if (spaceshipBullet[x].y < 0)
		{
			spaceshipBullet[x].alive = false;
			spaceshipBullet[x].x = 1100;
			spaceshipBullet[x].y = 800;
		}
	}

	// check to see if invader's bullets are on screen or not
	for (int x = 0; x < 5; x++)
	{
		if (invaderBullet[x].y > 1024)
		{
			invaderBullet[x].x = 1100;
			invaderBullet[x].y = 800;
			invaderBullet[x].alive = false;
		}
	}
	
	// check to see if boss bullet is on screen or not
	if (bossBullet.y > 1024)
	{
		bossBullet.alive = false;
		bossBullet.x = 1100;
		bossBullet.y = 800;
	}

	// if invader bullets are alive keep them moving
	for (int x = 0; x < 5; x++)
	{
		if (invaderBullet[x].alive == true)
		{
			invaderBullet[x].y += invaderBullet[x].vely;
		}
	}

	// if boss bullet is alive, keep it moving
	if (bossBullet.alive == true)
	{
		bossBullet.y += bossBullet.vely;
	}

	// detect if player is out of lives
	if (shipLife == 0)
	{
		gameEndLose = true;
		gameStart = false;
	}
	
	// increase invader speed after 20 kills
	if (invaderKilled == 20)
	{
		if (bb.velx < 0)
		bb.velx = -0.6f;
		if (bb.velx > 0)
		bb.velx = 0.6f;
		for (int x = 0; x < 60; x++)
		{
			invaderShip[x].velx = bb.velx;
		}
	}

	// increase invader speed more after 40 kills
	if (invaderKilled == 40)
	{
		if (bb.velx < 0)
		bb.velx = -0.8f;
		if (bb.velx > 0)
		bb.velx = 0.8f;
		for (int x = 0; x < 60; x++)
		{
			invaderShip[x].velx = bb.velx;
		}
	}
	

	//
	// BEGIN BOSS PHASE
	if (invaderKilled == 60)
	{
		bossFight = true;
	}

	if (bossLife == 20)
	{
		invaderAttackerCounter = 4;
	}

	if (bossLife == 15)
	{
		invaderAttackerCounter = 7;
	}

	if (bossLife == 10)
	{
		invaderAttackerCounter = 10;
	}

	if (bossLife == 5)
	{
		invaderAttackerCounter = 20;
	}
	// cause the boss to move to the player if alive
	if (bossFight == true)
	{
		invaderBoss.alive = true;
		if (invaderBoss.y != 75)
		invaderBoss.y+=.25;

		if (spaceShip.x != invaderBoss.x+20)
		{
			Arrival();
		}

	}

	// end game if boss is defeated
	if (bossLife == 0)
	{
		gameEndWin = true;
		gameStart = false;
	}

	//move and animate the invaders and their bb
	if (bossFight == false)
	{
		for (int y = 0; y < 60; y++)
			invaderShip[y].x += invaderShip[y].velx; 
		bb.x += bb.velx;
	}

	// move player's bullet
	for (int x = 0; x < 3; x++)
	{
	if (spaceshipBullet[x].alive == true)
		spaceshipBullet[x].y -= spaceshipBullet[x].vely;
	}

	// keep all sprites on the screen
	if (spaceShip.x < 0 || spaceShip.x > SCREENW-spaceShip.width) 
		spaceShip.velx *= -1;  
	if (spaceShip.y < 0 || spaceShip.y > SCREENH-spaceShip.height) 
		spaceShip.vely *= -1;

	// detect for collisions with invaders and player bullet
	for (int x = 0; x < 60; x++)
	{
		for (int s = 0; s < 3; s++)
		{
			if (invaderShip[x].alive == true)
			{
				if (Collision(invaderShip[x], spaceshipBullet[s])) 
				{ 
					PlaySound(sound_bounce);

					invaderShip[x].alive = false;
					invaderExplosion.alive = true;

					invaderExplosion.x = invaderShip[x].x;
					invaderExplosion.y = invaderShip[x].y;

					invaderShip[x].width = 0;
					invaderShip[x].height = 0;
					invaderShip[x].x = 1100;
					invaderShip[x].y = 800;
					invaderShip[x].vely = 0.0f;

					spaceshipBullet[s].alive = false;
					spaceshipBullet[s].x = 1100;
					spaceshipBullet[s].y = 800;

					invaderKilled++;

					sprintf(messageKills, "%d", invaderKilled);

					//wave->Play();

				}
		
			}
		}
	}
	// make the boss arrive to player's ship
	if (spaceShip.x != invaderBoss.x+20)
		{
			Arrival();
		}
	
	// move the bb down if it hits the sides of the screen, bringing invaders with it
	if (bb.x < 0 || bb.x > SCREENW-bb.width) 
	{
		bb.velx *= -1;
		bb.y += 200;
		for (int a = 0; a < 60; a++)
		{
			invaderShip[a].velx *= -1;  
			invaderShip[a].y += 20;
		}
	}
	// detect bossBullet collision with spaceShip
	if (Collision(spaceShip, bossBullet))
	{
		bossBullet.alive = false;
		shipLife--;
		if (shipLife == 3)
			messageLives = "Lives: X X";
		if (shipLife == 2)
			messageLives = "Lives: X";
		if (shipLife == 1)
			messageLives = "Lives: ";
	}

	// detect collisions with player's ship and invader bullets
	for (int x = 0; x < 5; x++)
	{
		if (Collision(spaceShip, invaderBullet[x])) 
		{ 
			invaderBullet[x].alive = false;
			shipLife--;
			if (shipLife == 3)
				messageLives = "Lives: X X";
			if (shipLife == 2)
				messageLives = "Lives: X";
			if (shipLife == 1)
				messageLives = "Lives: ";
		}
	}

	// collision state for spaceship bullet and invader BOSS
	for (int x = 0; x < 3; x++)
	{
		if (Collision(invaderBoss, spaceshipBullet[x])) 
		{ 
			spaceshipBullet[x].alive = false;
			spaceshipBullet[x].x = 1100;
			spaceshipBullet[x].y = 800;
			bossLife--;
			sprintf(messageBoss, "BOSS HP %d", bossLife);
		}
	}
	// move explosion off screen after 100 tics
	if (explosionTicCount == 100)
	{
		invaderExplosion.x = 1100;
		invaderExplosion.y = 800;
		explosionTicCount = 0;
	}
	explosionTicCount++;

	// end game if invaders reach player's ship
	for (int x = 0; x < 60; x++)
	{
		if (Collision(spaceShip, invaderShip[x])) 
		{ 
			gameEndLose = true;
			gameStart = false;
			/* OLD LOSE
			MessageBox(0,"The invaders got to your ship. You lose.","GAME OVER",0);
			gameover = true;
			*/
		}
	}
	// KEY IF STATEMENT
	//
	//
	// SEND INVADERS TO ATTACK THE PLAYER DURING BOSS FIGHT
	if (bossFight == true)
	{
		for (int x = 0; x < invaderAttackerCounter; x++)
		{
			if (invaderAttacker < invaderAttackerCounter)
			{
				// revive the two invaders and prepare them for attacking the player
				if (invaderShip[x].alive == false)
				{
					invaderShip[x].velx = 0.02f;
					invaderShip[x].vely = 0.02f;
					invaderShip[x].maxVelx = 0.12f;
					invaderShip[x].maxVely = 0.12f;
					invaderShip[x].x = rand()%1024;
					invaderShip[x].y = 0;
					invaderShip[x].width = 22;
					invaderShip[x].height = 24.5;
					invaderShip[x].alive = true;
					invaderAttacker++;
				
						
				}
				// send the invader ships at the player
				if (invaderShip[x].alive == true)
				{
					/*
					Seek(invaderShip[x].x, invaderShip[x].y, spaceShip.x, spaceShip.y, invaderShip[x].velx,
						invaderShip[x].vely, invaderShip[x].maxVelx, invaderShip[x].maxVely, invaderShip[x].mass);
						*/
					double targetVectorX =0, targetVectorY = 0, targetVectorZ = 0;
					double normX = 0, normY = 0, normZ = 0;
					double desiredVelX = 0, desiredVelY = 0, desiredVelZ = 0;
					double steerX = 0, steerY = 0, steerZ = 0;
					double accelX = 0, accelY = 0, accelZ = 0;

					// calculate the target vector
					targetVectorX = spaceShip.x - invaderShip[x].x;
					targetVectorY = spaceShip.y - invaderShip[x].y;

					//math.Normal(targetVectorX, targetVectorY, targetVectorZ);
	
					// normalize the target vector
					normX = targetVectorX / sqrt(pow(targetVectorX, 2) + pow(targetVectorY, 2));
					normY = targetVectorY / sqrt(pow(targetVectorX, 2) + pow(targetVectorY, 2));

					// find the desired velocity to get to the target
					desiredVelX = normX * invaderShip[x].maxVelx;
					desiredVelY = normY * invaderShip[x].maxVely;

					// find the steering force required to get to the target
					steerX = desiredVelX - invaderShip[x].velx;
					steerY = desiredVelY - invaderShip[x].vely;

					// apply steering force with the mass of the object
					accelX = steerX/invaderShip[x].mass;
					accelY = steerY/invaderShip[x].mass;

					// add the acceleration to the player's velocity
					invaderShip[x].velx += accelX;
					invaderShip[x].vely += accelY;

					// change the player's positioning
					invaderShip[x].x += invaderShip[x].velx;
					invaderShip[x].y += invaderShip[x].vely;
				}
				if (invaderAttacker == invaderAttackerCounter)
					invaderAttacker = 0;
			}
		}
	}


	} // END START GAME

	

	// draw sprites
	if (d3ddev->BeginScene()) 
	{ 
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		Sprite_Transform_Draw(imgBackground, background.x, background.y, background.width, background.height, 
			background.frame, background.columns);

		Sprite_Transform_Draw(imgspaceShip, Lives.x, Lives.y, Lives.width, Lives.height, Lives.frame,
			Lives.columns);
		FontPrint(fontTimesNewRoman40, 40, 0, messageLives, D3DCOLOR_XRGB(255,0,0));

		Sprite_Transform_Draw(imginvaderShip, Kills.x, Kills.y, Kills.width, Kills.height, Kills.frame,
			Kills.columns);
			
		FontPrint(fontTimesNewRoman40, 40, 0, messageKills, D3DCOLOR_XRGB(255,0,0));

		Sprite_Transform_Draw(imgBB, Boss.x, Boss.y, Boss.width, Boss.height, Boss.frame,
			Boss.columns);

		FontPrint(fontTimesNewRoman40, 40, 0, messageBoss, D3DCOLOR_XRGB(255,0,0));

		if (gameMenu == true) // MENU DRAWING
		{
		Sprite_Transform_Draw(imgBB, Info.x, Info.y, Info.width, Info.height, Info.frame, 
			Info.columns);

		FontPrint(fontTimesNewRoman40, 40, 0, messageGameInfo, D3DCOLOR_XRGB(255,0,0));

		Sprite_Transform_Draw(imgBB, GameStart.x, GameStart.y, GameStart.width, GameStart.height, GameStart.frame, 
			GameStart.columns);

		FontPrint(fontTimesNewRoman40, 40, 0, messageGameStart, D3DCOLOR_XRGB(255,0,0));

		Sprite_Transform_Draw(imgBB, bbDiff.x, bbDiff.y, bbDiff.width, bbDiff.height, bbDiff.frame,
			bbDiff.columns);

		FontPrint(fontTimesNewRoman40, 40, 0, messageDifficulty, D3DCOLOR_XRGB(255,0,0));
		}
		
		if (gameStart == true) // GAME DRAWING
		{
		Sprite_Transform_Draw(imgBB, bb.x, bb.y, bb.width, bb.height, bb.frame, bb.columns); 
		//Sprite_Transform_Draw(imgShip, ship.x, ship.y, ship.width, ship.height, ship.frame, ship.columns); 
		// invader animation
		for (int x = 0; x < 60; x++)
		Sprite_Animate( invaderShip[x].frame, invaderShip[x].startframe, invaderShip[x].endframe, 1,
		invaderShip[x].starttime, invaderShip[x].delay );
		// boss animation
		Sprite_Animate( invaderBoss.frame, invaderBoss.startframe, invaderBoss.endframe, 1,
		invaderBoss.starttime, invaderBoss.delay);
		// explosion animation
		Sprite_Animate( invaderExplosion.frame, invaderExplosion.startframe, invaderExplosion.endframe, 1,
		invaderExplosion.starttime, invaderExplosion.delay);
		// boss bullet animation
		Sprite_Animate(bossBullet.frame, bossBullet.startframe, bossBullet.endframe, 1,
			bossBullet.starttime, bossBullet.delay);

		// draw the sprites
		// invader ships
		for (int x = 0; x < 60; x++) 
		Sprite_Transform_Draw(imginvaderShip, invaderShip[x].x, invaderShip[x].y, invaderShip[x].width, 
			invaderShip[x].height, invaderShip[x].frame, invaderShip[x].columns);
		// player ship
		Sprite_Transform_Draw(imgspaceShip, spaceShip.x, spaceShip.y, spaceShip.width, spaceShip.height, spaceShip.frame,
			spaceShip.columns);
		// boss ship
		Sprite_Transform_Draw(imginvaderBoss, invaderBoss.x, invaderBoss.y, invaderBoss.width, invaderBoss.height, invaderBoss.frame,
			invaderBoss.columns);
		// invader bullets
		for (int x = 0; x < 5; x++)
		Sprite_Transform_Draw(imgSheet, invaderBullet[x].x, invaderBullet[x].y, invaderBullet[x].width, invaderBullet[x].height, 
			invaderBullet[x].frame, invaderBullet[x].columns);
		// player bullets
		for (int x = 0; x < 3; x++)
		{
		Sprite_Transform_Draw(imgSheet, spaceshipBullet[x].x, spaceshipBullet[x].y, spaceshipBullet[x].width, spaceshipBullet[x].height, 
			spaceshipBullet[x].frame, spaceshipBullet[x].columns);
		}
		// invader explosion
		Sprite_Transform_Draw(imginvaderExplosion, invaderExplosion.x, invaderExplosion.y, invaderExplosion.width, invaderExplosion.height, 
			invaderExplosion.frame, invaderExplosion.columns);
		//boss bullets
		Sprite_Transform_Draw(imgbossBullet, bossBullet.x, bossBullet.y, bossBullet.width, bossBullet.height,
			bossBullet.frame, bossBullet.columns);
		} // END GAME DRAWING
		
		if (gameEndLose == true) // GAME END LOSE
		{
			Sprite_Transform_Draw(imgBB, GameStart.x, GameStart.y, GameStart.width, GameStart.height, GameStart.frame, 
			GameStart.columns);

			FontPrint(fontTimesNewRoman40, 40, 0, messageGameEndLose, D3DCOLOR_XRGB(255,0,0));
		}

		if (gameEndWin == true) // GAME END WIN
		{
			Sprite_Transform_Draw(imgBB, GameStart.x, GameStart.y, GameStart.width, GameStart.height, GameStart.frame, 
			GameStart.columns);

			FontPrint(fontTimesNewRoman40, 40, 0, messageGameEndWin, D3DCOLOR_XRGB(255,0,0));
		}
		spriteobj->End();
		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL); 
	} 

	if (KEY_DOWN(VK_ESCAPE)) 
		gameover = true;
	if (controllers[0].wButtons & XINPUT_GAMEPAD_BACK) 
		gameover = true; 
} 
void Game_End() 
{ 
	// release memory
	if (imgBackground) imgBackground->Release();
	if (imgSheet) imgSheet->Release();
	if (imgspaceShip) imgspaceShip->Release();
	if (imginvaderShip) imginvaderShip->Release();
	if (imginvaderBoss) imginvaderBoss->Release();
	if (imginvaderExplosion) imginvaderExplosion->Release();
	if (imgbossBullet) imgbossBullet->Release();
	if (sound_bounce) delete sound_bounce;
	DirectInput_Shutdown(); 
	Direct3D_Shutdown(); 
}