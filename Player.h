#ifndef DEF_PLAYER_H //��dinclude�h�~

#define DEF_PLAYER_H

typedef struct {
	int x;
	int y;
	int statas;
} Player_t;

// �������v�Z����
void Player_Update(); //�i�[�ϐ��cPlayer_t�^�̕ϐ��Ƃ��̋�̃X�e�[�^�X

// �}�E�X�̌�Տ�̍��W���擾����
void Player_MouseUpdate();

// ���I�����A���̎��I������
void Player_Select();

// ��u���邩�ǂ����H
void Player_Place();

// ������̏���

void Player_Motigoma();

// �`�悷��
void Player_Draw();

// ����ȉ��A�u���邩�ǂ����̔���̊֐��������i�X�e�[�W�v���C�X�X�V�j����

void ou_way(int x_1, int y_1);

void kin_way(int x_1, int y_1);

void gin_way(int x_1, int y_1);

void keima_way(int x_1, int y_1);

void hohei_way(int x_1, int y_1);

void kou_way(int x_1, int y_1);

void hisha_way(int x_1, int y_1);

void kaku_way(int x_1, int y_1);


// ����ȉ��A�u���邩�ǂ����̔���̊֐��������i�X�e�[�W�v���C�X�X�V�j�G

void ou_way_Enemy(int x_1, int y_1);

void kin_way_Enemy(int x_1, int y_1);

void gin_way_Enemy(int x_1, int y_1);

void keima_way_Enemy(int x_1, int y_1);

void hohei_way_Enemy(int x_1, int y_1);

void kou_way_Enemy(int x_1, int y_1);

void hisha_way_Enemy(int x_1, int y_1);

void kaku_way_Enemy(int x_1, int y_1);

#endif
