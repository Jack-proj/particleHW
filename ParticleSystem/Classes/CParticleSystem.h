#ifndef __CPARTICLE_SYSTEM_H__
#define __CPARTICLE_SYSTEM_H__

#include "cocos2d.h"
#include "CParticle.h"
#include "SimpleAudioEngine.h"
#include <list>

using namespace std;


class CParticleSystem
{
private:
	CParticle* _pParticles;
	list<CParticle*> _FreeList;
	list<CParticle*> _InUsedList;
	int _iFree;	// �i�Ϊ�  Particle �Ӽ�
	int _iInUsed;  // ���b�ϥΪ� Particle �Ӽ�
	int _iType;  // �ثe������l�B�ʼҦ��O���@�ث��A
	int _iEType = 100;//�ثe�O���@��Emitter���A

	bool _bEmitterOn; // Emitter �O�_���
	cocos2d::Point _pos;
public:
	cocos2d::Color3B _color;	// ���l���C��(gonna use)

	unsigned int eid;

	// Emitter �]�w�������Ѽ�
	cocos2d::Point _emitterPt;// Emitter ����m	
	float    _fDir; // Emitter ���Q�g��V�A0 �� 360�סA�f�ɰw��V����
	int		_iNumParticles;	// �C�������ͪ����l�Ӽ�
	int     _iGenParticles; // ��ثe����Ҳ��ͪ����l�Ӽ�(�H�C�����)
	float	_fSpread;	 // 0 �� 180 �סA�w�]�� 180 
	float   _fVelocity;	 // ���l�����}�t��
	float   _fLifeTime;	 // ���l���s���ɶ�
	float	_fSpin;		// ���l������q degree/sec
	float   _fGravity;
	float _fOpacity;
	float _fR;
	float _fG;
	float _fB;

	float   _fElpasedTime;	// �g�L�h�֮ɶ��A�H�����A�@�����ͤ��l���p��̾�
	cocos2d::Point _windDir;  // ������V�A���d�ҨS����@

	CParticleSystem();
	~CParticleSystem();
	void init(cocos2d::Layer &inlayer);
	void doStep(float dt);
	void setEmitter(bool bEm);
	void setType(int type) { _iType = type; }
	void setEType(int type) { _iEType = type; }

 //   void onTouchesEnded(const cocos2d::CCPoint &touchPoint);
    void onTouchesBegan(const cocos2d::CCPoint &touchPoint);
    void onTouchesMoved(const cocos2d::CCPoint &touchPoint);
	void onTouchesEnd(const cocos2d::CCPoint &touchPoint);
	void setGravity(float fGravity);
	void setSpin(float fSpin);
	void setOpacity(float fOpacity);
	void setSpeed(float fSpeed);
	void setRed(float fRed);
	void setGreen(float fGreen);
	void setBlue(float fBlue);
	void setNewPic(const char *pngName, cocos2d::Layer &inlayer);
};

#endif