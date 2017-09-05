#include "stdafx.h"
#include <assert.h>
#include "ListCtrlEx.h"
#include "utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx

IMPLEMENT_DYNCREATE(CListCtrlEx, CListCtrl)

BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx construction/destruction

#define COLOR_HOTLIGHT 26
CListCtrlEx::CListCtrlEx()
{
	m_clrText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_clrBkgnd = ::GetSysColor(COLOR_WINDOW);
	m_clrHText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_clrHBkgnd = ::GetSysColor(COLOR_HIGHLIGHT);
	//m_clrPercent = RGB(0, 128, 128);
	m_clrHPercent = RGB(0, 128, 128);
}

CListCtrlEx::~CListCtrlEx()
{
}

void CListCtrlEx::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;

    switch(lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
	   *pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
 	    *pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:
		{
			int iCol = lplvcd->iSubItem;
			int iRow = lplvcd->nmcd.dwItemSpec;
			CString sItem = GetItemText(iRow, iCol);
			CRect rcItem;
			GetCellRect(iRow, iCol, LVIR_BOUNDS, rcItem);
			
			CDC *pDC= CDC::FromHandle(lplvcd->nmcd.hdc);
			int State = ListView_GetItemState(m_hWnd, iRow, LVIS_CUT|LVIS_SELECTED|LVIS_FOCUSED);

			pDC->FillSolidRect(&rcItem, (State &LVIS_SELECTED) ? m_clrHBkgnd : m_clrBkgnd);
			rcItem.DeflateRect(2, 1);
			if(sItem.Left(7)=="SPECIAL")  //show funky bar control
			{
//"SPECIAL:" then the period interval,recording maxduration, offset, all in seconds then follow-on
				CString temp=Word(sItem,1,':');
				float interval = 100*(float)atof(Word(temp,0,','));
				float duration = 100*(float)atof(Word(temp,1,','));
				float offset = 100*(float)atof(Word(temp,2,','));
				bool isAfterMsg = !!atoi(Word(temp,3,','));
				
				pDC->Rectangle(rcItem);
				rcItem.DeflateRect(1, 1);
				CRect rcLeft, rcMiddle, rcRight;
				rcLeft = rcMiddle = rcRight = rcItem;
				rcLeft.right = rcLeft.left + MulDiv((int)offset, rcItem.Width(), (int)interval);
				rcMiddle.left = rcLeft.right;
				rcMiddle.right = rcMiddle.left +  MulDiv((int)duration, rcItem.Width(), (int)interval);
				rcRight.left = rcMiddle.right;

				if((State &LVIS_SELECTED))
				{
					pDC->FillSolidRect(rcLeft, m_clrBkgnd);
					pDC->FillSolidRect(rcMiddle, m_clrHPercent);
					pDC->FillSolidRect(rcRight, m_clrBkgnd);
				//	pDC->SetTextColor(m_clrText);
				//	pDC->DrawText(temp, rcItem, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
				}
				else
				{
					pDC->FillSolidRect(rcLeft, m_clrPercent);
					pDC->FillSolidRect(rcMiddle, m_clrHPercent);
					pDC->FillSolidRect(rcRight, m_clrPercent);
				/*	CRgn rgn;
					rgn.CreateRectRgnIndirect(rcLeft);
					pDC->SelectClipRgn(&rgn);
					pDC->SetTextColor(m_clrBkgnd);*/
				//	pDC->DrawText(temp, rcItem, DT_VCENTER|DT_CENTER|DT_SINGLELINE);

			/*		rgn.SetRectRgn(rcRight);
					pDC->SelectClipRgn(&rgn);
					pDC->SetTextColor(m_clrText);
					pDC->DrawText(sItem, rcItem, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
					pDC->SelectClipRgn(NULL);*/
				}
				
				if (isAfterMsg)
					{
					pDC->MoveTo(CPoint(rcMiddle.left, rcMiddle.bottom));
					pDC->LineTo(CPoint(rcMiddle.left,rcItem.top -4));
					}
			}
			else
			{
				pDC->SetTextColor((State & LVIS_SELECTED) ? m_clrHText:m_clrText);
				pDC->DrawText(sItem, -1, &rcItem, DT_NOCLIP | DT_LEFT | DT_VCENTER);
			}
		}
		*pResult= CDRF_SKIPDEFAULT;
		break;
	}
}

BOOL CListCtrlEx::GetCellRect(int iRow, int iCol, int nArea, CRect &rect)
{
	if(iCol)
		return GetSubItemRect(iRow, iCol, nArea, rect);
	
	if(GetHeaderCtrl()->GetItemCount() == 1)
		return GetItemRect(iRow, rect, nArea);
	
	CRect rCol1;
	if(!GetSubItemRect(iRow, 1, nArea, rCol1))
		return FALSE;
	
	if(!GetItemRect(iRow, rect, nArea))
		return FALSE;
	
	rect.right = rCol1.left;
	
	return TRUE;
}
/*
void CListCtrlEx::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 16;
}

void CListCtrlEx::Init()
{
	// WM_WINDOWPOSCHANGED를 일부러 발생시켜서 부모 윈도우가
	// WM_MEASUREITEM 메세지를 보내도록 한다.
	CRect rc;
	GetWindowRect( &rc );

	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rc.Width();
	wp.cy = rc.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage( WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp );
}
*/