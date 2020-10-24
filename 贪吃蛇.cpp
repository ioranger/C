#include<stdio.h>
#include<stdlib.h> 
#include<time.h>
#include<windows.h> 
#include<Mmsystem.h>
#include <graphics.h>
#define Length 1200//地图的长
#define Width 800//地图的宽 
#define sizePaint 20//像素块的大小 
#define N 100//障碍物的最大数量 
#pragma comment(lib,"winmm.lib")
typedef struct Snake//存放蛇的位置坐标 
{
	int x;
	int y;
	struct Snake* next;
}snake;
struct Obstacle//存放障碍物的位置坐标 
{
	int x[N];
	int y[N];
}obstacle;
struct Food //存放食物的位置坐标 
{
	int x;
	int y;
}tomato, hamburger, mushroom;
struct rec//记录游戏得分，游戏时间,玩家的ID 
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	int highscore;
	char name[10];
}record[10];
struct cm
{	/*定义结构体类型*/
	int nian;
	int yue;
	int ri;
	int num;
}chenmi;
mouse_msg msg = { 0 };//定义鼠标结构体

snake* head;
int speed;//休眠时间 
int speed1;//相对速度 
int level;//难度等级 
int getTomatoNum;//吃到的番茄个数 
int getHamburgerNum;//吃到的汉堡个数 
int getMushroomNum;//吃到的蘑菇个数 
int direction;//表示蛇运动的方向 
int obstacleNum;//障碍物的个数 
int snakelen;//蛇的长度 
int score;//分数 
char name[10];
PIMAGE menu, paiming, help, lange, fanqie, zhadan, Mushroom, hanbao, snakeHead, snakeBody, white, over, reconds, difficulty, fangchenmi;

void getPaint();//加载所有要用到的图片  
void showMenu();//显示主菜单 
void playmusic();//背景音乐
void showOver();//显示游戏结束界面 
int fcm();//防沉迷 
int mouse(int, int, int, int);//判断鼠标的点击 
void data();//初始化各参数 
void createMap();//生成地图 
void createFood(int);//生成食物 
void createSnake();//创造开局时的蛇 
void createObstacle();//生成障碍物 
int snakeControl();//控制蛇的移动 
void snakeMove();//显示蛇的移动 
int snakeDie();//判断蛇是否死亡 
void snakeStop();//使游戏暂停 
void putScore();//输出目前的分数，速度 
void getHighScore(struct rec[]);//新建文件或将分数存入文件中 
void storeHighScore(struct rec[]);//将分数从文件中取出，存到结构体中 
void Ranking(struct rec[]);//排序画面 ，将分数和排名显示在画面上 
void recond();//记录所得分数 

