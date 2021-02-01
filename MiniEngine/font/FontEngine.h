#pragma once

/// <summary>
/// �t�H���g�G���W���B
/// </summary>
/// <remarks>
/// DirectXTK�̃t�H���g�\�������̏������`������s���N���X�B
/// </remarks>
class FontEngine {
	
public:
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~FontEngine();
	/// <summary>
	/// �������B
	/// </summary>
	void Init();
	/// <summary>
	/// �`��J�n�B
	/// </summary>
	void BeginDraw(RenderContext& rc);
	/// <summary>
	/// �������`��B
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="color">�J���[</param>
	/// <param name="rotation">��]</param>
	/// <param name="scale">�g��</param>
	/// <param name="pivot">�s�{�b�g</param>
	void Draw(
		const wchar_t* text,
		const Vector2& position,
		const Vector4& color,
		float rotation,
		float scale,
		Vector2 pivot
	);
	/// <summary>
	/// �`��I���B
	/// </summary>
	void EndDraw(RenderContext& rc);
private:
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;	//�X�v���C�g�o�b�`�B
	std::unique_ptr<DirectX::SpriteFont>	m_spriteFont;	//�X�v���C�g�t�H���g�B
	std::unique_ptr< DirectX::ResourceUploadBatch> m_spriteResourceUploadBatch;
	std::unique_ptr< DirectX::SpriteBatchPipelineStateDescription> m_spriteBatchPipelineStateDescription;
	ID3D12DescriptorHeap* m_textureDescriptorHeap = nullptr;	//�f�B�X�N���v�^�q�[�v�B
};
