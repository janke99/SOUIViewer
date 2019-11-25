#pragma once

#include <interface/SRender-i.h>
namespace SOUI
{
    class SSkinAni : public SSkinObjBase
    {
        SOUI_CLASS_NAME(SSkinAni,L"skinani")

		class SAniFrame
		{
		public:
			SAutoRefPtr<IBitmap> pBmp;
			int                  nDelay;
		};
    public:
        SSkinAni():m_nFrames(0),m_iFrame(0),m_bTile(FALSE),m_filterLevel(kLow_FilterLevel)
        {

        }

        virtual ~SSkinAni()
        {
        }

		virtual long GetFrameDelay(int iFrame=-1) const = 0;

		virtual int LoadFromFile(LPCTSTR pszFileName) { return 0; }

        /**
         * LoadFromMemory
         * @brief    ���ڴ����APNG
         * @param    LPVOID pBits --  �ڴ��ַ
         * @param    size_t szData --  �ڴ����ݳ���
         * @return   int -- APNG֡����0-ʧ��
         * Describe  
         */    
        virtual int LoadFromMemory(LPVOID pBits,size_t szData) { return 0; }

		/**
        * ActiveNextFrame
        * @brief    ������һ֡
        * @return   void 
        * Describe  
        */    
        void ActiveNextFrame()
        {
            if(m_nFrames>1)
            {
                m_iFrame++;
                if(m_iFrame==m_nFrames) m_iFrame=0;
                SelectActiveFrame(m_iFrame);
            }
        }

        /**
        * SelectActiveFrame
        * @brief    ����ָ��֡
        * @param    int iFrame --  ֡��
        * @return   void
        * Describe  
        */    
        void SelectActiveFrame(int iFrame) const
        {
            if(m_nFrames>1 && iFrame<m_nFrames)
            {
                m_iFrame = iFrame;
            }
        }

		virtual void _DrawByIndex(IRenderTarget *pRT, LPCRECT rcDraw, int iFrame, BYTE byAlpha/*=0xFF*/) const
		{
			if (iFrame != -1) SelectActiveFrame(iFrame);
			_DrawByIndex2(pRT, rcDraw, m_iFrame, byAlpha);
		}

		LONG GetExpandCode() const{
			return MAKELONG(m_bTile?EM_TILE:EM_STRETCH,m_filterLevel);
		}
	public:
		SOUI_ATTRS_BEGIN()
			ATTR_RECT(L"margin",m_rcMargin,TRUE)
			ATTR_MARGIN(L"margin2",m_rcMargin,TRUE)
			ATTR_BOOL(L"tile",m_bTile,TRUE)
			ATTR_ENUM_BEGIN(L"filterLevel",FilterLevel,TRUE)
				ATTR_ENUM_VALUE(L"none",kNone_FilterLevel)
				ATTR_ENUM_VALUE(L"low",kLow_FilterLevel)
				ATTR_ENUM_VALUE(L"medium",kMedium_FilterLevel)
				ATTR_ENUM_VALUE(L"high",kHigh_FilterLevel)
			ATTR_ENUM_END(m_filterLevel)
		SOUI_ATTRS_END()
	protected:
		virtual void _DrawByIndex2(IRenderTarget *pRT, LPCRECT rcDraw, int iFrame, BYTE byAlpha/*=0xFF*/) const = 0;

	protected:
		int m_nFrames;
        mutable int m_iFrame;
		CRect		m_rcMargin;
		FilterLevel	m_filterLevel;
		BOOL		m_bTile;
    };

}