int main()
{
	int flag = 0, c = 0;
	playmusic();
	initgraph(Length, Width);//初始化界面 
	getPaint();//加载所要用到的图片 
	do
	{
		if (c == 0)
			showMenu();//显示主界面 
		else if (c == 1)
		{
			showOver();//显示游戏结束界面 ，按任意键后返回主菜单 
			getch();
			showMenu();
		}
		for (; is_run(); delay_fps(60))
		{
			while (mousemsg())//获取鼠标消息，这个函数会等待，等待到有消息为止
			{
				msg = getmouse();//将鼠标信息存入鼠标结构体
			}
			if (c == 1 && mouse(0, 0, Length, Width) == 1)
			{
				c = 0; flag = 0;	break;
			}
			if (mouse(70, 275, 410, 346) == 1)//鼠标左键单击开始游戏 
			{
				flag = 1;	break;
			}
			if (mouse(80, 386, 406, 452) == 1)//鼠标左键单击排行榜 
			{
				flag = 2;	break;
			}
			if (mouse(80, 489, 408, 557) == 1)//鼠标左键单机帮助 
			{
				flag = 3;	break;
			}
			if (mouse(80, 598, 408, 666) == 1)//鼠标左键单机退出 
			{
				flag = 4;	break;
			}
		}
		if (flag == 1)//鼠标单击了开始游戏后，开始选择游戏难度 
		{
			if (fcm() == 1)
			{
				putimage(0, 0, 1200, 800, fangchenmi, 0, 0, 1200, 800);
				getch();
				closegraph();
				return 0;
			}
			putimage(0, 0, 1200, 800, difficulty, 0, 0, 1200, 800);
			for (; is_run(); delay_fps(60))
			{
				while (mousemsg())//获取鼠标消息，这个函数会等待，等待到有消息为止
				{
					msg = getmouse();//将鼠标信息存入鼠标结构体
				}
				if (mouse(461, 180, 869, 277) == 1)//简单模式 
				{
					level = 1;	break;
				}
				if (mouse(455, 349, 866, 447) == 1)//一般模式 
				{
					level = 2;	break;
				}
				if (mouse(454, 528, 869, 626) == 1)//困难模式 
				{
					level = 3;	break;
				}
			}
		}

		switch (flag)
		{
		case 1://开始游戏 
			data();
			createMap();
			createSnake();
			createFood(3);
			c = snakeControl();
			inputbox_getline("游戏结束！","请输入你的昵称", name, 10);
			recond();
			system("cls");//清屏 
			break;

		case 2://显示排行榜 
			putimage(0, 0, 1200, 800, paiming, 0, 0, 1200, 800);
			score = 0;
			recond();
			Ranking(record);
			getch();
			break;

		case 3://显示帮助界面 
			putimage(0, 0, 1200, 800, help, 0, 0, 1200, 800);
			int i;
			for (i = 1; i <= 60; i++)
			{
				putimage(20 * (i - 1), 0, 20, 20, lange, 0, 0, 20, 20);
				putimage(20 * (i - 1), 780, 20, 20, lange, 0, 0, 20, 20);
			}
			for (i = 1; i <= 38; i++)
			{
				putimage(0, 20 * i, 20, 20, lange, 0, 0, 20, 20);
				putimage(1180, 20 * i, 20, 20, lange, 0, 0, 20, 20);
			}
			getch();
			break;

		case 4://退出程序 
			closegraph();
			return 0;
		}
	} while (1);
	getch();
	closegraph();

	return 0;
}



int mouse(int x1, int y1, int x2, int y2)   //判断在(x1,y1)(x2,y2)矩形范围内单机左键 
{
	if (msg.x > x1 && msg.x<x2 && msg.y>y1 && msg.y < y2 && (int)msg.is_down() == 1 && (int)msg.is_left() == 1)
		return 1;
	return 0;
}
void showMenu()//显示主菜单界面 
{
	putimage(0, 0, 1200, 800, menu, 0, 0, 1200, 800);
}
void showOver()//显示游戏结束界面 
{
	putimage(0, 0, 1200, 800, over, 0, 0, 1200, 800);
}
void playmusic()//背景音乐
{
	PlaySound(TEXT("音乐.wav"), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);//异步循环播放音乐
}
void createMap()//创建开局时的游戏地图 
{
	setfillcolor(EGERGB(240, 255, 240));
	bar(20, 20, 780, 780);
	int i;
	for (i = 1; i <= 40; i++)//j围成游戏界面的边框
	{
		putimage(20 * (i - 1), 0, 20, 20, lange, 0, 0, 20, 20);
		putimage(20 * (i - 1), 780, 20, 20, lange, 0, 0, 20, 20);
	}
	for (i = 1; i <= 38; i++)
	{
		putimage(0, 20 * i, 20, 20, lange, 0, 0, 20, 20);
		putimage(780, 20 * i, 20, 20, lange, 0, 0, 20, 20);
	}
	putimage(800, 0, 400, 800, reconds, 0, 0, 400, 800);//显示游戏记录界面 
	putimage(830, 320, 20, 20, fanqie, 0, 0, 20, 20);
	putimage(830, 353, 20, 20, hanbao, 0, 0, 20, 20);
	putimage(830, 385, 20, 20, Mushroom, 0, 0, 20, 20);
	putimage(904, 416, 20, 20, zhadan, 0, 0, 20, 20);
}

