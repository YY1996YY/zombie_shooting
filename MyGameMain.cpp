#include "MyGameMain.h"

//ゲーム情報
DG::Image::SP img0,imgZ1,imgZ2,imgBlood,imgScope,imgWhite,imgBackground;
XI::Mouse::SP mouse;
DG::Font::SP fontA;
int score;
int ammo;
int posX, posY;

enum class State { Normal,Hit,Non };

struct Target {
	State state;
	int x;
	int y;
	ML::Box2D hitBase;
	int timeCnt;
	int timeMax;
	int score;
	int limit;
	int zType;
	int bloodAnimattion;
};

Target targets[5];


float distance(int x1, int y1, int x2, int y2)
{
	float s;
	s = sqrt(float(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
	return s;
}

//-----------------------------------------------------------------------------
//初期化処理
//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
//-----------------------------------------------------------------------------
void  MyGameMain_Initialize()
{
	img0 = DG::Image::Create("./data/image/Target.bmp");
	imgZ1 = DG::Image::Create("./data/image/smartphone_zombie_man.png");
	imgZ2 = DG::Image::Create("./data/image/smartphone_zombie_woman.png");
	imgBlood = DG::Image::Create("./data/image/bomb4.png");
	imgScope = DG::Image::Create("./data/image/gun_syoujun_scope.png");
	imgBackground = DG::Image::Create("./data/image/virus_lockdown_city.jpg");
	imgWhite = DG::Image::Create("./data/image/white.png");

	mouse = XI::Mouse::Create(2, 2);
	posX = 0;
	posY = 0;

	fontA = DG::Font::Create("HGS教科書体", 10, 20);
	score = 0;
	ammo = 6;
	//-----------------------------------------

	for (int i = 0; i < 5; i++)
	{
		targets[i].state = State::Normal;
		targets[i].x = -1000;
		targets[i].y = 0;
		targets[i].hitBase = ML::Box2D(0, 0, 32, 32);
		targets[i].timeCnt = 250;
		targets[i].timeMax = 300;
		targets[i].score = 100;
		targets[i].limit = 10000 * 60;
		targets[i].zType = rand() % 2;
		targets[i].bloodAnimattion = 0;
		
	}

}
//-----------------------------------------------------------------------------
//解放処理
//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
//-----------------------------------------------------------------------------
void  MyGameMain_Finalize( )
{
	img0.reset();
	mouse.reset();
	fontA.reset();
}
//-----------------------------------------------------------------------------
//更新処理
//機能概要：ゲームの１フレームに当たる処理
//-----------------------------------------------------------------------------
void  MyGameMain_UpDate()
{
	auto ms = mouse->GetState();
	posX = ms.pos.x;
	posY = ms.pos.y;
	//-------------



	for (int i = 0; i < 5; i++) {
	/*	if (targets[i].state != State::Non) {*/
		if (true)
		{
			targets[i].limit--;
			if (targets[i].limit <= 0) {
				targets[i].state = State::Non;
			}
			else
			{
				targets[i].timeCnt++;
				if (targets[i].timeCnt >= targets[0].timeMax) {
					targets[i].timeCnt = 0;
					targets[i].state = State::Normal;
					targets[i].bloodAnimattion = 0;
					targets[i].x = rand() % ((480 + 1) - targets[i].hitBase.h);
					targets[i].y = rand() % ((270 + 1) - targets[i].hitBase.w);
				}
			}

		}
	}
	if (ms.RB.down&&ammo==0) {
		ammo = 6;
	}


	if (ms.LB.down) {
		if (ammo > 0) {
			ammo--;
			for (int i = 0; i < 5; ++i) {
				if (targets[i].state == State::Normal) {
					ML::Box2D hit = targets[i].hitBase;
					hit.x += targets[i].x;
					hit.y += targets[i].y;

					//if (hit.x <= posX && posX < hit.x + hit.w && hit.y <= posY && posY < hit.y + hit.h) {
					if (distance(posX, posY, hit.x + 30, hit.y + 19) < 18)
					{
						targets[i].state = State::Hit;
						targets[i].bloodAnimattion = 9;
						if (targets[i].timeCnt > targets[i].timeMax / 2) {
							score += targets[i].score / 2;
						}
						else
						{
							score += targets[i].score;
						}
					}
				}
			}
		}

	}


}


//-----------------------------------------------------------------------------
//描画処理
//機能概要：ゲームの１フレームに当たる表示処理 ２Ｄ
//-----------------------------------------------------------------------------
void  MyGameMain_Render2D( )
{
	ML::Box2D drawW(0, 0, 480, 270);
	ML::Box2D srcW(0, 0, 989, 926);

	imgWhite->Draw(drawW, srcW);

	ML::Box2D drawBackground(0, 0, 480,270 );
	ML::Box2D srcBackground(0, 0, 989, 926);

	imgBackground->Draw(drawBackground, srcBackground);







	for (int i = 0; i < 5; i++)
	{
		if (targets[i].state != State::Non) {
			ML::Box2D draw = targets[i].hitBase;
			ML::Box2D src(0, 0, 0, 0);
			if (targets[i].state == State::Normal) { src = ML::Box2D(0, 0, 669, 800); }
			if (targets[i].state == State::Hit) { src = ML::Box2D(64, 0, 669, 800); }

			draw.x += targets[i].x;
			draw.y += targets[i].y;
			draw.h = 70;
			draw.w = 70;
			if (targets[i].zType==0) {
				imgZ1->Draw(draw, src);
			}else
			{
				imgZ2->Draw(draw, src);
			}
			
		}
	}

	for (int i = 0; i < 5; i++)
	{
		if (targets[i].bloodAnimattion != 0) {
			//while (targets[i].bloodAnimattion)
			{

				ML::Box2D draw = targets[i].hitBase;
				ML::Box2D src(120 * targets[i].bloodAnimattion, 0, 120, 120);
				draw.x += targets[i].x-10;
				draw.y += targets[i].y-15;
				draw.h = 70;
				draw.w = 70;
				imgBlood->Draw(draw, src);
				
			}
		}
		if (targets[i].bloodAnimattion != 0)
		{
			targets[i].bloodAnimattion--;
			if (targets[i].bloodAnimattion==0)
			{
				targets[i].state = State::Non;
			}
		}

	}



	ML::Box2D textBox(0, 0, 480, 270);
	string text = "得点：" + to_string(score);
	fontA->Draw(textBox, text, ML::Color(0.8f, 0.5f, 0.0f, 0.2f));

	ML::Box2D textBoxAmmo(400, 250,100, 100);
	string textAmmo = to_string(ammo)+"｜∞";
	fontA->Draw(textBoxAmmo, textAmmo, ML::Color(0.8f, 0.5f, 0.0f, 0.2f));

	if (ammo == 0) {
		ML::Box2D textBoxRe(200, 150,100, 100);
		string textRe = "RELOAD!!";
		fontA->Draw(textBoxRe, textRe, ML::Color(0.8f, 0.5f, 0.0f, 0.2f));
	}
	

	ML::Box2D drawScope(posX-40,posY-40,80,80);
	ML::Box2D srcScope(0, 0, 680, 680);

	imgScope->Draw(drawScope, srcScope);

}
