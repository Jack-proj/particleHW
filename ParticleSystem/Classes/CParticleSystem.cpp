#include "CParticleSystem.h"

#define NUMBER_PARTICLES 1000 // �w�]�@�����o 1000 �� Particles

USING_NS_CC;

using namespace CocosDenshion;

CParticleSystem::CParticleSystem()
{
	_fGravity = 0;
	_bEmitterOn = false;
	_fOpacity = 255;
	_color = Color3B(128 + rand() % 128, 128 + rand() % 128, 128 + rand() % 128);
}

void CParticleSystem::setEmitter(bool bEm)
{
	_bEmitterOn = bEm;
}

void CParticleSystem::init(cocos2d::Layer &inlayer)
{
	SimpleAudioEngine::getInstance()->preloadEffect("CRAM_MOVE.mp3");	// �w�����J������
	_iFree = NUMBER_PARTICLES;
	_iInUsed = 0;
	_pParticles = new CParticle[NUMBER_PARTICLES]; // ���o�һݭn�� particle �Ŷ�
	 // Ū�J�x�s�h�i�Ϥ��� plist ��
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("particletexture.plist");
	for (int i = 0; i < NUMBER_PARTICLES; i++) {
		_pParticles[i].setParticle("flare.png", inlayer);
		_FreeList.push_front(&_pParticles[i]);
	}
}


//�U���O�쥻��dostep
/*void CParticleSystem::doStep(float dt)
{
	CParticle *get;

	list <CParticle *>::iterator it;	
	if (_bEmitterOn) { 
		// �ھ� Emitter �]�w�������ѼơA���ͬ۹��������l
		// ���p��b�֥[
		int n = (int)(_fElpasedTime * _iNumParticles); // ��ثe�������Ӳ��ͪ����l�Ӽ�
		if (n > _iGenParticles) {  // ���ͪ����l�ӼƤ����A���ͨ� n �Ӥ��l
			for (int i = 0; i < n - _iGenParticles; i++) {	
				// �ھ� Emitter �������ѼơA�]�w�Ҳ��ͤ��l���Ѽ�
				if (_iFree != 0) {
					get = _FreeList.front();
					get->setBehavior(EMITTER_DEFAULT);
					get->setVelocity(_fVelocity);
					get->setLifetime(_fLifeTime);
					get->setOpacity(_fOpacity);
					get->setGravity(_fGravity);
					get->setPosition(_emitterPt);
					get->setSize(0.125f);
					get->setSpin(_fSpin);
					get->_color = _color;
					// �ھ� _fSpread �P _vDir ���ͤ�V
					float t = (rand() % 1001) / 1000.0f; // ���ͤ��� 0 �� 1 ������
					t = _fSpread - t * _fSpread * 2;
					//  ���ͪ����סA�ন����
					t = (_fDir + t)* M_PI / 180.0f;
					Vec2 vdir(cosf(t), sinf(t));
					get->setDirection(vdir);
					_FreeList.pop_front();
					_InUsedList.push_front(get);
					_iFree--; _iInUsed++;
				}
			}
			_iGenParticles = n; // �ثe�w�g���� n �Ӥ��l		
		}
		if (_fElpasedTime >= 1.0f) {
			_fElpasedTime -= 1.0f;
			if (_iGenParticles >= _iNumParticles) _iGenParticles -= _iNumParticles;
			else _iGenParticles = 0;
		}
		_fElpasedTime += dt;
	}
	if (_iInUsed != 0) { // �����l�ݭn��s��
		for (it = _InUsedList.begin(); it != _InUsedList.end(); ) {
			if ((*it)->doStep(dt))
			{ // ���l�ͩR�g���w�g��F
				 // �N�ثe�o�@�Ӹ`�I�����e��^ _FreeList
				_FreeList.push_front((*it));
				it = _InUsedList.erase(it); // �����ثe�o�@��, 
				_iFree++; _iInUsed--;
			}
			else it++;
		}
	}
	
}
*/