void createFood(int n)//随机生成食物 
{
	srand((unsigned)time(NULL));//生成随机数种子 
	int i, flag;//flag是判断随机出的食物是否符合条件 
	snake* body;
	if (n == 0)//吃掉了一种食物，先将另外两种食物用背景色覆盖掉，之后再随机生成这三种食物 ，起到刷新效果 
	{
		putimage(hamburger.x, hamburger.y, 20, 20, white, 0, 0, 20, 20);
		putimage(mushroom.x, mushroom.y, 20, 20, white, 0, 0, 20, 20);
	}
	else if (n == 1)
	{
		putimage(tomato.x, tomato.y, 20, 20, white, 0, 0, 20, 20);
		putimage(mushroom.x, mushroom.y, 20, 20, white, 0, 0, 20, 20);
	}
	else if (n == 2)
	{
		putimage(tomato.x, tomato.y, 20, 20, white, 0, 0, 20, 20);
		putimage(hamburger.x, hamburger.y, 20, 20, white, 0, 0, 20, 20);
	}

	do
	{
		flag = 1;
		body = head;//获取蛇链表的头节点信息 
		tomato.x = (rand() % 38 + 1) * sizePaint;//随机生成番茄，汉堡，蘑菇的坐标 
		tomato.y = (rand() % 38 + 1) * sizePaint;
		hamburger.x = (rand() % 38 + 1) * sizePaint;
		hamburger.y = (rand() % 38 + 1) * sizePaint;
		mushroom.x = (rand() % 38 + 1) * sizePaint;
		mushroom.y = (rand() % 38 + 1) * sizePaint;
		if ((tomato.x == hamburger.x && tomato.y == hamburger.y) || (tomato.x == mushroom.x && tomato.y == mushroom.y) || (hamburger.x == mushroom.x && hamburger.y == mushroom.y))//防止食物生成重合 
		{
			flag = 0;	break;
		}
		while (body != NULL && flag != 0)//防止食物生成在蛇身体里面 
		{
			if ((tomato.x == body->x && tomato.y == body->y) || (hamburger.x == body->x && hamburger.y == body->y) || (mushroom.x == body->x && mushroom.y == body->y))//判断食物坐标是否等于蛇身体坐标 
			{
				flag = 0;	break;
			}
			body = body->next;
		}
		for (i = 0; i <= obstacleNum; i++)//防止食物生成在随机生成的障碍物里 
		{
			if ((tomato.x == obstacle.x[i] && tomato.y == obstacle.y[i]) || (hamburger.x == obstacle.x[i] && hamburger.y == obstacle.y[i]) || (mushroom.x == obstacle.x[i] && mushroom.y == obstacle.y[i]) || flag == 0)//判断食物坐标是否等于障碍物坐标 
			{
				flag = 0;	break;
			}
		}
		if (flag == 1)//如果随机生成的食物符合条件则生成食物 
		{
			putimage(tomato.x, tomato.y, 20, 20, fanqie, 0, 0, 20, 20);
			putimage(hamburger.x, hamburger.y, 20, 20, hanbao, 0, 0, 20, 20);
			putimage(mushroom.x, mushroom.y, 20, 20, Mushroom, 0, 0, 20, 20);
		}
	} while (!flag);
}

