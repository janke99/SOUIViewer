/********************************************************************
created:	2014/10/01
created:	01:10:2014   16:32
filename: 	SSkinAPNG.h
file base:	SSkinAPNG
file ext:	h
author:		huangjianxiong

purpose:	����PNG��APNG����չ֧�֣������ڵ�ǰʹ�õ�ͼƬ��������
            Ŀǰֻ��imgdecoder-png֧��APNG����
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
    class SSkinAPNG : public SSkinAni
    {
        SOUI_CLASS_NAME(SSkinAPNG, L"apng")
    public:
		SSkinAPNG():m_pFrames(NULL)
        {

        }
		~SSkinAPNG()
		{
			if(m_pFrames) delete [] m_pFrames;
		}
        
        /**
         * LoadFromFile
         * @brief    ���ļ�����APNG
         * @param    LPCTSTR pszFileName --  �ļ���
         * @return   int -- APNG֡����0-ʧ��
         * Describe  
         */    
        int LoadFromFile(LPCTSTR pszFileName);

        /**
         * LoadFromMemory
         * @brief    ���ڴ����APNG
         * @param    LPVOID pBits --  �ڴ��ַ
         * @param    size_t szData --  �ڴ����ݳ���
         * @return   int -- APNG֡����0-ʧ��
         * Describe  
         */    
        int LoadFromMemory(LPVOID pBits,size_t szData);

        SOUI_ATTRS_BEGIN()
            ATTR_CUSTOM(L"src",OnAttrSrc)   //XML�ļ���ָ����ͼƬ��Դ��,(type:name)
        SOUI_ATTRS_END()

	protected:
		
        /**
        * GetStates
        * @brief    ���GIF֡��
        * @return   int -- ֡��
        * Describe  
        */    
        virtual int GetStates() const override{return m_nFrames;}

        /**
        * GetSkinSize
        * @brief    ���ͼƬ��С
        * @return   SIZE -- ͼƬ��С
        * Describe  
        */    
        virtual SIZE GetSkinSize() const override;

        /**
        * GetFrameDelay
        * @brief    ���ָ��֡����ʾʱ��
        * @param    int iFrame --  ֡��,Ϊ-1ʱ�����õ�ǰ֡����ʱ
        * @return   long -- ��ʱʱ��(*10ms)
        * Describe  
        */    
        virtual long GetFrameDelay(int iFrame=-1) const override;
        
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
        virtual void _DrawByIndex2(IRenderTarget *pRT, LPCRECT rcDraw, int dwState,BYTE byAlpha=0xFF) const  override;


		IBitmap * GetFrameImage(int iFrame=-1);

    protected:
        HRESULT OnAttrSrc(const SStringW &strValue,BOOL bLoading);
        
        int _InitImgFrame(IImgX *pImgX);

	protected:
		SAniFrame * m_pFrames;
    };
}//end of name space SOUI
