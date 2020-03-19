#include "stdafx.h"
#include "XmlData.h"
#include "Markup.h"
#include "XhCharString.h"

//////////////////////////////////////////////////////////////////////////
//CHangPhase
CHangPhase::CHangPhase()
{
	m_dwHangPhase=1;			//�����
	m_xBackHangPtArr.Empty();	//���ҵ�
	m_xFrontHangPtArr.Empty();	//ǰ��ҵ�
	m_xWireJHangPtArr.Empty();	//���߹ҵ�
}
BOOL CHangPhase::GetBKVirtualHangPt(HANG_POINT& hang_pt_v)
{
	int nSize=m_xBackHangPtArr.GetSize();
	if(nSize<=0)
		return FALSE;
	f3dPoint pos;
	for(int i=0;i<nSize;i++)
		pos+=m_xBackHangPtArr[i].m_xHangPos;
	pos/=nSize;
	hang_pt_v.m_xHangPos=pos;
	strcpy(hang_pt_v.m_sHangName,"����ҵ�");
	strcpy(hang_pt_v.m_sHangOrder,"1");
	hang_pt_v.m_ciWireType=m_xBackHangPtArr[0].m_ciWireType;
	hang_pt_v.m_ciTensionType=m_xBackHangPtArr[0].m_ciTensionType;
	hang_pt_v.m_ciHangingStyle=m_xBackHangPtArr[0].m_ciHangingStyle;
	hang_pt_v.m_ciHangDirect=m_xBackHangPtArr[0].m_ciHangDirect;
	return TRUE;
}
BOOL CHangPhase::GetFTVirtualHangPt(HANG_POINT& hang_pt_v)
{
	int nSize=m_xFrontHangPtArr.GetSize();
	if(nSize<=0)
		return FALSE;
	f3dPoint pos;
	for(int i=0;i<nSize;i++)
		pos+=m_xFrontHangPtArr[i].m_xHangPos;
	pos/=nSize;
	hang_pt_v.m_xHangPos=pos;
	strcpy(hang_pt_v.m_sHangName,"����ҵ�");
	if(m_xWireJHangPtArr.GetSize()<=0)
		strcpy(hang_pt_v.m_sHangOrder,"2");
	else
		strcpy(hang_pt_v.m_sHangOrder,"3");
	hang_pt_v.m_ciWireType=m_xFrontHangPtArr[0].m_ciWireType;
	hang_pt_v.m_ciTensionType=m_xFrontHangPtArr[0].m_ciTensionType;
	hang_pt_v.m_ciHangingStyle=m_xFrontHangPtArr[0].m_ciHangingStyle;
	hang_pt_v.m_ciHangDirect=m_xFrontHangPtArr[0].m_ciHangDirect;
	return TRUE;
}
BOOL CHangPhase::GetWJVirtualHangPt(HANG_POINT& hang_pt_v)
{
	int nSize=m_xWireJHangPtArr.GetSize();
	if(nSize<=0)
		return FALSE;
	f3dPoint pos;
	for(int i=0;i<nSize;i++)
		pos+=m_xWireJHangPtArr[i].m_xHangPos;	
	pos/=nSize;
	hang_pt_v.m_xHangPos=pos;
	strcpy(hang_pt_v.m_sHangName,"����ҵ�");
	strcpy(hang_pt_v.m_sHangOrder,"2");
	hang_pt_v.m_ciWireType=m_xWireJHangPtArr[0].m_ciWireType;
	hang_pt_v.m_ciTensionType=m_xWireJHangPtArr[0].m_ciTensionType;
	hang_pt_v.m_ciHangingStyle=m_xWireJHangPtArr[0].m_ciHangingStyle;
	hang_pt_v.m_ciHangDirect=m_xWireJHangPtArr[0].m_ciHangingStyle;
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//CXmlModel
CXmlModel::CXmlModel(BOOL bUseUnitM/*=FALSE*/)
{
	m_bTransPtMMtoM=bUseUnitM;
	m_ciModel=0;
	strcpy(m_sCategory,"����");
	strcpy(m_sTowerFile,"");
	strcpy(m_sTowerName,"");
	strcpy(m_xFundation[0].m_sFeetName,"A");
	strcpy(m_xFundation[1].m_sFeetName,"B");
	strcpy(m_xFundation[2].m_sFeetName,"C");
	strcpy(m_xFundation[3].m_sFeetName,"D");
	m_hashTowerLoop.Empty();
}
CXmlModel::~CXmlModel()
{

}
void CXmlModel::AmendHangInfo()
{
	//���߱��
	for(CTowerLoop* pLoop=m_hashTowerLoop.GetFirst();pLoop;pLoop=m_hashTowerLoop.GetNext())
	{
		for(CHangPhase* pPhase=pLoop->m_hashHangPhase.GetFirst();pPhase;pPhase=pLoop->m_hashHangPhase.GetNext())
		{
			int iWireCode=1,iJumpCode=1,iOrder=1,index=1;
			for(HANG_POINT* pHangPt=pPhase->m_xBackHangPtArr.GetFirst();pHangPt;pHangPt=pPhase->m_xBackHangPtArr.GetNext(),iOrder++)
			{
				if(pHangPt->m_ciTensionType==0)
					sprintf(pHangPt->m_sHangName,"P%dX%d",pPhase->m_dwHangPhase,iWireCode);
				else
					sprintf(pHangPt->m_sHangName,"P%dN%d",pPhase->m_dwHangPhase,iWireCode);
				sprintf(pHangPt->m_sHangOrder,"1-%d",iOrder);
				iWireCode++;
			}
			for(HANG_POINT* pHangPt=pPhase->m_xWireJHangPtArr.GetFirst();pHangPt;pHangPt=pPhase->m_xWireJHangPtArr.GetNext(),iOrder++)
			{
				sprintf(pHangPt->m_sHangName,"P%dT%d",pPhase->m_dwHangPhase,iJumpCode);
				sprintf(pHangPt->m_sHangOrder,"2-%d",iOrder);
				iJumpCode++;
			}
			int ii=(pPhase->m_xWireJHangPtArr.GetSize()>0)?3:2;
			for(HANG_POINT* pHangPt=pPhase->m_xFrontHangPtArr.GetFirst();pHangPt;pHangPt=pPhase->m_xFrontHangPtArr.GetNext(),iOrder++)
			{
				if(pHangPt->m_ciTensionType==0)
					sprintf(pHangPt->m_sHangName,"P%dX%d",pPhase->m_dwHangPhase,iWireCode);
				else
					sprintf(pHangPt->m_sHangName,"P%dN%d",pPhase->m_dwHangPhase,iWireCode);
				sprintf(pHangPt->m_sHangOrder,"%d-%d",ii,iOrder);
				iWireCode++;
			}
		}
		if(pLoop->m_hashHangPhase.GetNodeNum()==3)
			pLoop->m_sCurrent.Copy("����");
		else //if(pLoop->m_hashHangPhase.GetNodeNum()==2)
			pLoop->m_sCurrent.Copy("ֱ��");
	}
	//���߱��
	for(CEarthLoop* pLoop=m_hashEarthLoop.GetFirst();pLoop;pLoop=m_hashEarthLoop.GetNext())
	{
		int iCode=1;
		for(HANG_POINT* pHangPt=pLoop->m_xHangPtList.GetFirst();pHangPt;pHangPt=pLoop->m_xHangPtList.GetNext())
		{
			if(pHangPt->m_ciTensionType==0)
				sprintf(pHangPt->m_sHangName,"G%dX%d",pLoop->m_dwLoop,iCode);
			else
				sprintf(pHangPt->m_sHangName,"G%dN%d",pLoop->m_dwLoop,iCode);
			sprintf(pHangPt->m_sHangOrder,"%d",iCode);
			iCode++;
		}
	}
}
//����������������ԣ�0.����|1.����
BYTE CXmlModel::get_ciHangingType()
{
	CTowerLoop* pLoop=m_hashTowerLoop.GetFirst();
	if(pLoop==NULL)
		return 0xff;
	CHangPhase* pPhase=pLoop->m_hashHangPhase.GetFirst();
	if(pPhase==NULL)
		return 0xff;
	//Ŀǰ�߼���������������ҵ����m_xBackHangPtArr�У���m_xFrontHangPtArrΪ�գ���ֻ����ݺ��ҵ㼴��ͳ�Ƴ������������� wjh-2019.4.25
	HANG_POINT* pWirePoint;
	UINT uiXuanChuiCount=0,uiNaiZhangCount=0;
	for(pWirePoint=pPhase->m_xBackHangPtArr.GetFirst();pWirePoint;pWirePoint=pPhase->m_xBackHangPtArr.GetNext())
	{
		if(pWirePoint->m_ciTensionType==0)
			uiXuanChuiCount++;
		else if(pWirePoint->m_ciTensionType==1)
			uiNaiZhangCount++;
	}
	if(uiXuanChuiCount>uiNaiZhangCount)
		return 0;	//�����ж�Ϊ������
	else if(uiXuanChuiCount<uiNaiZhangCount)
		return 1;	//�����ж�Ϊ������
	else
		return 0xff;//���ݳ���
}
void CXmlModel::CreateOldXmlFile(const char* sFilePath)
{
	CMarkup xml;
	//xml.SetDoc("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	xml.AddElem("Xml");
	xml.AddAttrib("Category",(char*)m_sCategory);
	xml.AddAttrib("Model",m_ciModel);
	xml.AddAttrib("File",(char*)m_sTowerFile);
	if(this->ciHangingType==0)
		xml.AddAttrib("Type","��������");
	else
		xml.AddAttrib("Type","���ų���");
	xml.AddAttrib("Name",(char*)m_sTowerName);
	xml.AddAttrib("Loop",m_hashTowerLoop.GetNodeNum());
	//����LoopRoot��Ϣ�ڼ�
	xml.IntoElem();
	xml.AddElem("TowerLoopRoot");
	for(CTowerLoop* pLoop=m_hashTowerLoop.GetFirst();pLoop;pLoop=m_hashTowerLoop.GetNext())
		AddWireLoop(xml,pLoop);
	for(CEarthLoop* pLoop=m_hashEarthLoop.GetFirst();pLoop;pLoop=m_hashEarthLoop.GetNext())
		AddEarthLoop(xml,pLoop);
	xml.OutOfElem();
	//����PropertyRoot��Ϣ�ڼ�
	xml.IntoElem();
	xml.AddElem("PropertyRoot");
		xml.IntoElem();
		xml.AddElem("Property");
		xml.AddAttrib("PropertyName",(char*)m_sPropName);
		xml.AddAttrib("PropertyValue",(char*)m_sPropValue);
		xml.OutOfElem();
	xml.OutOfElem();
	//����FundationRoot��Ϣ�ڼ�
	xml.IntoElem();
	xml.AddElem("FundationRoot");
	for(int i=0;i<4;i++)
	{
		GEPOINT pos=m_xFundation[i].m_xFeetPos;
		if(m_bTransPtMMtoM)
			pos*=0.001;
		xml.IntoElem();
		xml.AddElem("FeetPoint");
		xml.AddAttrib("FeetName",m_xFundation[i].m_sFeetName);
		xml.AddAttrib("Position",(char*)CXhChar100("(%.6f,%.6f,%.6f)",pos.x,pos.y,pos.z));
		xml.OutOfElem();
	}
	xml.OutOfElem();
	//�������ʽANSIת��ΪUNICODE
	CString strvalue=xml.GetDoc();
	int nLen=MultiByteToWideChar(CP_ACP,0,strvalue.GetBuffer(),-1,NULL,0 );
	wchar_t* sResult=(wchar_t*)malloc((nLen+1)*sizeof(wchar_t));
	memset(sResult,0,sizeof(wchar_t)*(nLen+1));
	MultiByteToWideChar(CP_ACP,0,strvalue.GetBuffer(),-1,(LPWSTR)sResult,nLen);
	//
	WORD wSignature = 0xFEFF;
	FILE* fp=fopen(sFilePath,"wb");
	if(fp)
	{
		fwrite(&wSignature,2,1,fp);	//UNICODE�����ͷ
		fwrite(sResult,(nLen+1)*sizeof(wchar_t),1,fp);
		fclose(fp);
	}
	free(sResult);
}
void CXmlModel::AddEarthLoop(CMarkup& xml,CEarthLoop* pEarthLoop)
{
	xml.IntoElem();
	xml.AddElem("TowerLoop");
	xml.AddAttrib("LoopTag",(char*)CXhChar50("����%d",pEarthLoop->m_dwLoop));
	for(HANG_POINT* pHangPt=pEarthLoop->m_xHangPtList.GetFirst();pHangPt;pHangPt=pEarthLoop->m_xHangPtList.GetNext())
		AddHangPoint(xml,pHangPt);
	xml.OutOfElem();
}
void CXmlModel::AddWireLoop(CMarkup& xml,CTowerLoop* pWireLoop)
{
	xml.IntoElem();
	xml.AddElem("TowerLoop");
	xml.AddAttrib("Current",(char*)pWireLoop->m_sCurrent);
	xml.AddAttrib("LoopTag",(char*)CXhChar50("��·%d",pWireLoop->m_dwLoop));
	xml.AddAttrib("LoopCode","00");
	for(CHangPhase* pHangPhase=pWireLoop->m_hashHangPhase.GetFirst();pHangPhase;pHangPhase=pWireLoop->m_hashHangPhase.GetNext())
	{
		xml.IntoElem();
		xml.AddElem("Phase");
		xml.AddAttrib("HangPhase",(char*)CXhChar50("��%d",pHangPhase->m_dwHangPhase));
		HANG_POINT xHangNode;
		if(pHangPhase->GetBKVirtualHangPt(xHangNode))
			AddHangPoint(xml,&xHangNode,&(pHangPhase->m_xBackHangPtArr));
		if(pHangPhase->GetWJVirtualHangPt(xHangNode))
			AddHangPoint(xml,&xHangNode,&(pHangPhase->m_xWireJHangPtArr));
		if(pHangPhase->GetFTVirtualHangPt(xHangNode))
			AddHangPoint(xml,&xHangNode,&(pHangPhase->m_xFrontHangPtArr));
		xml.OutOfElem();
	}
	xml.OutOfElem();
}
void CXmlModel::AddHangPoint(CMarkup& xml,HANG_POINT* pHangPt,ARRAY_LIST<HANG_POINT>* pHangPtArr/*=NULL*/)
{
	GEPOINT pos=pHangPt->m_xHangPos;
	if(m_bTransPtMMtoM)
		pos*=0.001;
	xml.IntoElem();
	xml.AddElem("TowerHangPoint");
	xml.AddAttrib("HangName",(char*)pHangPt->m_sHangName);
	if(pHangPt->m_ciWireType=='J')
		xml.AddAttrib("HangType","����");
	else if(pHangPt->m_ciTensionType==0)
		xml.AddAttrib("HangType","����");
	else
		xml.AddAttrib("HangType","����");
	if(pHangPt->m_ciHangingStyle=='S'||pHangPt->m_ciHangingStyle=='V')
		xml.AddAttrib("HangPresen","��ҵ�");
	else
		xml.AddAttrib("HangPresen","���ҵ�");
	if(pHangPt->m_ciHangDirect=='Q')
		xml.AddAttrib("HangDirect","ǰ��");
	else if(pHangPt->m_ciHangDirect=='H')
		xml.AddAttrib("HangDirect","����");
	else
		xml.AddAttrib("HangDirect","");
	xml.AddAttrib("HangOrder",pHangPt->m_sHangOrder);
	xml.AddAttrib("Position",(char*)CXhChar100("(%.6f,%.6f,%.6f)",pos.x,pos.y,pos.z));
	if(pHangPtArr)
	{
		for(int i=0;i<pHangPtArr->GetSize();i++)
			AddHangPoint(xml,pHangPtArr->GetByIndex(i));
	}
	xml.OutOfElem();
}