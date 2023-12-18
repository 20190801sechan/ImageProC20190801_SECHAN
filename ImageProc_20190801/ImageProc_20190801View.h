
// ImageProc_20190801View.h: CImageProc_20190801View 클래스의 인터페이스
//

#pragma once


class CImageProc_20190801View : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CImageProc_20190801View() noexcept;
	DECLARE_DYNCREATE(CImageProc_20190801View)

// 특성입니다.
public:
	CImageProc_20190801Doc* GetDocument() const;
	int viewMode;
	

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CImageProc_20190801View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPixelAdd();
	afx_msg void OnPixelMul();
	afx_msg void OnPixelHistoEqu();
	afx_msg void OnPixelTwoImageAdd();
	afx_msg void OnRegionMeaning();
	void convolve(unsigned char** inimg, unsigned char** outimg, int cols, int rows, float mask[][3], int bias, int depth);
	afx_msg void OnRegionSharpening();
	afx_msg void OnRegionEmbossing();
	afx_msg void OnRegionPrewitt();
	afx_msg void OnRegionRoberts();
	afx_msg void OnRegionSobel();
	afx_msg void OnRegionAverageFiltering();
	afx_msg void OnRegionMedianFiltering();
	afx_msg void OnMopologyColorToGray();
	afx_msg void OnMopologyBinarization();
	afx_msg void OnMopologyErosion();
	afx_msg void OnMopologyDilation();
	afx_msg void OnMopologyOpening();
	void CopyResultToInput();
	afx_msg void OnMopologyClosing();
	afx_msg void OnGeometryZoominPixelCopy();
	afx_msg void OnGeometryZoomimBilinearInterpolation();
	afx_msg void OnGeometryZoomoutSubsampling();
	afx_msg void OnGeometryZoomoutMeanSub();
	afx_msg void OnGeometryAvgSampling();
	afx_msg void OnGeometryRotation();
	afx_msg void OnPixelSub();
	afx_msg void OnPixelDiv();
	afx_msg void OnPixelContraststrectching();
	afx_msg void OnPixelBinarization();
	afx_msg void OnPixelTwoimageadd();
	void LoadTwoImage();
	afx_msg void OnPixelTwoimagesub();
	afx_msg void OnGeometryMirror();
	afx_msg void OnGeometryFlip();
	afx_msg void OnGeometryWarping();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnAviView();
	bool bAviMode;
	CString AVIFileName;
	void LoadAviFile(CDC* pDC);
	afx_msg void OnGeometryMorphing();
	
};

#ifndef _DEBUG  // ImageProc_20190801View.cpp의 디버그 버전
inline CImageProc_20190801Doc* CImageProc_20190801View::GetDocument() const
   { return reinterpret_cast<CImageProc_20190801Doc*>(m_pDocument); }
#endif

