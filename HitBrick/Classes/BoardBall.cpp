#include"cocos2d.h"
#include"BoardBall.h"
USING_NS_CC;

BoardBall::BoardBall() : style(0),color(0)
{

}

std::string Board::stylename()
{
	style = UserDefault::getInstance()->getIntegerForKey("boardstyle",0);
	std::string name;
	switch (style) 
	{
	case 1:
		name = "board/board.png";
		break;
	case 2:
		name = "board/board2.png";
		break;
	case 3:
		name = "board/board3.png";
		break;
	}
	return name;
}


Color3B Board::colorname()
{
	color = UserDefault::getInstance()->getIntegerForKey("boardcolor",0);
	Color3B color3B;
	switch (color)
	{
	case 1:
		color3B = Color3B::RED;
		break;
	case 2:
		color3B = Color3B::BLUE;
		break;
	case 3:
		color3B = Color3B::GREEN;
		break;
	case 4:
		color3B = Color3B::WHITE;
		break;
	}

	return color3B;
}


std::string Ball::stylename()
{
	std::string name;
	style = UserDefault::getInstance()->getIntegerForKey("ballstyle");
	switch (style)
	{
	case 1:
		name = "ball/ball.png";
		break;
	case 2:
		name = "ball/ball2.png";
		break;
	case 3:
		name = "ball/ball3.png";
		break;
	}
	return name;
}

Color3B Ball::colorname()
{
	color = UserDefault::getInstance()->getIntegerForKey("ballcolor");
	Color3B color3B;
	switch (color)
	{
	case 1:
		color3B = Color3B::RED;
		break;
	case 2:
		color3B = Color3B::BLUE;
		break;
	case 3:
		color3B = Color3B::GREEN;
		break;

	case 4:
		color3B = Color3B::WHITE;
		break;
	}

	return color3B;
}