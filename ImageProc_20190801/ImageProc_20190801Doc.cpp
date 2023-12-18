
// ImageProc_20190801Doc.cpp: CImageProc_20190801Doc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageProc_20190801.h"
#endif

#include "ImageProc_20190801Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageProc_20190801Doc

IMPLEMENT_DYNCREATE(CImageProc_20190801Doc, CDocument)

BEGIN_MESSAGE_MAP(CImageProc_20190801Doc, CDocument)
END_MESSAGE_MAP()


// CImageProc_20190801Doc 생성/소멸

CImageProc_20190801Doc::CImageProc_20190801Doc() noexcept
{
	inputImg = NULL;
	inputImg2 = NULL;
	resultImg = NULL;

	gResultImg = NULL;

}

CImageProc_20190801Doc::~CImageProc_20190801Doc()
{
	int i;

	if (inputImg != NULL)
	{
		for (i = 0; i < imageHeight; i++)
			free(inputImg[i]);
		free(inputImg);
	}

	if (inputImg2 != NULL)
	{
		for (i = 0; i < imageHeight; i++)
			free(inputImg2[i]);
		free(inputImg2);
	}

	if (resultImg != NULL)
	{
		for (i = 0; i < imageHeight; i++)
			free(resultImg[i]);
		free(resultImg);
	}

	if (gResultImg != NULL)
	{
		for (i = 0; i < gImageHeight; i++)
			free(gResultImg[i]);
		free(gResultImg);
	}
}

BOOL CImageProc_20190801Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CImageProc_20190801Doc serialization

void CImageProc_20190801Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		LoadImageFile(ar);
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CImageProc_20190801Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CImageProc_20190801Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CImageProc_20190801Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImageProc_20190801Doc 진단

#ifdef _DEBUG
void CImageProc_20190801Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageProc_20190801Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageProc_20190801Doc 명령





int CImageProc_20190801Doc::LoadImageFile(CArchive& ar)
{
	int maxValue, i;
	char type[16], buf[256];
	CFile *fp = ar.GetFile();
	CString fname = fp->GetFilePath();
	bool isbmp = false;

	if (strcmp(strchr(fname, '.'), ".ppm") == 0 || strcmp(strchr(fname, '.'), ".PPM") == 0 ||
		strcmp(strchr(fname, '.'), ".pgm") == 0 || strcmp(strchr(fname, '.'), ".PGM") == 0)
	{
		ar.ReadString(type, 15);
		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');
		sscanf(buf, "%d %d", &imageWidth, &imageHeight);

		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');
		sscanf(buf, "%d", &maxValue);

		if (strcmp(type, "P5") == 0)	depth = 1;
		else                            depth = 3;
	}
	else if (strcmp(strchr(fname, '.'), ".bmp") == 0 || strcmp(strchr(fname, '.'), ".BMP") == 0)
	{
		//bitmap file header읽기
		BITMAPFILEHEADER bmfh;
		ar.Read((LPSTR)&bmfh, sizeof(bmfh));
		//bmp화일임을 나타내는 "BM"마커가 있는지 확인
		if (bmfh.bfType != (WORD)(('M' << 8) | 'B'))		return 0;

		//bitmap info header읽기
		BITMAPINFOHEADER bih;
		ar.Read((LPSTR)&bih, sizeof(bih));
		imageWidth = bih.biWidth;
		imageHeight = bih.biHeight;
		depth = bih.biBitCount / 8;

		//palette처리
		if (depth == 1)
		{	//palette존재
			BYTE palette[256 * 4];
			ar.Read(palette, 256 * 4);
		}
		isbmp = true;
	}
	else if (strcmp(strchr(fname, '.'), ".raw") == 0 || strcmp(strchr(fname, '.'), ".RAW") == 0)
	{
		imageWidth = 256;
		imageHeight = 256;
		depth = 1;
	}

	// 메모리 할당
	inputImg = (unsigned char **)malloc(imageHeight * sizeof(unsigned char*));
	resultImg = (unsigned char **)malloc(imageHeight * sizeof(unsigned char*));
	for (i = 0; i < imageHeight; i++)
	{
		inputImg[i] = (unsigned char*)malloc(imageWidth*depth);
		resultImg[i] = (unsigned char*)malloc(imageWidth*depth);
	}

	if (!isbmp)
	{
		// 파일에서 읽어서 저장
		for (i = 0; i < imageHeight; i++)
			ar.Read(inputImg[i], imageWidth * depth);
	}
	else
	{
		// 파일에서 읽어서 저장
		BYTE nu[4*3];
		int widthfile;
		widthfile = (imageWidth * 8 + 31) / 32 * 4;
		for (i = 0; i < imageHeight; i++)
		{
			if(depth==1)
				ar.Read(inputImg[imageHeight - 1 - i], imageWidth * depth);
			else
			{
				// BGR=>RGB
				BYTE r, g, b;
				for (int j = 0; j < imageWidth; j++)
				{
					ar.Read(&b, 1);	ar.Read(&g, 1);	ar.Read(&r, 1);
					inputImg[imageHeight - 1 - i][3 * j + 0] = r;
					inputImg[imageHeight - 1 - i][3 * j + 1] = g;
					inputImg[imageHeight - 1 - i][3 * j + 2] = b;
				}
			}

			if ((widthfile - imageWidth) != 0)
				ar.Read(nu, (widthfile - imageWidth) * depth);
		}
	}

	return 0;
}


