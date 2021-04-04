#include "stdafx.h"
#include "OBBWorld.h"

//�B��̃C���X�^���X�̏�����
COBBWorld* COBBWorld::m_instance = nullptr;

/// <summary>
/// OBB��OBBWorld�ɓo�^����֐�
/// </summary>
/// <param name="obb">�o�^����OBB</param>
void COBBWorld::AddOBB(COBB* obb)
{
	//������OBB��o�^����
	m_worldOBBs.push_back(obb);
}

/// <summary>
/// OBBWorld�ɓo�^���Ă���OBB����������֐�
/// </summary>
/// <param name="obb">�o�^����OBB</param>
void COBBWorld::RemoveOBB(COBB* obb)
{
	//�o�^���Ă���OBB�B��S�����ׂ�
	std::vector<COBB*>::iterator itr;
	for (itr = m_worldOBBs.begin(); itr != m_worldOBBs.end(); itr++)
	{
		//������OBB�ƈ�v����OBB�̓o�^����������
		if ((*itr) == obb)
		{
			m_worldOBBs.erase(itr);
			break;
		}
	}
}

/// <summary>
/// OBBWorld�ɓo�^���Ă��邷�ׂĂ�OBB�𒲂ׂāA
/// ��������OBB�ƏՓ˂��Ă����ԋ߂�OBB����
/// �߂��B
/// �ǂ�OBB�Ƃ��Փ˂��Ă��Ȃ�������Anullptr��
/// �߂��ė���B
/// </summary>
/// <param name="myOBB">���g��OBB</param>
/// <returns>�Փ˂��Ă���OBB���Anullptr</returns>
COBB* COBBWorld::HitAllOBB(const COBB& myOBB)
{
	//��ԋ߂�OBB��T�����߂̋���������ϐ�
	float dist = FLT_MAX;

	//�Փ˂���OBB��ێ����Ă���OBB�̃|�C���^
	COBB* pOBB = nullptr;

	//OBBWorld�ɓo�^���Ă���OBB��������o���āA���ׂ�
	for (auto OBB : m_worldOBBs)
	{
		//�����œn���ꂽ���g��OBB�ƁAOBBWorld������o����OBB�Ƃ�
		//�Փ˔���
		if (CollisionOBBs(myOBB, *OBB))
		{
			//�Փ˂��Ă���

			//���g��OBB�ƁA�Փ˂���OBB�Ƃ̊Ԃ̃x�N�g��
			Vector3 lenVec = myOBB.GetPosition() - OBB->GetPosition();
			//���g��OBB�ƁA�Փ˂���OBB�Ƃ̊Ԃ̋���
			float len = lenVec.Length();

			//��������ԋ߂�OBB���Փ˂���OBB�Ƃ��ĕێ����Ă���
			if (len <= dist)
			{
				//OBB��ێ�����
				pOBB = OBB;
				//��ԋ߂��������X�V����
				dist = len;
			}
		}
	}

	//�Փ˂���OBB������ꍇ�́A�Փ˂�����ԋ߂�OBB�̃|�C���^���A
	//�Փ˂���OBB���Ȃ��ꍇ�́Anullptr��߂�
	return pOBB;
}