void createObstacle()//生成障碍物 
{
	snake* body;
	int i, flag, x, y;
	srand((unsigned)time(NULL));//生成随机数种子 
	do
	{
		body = head;//获取蛇链表的头节点信息
		flag = 1;
		x = (rand() % 38 + 1) * sizePaint;//随机生成障碍物的坐标 (1到38)
		y = (rand() % 38 + 1) * sizePaint;
		for (i = 0; i < obstacleNum; i++) //判断新生成的障碍物坐标是否等于旧障碍物坐标
		{
			if (obstacle.x[i] == x && obstacle.y[i] == y)
			{
				flag = 0;	break;
			}
		}
		while (body != NULL)//防止食物生成在蛇身体里面 
		{
			if (x == body->x && y == body->y)//判断新生成的障碍物坐标是否等于蛇身体坐标 
			{
				flag = 0;	break;
			}
			body = body->next;
		}
		if ((x == tomato.x && y == tomato.y) || (x == hamburger.x && y == hamburger.y) || (x == mushroom.x && y == mushroom.y))//防止障碍物和食物重合 
			flag = 0;
		if ((x == (head->x) || x == (head->x - 1 * sizePaint) || x == (head->x + 1 * sizePaint) || x == (head->x + 2 * sizePaint)) && (y == (head->y - 1 * sizePaint) || y == (head->y) || y == (head->y + 1 * sizePaint) || y == (head->y + 2 * sizePaint)) && flag == 1)//判断新的障碍物是否在蛇头旁边  防止蛇突然死亡 
			flag = 0;
	} while (!flag);

	obstacle.x[obstacleNum] = x;//将新生成的障碍物的横坐标存入结构体数组中 
	obstacle.y[obstacleNum] = y;//将新生成的障碍物的纵坐标存入结构体数组中 
	obstacleNum++;//障碍物的数量加1 
	for (i = 0; i < obstacleNum; i++)//打印障碍物 
	{
		putimage(obstacle.x[i], obstacle.y[i], 20, 20, zhadan, 0, 0, 20, 20);
	}

}

void createSnake()//创建开局时的蛇 
{
	snake* p;
	int i;
	p = (snake*)malloc(sizeof(snake));//给p分配空间 
	p->x = 2 * sizePaint;//初始化蛇尾位置的横坐标 
	p->y = 10 * sizePaint;//初始化蛇尾位置的纵坐标 
	p->next = NULL;
	for (i = 0; i < snakelen; i++)//存入蛇的位置   倒插法 
	{
		head = (snake*)malloc(sizeof(snake));//给head申请空间 
		head->x = (2 + i) * sizePaint;
		head->y = 10 * sizePaint;
		head->next = p;
		p = head;
	}
	putimage(p->x, p->y, 20, 20, snakeHead, 0, 0, 20, 20);//打印蛇头 
	p = p->next;
	while (p != NULL)//打印蛇身 
	{
		putimage(p->x, p->y, 20, 20, snakeBody, 0, 0, 20, 20);
		p = p->next;
	}
}

int snakeControl()//控制蛇的移动 
{
	int a = 0;
	while (1)//无限循环使其能一直运动 
	{
		if (GetAsyncKeyState('W') && direction != 3)//当键盘输入上且蛇不朝下移动 
			direction = 2;
		else if (GetAsyncKeyState('S') && direction != 2)//当键盘输入下且蛇不朝上移动 
			direction = 3;
		else if (GetAsyncKeyState('D') && direction != 1)//当键盘输入右且蛇不朝左移动 
			direction = 0;
		else if (GetAsyncKeyState('A') && direction != 0)//当键盘输入左且蛇不朝右移动 
			direction = 1;
		else if (GetAsyncKeyState(VK_SPACE))//当键盘输入空格时 暂停 
			snakeStop();
		Sleep(speed);//通过控制休眠时间来控制循环速度  从而控制蛇的速度 
		snakeMove();//显示蛇的移动效果 
		putScore();//更新右侧的游戏记录 
		a = snakeDie();//判断蛇是否死亡 
		if (a == 1)
			return 1;
	}
	return 0;
}

