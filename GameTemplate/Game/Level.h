#pragma once
#include "MapChip.h"
#include "Skeleton.h"
#include "TklFile.h"


struct LevelObjectData {
	Vector3 position;		//<座標。
	Quaternion rotation;	//!<回転。
	Vector3 scale;			//!<拡大率。
	const wchar_t* name;	//!<名前。
	/*!
	* @brief	引数で渡したオブジェクト名のオブジェクトか調べる。
	*@param[in]	objName		調べる名前。
	*@return	名前が同じ場合にtrueを返します。
	*/
	bool EqualObjectName(const wchar_t* objName)
	{
		return wcscmp(objName, name) == 0;
	}
	/*!
	* @brief	名前が前方一致するか調べる。
	*/
	bool ForwardMatchName(const wchar_t* n)
	{
		auto len = wcslen(n);
		auto namelen = wcslen(name);
		if (len > namelen) {
			//名前が長い。不一致。
			return false;
		}
		return wcsncmp(n, name, len) == 0;
	}
};


class CLevel
{
private:
	using CMapChipPtr = std::unique_ptr<CMapChip>;
public:
	/*!
	 * @brief	レベルを初期化。
	 *@param[in]	levelDataFilePath		tklファイルのファイルパス。
	 *@param[in] hookFunc				オブジェクトを作成する時の処理をフックするための関数オブジェクト。
	 *   フックしないならnullptrを指定すればよい、
	 * この関数オブジェクトがfalseを返すと、オブジェクトの情報から、
	 * 静的オブジェクトのMapChipクラスのインスタンスが生成されます。
	 * オブジェクトの名前などで、ドアやジャンプ台、アイテムなどの特殊なクラスのインスタンスを生成したときに、
	 * デフォルトで作成されるMapChipクラスのインスタンスが不要な場合はtrueを返してください。
	 * 例えば、フック関数の中で、渡されたオブジェクトデータの名前のモデルを描画するクラスのインスタンスを
	 * 生成したときに、falseを返してしまうと、同じモデルが二つ描画されることになります。
	 */
	void Init(/*const wchar_t**/const char* filePath, std::function<bool(LevelObjectData& objData)> hookFunc);


	void Draw(RenderContext& rc)
	{
		std::vector< CMapChipPtr>::iterator it = m_mapChipPtrs.begin();
		for (; it != m_mapChipPtrs.end(); it++)
		{
			it->get()->Draw(rc);
		}
	};

private:
	std::vector<CMapChipPtr> m_mapChipPtrs;			//マップチップの可変長配列。
	TklFile m_tklFile;								//Levelを読み込むクラス
};