int CImageProc_20190801Doc::LoadSecondImageFile(CArchive & ar)
{
	int w, h, d;
	int maxValue, i;
	char type[16], buf[256];
	CFile *fp = ar.GetFile();
	CString fname = fp->GetFilePath();
	bool isbmp = false;

	if (strcmp(strchr(fname, '.'), ".ppm") == 0 || strcmp(strchr(fname, '.'), ".PPM") == 0 ||
		strcmp(strchr(fname, '.'), ".pgm") == 0 || strcmp(strchr(fname, '.'), ".PGM") == 0)
	{
		ar.ReadString(type, 15);
		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');
		sscanf(buf, "%d %d", &w, &h);

		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');
		sscanf(buf, "%d", &maxValue);

		if (strcmp(type, "P5") == 0)	d = 1;
		else                            d = 3;
	}
	else if (strcmp(strchr(fname, '.'), ".bmp") == 0 || strcmp(strchr(fname, '.'), ".BMP") == 0)
	{
		//bitmap file header읽기
		BITMAPFILEHEADER bmfh;
		ar.Read((LPSTR)&bmfh, sizeof(bmfh));
		//bmp화일임을 나타내는 "BM"마커가 있는지 확인
		if (bmfh.bfType != (WORD)(('M' << 8) | 'B'))		return 0;

		//bitmap info header읽기
		BITMAPINFOHEADER bih;
		ar.Read((LPSTR)&bih, sizeof(bih));
		w = bih.biWidth;
		h = bih.biHeight;
		d = bih.biBitCount / 8;

		//palette처리
		if (d == 1)
		{	//palette존재
			BYTE palette[256 * 4];
			ar.Read(palette, 256 * 4);
		}
		isbmp = true;
	}
	else if (strcmp(strchr(fname, '.'), ".raw") == 0 || strcmp(strchr(fname, '.'), ".RAW") == 0)
	{
		w = 256;
		h = 256;
		d = 1;
	}

	if (imageWidth != w || imageHeight != h || depth != d)
	{
		AfxMessageBox("가로,세로,색상수가 같아야만 처리할수 있습니다.");
		return 0;
	}

	// 메모리 할당
	inputImg2 = (unsigned char **)malloc(imageHeight * sizeof(unsigned char*));
	for (i = 0; i < imageHeight; i++)
	{
		inputImg2[i] = (unsigned char*)malloc(imageWidth*depth);
	}

	if (!isbmp)
	{
		// 파일에서 읽어서 저장
		for (i = 0; i < imageHeight; i++)
			ar.Read(inputImg2[i], imageWidth * depth);
	}
	else
	{
		// 파일에서 읽어서 저장
		BYTE nu[4 * 3];
		int widthfile;
		widthfile = (imageWidth * 8 + 31) / 32 * 4;
		for (i = 0; i < imageHeight; i++)
		{
			if (depth == 1)
				ar.Read(inputImg2[imageHeight - 1 - i], imageWidth * depth);
			else
			{
				// BGR=>RGB
				BYTE r, g, b;
				for (int j = 0; j < imageWidth; j++)
				{
					ar.Read(&b, 1);	ar.Read(&g, 1);	ar.Read(&r, 1);
					inputImg2[imageHeight - 1 - i][3 * j + 0] = r;
					inputImg2[imageHeight - 1 - i][3 * j + 1] = g;
					inputImg2[imageHeight - 1 - i][3 * j + 2] = b;
				}
			}

			if ((widthfile - imageWidth) != 0)
				ar.Read(nu, (widthfile - imageWidth) * depth);
		}
	}

	return 0;
}