void snakeMove()//显示蛇的移动 
{
	snake* headnext, *q;
	headnext = (snake*)malloc(sizeof(snake));//给headnext申请空间 
	if (direction == 0)//向右移动 
	{
		headnext->x = head->x + sizePaint;
		headnext->y = head->y;
	}
	else if (direction == 1)//向左移动 
	{
		headnext->x = head->x - sizePaint;
		headnext->y = head->y;
	}
	else if (direction == 2)//向上移动 
	{
		headnext->x = head->x;
		headnext->y = head->y - sizePaint;
	}
	else if (direction == 3)//向下移动 
	{
		headnext->x = head->x;
		headnext->y = head->y + sizePaint;
	}
	headnext->next = head;
	head = headnext;
	q = head;
	putimage(q->x, q->y, 20, 20, snakeHead, 0, 0, 20, 20);
	q = q->next;
	while (q->next->next != NULL)//打印蛇身体，使其往前进方向进一格 
	{
		putimage(q->x, q->y, 20, 20, snakeBody, 0, 0, 20, 20);
		q = q->next;
	}
	if (head->x == tomato.x && head->y == tomato.y)//判断是否吃到食物 
	{
		snakelen++;//每吃一个食物蛇的理论长度加1
		getTomatoNum++;
		createFood(0);//生成新的食物 
		createObstacle();//生成障碍物 
		if (level == 1)
			score = score + 10;
		else if (level == 2)
			score = score + 15;
		else if (level == 3)
			score = score + 20;
	}
	else if (head->x == hamburger.x && head->y == hamburger.y)//判断是否吃到食物 
	{
		snakelen++;
		getHamburgerNum++;
		createFood(1);//生成新的食物 
		createObstacle();//生成障碍物
		if (level == 1)
			score = score + 10;
		else if (level == 2)
			score = score + 15;
		else if (level == 3)
			score = score + 20;
	}
	else if (head->x == mushroom.x && head->y == mushroom.y)//判断是否吃到食物 
	{
		snakelen++;
		getMushroomNum++;
		createFood(2);//生成新的食物 
		createObstacle();//生成障碍物
		if (level == 1)
			score = score + 10;
		else if (level == 2)
			score = score + 15;
		else if (level == 3)
			score = score + 20;
	}
	else
	{
		putimage(q->x, q->y, 20, 20, white, 0, 0, 20, 20);
		free(q->next);//将代表移动之前蛇尾位置的空间释放掉 
		q->next = NULL;//让心的蛇尾结构体的next为空  方便以后找到蛇尾位置 
	}

	if (level == 2)//判断游戏速度  简单难度游戏速度不变 
	{
		if (score <= 45)
			speed = 100;
		else if (score <= 105)
			speed = 90;
		else if (score <= 180)
			speed = 80;
		else if (score <= 255)
			speed = 70;
		else if (score <= 345)
			speed = 60;
		else if (score <= 465)
			speed = 50;
	}
	else if (level == 3)
	{
		if (score <= 40)
			speed = 100;
		else if (score <= 100)
			speed = 85;
		else if (score <= 200)
			speed = 70;
		else if (score <= 400)
			speed = 55;
		else if (score <= 600)
			speed = 40;
		else if (score <= 800)
			speed = 25;
	}
}
void snakeStop()//游戏暂停 
{
	while (1)
	{
		Sleep(300);
		if (GetAsyncKeyState(VK_SPACE))//当暂停时按空格键解除暂停状态 
			break;
	}
}

