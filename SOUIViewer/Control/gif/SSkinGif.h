/********************************************************************
created:	2012/12/27
created:	27:12:2012   14:55
filename: 	SSkinGif.h
file base:	SSkinGif
file ext:	h
author:		huangjianxiong

purpose:	�Զ���Ƥ������
*********************************************************************/
#pragma once
#include <interface/SSkinobj-i.h>
#include <unknown/obj-ref-impl.hpp>

#include "SSkinAni.h"

namespace Gdiplus
{
class Bitmap;
}

namespace SOUI
{

    /**
    * @class     SSkinGif
    * @brief     GIFͼƬ���ؼ���ʾ����
    * 
    * Describe
    */
    class SSkinGif : public SSkinAni
    {
        SOUI_CLASS_NAME(SSkinGif, L"gif")
    public:
        SSkinGif():m_pFrames(NULL)
        {
        }

		~SSkinGif()
		{
			if (m_pFrames) delete[]m_pFrames;
		}

		/**
		* GetFrameDelay
		* @brief    ���ָ��֡����ʾʱ��
		* @param    int iFrame --  ֡��,Ϊ-1ʱ�����õ�ǰ֡����ʱ
		* @return   long -- ��ʱʱ��(*10ms)
		* Describe
		*/
		long GetFrameDelay(int iFrame = -1) const;

        //��ʼ��GDI+����������������Ҫʹ��GDI+������GIF�ļ���ʽ
        static BOOL Gdiplus_Startup();
        //�˳�GDI+����
        static void Gdiplus_Shutdown();

        /**
         * LoadFromFile
         * @brief    ���ļ�����GIF
         * @param    LPCTSTR pszFileName --  �ļ���
         * @return   int -- GIF֡����0-ʧ��
         * Describe  
         */    
        int LoadFromFile(LPCTSTR pszFileName);

        /**
         * LoadFromMemory
         * @brief    ���ڴ����GIF
         * @param    LPVOID pBits --  �ڴ��ַ
         * @param    size_t szData --  �ڴ����ݳ���
         * @return   int -- GIF֡����0-ʧ��
         * Describe  
         */    
        int LoadFromMemory(LPVOID pBits,size_t szData);

	public:
		virtual SIZE GetSkinSize() const override;
		virtual int GetStates() const  override;

	public:
        SOUI_ATTRS_BEGIN()
            ATTR_CUSTOM(L"src",OnAttrSrc)   //XML�ļ���ָ����ͼƬ��Դ��,(type:name)
        SOUI_ATTRS_END()
    protected:
		virtual void _DrawByIndex2(IRenderTarget *pRT, LPCRECT rcDraw, int dwState, BYTE byAlpha = 0xFF) const  override;

        HRESULT OnAttrSrc(const SStringW &strValue,BOOL bLoading);
        int LoadFromGdipImage(Gdiplus::Bitmap * pImg);

		SAniFrame * m_pFrames;
    };
}//end of name space SOUI
