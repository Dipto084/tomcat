#include "basic.h"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


// Define a class for the app
class MyApp: public wxApp{
	public:
		virtual bool OnInit(); // This constructor is called on init

};


// This is out main function call
wxIMPLEMENT_APP(MyApp);


// IN our init, we create the frmae
bool MyApp::OnInit()
{
	MyFrame1* frame = new MyFrame1(NULL, 
									NULL,
									"Tomcat Dashboard",
									wxPoint(50,50),
									wxSize(450, 340),
									wxDEFAULT_FRAME_STYLE
					);
	frame->Show(true);
	return true;
}



// Frame is defined here
MyFrame1::MyFrame1( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );

	m_button22 = new wxButton( this, wxID_ANY, wxT("Tab 1"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_button22, 0, wxALL, 5 );

	m_button23 = new wxButton( this, wxID_ANY, wxT("Tab 2"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_button23, 0, wxALL, 5 );

	m_button24 = new wxButton( this, wxID_ANY, wxT("Tab 3"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_button24, 0, wxALL, 5 );

	m_button25 = new wxButton( this, wxID_ANY, wxT("Tab 4"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_button25, 0, wxALL, 5 );

	m_button26 = new wxButton( this, wxID_ANY, wxT("Tab 5"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_button26, 0, wxALL, 5 );


	bSizer1->Add( bSizer4, 0, wxEXPAND, 5 );

	m_scintilla2 = new wxStyledTextCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, wxEmptyString );
	m_scintilla2->SetUseTabs( true );
	m_scintilla2->SetTabWidth( 4 );
	m_scintilla2->SetIndent( 4 );
	m_scintilla2->SetTabIndents( true );
	m_scintilla2->SetBackSpaceUnIndents( true );
	m_scintilla2->SetViewEOL( false );
	m_scintilla2->SetViewWhiteSpace( false );
	m_scintilla2->SetMarginWidth( 2, 0 );
	m_scintilla2->SetIndentationGuides( true );
	m_scintilla2->SetReadOnly( false );
	m_scintilla2->SetMarginType( 1, wxSTC_MARGIN_SYMBOL );
	m_scintilla2->SetMarginMask( 1, wxSTC_MASK_FOLDERS );
	m_scintilla2->SetMarginWidth( 1, 16);
	m_scintilla2->SetMarginSensitive( 1, true );
	m_scintilla2->SetProperty( wxT("fold"), wxT("1") );
	m_scintilla2->SetFoldFlags( wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED );
	m_scintilla2->SetMarginType( 0, wxSTC_MARGIN_NUMBER );
	m_scintilla2->SetMarginWidth( 0, m_scintilla2->TextWidth( wxSTC_STYLE_LINENUMBER, wxT("_99999") ) );
	m_scintilla2->MarkerDefine( wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS );
	m_scintilla2->MarkerSetBackground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("BLACK") ) );
	m_scintilla2->MarkerSetForeground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("WHITE") ) );
	m_scintilla2->MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS );
	m_scintilla2->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("BLACK") ) );
	m_scintilla2->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("WHITE") ) );
	m_scintilla2->MarkerDefine( wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY );
	m_scintilla2->MarkerDefine( wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUS );
	m_scintilla2->MarkerSetBackground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("BLACK") ) );
	m_scintilla2->MarkerSetForeground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("WHITE") ) );
	m_scintilla2->MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUS );
	m_scintilla2->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("BLACK") ) );
	m_scintilla2->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("WHITE") ) );
	m_scintilla2->MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY );
	m_scintilla2->MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY );
	m_scintilla2->SetSelBackground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );
	m_scintilla2->SetSelForeground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
	bSizer1->Add( m_scintilla2, 3, wxEXPAND | wxALL, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
	m_menubar2 = new wxMenuBar( 0 );
	m_menu3 = new wxMenu();
	m_menu2 = new wxMenu();
	wxMenuItem* m_menu2Item = new wxMenuItem( m_menu3, wxID_ANY, wxT("Placeholder"), wxEmptyString, wxITEM_NORMAL, m_menu2 );
	m_menu3->Append( m_menu2Item );

	m_menubar2->Append( m_menu3, wxT("File") );

	m_menu4 = new wxMenu();
	m_menubar2->Append( m_menu4, wxT("Help") );

	this->SetMenuBar( m_menubar2 );


	this->Centre( wxBOTH );
}

MyFrame1::~MyFrame1()
{
}
