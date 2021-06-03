#pragma once

/**
 * @brief GameObjectをNewGOするときの優先度（priority)の列挙型
*/
enum EnPriority
{
	PRIORITY_FIRST,			//優先度0
	PRIORITY_SECOND = 10,	//優先度10
	PRIORITY_THIRD = 20,	//優先度20
	PRIORITY_FOURTH = 30,	//優先度30
};
