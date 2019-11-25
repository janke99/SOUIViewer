/********************************************************************
created:	2014/10/01
created:	01:10:2014   16:32
filename: 	SSkinPNGX.h
file base:	SSkinPNGX
file ext:	h
author:		huangjianxiong
*********************************************************************/
#pragma once
#include <interface/SSkinobj-i.h>
#include <unknown/obj-ref-impl.hpp>

#include "SSkinAni.h"

namespace SOUI
{

    /**
    * @class     SSkinAPNG
    * @brief     APNGͼƬ���ؼ���ʾ����
    * 
    * Describe
    */
    class SSkinPNGX : public SSkinAni
    {
        SOUI_CLASS_NAME(SSkinPNGX, L"pngx")
    public:
		SSkinPNGX():m_bVert(FALSE)
        {

        }

        SOUI_ATTRS_BEGIN()
            ATTR_IMAGEAUTOREF(L"src",m_pngx,FALSE)   //XML�ļ���ָ����ͼƬ��Դ��,(type:name)
			ATTR_CUSTOM(L"delay",OnAttrDelay) //delay, format:10[5],20,30 '[5]' is optional repeat count. 
			ATTR_BOOL(L"vert",m_bVert,FALSE)
        SOUI_ATTRS_END()
	protected:
		virtual int GetStates() const  override;

        virtual SIZE GetSkinSize() const override;

        /**
        * GetFrameDelay
        * @brief    ���ָ��֡����ʾʱ��
        * @param    int iFrame --  ֡��,Ϊ-1ʱ�����õ�ǰ֡����ʱ
        * @return   long -- ��ʱʱ��(*10ms)
        * Describe  
        */    
        virtual long GetFrameDelay(int iFrame=-1) const;
        
        /**
        * Draw
        * @brief    ����ָ��֡��GIFͼ
        * @param    IRenderTarget * pRT --  ����Ŀ��
        * @param    LPCRECT rcDraw --  ���Ʒ�Χ
        * @param    DWORD dwState --  ����״̬�����ﱻ����Ϊ֡��
        * @param    BYTE byAlpha --  ͸����
        * @return   void
        * Describe  
        */    
        virtual void _DrawByIndex2(IRenderTarget *pRT, LPCRECT rcDraw, int dwState,BYTE byAlpha=0xFF) const override;
    protected:
		HRESULT OnAttrDelay(const SStringW &strValue,BOOL bLoading);
 
		CAutoRefPtr<IBitmap> m_pngx;
		SArray<int>			 m_nDelays;
		BOOL				 m_bVert;
    };
}//end of name space SOUI