//dostep��G
void CParticleSystem::doStep(float dt)
{
	

	CParticle *get;
	list <CParticle *>::iterator it;
	if (_bEmitterOn) {
		// �ھ� Emitter �]�w�������ѼơA���ͬ۹��������l
		// ���p��b�֥[
		int n = (int)(_fElpasedTime * _iNumParticles); // ��ثe�������Ӳ��ͪ����l�Ӽ�
		if (n > _iGenParticles) {  // ���ͪ����l�ӼƤ����A���ͨ� n �Ӥ��l
			for (int i = 0; i < n - _iGenParticles; i++) {
				
				//switch �_�l
				switch (_iEType)
				{

				case EMITTER_DEFAULT:
					if (_iFree != 0) {
						get = _FreeList.front();
						get->setBehavior(EMITTER_DEFAULT);
						get->setVelocity(_fVelocity);
						get->setLifetime(_fLifeTime);
						get->setOpacity(_fOpacity);
						get->setGravity(_fGravity);
						get->setPosition(_emitterPt);
						get->setSize(0.125f);
						get->setSpin(_fSpin);
						get->_color = _color;
						// �ھ� _fSpread �P _vDir ���ͤ�V
						float t = (rand() % 1001) / 1000.0f; // ���ͤ��� 0 �� 1 ������
						t = _fSpread - t * _fSpread * 2;
						//  ���ͪ����סA�ন����
						t = (_fDir + t)* M_PI / 180.0f;
						Vec2 vdir(cosf(t), sinf(t));
						get->setDirection(vdir);
						_FreeList.pop_front();
						_InUsedList.push_front(get);
						_iFree--; _iInUsed++;
					}
					break;

				case EMITTER_FIREWORKS:
					if (_iFree != 0) {
						get = _FreeList.front();
						get->setBehavior(EMITTER_FIREWORKS);
						get->setVelocity(_fVelocity);
						get->setLifetime(_fLifeTime);
						get->setOpacity(_fOpacity);
						get->setGravity(_fGravity);
						get->setPosition(_emitterPt);
						get->setSize(0.125f);
						get->setSpin(_fSpin);
						//get->_color = _color;
						// �ھ� _fSpread �P _vDir ���ͤ�V
						float t = (rand() % 1001) / 1000.0f; // ���ͤ��� 0 �� 1 ������
						t = _fSpread - t * _fSpread * 2;
						//  ���ͪ����סA�ন����
						t = (_fDir + t)* M_PI / 180.0f;
						Vec2 vdir(cosf(t), sinf(t));
						get->setDirection(vdir);
						_FreeList.pop_front();
						_InUsedList.push_front(get);
						_iFree--; _iInUsed++;
					}
					break;

				case EMITTER_TRACK:
					if (_iFree != 0) {
						get = _FreeList.front();
						get->setBehavior(EMITTER_DEFAULT);
						get->setVelocity(_fVelocity);
						get->setLifetime(_fLifeTime);
						get->setOpacity(_fOpacity);
						get->setGravity(_fGravity);
						get->setPosition(_emitterPt);
						get->setSize(0.125f);
						get->setSpin(_fSpin);
						get->_color = _color;
						// �ھ� _fSpread �P _vDir ���ͤ�V
						float t = (rand() % 1001) / 1000.0f; // ���ͤ��� 0 �� 1 ������
						t = _fSpread - t * _fSpread * 2;
						//  ���ͪ����סA�ন����
						t = (_fDir + t)* M_PI / 180.0f;
						Vec2 vdir(cosf(t), sinf(t));
						get->setDirection(vdir);
						_FreeList.pop_front();
						_InUsedList.push_front(get);
						_iFree--; _iInUsed++;
					}
					break;

				case EMITTER_DISCO:
					if (_iFree != 0) {
						get = _FreeList.front();
						get->setBehavior(EMITTER_DEFAULT);
						get->setVelocity(_fVelocity);
						get->setLifetime(_fLifeTime);
						get->setOpacity(_fOpacity);
						get->setGravity(_fGravity);
						get->setPosition(_emitterPt);
						get->setSize(0.125f);
						get->setSpin(_fSpin);
						get->_color = _color;
						// �ھ� _fSpread �P _vDir ���ͤ�V
						float t = (rand() % 1001) / 1000.0f; // ���ͤ��� 0 �� 1 ������
						t = _fSpread - t * _fSpread * 2;
						//  ���ͪ����סA�ন����
						t = (_fDir + t)* M_PI / 180.0f;
						Vec2 vdir(cosf(t), sinf(t));
						get->setDirection(vdir);
						_FreeList.pop_front();
						_InUsedList.push_front(get);
						_iFree--; _iInUsed++;
					}
					break;

				case EMMITER_SONAR:
					if (_iFree != 0) {
						get = _FreeList.front();
						get->setBehavior(EMITTER_DEFAULT);
						get->setVelocity(_fVelocity);
						get->setLifetime(_fLifeTime);
						get->setOpacity(_fOpacity);
						get->setGravity(_fGravity);
						get->setPosition(_emitterPt);
						get->setSize(0.125f);
						get->setSpin(_fSpin);
						get->_color = _color;
						// �ھ� _fSpread �P _vDir ���ͤ�V
						float t = (rand() % 1001) / 1000.0f; // ���ͤ��� 0 �� 1 ������
						t = _fSpread - t * _fSpread * 2;
						//  ���ͪ����סA�ন����
						t = (_fDir + t)* M_PI / 180.0f;
						Vec2 vdir(cosf(t), sinf(t));
						get->setDirection(vdir);
						_FreeList.pop_front();
						_InUsedList.push_front(get);
						_iFree--; _iInUsed++;
					}
					break;
				}
				//switch �פF

			}
			_iGenParticles = n; // �ثe�w�g���� n �Ӥ��l		
		}
		if (_fElpasedTime >= 1.0f) {
			_fElpasedTime -= 1.0f;
			if (_iGenParticles >= _iNumParticles) _iGenParticles -= _iNumParticles;
			else _iGenParticles = 0;
		}
		_fElpasedTime += dt;
	}
	if (_iInUsed != 0) { // �����l�ݭn��s��
		for (it = _InUsedList.begin(); it != _InUsedList.end(); ) {
			if ((*it)->doStep(dt))
			{ // ���l�ͩR�g���w�g��F
			  // �N�ثe�o�@�Ӹ`�I�����e��^ _FreeList
				_FreeList.push_front((*it));
				it = _InUsedList.erase(it); // �����ثe�o�@��, 
				_iFree++; _iInUsed--;
			}
			else it++;
		}
	}

}

