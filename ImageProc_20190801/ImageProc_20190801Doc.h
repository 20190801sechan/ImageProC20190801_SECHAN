
// ImageProc_20190801Doc.h: CImageProc_20190801Doc 클래스의 인터페이스
//


#pragma once


class CImageProc_20190801Doc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CImageProc_20190801Doc() noexcept;
	DECLARE_DYNCREATE(CImageProc_20190801Doc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CImageProc_20190801Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	// 입력이미지
	unsigned char **inputImg;		// [y][x]
	unsigned char **inputImg2;		// [y][x]
	// 출력이미지
	unsigned char **resultImg;		// [y][x]
	int imageWidth;
	int imageHeight;
	int depth;		// 1=흑백,3=칼라

	int gImageWidth;
	int gImageHeight;
	// 출력이미지
	unsigned char **gResultImg;		// [y][x]
	
	unsigned char **morphedImg[10];

	int LoadImageFile(CArchive& ar);
	int LoadSecondImageFile(CArchive & ar);
	
	void GeometryMorphing();
	void LoadTwoImages();
};
