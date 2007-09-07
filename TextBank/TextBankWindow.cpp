#include "TextBankWindow.h"
//------------------------------------------------------------------------------
// BeOS
#include <Application.h>
#include <Locale.h>
#include <Menu.h>
#include <MenuItem.h>
// Add2
#include <add2/FUConstants.h>
#include <add2/MiscDebug.h>
#include <add2/MiscUtils.h>
#include <add2/SHINTAConstants.h>
// Proj
#include "Common.h"
#include "Constants.h"
#include "LKKConstants.h"
#include "TextBankView.h"
//==============================================================================
TTextBankWindow::TTextBankWindow(BRect oFrame)
		: SUPER(oFrame, _T(gAppNameEnUs), B_DOCUMENT_WINDOW, B_ASYNCHRONOUS_CONTROLS, B_ALL_WORKSPACES)
{
	SetWinTitle();
	// ツールバーの作成
	mToolBar = new TToolBar(BRect(0, 0, 0, 0), "ToolBar");
	AddChild(mToolBar);
	CreateToolBar();
	// メニューと主ビューの作成
	oFrame.OffsetTo(B_ORIGIN);
	oFrame.top += mToolBar->Frame().Height()+1;
	mTextBankView = new TTextBankView(oFrame);
	AddChild(mTextBankView);
	mToolBar->SetTargetForItems(mTextBankView);
	// ショートカットなど
	AddShortcut(B_DOWN_ARROW, B_COMMAND_KEY, new BMessage(POP_MITEM_MOVE_DOWN_CLICKED), NULL);
	AddShortcut(B_UP_ARROW, B_COMMAND_KEY, new BMessage(POP_MITEM_MOVE_UP_CLICKED), NULL);
	AddShortcut('C', B_COMMAND_KEY, new BMessage(POP_MITEM_SEND_TO_CLIPBOARD_CLICKED), NULL);
	AddShortcut('D', B_COMMAND_KEY, new BMessage(POP_MITEM_DELETE_CLICKED), NULL);
	AddShortcut('N', B_COMMAND_KEY, new BMessage(POP_MITEM_NEW_GROUP_CLICKED), NULL);
	AddShortcut('S', B_COMMAND_KEY, new BMessage(POP_MITEM_SAVE_AS_TEMPLATE_CLICKED), NULL);
	AddShortcut('T', B_COMMAND_KEY, new BMessage(POP_MITEM_PROPERTY_CLICKED), NULL);
}
//------------------------------------------------------------------------------
TTextBankWindow::~TTextBankWindow()
{
}
//------------------------------------------------------------------------------
void	TTextBankWindow::ActivateClipboardViewRequested()
{
	mTButtonClipboard->SetValue(B_CONTROL_ON);
	mTButtonClipboard->Invoke();
}
//------------------------------------------------------------------------------
void	TTextBankWindow::ActivateTemplateViewRequested()
{
	mTButtonTemplate->SetValue(B_CONTROL_ON);
	mTButtonTemplate->Invoke();
}
//------------------------------------------------------------------------------
void	TTextBankWindow::CreateToolBar()
{
	// クリップボード
	mTButtonClipboard = new TToolButton(BRect(), "TButtonClipboard", NULL, "",
			new BMessage(TBUTTON_CLIPBOARD_CLICKED), B_TWO_STATE_BUTTON);
	mToolBar->AddItem(mTButtonClipboard);
	// 定型文
	mTButtonTemplate = new TToolButton(BRect(), "TButtonTemplate", NULL, "",
			new BMessage(TBUTTON_TEMPLATE_CLICKED), B_TWO_STATE_BUTTON);
	mToolBar->AddItem(mTButtonTemplate);
	// 設定
	mTButtonClipboard->SetRadioMode(true);
	mTButtonClipboard->SetValue(B_CONTROL_ON);
	SetToolBarLabels();
	SetToolBarStyle();
}
//------------------------------------------------------------------------------
void	TTextBankWindow::DestroyToolBar()
{
}
//------------------------------------------------------------------------------
void	TTextBankWindow::IAPChanged(BMessage* oMsg)
{
	SetToolBarStyle();
}
//------------------------------------------------------------------------------
void	TTextBankWindow::LanguageChanged()
{
	SetToolBarLabels();
}
//------------------------------------------------------------------------------
void	TTextBankWindow::MessageReceived(BMessage* oMsg)
{
	switch( oMsg->what ) {
	// 転送
	case DELETE_TOP_HISTORY_REQUESTED:
	case PREF_NUM_CLIPBOARD_HISTORIES_CHANGED:
	case SHOW_POP_UP_REQUESTED:
		BMessenger(mTextBankView).SendMessage(oMsg);
		break;
	// 自前処理
	case ACTIVATE_CLIPBOARD_VIEW_REQUESTED:
		ActivateClipboardViewRequested();
		break;
	case ACTIVATE_TEMPLATE_VIEW_REQUESTED:
		ActivateTemplateViewRequested();
		break;
	default:
		SUPER::MessageReceived(oMsg);
	}
}
//------------------------------------------------------------------------------
void	TTextBankWindow::Quit()
{
	gExitStates.mFrame = Frame();
	gExitStates.mMinimized = IsMinimized();
//	DBEXP("TTextBankWindow::Quit() - IsMinimized", IsMinimized());
//	DBEXP("TTextBankWindow::Quit() - IsHidden", IsHidden());
	SUPER::Quit();
}
//------------------------------------------------------------------------------
bool	TTextBankWindow::QuitRequested()
{
	be_app_messenger.SendMessage(B_QUIT_REQUESTED);
	return SUPER::QuitRequested();
}
//------------------------------------------------------------------------------
void	TTextBankWindow::SetToolBarLabels()
{
	mTButtonClipboard->SetLabel(_T(LKK_TBUTTON_CLIPBOARD));
	mTButtonTemplate->SetLabel(_T(LKK_TBUTTON_TEMPLATE));
	mToolBar->RelocateItems();
}
//------------------------------------------------------------------------------
void	TTextBankWindow::SetToolBarStyle()
{
	bool	aFlat;

	gIAPs.GetBool(IAP_NAME_FLAT_TOOL_BAR, &aFlat);
	mToolBar->SetBarFlat(aFlat);
	mToolBar->SetButtonFlat(aFlat);
}
//------------------------------------------------------------------------------
void	TTextBankWindow::SetWinTitle()
{
#ifdef DEBUG
	char		aTitle[256];
	strcpy(aTitle, "［デバッグ］ ");
	strcat(aTitle, _T(gAppNameEnUs));
	SetTitle(aTitle);
#else
	SetTitle(_T(gAppNameEnUs));
#endif
}
//------------------------------------------------------------------------------
//==============================================================================