void CParticleSystem::setGravity(float fGravity)
{
	// �]�w�ثe particle �O�b inused �� gravity
	_fGravity = fGravity;
	list <CParticle *>::iterator it;
	if (_iInUsed != 0) { // �����l�ݭn��s��
		for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {
			(*it)->setGravity(_fGravity);
		}
	}
}

void CParticleSystem::setSpin(float fSpin)
{
	_fSpin = fSpin;
}

void CParticleSystem::setOpacity(float fOpacity)
{
	_fOpacity = fOpacity;
	list <CParticle *>::iterator it;
	if (_iInUsed != 0) { // �����l�ݭn��s��
		for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {
			(*it)->setOpacity(_fOpacity);
		}
	}
}

void CParticleSystem::setSpeed(float fSpeed) {

	_fVelocity = fSpeed;

}

void CParticleSystem::setRed(float fRed)
{
	_color.r = fRed;
}

void CParticleSystem::setGreen(float fGreen)
{
	_color.g = fGreen;
}

void CParticleSystem::setBlue(float fBlue)
{
	_color.b = fBlue;
}

void CParticleSystem::setWind(float fWind)
{
	_fWind = fWind;
	list <CParticle *>::iterator it;
	if (_iInUsed != 0) { // �����l�ݭn��s��
		for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {
			(*it)->setWind(_fWind);
		}
	}
}

void CParticleSystem::setNewPic(const char *pngName, cocos2d::Layer &inlayer)
{
	const char * _pngName = pngName;
	list <CParticle *>::iterator it;
	if (_iInUsed !=0 )
	{ // �����l�ݭn��s��
		for (it = _InUsedList.begin(); it != _InUsedList.end();it++ ) {
			(*it)->pic(pngName);
		}
		for (it = _FreeList.begin(); it != _FreeList.end(); it++) {
			(*it)->pic(pngName);
		}
	}
}


CParticleSystem::~CParticleSystem()
{
	if (_iInUsed != 0) _InUsedList.clear(); // �����Ҧ��� NODE
	if (_iFree != 0) _FreeList.clear();
	delete[] _pParticles; // ����Ҧ����o�귽
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("particletexture.plist");

	SimpleAudioEngine::getInstance()->unloadEffect("CRAM_MOVE.mp3");
}

