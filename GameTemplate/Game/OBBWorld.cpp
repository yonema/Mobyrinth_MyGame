#include "stdafx.h"
#include "OBBWorld.h"

//唯一のインスタンスの初期化
COBBWorld* COBBWorld::m_instance = nullptr;

/// <summary>
/// OBBをOBBWorldに登録する関数
/// </summary>
/// <param name="obb">登録するOBB</param>
void COBBWorld::AddOBB(COBB* obb)
{
	//引数のOBBを登録する
	m_worldOBBs.push_back(obb);
}

/// <summary>
/// OBBWorldに登録してあるOBBを解除する関数
/// </summary>
/// <param name="obb">登録するOBB</param>
void COBBWorld::RemoveOBB(COBB* obb)
{
	//登録してあるOBB達を全部調べる
	std::vector<COBB*>::iterator itr;
	for (itr = m_worldOBBs.begin(); itr != m_worldOBBs.end(); itr++)
	{
		//引数のOBBと一致するOBBの登録を解除する
		if ((*itr) == obb)
		{
			m_worldOBBs.erase(itr);
			break;
		}
	}
}

/// <summary>
/// OBBWorldに登録してあるすべてのOBBを調べて、
/// 第一引数のOBBと衝突している一番近いOBBをを
/// 戻す。
/// どのOBBとも衝突していなかったら、nullptrが
/// 戻って来る。
/// </summary>
/// <param name="myOBB">自身のOBB</param>
/// <returns>衝突しているOBBか、nullptr</returns>
COBB* COBBWorld::HitAllOBB(const COBB& myOBB)
{
	//一番近いOBBを探すための距離を入れる変数
	float dist = FLT_MAX;

	//衝突したOBBを保持しておくOBBのポインタ
	COBB* pOBB = nullptr;

	//OBBWorldに登録してあるOBBを一つずつ取り出して、調べる
	for (auto OBB : m_worldOBBs)
	{
		//引数で渡された自身のOBBと、OBBWorldから取り出したOBBとの
		//衝突判定
		if (CollisionOBBs(myOBB, *OBB))
		{
			//衝突している

			//自身のOBBと、衝突したOBBとの間のベクトル
			Vector3 lenVec = myOBB.GetPosition() - OBB->GetPosition();
			//自身のOBBと、衝突したOBBとの間の距離
			float len = lenVec.Length();

			//距離が一番近いOBBを衝突したOBBとして保持しておく
			if (len <= dist)
			{
				//OBBを保持して
				pOBB = OBB;
				//一番近い距離も更新する
				dist = len;
			}
		}
	}

	//衝突したOBBがある場合は、衝突した一番近いOBBのポインタを、
	//衝突したOBBがない場合は、nullptrを戻す
	return pOBB;
}



