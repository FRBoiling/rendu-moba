/*
* file name			:SSMoveMgr.h
* file mark			:
* summary			:
*
* version			:1.0
* author			:LiuLu
* complete date		:August 7 2014
* summary			:
*
*/

#ifndef __SSMoveMgr_h__
#define __SSMoveMgr_h__

#include "stdafx.h"
#include <hash_map>
#include <set>
#include <hash_set>
#include "SSIntersectObject.h"
#include "SSAStar.h"

namespace SceneServer{


const UINT8 c_maxISSMoveObjectNum = 128;
const UINT16	c_maxDynamicBlockNum = 256;

class ISSMoveObject;

//ֹͣ�ƶ�ʹ�õ�ֹͣ���ͣ�ֻ��ָֹͣ�����͵��ƶ�,�����ƶ������л�ʱ���µĴ���ֹͣ��
enum EnumAskStopMoveType{
	EnumAskStopMoveType_ALL = 0,
	EnumAskStopMoveType_DIR,
	EnumAskStopMoveType_TAR,
	EnumAskStopMoveType_FORCEMOVE,
};

enum ETriDir{
	ETriDir_LeftUp = 0,
	ETriDir_RightUp,
	ETriDir_RightBelow,
	ETriDir_LeftBelow,
};

struct SIntTri{
	UINT16 x;
	UINT16 y;
	ETriDir eDir;
	SIntTri(UINT16 x,UINT16 y,ETriDir dir): x(x), y(y),eDir(dir){}
	SIntTri(){}
};

class CSSMoveMgr
{
private:
	CSSAStar								m_Astar;//A��Ѱ·��
	hash_set<ISSMoveObject*>				m_mObjectMap[6];//��ײ�����������keyΪ��ID
	hash_set<ISSMoveObject*>				m_allMObjectSet;//������ײ����
	ISSMoveObject*							m_heartBeatTempArray[1024];//����ʱ����ʱ����
	bool*									m_staticBlockArray;//��̬��ײ����Ϣ
	map<UINT64,SIntTri>						m_staticTriBlockMap;//�����ξ�̬��ײmap
	hash_map<UINT64,SIntPoint*>				m_MapCacheMap;//�����Ѱ·�ڵ��������������޾�̬�赲��Ѱ·����

	UINT16								m_regionWidth;//��ͼ��ȣ���λ����
	UINT16								m_regionHeight;//��ͼ�߶ȣ���λ����
	UINT16								m_regionSize;//���ӳߴ磬������

	UINT16								GetRegionIndexX(float x){UINT16 uiX = x/m_regionSize; if(uiX < 0) uiX = 0;if(uiX > m_regionWidth) uiX = m_regionWidth; return uiX;}//��ͨ����ת��������
	UINT16								GetRegionIndexY(float y){UINT16 uiY = y/m_regionSize; if(uiY < 0) uiY = 0;if(uiY > m_regionHeight) uiY = m_regionHeight; return uiY;}//��ͨ����ת��������
	UINT32								GetRegionID_INT(UINT16 x, UINT16 y){return x + y * m_regionWidth;}
	UINT32								GetRegionID_FLOAT(float x, float y){return GetRegionID_INT(GetRegionIndexX(x),GetRegionIndexY(y));}

	UINT64								GetPathID(UINT32 startRegionID, UINT32 targetRegionID){return ((UINT64)startRegionID) << 32 | (UINT64)targetRegionID;}//��ȡ·����key�����ڻ���·��
	SIntPoint*							GetPathByCache(UINT64 ID){auto iter = m_MapCacheMap.find(ID);return iter==m_MapCacheMap.end()?NULL:iter->second;}
	void								SavePathToCache(UINT64 ID, SIntPoint* path, const UINT16 length);
	void								DelAllPathCache();