int snakeDie()//判断蛇是否死亡 
{
	int i, flag = 1;
	snake* body;
	body = head->next;
	if (head->x == 0 || head->x == 780 || head->y == 0 || head->y == 780)//触碰边界就死亡 
		return 1;
	for (i = 0; i < obstacleNum; i++)
	{
		if (head->x == obstacle.x[i] && head->y == obstacle.y[i])//判断蛇是否碰到障碍物 
		{
			return 1;
		}
	}
	while (body != NULL)
	{
		if (head->x == body->x && head->y == body->y)//判断蛇头是否撞上了蛇身 
		{
			return 1;
		}
		body = body->next;
	}

	return 0;
}
void putScore()
{
	speed1 = 110 - speed;//计算相对速度 
	char highestScore[4], playerScore[4], snakeSpeed[2], Num1[2], Num2[2], Num3[2];
	sprintf(highestScore, "%2d", record[0].highscore);//将整型转化成字符型 
	sprintf(playerScore, "%2d", score);
	sprintf(snakeSpeed, "%d", speed1);
	sprintf(Num1, "%d", getTomatoNum);
	sprintf(Num2, "%d", getHamburgerNum);
	sprintf(Num3, "%d", getMushroomNum);
	setbkcolor(EGERGB(240, 255, 240));
	setcolor(EGERGB(191, 293, 255));
	setfont(20, 0, "宋体");

	if (level == 1)//显示不同模式下食物的分值 
		outtextrect(937, 163, 40, 20, "10");
	else if (level == 2)
		outtextrect(937, 163, 40, 20, "15");
	else if (level == 3)
		outtextrect(937, 163, 40, 20, "20");

	outtextrect(878, 193, 100, 20, playerScore);//在指定位置显示分值 
	outtextrect(933, 222, 80, 20, snakeSpeed);
	outtextrect(957, 70, 100, 20, highestScore);
	outtextrect(860, 320, 10, 20, Num1);
	outtextrect(860, 353, 10, 20, Num2);
	outtextrect(860, 385, 10, 20, Num3);
}
void getPaint()
{
	menu = newimage();
	getimage(menu, L"Menu.jpg", 0, 0);
	difficulty = newimage();
	getimage(difficulty, L"difficulty.jpg");
	fangchenmi = newimage();
	getimage(fangchenmi, L"chenmi.jpg");
	white = newimage();
	getimage(white, L"white.png", 0, 0);
	help = newimage();
	getimage(help, L"help.jpg", 0, 0);
	snakeBody = newimage();
	getimage(snakeBody, L"snakeBody.jpg", 0, 0);
	snakeHead = newimage();
	getimage(snakeHead, L"snakeHead.png", 0, 0);
	lange = newimage();
	getimage(lange, L"lange.jpg", 0, 0);
	over = newimage();
	getimage(over, L"over.jpg", 0, 0);
	reconds = newimage();
	getimage(reconds, L"recond.jpg", 0, 0);
	paiming = newimage();
	getimage(paiming, L"paiming.jpg", 0, 0);
	zhadan = newimage();
	getimage(zhadan, L"zhadan.jpg", 0, 0);
	fanqie = newimage();
	getimage(fanqie, L"fanqie.png", 0, 0);
	hanbao = newimage();
	getimage(hanbao, L"hanbao.png", 0, 0);
	Mushroom = newimage();
	getimage(Mushroom, L"mushroom.png", 0, 0);
}
void data()//初始化各参数 
{
	score = 0;//初始化分数为0 
	speed1 = 0;//初始化速度0 
	speed = 100;
	snakelen = 3;//初始化蛇的长度为3节 
	direction = 0;//初始化开局时，蛇向右移动 
	obstacleNum = 0;//初始化障碍物的个数为
	getTomatoNum = 0;//初始化吃到的番茄个数 
	getHamburgerNum = 0;//初始化吃到的汉堡个数 
	getMushroomNum = 0;//初始化吃到的蘑菇个数 
}