void CParticleSystem::onTouchesBegan(const cocos2d::CCPoint &touchPoint)
{
	CParticle *get;
	switch (_iType)
	{
	case STAY_FOR_TWOSECONDS:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(STAY_FOR_TWOSECONDS);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case RANDOMS_FALLING :
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(RANDOMS_FALLING);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case FREE_FLY:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(FREE_FLY);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case EXPLOSION:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
		if (_iFree > 100) {
			for (int i = 0; i < 100; i++) {
				get = _FreeList.front();
				get->setBehavior(EXPLOSION);
				get->setPosition(touchPoint);
				get->setGravity(_fGravity);
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case HEARTSHAPE:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
		if (_iFree > 150) {
			for (int i = 0; i < 150; i++) {
				get = _FreeList.front();
				get->setBehavior(HEARTSHAPE);
				get->setPosition(touchPoint);
				get->setGravity(_fGravity);
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case BUTTERFLYSHAPE:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
		if (_iFree > 150) {
			for (int i = 0; i < 150; i++) {
				get = _FreeList.front();
				get->setBehavior(BUTTERFLYSHAPE);
				get->setPosition(touchPoint);
				get->setGravity(_fGravity);
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case CRAM:
		eid = SimpleAudioEngine::getInstance()->playEffect("CRAM_MOVE.mp3", true,0.0f,0.0f,0.0f);  // ���񭵮���
		//SimpleAudioEngine::getInstance()->playBackgroundMusic("CRAM_BG.mp3", false);
		break;
	}
}

void CParticleSystem::onTouchesMoved(const cocos2d::CCPoint &touchPoint)
{
	CParticle *get;
	switch (_iType)
	{
	case STAY_FOR_TWOSECONDS:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(STAY_FOR_TWOSECONDS);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case RANDOMS_FALLING:
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(RANDOMS_FALLING);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case FREE_FLY:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(FREE_FLY);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case CRAM:
		//SimpleAudioEngine::getInstance()->stopEffect(eid);
		//eid = SimpleAudioEngine::getInstance()->playEffect("CRAM_MOVE.mp3", false);  // ���񭵮���
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
		if (_iFree != 0) {

			

			get = _FreeList.front();
			get->setBehavior(CRAM);
			get->setPosition(Vec2(950, 100));
			get->setDirection((touchPoint- Vec2(950, 100))/100);
			get->setGravity(_fGravity);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;

			get = _FreeList.front();
			get->setBehavior(CRAM);
			get->setPosition(Vec2(950, 100));
			get->setColor(cocos2d::Color3B(0,255,0));
			get->setOpacity(_fOpacity);
			//get->setDirection((touchPoint - Vec2(950, 300)) / 90);
			get->setDirection((touchPoint - Vec2(950, 100)) / 25);
			get->setGravity(_fGravity);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;

			
		}
		else return;// �S�����l, �ҥH�N������
		break;

	case APIT:
		//SimpleAudioEngine::getInstance()->stopEffect(eid);
		//eid = SimpleAudioEngine::getInstance()->playEffect("CRAM_MOVE.mp3", false);  // ���񭵮���
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
		if (_iFree != 0) {



			get = _FreeList.front();
			get->setBehavior(APIT);
			get->setPosition(Vec2(950, 100));
			get->setDirection((touchPoint - Vec2(950, 100)) / 100);
			get->setGravity(_fGravity);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;

			get = _FreeList.front();
			get->setBehavior(APIT);
			get->setPosition(Vec2(950, 600));
			get->setColor(cocos2d::Color3B(0, 255, 0));
			get->setOpacity(_fOpacity);
			get->setDirection((touchPoint - Vec2(950, 600)) / 90);
			//get->setDirection((touchPoint - Vec2(950, 100)) / 25);
			get->setGravity(_fGravity);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;


		}
		else return;// �S�����l, �ҥH�N������
		break;
	}
}
void CParticleSystem::onTouchesEnd(const cocos2d::CCPoint &touchPoint) {
	switch (_iType) {
	case CRAM:
		SimpleAudioEngine::getInstance()->stopEffect(eid);
		break;
	}
}