	bool								AskStartCheck(ISSMoveObject* pMObject);//����λ��ǰ�ü��
	bool								TestNextStepMove(ISSMoveObject* pMObject, bool bIfCheckStaticBlock = TRUE, bool bIfCheckDynamicBlock = TRUE);//��ײ�����������(modify)//
	bool								IfDynamicImpact(ISSMoveObject* pMObject,bool pIfCheckNextStep);//��鶯̬��ײ
	bool								IfDynamicImpact(ISSMoveObject* pMObjectMove,ISSMoveObject* pMObjectBlock, bool pIfCheckNextStep);//��鶯̬��ײ
	//bool								IfStaticImpact(float x, float y){if(x<0||y<0||(x/100)>=m_regionWidth||(y/100)>=m_regionHeight) return TRUE; return m_staticBlockArray[GetRegionID_FLOAT(x,y)];}//��龲̬��ײ
	bool								IfStaticImpact(ISSMoveObject* pMObject);//��龲̬��ײ
	void								StopLastStep(ISSMoveObject* pMObject, bool bIfCallBack);//ֹͣ�ƶ��ж���ǰ������ʱ���ƶ������һС����
	bool								TryMove(ISSMoveObject* pMObject,TIME_MILSEC now, float &moveDist);//�����ƶ��������ƶ����롣�ƶ�ʧ�ܷ���0
	void								FindDynamicBlock(ISSMoveObject* pMObject, UINT32 *dynamicArray, UINT16 &length);//��ȡһ��������ߵĶ�̬�赲
	void								AddDnnamicBlock(ISSMoveObject* pMObject, ISSMoveObject* pTarget, UINT32 *dynamicArray, UINT16 &length, UINT16 &index);
	ColVector							GetRegionCenter(UINT16 x, UINT16 y){ColVector vec; vec.x = x*m_regionSize+ m_regionSize/2;vec.y = y*m_regionSize+ m_regionSize/2;return vec;}//��ȡ�������ĵ�����
	bool								SetNextMovePoint(ISSMoveObject* pMObject);//����һ��׼���ƶ��ĵ����м���
	void								CheckTargetMoveStatus(ISSMoveObject* pMObject, float movedLength);//����Ƿ��Ѿ��ƶ���Ŀ�����
	BOOLEAN								TryTurnDir(ISSMoveObject* pMObject);//���Ըı䷽���������ƶ�

public:
	CSSMoveMgr();
	~CSSMoveMgr();

	void								AddStaticBlockInfo(UINT16 x, UINT16 y){m_staticBlockArray[GetRegionID_INT(x,y)] = TRUE;}
	void								AddStaticTriBlockInfo(UINT16 x, UINT16 y, ETriDir dir);
	void								SetMapInfo(UINT16 width, UINT16 height, UINT16 size){m_regionWidth = width;m_regionHeight=height;m_regionSize = size; if(m_staticBlockArray == NULL) m_staticBlockArray = new bool[m_regionWidth*m_regionHeight](); m_Astar.Init(m_regionWidth,m_regionHeight,m_staticBlockArray);}

	INT32								OnHeartBeat(TIME_MILSEC tUTCMilsec, TIME_TICK tTickSpan);//����

	void								AddMoveObject(ISSMoveObject* pMObject, UINT8 ui8GrpID);//�����ײ��
	void								RemoveMoveObject(ISSMoveObject* pMObject);//�Ƴ���ײ��

	bool								GetAroundFreePos(ColVector pos, ColVector &outPos, ISSMoveObject *pGU, UINT8 ui8MoveGrpID);//Ѱ����Χһ�����԰��Ŷ���Ŀհ׿ռ�
	bool								ResetPos(ISSMoveObject *pGU, ColVector pos, bool bIfImpact);//������ȫ�ķ���һ������Χ
	bool								IfPosImpact(ISSMoveObject* pMObject, float x,float y, float r, UINT8 groupID);//���һ�����Ƿ����ײ

	bool								AskStartMoveDir(ISSMoveObject* pMObject, ColVector dir);//�����ƶ������ƶ�
	bool								AskStartMoveToTar(ISSMoveObject* pMObject, ColVector pos, bool bIfMoveToBlackPoint, bool bIfFliter);//�����ƶ�λ���ƶ�
	bool								AskStartMoveForced(ISSMoveObject* pMObject, ColVector dir, FLOAT speed, bool bIfImpact);//����ǿ���ƶ�
	bool								AskStopMoveObject(ISSMoveObject* pMObject,EnumAskStopMoveType type);//����ֹͣ�ƶ�������ö��ֵ��ͬ��ֻ��ָֹͣ�����͵��ƶ���
};

}

#endif