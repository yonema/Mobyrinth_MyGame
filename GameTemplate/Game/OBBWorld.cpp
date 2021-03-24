#include "stdafx.h"
#include "OBBWorld.h"

COBBWorld* COBBWorld::m_instance = nullptr;

void COBBWorld::AddOBB(COBB* obb)
{
	m_worldOBBs.push_back(obb);
}

void COBBWorld::RemoveOBB(COBB* obb)
{
	std::vector<COBB*>::iterator itr;
	for (itr = m_worldOBBs.begin(); itr != m_worldOBBs.end(); itr++)
	{
		if ((*itr) == obb)
		{
			m_worldOBBs.erase(itr);
			break;
		}
	}
}

COBB* COBBWorld::HitAllOBB(const COBB& myOBB, COBB* hitOBB)
{
	float dist = FLT_MAX;
	bool isHit = false;
	hitOBB = nullptr;
	COBB* pOBB = nullptr;
	for (auto OBB : m_worldOBBs)
	{
		if (CollisionOBBs(myOBB, *OBB))
		{

			Vector3 lenVec = myOBB.GetPosition() - OBB->GetPosition();
			float len = lenVec.Length();
			if (len <= dist)
			{
				isHit = true;
				dist = len;
				hitOBB = OBB;
				pOBB = OBB;
			}
		}
	}

	return pOBB;
}




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