typedef struct
{
	int Px;
	int Py;
	int Qx;
	int Qy;
} control_line;
#define NUM_FRAMES 10
void CImageProc_20190801Doc::GeometryMorphing()
{
	control_line source_lines[23] =
	{ {116,7,207,5},{34,109,90,21},{55,249,30,128},{118,320,65,261},
	 {123,321,171,321},{179,319,240,264},{247,251,282,135},{281,114,228,8},
	 {78,106,123,109},{187,115,235,114},{72,142,99,128},{74,150,122,154},
	 {108,127,123,146},{182,152,213,132},{183,159,229,157},{219,131,240,154},
	 {80,246,117,212},{127,222,146,223},{154,227,174,221},{228,252,183,213},
	 {114,255,186,257},{109,258,143,277},{152,278,190,262} };
	control_line dest_lines[23] =
	{ {120,8,200,6},{12,93,96,16},{74,271,16,110},{126,336,96,290},
	 {142,337,181,335},{192,335,232,280},{244,259,288,108},{285,92,212,13},
	 {96,135,136,118},{194,119,223,125},{105,145,124,134},{110,146,138,151},
	 {131,133,139,146},{188,146,198,134},{189,153,218,146},{204,133,221,140},
	 {91,268,122,202},{149,206,159,209},{170,209,181,204},{235,265,208,199},
	 {121,280,205,284},{112,286,160,301},{166,301,214,287} };

	double u;       // 수직 교차점의 위치   
	double h;       // 제어선으로부터 픽셀의 수직 변위 
	double d;       // 제어선과 픽셀 사이의 거리 
	double tx, ty;   // 결과영상 픽셀에 대응되는 입력 영상 픽셀 사이의 변위의 합  
	double xp, yp;  // 각 제어선에 대해 계산된 입력 영상의 대응되는 픽셀 위치     
	double weight;     // 각 제어선의 가중치       
	double totalWeight; // 가중치의 합          
	double a = 0.001, b = 2.0, p = 0.75;
	unsigned char** warpedImg;
	unsigned char** warpedImg2;
	int frame;
	double fweight;
	control_line warp_lines[23];
	double tx2, ty2, xp2, yp2;
	int dest_x1, dest_y1, dest_x2, dest_y2, source_x2, source_y2;
	int x1, x2, y1, y2, src_x1, src_y1, src_x2, src_y2;
	double src_line_length, dest_line_length;
	int i, j;
	int num_lines = 23;         // 제어선의 수 
	int line, x, y, source_x, source_y, last_row, last_col;

	LoadTwoImages();

	// 중간 프레임의 워핑 결과를 저장을 위한 기억장소 할당 
	warpedImg = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));
	for (i = 0; i < imageHeight; i++) {
		warpedImg[i] = (unsigned char*)malloc(imageWidth * depth);
	}

	warpedImg2 = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));
	for (i = 0; i < imageHeight; i++) {
		warpedImg2[i] = (unsigned char*)malloc(imageWidth * depth);
	}

	for (i = 0; i < NUM_FRAMES; i++) {
		morphedImg[i] = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));
		for (j = 0; j < imageHeight; j++) {
			morphedImg[i][j] = (unsigned char*)malloc(imageWidth * depth);
		}
	}
	last_row = imageHeight - 1;
	last_col = imageWidth - 1;

	// 각 중간 프레임에 대하여 
	for (frame = 1; frame <= NUM_FRAMES; frame++)
	{
		// 중간 프레임에 대한 가중치 계산 
		fweight = (double)(frame) / NUM_FRAMES;

		// 중간 프레임에 대한 제어선 계산 
		for (line = 0; line < num_lines; line++)
		{
			warp_lines[line].Px = (int)(source_lines[line].Px +
				(dest_lines[line].Px - source_lines[line].Px) * fweight);
			warp_lines[line].Py = (int)(source_lines[line].Py +
				(dest_lines[line].Py - source_lines[line].Py) * fweight);
			warp_lines[line].Qx = (int)(source_lines[line].Qx +
				(dest_lines[line].Qx - source_lines[line].Qx) * fweight);
			warp_lines[line].Qy = (int)(source_lines[line].Qy +
				(dest_lines[line].Qy - source_lines[line].Qy) * fweight);
		}

		// 출력 영상의 각 픽셀에 대하여 
		for (y = 0; y < imageHeight; y++)
		{
			for (x = 0; x < imageWidth; x++)
			{
				totalWeight = 0.0;
				tx = 0.0;
				ty = 0.0;
				tx2 = 0.0;
				ty2 = 0.0;
				// 각 제어선에 대하여 
				for (line = 0; line < num_lines; line++)
				{
					x1 = warp_lines[line].Px;
					y1 = warp_lines[line].Py;
					x2 = warp_lines[line].Qx;
					y2 = warp_lines[line].Qy;
					dest_line_length = sqrt((double)(x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
					// 수직교차점의 위치 및 픽셀의 수직 변위 계산 
					u = (double)((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) /
						(double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
					h = (double)((y - y1) * (x2 - x1) - (x - x1) * (y2 - y1)) / dest_line_length;

					// 제어선과 픽셀 사이의 거리 계산 
					if (u < 0) d = sqrt((double)(x - x1) * (x - x1) + (y - y1) * (y - y1));
					else if (u > 1) d = sqrt((double)(x - x2) * (x - x2) + (y - y2) * (y - y2));
					else d = fabs(h);

					src_x1 = source_lines[line].Px;
					src_y1 = source_lines[line].Py;
					src_x2 = source_lines[line].Qx;
					src_y2 = source_lines[line].Qy;
					src_line_length = sqrt((double)(src_x2 - src_x1) * (src_x2 - src_x1) +
						(src_y2 - src_y1) * (src_y2 - src_y1));
					dest_x1 = dest_lines[line].Px;
					dest_y1 = dest_lines[line].Py;
					dest_x2 = dest_lines[line].Qx;
					dest_y2 = dest_lines[line].Qy;
					dest_line_length = sqrt((double)(dest_x2 - dest_x1) * (dest_x2 - dest_x1) +
						(dest_y2 - dest_y1) * (dest_y2 - dest_y1));

					// 입력 영상 1에서의 대응 픽셀 위치 계산 
					xp = src_x1 + u * (src_x2 - src_x1) -
						h * (src_y2 - src_y1) / src_line_length;
					yp = src_y1 + u * (src_y2 - src_y1) +
						h * (src_x2 - src_x1) / src_line_length;

					// 입력 영상 2에서의 대응 픽셀 위치 계산 
					xp2 = dest_x1 + u * (dest_x2 - dest_x1) -
						h * (dest_y2 - dest_y1) / dest_line_length;
					yp2 = dest_y1 + u * (dest_y2 - dest_y1) +
						h * (dest_x2 - dest_x1) / dest_line_length;

					// 제어선에 대한 가중치 계산 
					weight = pow((pow((double)(dest_line_length), p) / (a + d)), b);

					// 입력 영상 1의 대응 픽셀과의 변위 계산 
					tx += (xp - x) * weight;
					ty += (yp - y) * weight;

					// 입력 영상 2의 대응 픽셀과의 변위 계산 
					tx2 += (xp2 - x) * weight;
					ty2 += (yp2 - y) * weight;

					totalWeight += weight;
				}
				// 입력 영상 1의 대응 픽셀 위치 계산     
				source_x = x + (int)(tx / totalWeight + 0.5);
				source_y = y + (int)(ty / totalWeight + 0.5);

				// 입력 영상 2의 대응 픽셀 위치 계산 
				source_x2 = x + (int)(tx2 / totalWeight + 0.5);
				source_y2 = y + (int)(ty2 / totalWeight + 0.5);

				// 영상의 경계를 벗어나는지 검사 
				if (source_x < 0) source_x = 0;
				if (source_x > last_col) source_x = last_col;
				if (source_y < 0) source_y = 0;
				if (source_y > last_row) source_y = last_row;

				if (source_x2 < 0) source_x2 = 0;
				if (source_x2 > last_col) source_x2 = last_col;
				if (source_y2 < 0) source_y2 = 0;
				if (source_y2 > last_row) source_y2 = last_row;

				// 워핑 결과 저장 
				warpedImg[y][x] = inputImg[source_y][source_x];
				warpedImg2[y][x] = inputImg2[source_y2][source_x2];
			}
		}

		// 모핑 결과 합병 
		for (y = 0; y < imageHeight; y++)
			for (x = 0; x < imageWidth; x++) {
				int val = (int)((1.0 - fweight) * warpedImg[y][x] +
					fweight * warpedImg2[y][x]);
				if (val < 0) val = 0;
				if (val > 255) val = 255;
				morphedImg[frame - 1][y][x] = val;
			}
	}
}



void CImageProc_20190801Doc::LoadTwoImages()
{
	CFile file;
	CFileDialog dlg(TRUE);
	AfxMessageBox("두번째 이미지를 선택하세요");

	if (dlg.DoModal() == IDOK) {   // 파일 선택 대화 상자 실행 
		file.Open(dlg.GetPathName(), CFile::modeRead);   // 파일 열기 
		CArchive ar(&file, CArchive::load);
		LoadSecondImageFile(ar);     // 영상 읽기 
		file.Close();               // 파일 닫기 
	}
}