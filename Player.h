#pragma once
#include "Collision.h"
#include "Camera.h"
#include "Input.h"
#include "DXInput.h"
#include "FbxModel.h"
#include "FbxObject3D.h"
#include "CubeModel.h"
#include "CubeObject3D.h"

class Player
{
	//�����o�֐�
public:
	//�ÓI�����o�֐�
	static void SetDevice(ID3D12Device* device) { Player::device = device; }
	static void SetCamera(Camera* camera) { Player::camera = camera; }
	static void SetInput(Input* input) { Player::input = input; }
	static void SetDXInput(DXInput* dxInput) { Player::dxInput = dxInput; }

	//�R���X�g���N�^
	Player() {};
	//�f�X�g���N�^
	~Player() {};
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//���f���Z�b�g
	void SetModel(FbxModel* model) { this->model = model; }
	//hitbox�p�̃��f��
	void SetCubeModel(CubeModel* model) { this->cubeModel = model; }

	//�V�[�����Ƃ̃Z�b�g
	void SetTitle();
	void SetTutorial();
	void SetStage();
	void SetStage2();
	void SetStage4();

	//�����蔻��X�V
	void UpdateCollision();
	//�����X�V
	void UpdateMove();

	//�Z�b�^�[
	//�ό`�s��Z�b�g
	void SetPosition0(DirectX::XMFLOAT3 pos) { position0 = pos; }
	void SetRotation0(DirectX::XMFLOAT3 rot) { rotation0 = rot; }
	void SetScale0(DirectX::XMFLOAT3 sca) { scale0 = sca; }
	//�����蔻��Z�b�g
	void SetCollisionFloor(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale);
	void SetCollisionObstacle(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale);
	void SetCollisionKey(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale);
	void SetCollisionGoal(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale);
	//�����蔻����N���A����֐�(�X�e�[�W�ς�閈�ɌĂ�)
	void ClearCollision();

	//�Q�b�^�[
	//�ό`�s��
	DirectX::XMFLOAT3 GetPosition0() { return position0; }
	DirectX::XMFLOAT3 GetRotation0() { return rotation0; }
	DirectX::XMFLOAT3 GetScale0() { return scale0; }

	//hitbox
	DirectX::XMFLOAT3 GetHitboxPosition0() { return hitboxPosition0; }
	DirectX::XMFLOAT3 GetHitboxRotation0() { return hitboxRotation0; }
	DirectX::XMFLOAT3 GetHitboxScale0() { return hitboxScale0; }

	bool GetGroundFlag0() { return groundFlag0; }
	bool GetMoveFlag() { return moveFlag; }

	int GetSceneFlag() { return sceneFlag; }

	//�����o�ϐ�
private:
	//�f�o�C�X
	static ID3D12Device* device;
	//�J����
	static Camera* camera;
	//�L�[�{�[�h
	static Input* input;
	//�R���g���[���[
	static DXInput* dxInput;
	//���f�����I�u�W�F�N�g(�v���C���[��2����̂�2����)
	/*std::unique_ptr<Metaball> metaball0;
	std::unique_ptr<Metaball> metaball1;*/

	//���f��
	FbxModel* model;
	CubeModel* cubeModel;
	FbxObject3D* object0 = nullptr;
	std::unique_ptr<CubeObject3D> cubeObject0;
	//�����蔻��
	std::list<std::unique_ptr<Collision>> collisionsFloor;
	std::list<std::unique_ptr<Collision>> collisionsObstacle;

	//�ό`�s��
	DirectX::XMFLOAT3 position0 = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 rotation0 = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale0 = { 0.01f,0.01f,0.01f };


	//hitbox�̕ό`�s��
	DirectX::XMFLOAT3 hitboxPosition0 = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 hitboxRotation0 = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 hitboxScale0 = { 0.01f,0.01f,0.01f };

	//�ړ��x�N�g��
	XMFLOAT3 velocity0 = { 0,0,0 };

	//�ړ��A�����p�ϐ�
	//�����x�N�g��
	XMFLOAT3 fallVelocity0 = { 0,0,0 };
	XMFLOAT3 fallVelocity1 = { 0,0,0 };
	//�����x�N�g��
	XMFLOAT3 playerDirection0 = { 0,0,0 };
	//�����^�C�}�[
	float fallTimer0 = 0.0f;
	float fallTimer1 = 0.0f;


	//�ڒn�t���O
	bool groundFlag0 = false;

	bool preGroundFlag0 = false;

	bool moveFlag = false;
	//���̃v���C���[�̑��x
	float speed = 0.4f;

	//�\���̗񋓌^
public:
	enum State
	{
		front,	//�\��Ԃ̂Ƃ�
		back,	//����Ԃ̂Ƃ�
	};
	//�Q�b�^�[
	State GetPlayerState() { return playerState; }
private:
	//�v���C���[�̏�Ԃ�\���t���O
	State playerState = front;
	State prePlayerState = front;
	//���̃t���O
	bool keyFlag = false;
	bool goalFlag = false;

	//����ւ��p�̃t���O
	bool changeFlag = false;

	int startTimer = 0;
	int sceneFlag = 0;
};