#include "PropertyView.h"
//------------------------------------------------------------------------------
// BeOS
#include <Locale.h>
// C++
// Add2
#include <add2/FUConstants.h>
#include <add2/MiscDebug.h>
// Proj
#include "Constants.h"
#include "LKKConstants.h"
//==============================================================================
TPropertyView::TPropertyView(BMessage* oArc, const TTextData& oData)
		: SUPER(oArc)
{
	// メンバ変数初期化
	GetChildPointers();
	mOrgData = oData;
	// ビューの設定
	mTxtCtrlTitle->SetModificationMessage(new BMessage(TXT_CTRL_TITLE_CHANGED));
	mCBoxPatternMatch->SetMessage(new BMessage(CBOX_PATTERN_MATCH_CLICKED));
	mButtonCancel->SetMessage(new BMessage(FUC_BUTTON_CANCEL_CLICKED));
	mButtonOK->SetMessage(new BMessage(FUC_BUTTON_OK_CLICKED));
	//その他
}
//------------------------------------------------------------------------------
TPropertyView::~TPropertyView()
{
}
//------------------------------------------------------------------------------
void	TPropertyView::AllAttached()
{
	// ビューの設定
	SetViewLabels();
	UpdateCompos();
	mButtonOK->MakeDefault(true);
	SetTargetToThis(this);
	mTxtCtrlTitle->MakeFocus(true);
}
//------------------------------------------------------------------------------
void	TPropertyView::ButtonCancelClicked()
{
	BMessage	aSendMsg(B_QUIT_REQUESTED);

//	DBEXP("TPropertyView::ButtonCancelClicked()", "");
	NotifyProperty(mOrgData, true);	// 他のプロパティウィンドウからの指令などで、中途半端な状態のプロパティが保存されている可能性があるので、オリジナルを保存し直す
	BMessenger(static_cast<BWindow*>(Window())).SendMessage(&aSendMsg);
}
//------------------------------------------------------------------------------
void	TPropertyView::ButtonOKClicked()
{
	TTextData	aNewData = mOrgData;

	aNewData.mTitle = mTxtCtrlTitle->Text();
	aNewData.mPatternMatch = mCBoxPatternMatch->Value();
	NotifyProperty(aNewData, true);
	Window()->Quit();	// メッセージ経由だと ButtonCancelClicked() が呼ばれてしまうので直接終了する
}
//------------------------------------------------------------------------------
void	TPropertyView::GetChildPointers()
{
	// タイトル
	mTxtCtrlTitle = dynamic_cast<BTextControl*>(FindView(VIEW_NAME_TXT_CTRL_TITLE));
	// テキスト
	mStrViewTextCaption = dynamic_cast<BStringView*>(FindView(VIEW_NAME_STR_VIEW_TEXT_CAPTION));
	mStrViewText = dynamic_cast<BStringView*>(FindView(VIEW_NAME_STR_VIEW_TEXT));
	// パターンマッチ
	mCBoxPatternMatch = dynamic_cast<BCheckBox*>(FindView(VIEW_NAME_CBOX_PATTERN_MATCH));
	// ボタン
	mButtonCancel = dynamic_cast<BButton*>(FindView(VIEW_NAME_BUTTON_CANCEL));
	mButtonOK = dynamic_cast<BButton*>(FindView(VIEW_NAME_BUTTON_OK));
}
//------------------------------------------------------------------------------
void	TPropertyView::IAPChanged(BMessage* oMsg)
{
	SetViewLabels();
}
//------------------------------------------------------------------------------
void	TPropertyView::LanguageChanged()
{
	SetViewLabels();
}
//------------------------------------------------------------------------------
void	TPropertyView::MessageReceived(BMessage* oMsg)
{
	switch ( oMsg->what ) {
	case FUC_BUTTON_CANCEL_CLICKED:
		ButtonCancelClicked();
		break;
	case FUC_BUTTON_OK_CLICKED:
		ButtonOKClicked();
		break;
	case CBOX_PATTERN_MATCH_CLICKED:
	case TXT_CTRL_TITLE_CHANGED:
		PropertyChanged();
		break;
	default:
		SUPER::MessageReceived(oMsg);
	}
}
//------------------------------------------------------------------------------
void	TPropertyView::NotifyProperty(const TTextData& oData, bool oNeedSave) const
{
	BMessage	aArc;
	BMessage	aSendMsg(DATA_PROPERTY_CHANGED);

	oData.Archive(&aArc);
	aSendMsg.AddMessage(MSG_NAME_DATA, &aArc);
	aSendMsg.AddBool(MSG_NAME_NEED_SAVE, oNeedSave);
	dynamic_cast<TDialogWindow*>(Window())->Target().SendMessage(&aSendMsg);
}
//------------------------------------------------------------------------------
TTextData	TPropertyView::OriginalData() const
{
	return mOrgData;
}
//------------------------------------------------------------------------------
void	TPropertyView::PropertyChanged()
{
	TTextData	aNewData = mOrgData;

	aNewData.mTitle = mTxtCtrlTitle->Text();
	aNewData.mPatternMatch = mCBoxPatternMatch->Value();
	NotifyProperty(aNewData, false);
}
//------------------------------------------------------------------------------
void	TPropertyView::SetViewLabels()
{
	// タイトル
	mTxtCtrlTitle->SetLabel(acc_label("T", string(_T(LKK_TXT_CTRL_TITLE))+_T(LKK_COLON_SPACE)).c_str());
	// テキスト
	mStrViewTextCaption->SetText((string(_T(LKK_STR_VIEW_TEXT_CAPTION))+_T(LKK_COLON_SPACE)).c_str());
	// パターンマッチ
	mCBoxPatternMatch->SetLabel(acc_label("R", _T(LKK_CBOX_PATTERN_MATCH)).c_str());
	// ボタン
	mButtonCancel->SetLabel(_T(LKK_BUTTON_CANCEL));
	mButtonOK->SetLabel(_T(LKK_BUTTON_OK));
}
//------------------------------------------------------------------------------
void	TPropertyView::UpdateCompos()
{
	const char*	aText;
	ssize_t		aSize;

	mTxtCtrlTitle->SetText(mOrgData.mTitle.c_str());
	if ( mOrgData.mGroup ) {
		mStrViewText->SetText(_T(LKK_STR_VIEW_TEXT_GROUP));
	} else {
		mOrgData.mData.FindData(FUC_MIME_TYPE_PLAIN_TEXT, B_MIME_TYPE, (const void**)&aText, &aSize);
		mStrViewText->SetText(string(aText, aSize).c_str());
	}
	mCBoxPatternMatch->SetEnabled(!mOrgData.mGroup);
	mCBoxPatternMatch->SetValue(mOrgData.mPatternMatch);
}
//------------------------------------------------------------------------------
//==============================================================================

