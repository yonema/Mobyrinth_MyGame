#pragma once

/**
 * @brief GameObjectをNewGOするときの優先度（priority)の列挙型
*/
enum EN_PRIORITY
{
	PRIORITY_FIRST,			//一番目。優先度0
	PRIORITY_SECOND = 10,	//二番目。優先10
	PRIORITY_THIRD = 20,	//三番目。優先度20
	PRIORITY_FOURTH = 30,	//四番目。優先度30
};