void recond()
{
	time_t t;
	struct tm* lt;
	time(&t);//获取Unix时间戳。
	lt = localtime(&t);//转为时间结构。
	//int score = 0;      //当前次，游戏得分
	int i, j;
	for (i = 0; i < 10; i++)
		record[i].highscore = 0;

	//存储最高分
	getHighScore(record);
	for (i = 0; i < 10; i++)
	{
		if (score > record[i].highscore)
		{
			for (j = 9; j > i; j--)
				record[j] = record[j - 1];
			record[i].highscore = score;
			record[i].year = lt->tm_year + 1900;
			record[i].month = lt->tm_mon + 1;
			record[i].day = lt->tm_mday;
			record[i].hour = lt->tm_hour;
			record[i].minute = lt->tm_min;
			record[i].second = lt->tm_sec;
			strcpy(record[i].name, name);
			break;
		}
	}
	//存入当前最高分
	storeHighScore(record);
}
//如果把此函数写在main函数后面，记得在main函数前面某行处，加上函数声明！
void getHighScore(struct rec record[10])
{
	FILE* fp = NULL;   //文件指针，指向成功打开的文件
	int i;
	//打开文件，以只读的方式r
	fp = fopen("highscore.txt", "r");

	//如果文件不存在，打开文件，以写入的方式w
	if (fp == NULL)
	{
		fp = fopen("highscore.txt", "w");
	}
	else
		for (i = 0; i < 10; i++)
			fscanf(fp, "%5d %4d %4d %4d %4d %4d %4d %10s", &record[i].highscore, &record[i].year, &record[i].month, &record[i].day, &record[i].hour, &record[i].minute, &record[i].second, record[i].name);

	//关闭文件
	fclose(fp);

}
//如果把此函数写在main函数后面，记得在main函数前面某行处，加上函数声明！
void storeHighScore(struct rec record[10])
{
	FILE* fp = NULL;
	int i;
	fp = fopen("highscore.txt", "w");
	for (i = 0; i < 10; i++)
		fprintf(fp, "%5d %4d %4d %4d %4d %4d %4d %10s", record[i].highscore, record[i].year, record[i].month, record[i].day, record[i].hour, record[i].minute, record[i].second, record[i].name);
	fclose(fp);
}
void Ranking(struct rec record[10])
{
	int i;
	char s[100];
	char k[100];
	char j[100];
	getbkcolor(paiming);
	setbkmode(TRANSPARENT);
	setbkcolor(getbkcolor(paiming));
	//设置背景颜色为
	setcolor(GREEN);
	for (i = 0; i < 10; i++)
	{
		sprintf(s, "第%d名    %s", i + 1, record[i].name);
		setfont(20, 0, "黑体");
		outtextxy(300, 100 + 50 * i, s);
		sprintf(j, "%d", record[i].highscore);
		outtextxy(500, 100 + 50 * i, j);
		sprintf(k, "%d/%d/%d           %d:%d:%d", record[i].year, record[i].month, record[i].day, record[i].hour, record[i].minute, record[i].second);
		setfont(20, 0, "黑体");
		outtextxy(700, 100 + 50 * i, k);
	}
	getch();
}

int fcm()
{
	time_t t;
	struct tm* lt;
	time(&t);//获取Unix时间戳。
	lt = localtime(&t);
	FILE* fp = NULL;   //文件指针，指向成功打开的文件
	fp = fopen("chenmi.txt", "r");
	//如果文件不存在，打开文件，以写入的方式w
	if (fp == NULL)
	{
		fp = fopen("chenmi.txt", "w");
	}
	else
		fscanf(fp, "%5d %4d %4d %4d", &chenmi.num, &chenmi.nian, &chenmi.yue, &chenmi.ri);
	//关闭文件
	if (chenmi.nian != lt->tm_year + 1900 || chenmi.yue != lt->tm_mon || chenmi.ri != lt->tm_mday)
	{
		fp = fopen("chenmi.txt", "w");
		chenmi.num = 0;
	}
	fclose(fp);
	chenmi.nian = lt->tm_year + 1900;
	chenmi.yue = lt->tm_mon;
	chenmi.ri = lt->tm_mday;
	if (chenmi.nian == lt->tm_year + 1900 && chenmi.yue == lt->tm_mon && chenmi.ri == lt->tm_mday)
		chenmi.num = chenmi.num + 1;
	if (chenmi.num <= 10)
	{
		FILE* fp = NULL;
		fp = fopen("chenmi.txt", "w");
		fprintf(fp, "%5d %4d %4d %4d", chenmi.num, chenmi.nian, chenmi.yue, chenmi.ri);
		fclose(fp);
		return 0;
	}
	else
		return 1;
}