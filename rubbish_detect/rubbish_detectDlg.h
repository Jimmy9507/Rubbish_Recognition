
// rubbish_detectDlg.h : header file
//

#pragma once


// Crubbish_detectDlg dialog
class Crubbish_detectDlg : public CDialogEx
{
// Construction
public:
	Crubbish_detectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_RUBBISH_DETECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedButton1();
	afx_msg void On_OpenCamera();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void On_CloseCamera();
	afx_msg void OnStnClickedPic6Static();
	afx_msg void OnStnClickedPic5Static();
	afx_msg void OnStnClickedPic3Static();
};
