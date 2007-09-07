#include "PropertyWindow.h"
//------------------------------------------------------------------------------
// BeOS
#include <Application.h>
#include <Locale.h>
#include <Resources.h>
// Add2
#include <add2/MiscDebug.h>
#include <add2/SHINTAConstants.h>
// Proj
#include "Constants.h"
#include "LKKConstants.h"
#include "PropertyView.h"
//==============================================================================
TPropertyWindow::TPropertyWindow(BRect oFrame, const TTextData& oData)
		: SUPER(oFrame, "PropertyWindow")
{
	const char*	aViewObj;
	size_t		aSize;
	BMessage	aViewArc;

	// メンバ変数初期化
	mInitCheck = B_NO_INIT;
	aViewObj = reinterpret_cast<const char*>(BApplication::AppResources()->LoadResource(B_ARCHIVED_OBJECT, RSRC_NAME_PROPERTY_VIEW, &aSize));
	if ( aViewObj == NULL )
		return;
	aViewArc.Unflatten(aViewObj);
	// ビューの作成
	mPropertyView = new TPropertyView(&aViewArc, oData);
	ResizeTo(mPropertyView->Frame().right, mPropertyView->Frame().bottom);
	AddChild(mPropertyView);
	SetWindowTitle();
	mInitCheck = B_OK;
}
//------------------------------------------------------------------------------
TPropertyWindow::~TPropertyWindow()
{
}
//------------------------------------------------------------------------------
status_t	TPropertyWindow::InitCheck() const
{
	return mInitCheck;
}
//------------------------------------------------------------------------------
void	TPropertyWindow::LanguageChanged()
{
	SetWindowTitle();
}
//------------------------------------------------------------------------------
void	TPropertyWindow::MessageReceived(BMessage* oMsg)
{
	switch( oMsg->what ) {
	// 転送のみ
		// 降臨
	// 転送＆処理
	default:
		SUPER::MessageReceived(oMsg);
	}
}
//------------------------------------------------------------------------------
TTextData	TPropertyWindow::OriginalData() const
{
	return mPropertyView->OriginalData();
}
//------------------------------------------------------------------------------
bool	TPropertyWindow::QuitRequested()
{
	BMessage*	aCurMsg;

//	DBEXP("TPropertyWindow::QuitRequested()", "");
	aCurMsg = CurrentMessage();
	if ( aCurMsg != NULL ) {	// クローズボタンが押された
		dynamic_cast<TPropertyView*>(mPropertyView)->ButtonCancelClicked();
	}
	return true;
}
//------------------------------------------------------------------------------
void	TPropertyWindow::SetWindowTitle()
{
	SetTitle(_T(LKK_TITLE_PROPERTY_WIN));
}
//------------------------------------------------------------------------------
//==============================================================================
