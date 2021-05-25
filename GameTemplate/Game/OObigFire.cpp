#include "stdafx.h"
#include "OObigFire.h"

//�X�^�[�g�֐�
bool OObigFire::StartSub()
{
	//���f���̏������ƃ^�C�v�̐ݒ�
	Init("Assets/modelData/flame.tkm", enBigFire);

	//OBB�̃T�C�Y��ݒ�
	Vector3 obbSize;
	obbSize = { 300.0f,300.0f,400.0f };
	//OBB�̕����x�N�g���̒�����ݒ�
	GetOBB().SetDirectionLength(obbSize);

	//�|�C���g���C�g�𐶐�
	m_pointLight = NewGO<CPointLight>(0);
	Vector3 ptLigPos = m_position;
	Vector3 up = g_vec3Up;
	m_rotation.Apply(up);
	up.Scale(300.0f);
	ptLigPos += up;
	m_pointLight->SetPosition(ptLigPos);
	m_pointLight->SetColor({ 500.0f,1.0f,1.0f,1.0f });
	m_pointLight->SetRange(500.0f);

	//���Ȕ����F��ݒ�
	SetEmissionColor({ 1.0f,0.0f,0.0f,1.0f });

	GetOBB().SetTag(COBB::enBigFire);

	//flameSE�̃T�E���h�L���[�𐶐�����
	m_flameSE = NewGO<CSoundCue>(0);
	//flameSE�̃T�E���h�L���[���Awave�t�@�C�����w�肵�ď���������B
	m_flameSE->Init(L"Assets/sound/flame.wav");

	//m_flame_water�G�t�F�N�g�̍쐬
	m_flame_water = NewGO<Effect>(0);
	m_flame_water->Init(u"Assets/effect/flame_water.efk");
	float scale = 200.0f;								//�������̂ő傫�����Ă���
	m_flame_water->SetScale({ scale ,scale ,scale });
	Vector3 upVec = g_vec3Up;
	m_rotation.Apply(upVec);
	//���̒l��ύX���č����𒲐߂���
	const float upVecLne = 100.0f;
	upVec.Scale(upVecLne);
	m_flame_water->SetPosition(m_position + upVec);		//���W��n��
	m_flame_water->SetRotation(m_rotation);

	//m_flame_waterSE�̃T�E���h�L���[�𐶐�����
	m_flame_waterSE = NewGO<CSoundCue>(0);
	//m_flame_waterSE�̃T�E���h�L���[���Awave�t�@�C�����w�肵�ď���������B
	m_flame_waterSE->Init(L"Assets/sound/flame_water.wav");
	//���ʒ���
	m_flame_waterSE->SetVolume(1.0f);

	return true;
}

//�f�X�g���N�^
OObigFire::~OObigFire()
{
	DeleteGO(m_pointLight);


	if (m_flameSE->IsPlaying()) {
		DeleteGO(m_flameSE);
	}

	if (m_flame_water->IsPlay()) {
		DeleteGO(m_flame_water);
	}

}

//�A�b�v�f�[�g�֐�
void OObigFire::UpdateSub()
{
	Burn();

	if (GetFlagIsHit())
		m_pointLight->SetColor({ 500.0f,1.0f,1.0f,1.0f });
	else
		m_pointLight->SetColor({ 0.0f,0.0f,0.0f,1.0f });
}

//�_���[�W���󂯂�
void OObigFire::Damage()
{
	if (--m_hp <= 0)
	{
		//HP��0�ȉ���������A���̃I�u�W�F�N�g������
		DeleteGO(this);
	}
	else
	{
		//OBB�̃T�C�Y��HP�ɉ����ď���������
		Vector3 obbSize;
		obbSize = { 300.0f,300.0f,100.0f };
		//�ł����΂̑傫���␳
		//�傫����1/3�ɂ���ƁA���������ăS�[���ł����Ⴄ����A
		//�傫����5/5��4/5��3/5��0�ƕω�������
		const int sizeComplement = 3;

		//m_hp��m_maxHp��int�^�B
		//int�^���m�̊���Z�����珬�����؂�̂Ă���
		//�]�������l��0�ɂȂ��Ă��܂��B
		//������Am_hp��float�ɃL���X�g����
		obbSize.Scale(static_cast<float>(m_hp + sizeComplement) / (m_maxHp + sizeComplement));
		GetOBB().SetDirectionLength(obbSize);


		//���f���̃T�C�Y��HP�ɉ����ď���������
			//���ꂪ��݂̂����ɐ؂�̂ĂɂȂ�Ȃ����R��
			//m_hp / m_maxHp�����g_vec3One * m_hp���Ă΂�
			//Vector3�̉��Z�q�̃I�[�o�[���C�h�̈�����float�^������
			//m_hp���n�����Ƃ��ɋ����^�ϊ����N����float�^�ɂȂ邩��
			//���ꂪSetScale(g_vec3One * (m_hp / m_maxHp))
			//�������炤�܂������Ȃ��B
		SetScale(g_vec3One * (m_hp + sizeComplement) / (m_maxHp + sizeComplement));
	}
	m_flame_water->Play();								//�Đ�
	m_flame_waterSE->Play(false);						//�Đ�
}	

//�v���C���[�����ɋ߂Â��ƔR���Ă鉹���o��
void OObigFire::Burn()
{
	Vector3 distance = m_position - m_pPlayer->GetPosition();
	const float MaxDist = 700;
	const float DistLen = distance.Length();

	if (DistLen < MaxDist) {
		float Le = MaxDist - DistLen;
		float SubLe = Le / MaxDist;
		float Vo = 2.0f * SubLe;
		
		//flameSE�����[�v�Đ����I���ōĐ�����B
		m_flameSE->Play(true);
		
		//���ʒ���
		m_flameSE->SetVolume(Vo);
		
	}
	else {
		if (m_flameSE->IsPlaying()) {
			m_flameSE->Stop();
		}
	}
}