#include "TextBankView.h"
//------------------------------------------------------------------------------
// BeOS
#include <Locale.h>
#include <MenuItem.h>
// Add2
#include <add2/MiscDebug.h>
// Proj
#include "Constants.h"
#include "LKKConstants.h"
#include "TextBankShared.h"
#include "VPTextBankView.h"
//==============================================================================
TTextBankView::TTextBankView(BRect oFrame)
		: SUPER(oFrame, "TextBankView", B_FOLLOW_ALL, B_WILL_DRAW|B_FRAME_EVENTS)
{
	BRect	aFrameTmp;

	// ビュー作成
	aFrameTmp = Bounds();
	aFrameTmp.right -= B_V_SCROLL_BAR_WIDTH;
	aFrameTmp.bottom -= B_H_SCROLL_BAR_HEIGHT*2;
		// クリップボードビュー
	mClipboardView = new TClipboardView(aFrameTmp, this);
	mScrlViewClipboard = new BScrollView("ScrlViewClipboard", mClipboardView, B_FOLLOW_ALL, 0, true, true);
	AddChild(mScrlViewClipboard);
		// テンプレートビュー
	mTemplateView = new TTemplateView(aFrameTmp, this);
	mScrlViewTemplate = new BScrollView("ScrlViewTemplate", mTemplateView, B_FOLLOW_ALL, 0, true, true);
	AddChild(mScrlViewTemplate);
	mScrlViewTemplate->Hide();
		// ステータスバー
	aFrameTmp.Set(0, Bounds().bottom-B_H_SCROLL_BAR_HEIGHT+1, Bounds().right-B_V_SCROLL_BAR_WIDTH, Bounds().bottom);
	mStrViewStatBar = new BStringView(aFrameTmp, "StatBar", "", B_FOLLOW_LEFT_RIGHT|B_FOLLOW_BOTTOM);
	AddChild(mStrViewStatBar);
}
//------------------------------------------------------------------------------
TTextBankView::~TTextBankView()
{
}
//------------------------------------------------------------------------------
void	TTextBankView::ActivateClipboardView()
{
	if ( !mScrlViewTemplate->IsHidden() ) {
		mScrlViewTemplate->Hide();
	}
	if ( mScrlViewClipboard->IsHidden() ) {
		mScrlViewClipboard->Show();
		mClipboardView->MakeFocus();
	}
}
//------------------------------------------------------------------------------
void	TTextBankView::ActivateTemplateView()
{
	if ( !mScrlViewClipboard->IsHidden() ) {
		mScrlViewClipboard->Hide();
	}
	if ( mScrlViewTemplate->IsHidden() ) {
		mScrlViewTemplate->Show();
		mTemplateView->MakeFocus();
	}
}
//------------------------------------------------------------------------------
void	TTextBankView::AllAttached()
{
	mClipboardView->MakeFocus(true);
	mStrViewStatBar->SetViewColor(ui_color(B_UI_PANEL_BACKGROUND_COLOR));
	mStrViewStatBar->SetLowColor(ui_color(B_UI_PANEL_BACKGROUND_COLOR));
}
//------------------------------------------------------------------------------
void	TTextBankView::AttachedToWindow()
{
}
//------------------------------------------------------------------------------
void	TTextBankView::ExpStatBarRequested(BMessage* oMsg)
{
	BString		aText;

	if ( oMsg->FindString(MSG_NAME_STAT_BAR, &aText) != B_OK )
		return;
	mStrViewStatBar->SetText(aText.String());
}
//------------------------------------------------------------------------------
void	TTextBankView::LanguageChanged()
{
	mStrViewStatBar->SetText("");
}
//------------------------------------------------------------------------------
void	TTextBankView::MessageReceived(BMessage* oMsg)
{
	switch ( oMsg->what ) {
	// 転送
	case DELETE_TOP_HISTORY_REQUESTED:
	case PREF_NUM_CLIPBOARD_HISTORIES_CHANGED:
		BMessenger(mClipboardView).SendMessage(oMsg);
		break;
	case SAVE_AS_TEMPLATE_REQUESTED:
		BMessenger(mTemplateView).SendMessage(oMsg);
		break;
	// 自前処理
	case EXP_STAT_BAR_REQUESTED:
		ExpStatBarRequested(oMsg);
		break;
	case SHOW_POP_UP_REQUESTED:
		ShowPopUpRequested(oMsg);
		break;
	case TBUTTON_CLIPBOARD_CLICKED:
		TButtonClipboardClicked();
		break;
	case TBUTTON_TEMPLATE_CLICKED:
		TButtonTemplateClicked();
		break;
	default:
		SUPER::MessageReceived(oMsg);
	}
}
//------------------------------------------------------------------------------
void	TTextBankView::ShowPopUpRequested(BMessage* oMsg)
{
	uint32		aButtons;
	BPoint		aPoint;
	BPopUpMenu*	aPopMenu;

//	DBEXP("TTextBankView::ShowPopUpRequested()", "");
	aPopMenu = new BPopUpMenu("PopMenu", false, false);
	// 履歴
	aPopMenu->AddItem(mClipboardView->CreateHoldDataMenu(_T(LKK_POP_MITEM_CLIPBOARD)));
	// 倉庫
	aPopMenu->AddItem(mTemplateView->CreateHoldDataMenu(_T(LKK_POP_MITEM_TEMPLATE)));
	// 設定
	aPopMenu->SetAsyncAutoDestruct(true);
	// 表示
	GetMouse(&aPoint, &aButtons);
	mClipboardView->SetMousePointShowPopUp(ConvertToScreen(aPoint));
	mTemplateView->SetMousePointShowPopUp(ConvertToScreen(aPoint));
	aPoint += BPoint(-3, 1);	// マウスのところだとマウスの影響でうまくキー操作できないため
	ConvertToScreen(&aPoint);
	aPopMenu->Go(aPoint, true, true, true);
}
//------------------------------------------------------------------------------
void	TTextBankView::TButtonClipboardClicked()
{
	ActivateClipboardView();
}
//------------------------------------------------------------------------------
void	TTextBankView::TButtonTemplateClicked()
{
	ActivateTemplateView();
}
//------------------------------------------------------------------------------
//==============================================================================
