#include "PrefsWindow.h"
//------------------------------------------------------------------------------
// BeOS
#include <Application.h>
#include <Locale.h>
#include <Resources.h>
// Add2
#include <add2/SHINTAConstants.h>
// Proj
#include "Constants.h"
#include "LKKConstants.h"
#include "PrefsView.h"
//==============================================================================
TPrefsWindow::TPrefsWindow(BRect oFrame)
		: SUPER(oFrame, "PrefsWindow")
{
	const char*	aViewObj;
	size_t		aSize;
	BMessage	aViewArc;

	// メンバ変数初期化
	mInitCheck = B_NO_INIT;
	aViewObj = reinterpret_cast<const char*>(BApplication::AppResources()->LoadResource(B_ARCHIVED_OBJECT, RSRC_NAME_PREFS_VIEW, &aSize));
	if ( aViewObj == NULL )
		return;
	aViewArc.Unflatten(aViewObj);
	// ビューの作成
	mPrefsView = new TPrefsView(&aViewArc);
	ResizeTo(mPrefsView->Frame().right, mPrefsView->Frame().bottom);
	AddChild(mPrefsView);
	SetWindowTitle();
	mInitCheck = B_OK;
}
//------------------------------------------------------------------------------
TPrefsWindow::~TPrefsWindow()
{
}
//------------------------------------------------------------------------------
status_t	TPrefsWindow::InitCheck() const
{
	return mInitCheck;
}
//------------------------------------------------------------------------------
void	TPrefsWindow::LanguageChanged()
{
	SetWindowTitle();
}
//------------------------------------------------------------------------------
void	TPrefsWindow::MessageReceived(BMessage* oMsg)
{
	switch( oMsg->what ) {
	// 転送のみ
	case PREF_LOCK_TEMPLATE_ORDER_CHANGED:
		BMessenger(mPrefsView).SendMessage(oMsg);
		break;
	// 転送＆処理
	default:
		SUPER::MessageReceived(oMsg);
	}
}
//------------------------------------------------------------------------------
bool	TPrefsWindow::QuitRequested()
{
	BMessage*	aCurMsg;
	
	aCurMsg = CurrentMessage();
	if ( aCurMsg != NULL ) {	// クローズボタンが押された
		dynamic_cast<TPrefsView*>(mPrefsView)->ButtonCancelClicked();
	}
	return SUPER::QuitRequested();
}
//------------------------------------------------------------------------------
void	TPrefsWindow::SetWindowTitle()
{
	SetTitle(_T(LKK_TITLE_PREFS_WIN));
}
//------------------------------------------------------------------------------
//==============================================================================
