#pragma once

#ifndef __BoardBall_H__
#define __BoardBall_H__

#include "cocos2d.h"
USING_NS_CC;

class BoardBall
{
public:
	int style;
	int color;
	virtual std::string stylename() {
		return "";
	};
	virtual Color3B colorname() {
		return Color3B::WHITE;
	};
	BoardBall();
};


class Board : public BoardBall
{
public:
	virtual std::string stylename();
	virtual Color3B colorname();
};

class Ball : public BoardBall
{
public:
	virtual std::string stylename();
	virtual Color3B colorname();
};

#endif