/// <summary>
/// とりあえず作ったけど、使わない予定。
/// テストもしてない
/// 一応、OBBとレイの交差点を求める関数
/// </summary>
/// <param name="start">レイの始点</param>
/// <param name="end">レイの終点</param>
/// <param name="pos">交差した際の交差点の座標</param>
/// <returns>交差したか？</returns>
bool COBBWorld::InIntersectLine(const Vector3& start, const Vector3& end, Vector3* pos)
{
	bool isHit = false;
	float dist = FLT_MAX;
	SOBBIndexBuffer indexBuffer;

	//OBBWorldに登録してあるOBBをひとつずつ取り出す
	for (auto OBB : m_worldOBBs)
	{
		//OBBの頂点の座標の配列を取り出す
		Vector3* OBBsVertex = OBB->GetBoxVertex();

		//OBBのインデックスバッファを1ポリゴンずつ取り出す。
		for (auto index : indexBuffer.m_indexBuffer)
		{
			//三角ポリゴンの頂点の数
			const int VertNum = 3;
			//ポリゴンの頂点
			Vector3 vertPos[VertNum];
			//OBBの頂点を入れる
			vertPos[0] = OBBsVertex[index.x];
			vertPos[1] = OBBsVertex[index.y];
			vertPos[2] = OBBsVertex[index.z];

			Vector3 normalVec;	//頂点の法線ベクトル
			Vector3 rNormalVec;
			{
				//法線は三頂点の外積で法線を出す。
				Vector3 vert0to1 = vertPos[1] - vertPos[0];	//頂点0から1へのベクトル
				Vector3 vert0to2 = vertPos[2] - vertPos[0];	//頂点0から2へのベクトル
				normalVec.Cross(vert0to1, vert0to2);	//外積で直交するベクトルを出す
				normalVec.Normalize();	//正規化しておく
				rNormalVec.Cross(vert0to2, vert0to1);
				rNormalVec.Normalize();
			}

			///
			///
			/// 手順
			/// 1、三角形を含む無限平面と線分の交差判定
			/// 2、交差している座標の計算
			/// 3、2で求めた座標が三角形の中にあるかどうか判定//外積使うよ


			//1、三角形を含む無限平面と線分の交差判定
			Vector3 VtoS = start - vertPos[0];	//頂点から線分の始点へのベクトル
			VtoS.Normalize();	//一応正規化しておく
			Vector3 VtoE = end - vertPos[0];	//頂点から線分の終点へのベクトル
			VtoE.Normalize();	//一応正規化しておく

			float VtoSdotN = Dot(normalVec, VtoS);	//vertVtoSと法線の内積
			float VtoEdotN = Dot(normalVec, VtoE);	//vertVtoEと法線の内積

			if (VtoSdotN * VtoEdotN >= 0)
			{
				//無限平面と交差していないのでスキップ
				continue;
			}

			//2、交差している座標の計算

			Vector3 addToStart;	//線ベクトルの始点に加算するベクトル
			addToStart = end - start;	//線分のベクトルを入れる

			Vector3 VtoS2 = start - vertPos[0];	//頂点から線分の始点へのベクトル
			Vector3 VtoE2 = end - vertPos[0];	//頂点から線分の終点へのベクトル
			float VtoSdotN2 = Dot(normalVec, VtoS2);	//vertVtoSと法線の内積
			float VtoEdotN2 = Dot(rNormalVec, VtoE2);	//vertVtoEと法線の内積


			float f = VtoSdotN2 / (VtoSdotN2 + VtoEdotN2);
			//addToStart *= VtoSdotN / (VtoSdotN - VtoEdotN);	//割合を計算する
			addToStart *= f;
			//交点の座標を計算する
			const Vector3 intersectPos = start + addToStart;	//交点の座標


			/// 3、2で求めた座標が三角形の中にあるかどうか判定

			Vector3 V0toV1 = vertPos[1] - vertPos[0];	//頂点0から頂点1へのベクトル
			Vector3 V0toI = intersectPos - vertPos[0];	//頂点0から交点へのベクトル
			Vector3 cross1;	//外積1
			cross1.Cross(V0toV1, V0toI);
			cross1.Normalize();

			Vector3 V1toV2 = vertPos[2] - vertPos[1];	//頂点1から頂点2へのベクトル
			Vector3 V1toI = intersectPos - vertPos[1];	//頂点1から交点へのベクトル
			Vector3 cross2;	//外積2
			cross2.Cross(V1toV2, V1toI);
			cross2.Normalize();


			Vector3 V2toV0 = vertPos[0] - vertPos[2];	//頂点2から頂点0へのベクトル
			Vector3 V2toI = intersectPos - vertPos[2];	//頂点2から交点へのベクトル
			Vector3 cross3;	//外積3
			cross3.Cross(V2toV0, V2toI);
			cross3.Normalize();

			float res1 = Dot(cross1, cross2);
			float res2 = Dot(cross1, cross3);


			if ((res1 > 0.0f && res2 > 0.0f))
			{
				//交点の座標が三角形の中にあるから、ヒット！
				isHit = true;
				float distTmp = addToStart.Length();
				if (distTmp < dist)
				{
					*pos = intersectPos;
					dist = distTmp;
				}
			}


		}
	}

	return isHit;

}