
// STLAlgorithmsDlg.h : header file
//

#pragma once


// CSTLAlgorithmsDlg dialog
class CSTLAlgorithmsDlg : public CDialogEx
{
private:
	void initalizeCatalogs();
	void initalizeOperation();

// Construction
public:
	CSTLAlgorithmsDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STLALGORITHMS_DIALOG };
#endif

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
	afx_msg void OnLbnSelchangeCatalog();

public:
	CListBox mLstCatalog;
	CListBox mLstOperation;
};
