#pragma once

//Enum's used for Game:
enum class GameState
{
	HostOrJoin,
	Play,
	Tag,
	Reset
};

enum class PlayerType
{
	Undecided,
	Host,
	Client
};