/// <summary>
/// �Ƃ肠������������ǁA�g��Ȃ��\��B
/// �e�X�g�����ĂȂ�
/// �ꉞ�AOBB�ƃ��C�̌����_�����߂�֐�
/// </summary>
/// <param name="start">���C�̎n�_</param>
/// <param name="end">���C�̏I�_</param>
/// <param name="pos">���������ۂ̌����_�̍��W</param>
/// <returns>�����������H</returns>
bool COBBWorld::InIntersectLine(const Vector3& start, const Vector3& end, Vector3* pos)
{
	bool isHit = false;
	float dist = FLT_MAX;
	SOBBIndexBuffer indexBuffer;

	//OBBWorld�ɓo�^���Ă���OBB���ЂƂ����o��
	for (auto OBB : m_worldOBBs)
	{
		//OBB�̒��_�̍��W�̔z������o��
		Vector3* OBBsVertex = OBB->GetBoxVertex();

		//OBB�̃C���f�b�N�X�o�b�t�@��1�|���S�������o���B
		for (auto index : indexBuffer.m_indexBuffer)
		{
			//�O�p�|���S���̒��_�̐�
			const int VertNum = 3;
			//�|���S���̒��_
			Vector3 vertPos[VertNum];
			//OBB�̒��_������
			vertPos[0] = OBBsVertex[index.x];
			vertPos[1] = OBBsVertex[index.y];
			vertPos[2] = OBBsVertex[index.z];

			Vector3 normalVec;	//���_�̖@���x�N�g��
			Vector3 rNormalVec;
			{
				//�@���͎O���_�̊O�ςŖ@�����o���B
				Vector3 vert0to1 = vertPos[1] - vertPos[0];	//���_0����1�ւ̃x�N�g��
				Vector3 vert0to2 = vertPos[2] - vertPos[0];	//���_0����2�ւ̃x�N�g��
				normalVec.Cross(vert0to1, vert0to2);	//�O�ςŒ�������x�N�g�����o��
				normalVec.Normalize();	//���K�����Ă���
				rNormalVec.Cross(vert0to2, vert0to1);
				rNormalVec.Normalize();
			}

			///
			///
			/// �菇
			/// 1�A�O�p�`���܂ޖ������ʂƐ����̌�������
			/// 2�A�������Ă�����W�̌v�Z
			/// 3�A2�ŋ��߂����W���O�p�`�̒��ɂ��邩�ǂ�������//�O�ώg����


			//1�A�O�p�`���܂ޖ������ʂƐ����̌�������
			Vector3 VtoS = start - vertPos[0];	//���_��������̎n�_�ւ̃x�N�g��
			VtoS.Normalize();	//�ꉞ���K�����Ă���
			Vector3 VtoE = end - vertPos[0];	//���_��������̏I�_�ւ̃x�N�g��
			VtoE.Normalize();	//�ꉞ���K�����Ă���

			float VtoSdotN = Dot(normalVec, VtoS);	//vertVtoS�Ɩ@���̓���
			float VtoEdotN = Dot(normalVec, VtoE);	//vertVtoE�Ɩ@���̓���

			if (VtoSdotN * VtoEdotN >= 0)
			{
				//�������ʂƌ������Ă��Ȃ��̂ŃX�L�b�v
				continue;
			}

			//2�A�������Ă�����W�̌v�Z

			Vector3 addToStart;	//���x�N�g���̎n�_�ɉ��Z����x�N�g��
			addToStart = end - start;	//�����̃x�N�g��������

			Vector3 VtoS2 = start - vertPos[0];	//���_��������̎n�_�ւ̃x�N�g��
			Vector3 VtoE2 = end - vertPos[0];	//���_��������̏I�_�ւ̃x�N�g��
			float VtoSdotN2 = Dot(normalVec, VtoS2);	//vertVtoS�Ɩ@���̓���
			float VtoEdotN2 = Dot(rNormalVec, VtoE2);	//vertVtoE�Ɩ@���̓���


			float f = VtoSdotN2 / (VtoSdotN2 + VtoEdotN2);
			//addToStart *= VtoSdotN / (VtoSdotN - VtoEdotN);	//�������v�Z����
			addToStart *= f;
			//��_�̍��W���v�Z����
			const Vector3 intersectPos = start + addToStart;	//��_�̍��W


			/// 3�A2�ŋ��߂����W���O�p�`�̒��ɂ��邩�ǂ�������

			Vector3 V0toV1 = vertPos[1] - vertPos[0];	//���_0���璸�_1�ւ̃x�N�g��
			Vector3 V0toI = intersectPos - vertPos[0];	//���_0�����_�ւ̃x�N�g��
			Vector3 cross1;	//�O��1
			cross1.Cross(V0toV1, V0toI);
			cross1.Normalize();

			Vector3 V1toV2 = vertPos[2] - vertPos[1];	//���_1���璸�_2�ւ̃x�N�g��
			Vector3 V1toI = intersectPos - vertPos[1];	//���_1�����_�ւ̃x�N�g��
			Vector3 cross2;	//�O��2
			cross2.Cross(V1toV2, V1toI);
			cross2.Normalize();


			Vector3 V2toV0 = vertPos[0] - vertPos[2];	//���_2���璸�_0�ւ̃x�N�g��
			Vector3 V2toI = intersectPos - vertPos[2];	//���_2�����_�ւ̃x�N�g��
			Vector3 cross3;	//�O��3
			cross3.Cross(V2toV0, V2toI);
			cross3.Normalize();

			float res1 = Dot(cross1, cross2);
			float res2 = Dot(cross1, cross3);


			if ((res1 > 0.0f && res2 > 0.0f))
			{
				//��_�̍��W���O�p�`�̒��ɂ��邩��A�q�b�g�I
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