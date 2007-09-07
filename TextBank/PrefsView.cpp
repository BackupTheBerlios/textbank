#include "PrefsView.h"
//------------------------------------------------------------------------------
// BeOS
#include <Application.h>
#include <Locale.h>
#include <Messenger.h>
// C++
// Add2
#include <add2/MiscDebug.h>
#include <add2/MiscUtils.h>
// Proj
#include "Constants.h"
#include "LKKConstants.h"
//==============================================================================
TPrefsView::TPrefsView(BMessage* oArc)
		: SUPER(oArc)
{
//	BRect			aFrame;

	// メンバ変数初期化
	GetChildPointers();
	// ビューの設定
		// 全般タブ
	mTxtCtrlNumClipboardHistories->SetModificationMessage(new BMessage(TXT_CTRL_NUM_CLIPBOARD_HISTORIES_CHANGED));
	mCBoxSaveClipboard->SetMessage(new BMessage(CBOX_SAVE_CLIPBOARD_CLICKED));
	mCBoxLoseFocus->SetMessage(new BMessage(CBOX_LOSE_FOCUS_CLICKED));
	mCBoxMinimizedBoot->SetMessage(new BMessage(CBOX_MINIMIZED_BOOT_CLICKED));
	mCBoxLockTemplateOrder->SetMessage(new BMessage(CBOX_LOCK_TEMPLATE_ORDER_CLICKED));
		// ホットキータブ
			// Activate
	mCBoxActivateShift->SetMessage(new BMessage(CBOX_ACTIVATE_CLICKED));
	mCBoxActivateCtrl->SetMessage(new BMessage(CBOX_ACTIVATE_CLICKED));
	mCBoxActivateAlt->SetMessage(new BMessage(CBOX_ACTIVATE_CLICKED));
	mTxtCtrlActivateChar->SetModificationMessage(new BMessage(TXT_CTRL_ACTIVATE_CHAR_CHANGED));
			// Delete
	mCBoxDeleteShift->SetMessage(new BMessage(CBOX_DELETE_CLICKED));
	mCBoxDeleteCtrl->SetMessage(new BMessage(CBOX_DELETE_CLICKED));
	mCBoxDeleteAlt->SetMessage(new BMessage(CBOX_DELETE_CLICKED));
	mTxtCtrlDeleteChar->SetModificationMessage(new BMessage(TXT_CTRL_DELETE_CHAR_CHANGED));
			// Pop Up
	mCBoxPopUpShift->SetMessage(new BMessage(CBOX_POP_UP_CLICKED));
	mCBoxPopUpCtrl->SetMessage(new BMessage(CBOX_POP_UP_CLICKED));
	mCBoxPopUpAlt->SetMessage(new BMessage(CBOX_POP_UP_CLICKED));
	mTxtCtrlPopUpChar->SetModificationMessage(new BMessage(TXT_CTRL_POP_UP_CHAR_CHANGED));
		// アシスタントタブ
	mRButtonEnableAutoInstallAsst->SetMessage(new BMessage(RBUTTON_AUTO_INSTALL_ASST_CLICKED));
	mRButtonDisableAutoInstallAsst->SetMessage(new BMessage(RBUTTON_AUTO_INSTALL_ASST_CLICKED));
	mButtonInstallAsst->SetMessage(new BMessage(BUTTON_INSTALL_ASST_CLICKED));
	mButtonUninstallAsst->SetMessage(new BMessage(BUTTON_UNINSTALL_ASST_CLICKED));
		// ボタン
	mButtonHelp->SetMessage(new BMessage(FUC_BUTTON_HELP_CLICKED));
	mButtonDefaults->SetMessage(new BMessage(FUC_BUTTON_DEFAULTS_CLICKED));
	mButtonCancel->SetMessage(new BMessage(FUC_BUTTON_CANCEL_CLICKED));
	mButtonOK->SetMessage(new BMessage(FUC_BUTTON_OK_CLICKED));
	//その他
	mPrevPrefs = gPrefs;
}
//------------------------------------------------------------------------------
TPrefsView::~TPrefsView()
{
}
//------------------------------------------------------------------------------
void	TPrefsView::AllAttached()
{
	// ビューの設定
	SetViewLabels();
	for ( int32 i = 0 ; i < PT_END ; i++ ) {
		SetTargetToThis(mTabViewPrefs->TabAt(i)->View());
	}
	UpdateCompos();
	mButtonOK->MakeDefault(true);
	SetTargetToThis(this);
	mTxtCtrlNumClipboardHistories->MakeFocus(true);
}
//------------------------------------------------------------------------------
void	TPrefsView::ButtonCancelClicked()
{
	TPrefs	aOldPrefs = gPrefs;

	Window()->Hide();
	// 元に戻す
	gPrefs = mPrevPrefs;
	gPrefs.SaveToFile();	// ポップアップメニューでの変更時に変更中の設定が保存されている場合があるため
	// 反映
	NotifyPrefDifferences(aOldPrefs, gPrefs);
	UpdateCompos();
}
//------------------------------------------------------------------------------
void	TPrefsView::ButtonDefaultsClicked()
{
	TPrefs	aOldPrefs = gPrefs;

	// 変更
	gPrefs = TPrefs();
	// 反映
	NotifyPrefDifferences(aOldPrefs, gPrefs);
	UpdateCompos();
}
//------------------------------------------------------------------------------
void	TPrefsView::ButtonHelpClicked()
{
	be_app_messenger.SendMessage(FUC_MITEM_HELP_CLICKED);
}
//------------------------------------------------------------------------------
void	TPrefsView::ButtonInstallAsstClicked()
{
	install_asst();
	run_asst(_T(LKK_ASK_RUN_ASST_INSTALL_NOW));
	mRButtonEnableAutoInstallAsst->SetValue(B_CONTROL_ON);
	mRButtonEnableAutoInstallAsst->Invoke();
	UpdateAsstInfo();
	UpdateAsstCompos();
}
//------------------------------------------------------------------------------
void	TPrefsView::ButtonOKClicked()
{
	Window()->Hide();
	gPrefs.SaveToFile();
	mPrevPrefs = gPrefs;
}
//------------------------------------------------------------------------------
void	TPrefsView::ButtonUninstallAsstClicked()
{
	uninstall_asst();
	mRButtonDisableAutoInstallAsst->SetValue(B_CONTROL_ON);
	mRButtonDisableAutoInstallAsst->Invoke();
	UpdateAsstInfo();
	UpdateAsstCompos();
}
//------------------------------------------------------------------------------
void	TPrefsView::CBoxLockTemplateOrderClicked()
{
	gPrefs.mLockTemplateOrder = mCBoxLockTemplateOrder->Value();
}
//------------------------------------------------------------------------------
void	TPrefsView::CBoxLoseFocusClicked()
{
	gPrefs.mLoseFocus = mCBoxLoseFocus->Value();
}
//------------------------------------------------------------------------------
void	TPrefsView::CBoxMinimizedBootClicked()
{
	gPrefs.mMinimizedBoot = mCBoxMinimizedBoot->Value();
}
//------------------------------------------------------------------------------
void	TPrefsView::CBoxSaveClipboardClicked()
{
	gPrefs.mSaveClipboard = mCBoxSaveClipboard->Value();
}
//------------------------------------------------------------------------------
void	TPrefsView::GetChildPointers()
{
	// タブ別に取得
		// （コンテナ）
	mTabViewPrefs = dynamic_cast<BTabView*>(FindView(VIEW_NAME_TAB_VIEW_PREFS));
	mTabGeneral = mTabViewPrefs->TabAt(PT_GENERAL);
	mTabHotKey = mTabViewPrefs->TabAt(PT_HOT_KEY);
	mTabAsst = mTabViewPrefs->TabAt(PT_ASST);
		// 全般タブ
	mBoxClipboard = dynamic_cast<BBox*>(mTabGeneral->View()->FindView(VIEW_NAME_BOX_CLIPBOARD));
	mTxtCtrlNumClipboardHistories = dynamic_cast<BTextControl*>(mTabGeneral->View()->FindView(VIEW_NAME_TXT_CTRL_NUM_CLIPBOARD_HISTORIES));
	mCBoxSaveClipboard = dynamic_cast<BCheckBox*>(mTabGeneral->View()->FindView(VIEW_NAME_CBOX_SAVE_CLIPBOARD));
	mCBoxLoseFocus = dynamic_cast<BCheckBox*>(mTabGeneral->View()->FindView(VIEW_NAME_CBOX_LOSE_FOCUS));
	mCBoxMinimizedBoot = dynamic_cast<BCheckBox*>(mTabGeneral->View()->FindView(VIEW_NAME_CBOX_MINIMIZED_BOOT));
	mCBoxLockTemplateOrder = dynamic_cast<BCheckBox*>(mTabGeneral->View()->FindView(VIEW_NAME_CBOX_LOCK_TEMPLATE_ORDER));
		// ホットキータブ
	mBoxActivate = dynamic_cast<BBox*>(mTabHotKey->View()->FindView(VIEW_NAME_BOX_ACTIVATE));
	mCBoxActivateShift = dynamic_cast<BCheckBox*>(mTabHotKey->View()->FindView(VIEW_NAME_CBOX_ACTIVATE_SHIFT));
	mCBoxActivateCtrl = dynamic_cast<BCheckBox*>(mTabHotKey->View()->FindView(VIEW_NAME_CBOX_ACTIVATE_CTRL));
	mCBoxActivateAlt = dynamic_cast<BCheckBox*>(mTabHotKey->View()->FindView(VIEW_NAME_CBOX_ACTIVATE_ALT));
	mTxtCtrlActivateChar = dynamic_cast<BTextControl*>(mTabHotKey->View()->FindView(VIEW_NAME_TXT_CTRL_ACTIVATE_CHAR));
	mBoxDelete = dynamic_cast<BBox*>(mTabHotKey->View()->FindView(VIEW_NAME_BOX_DELETE));
	mCBoxDeleteShift = dynamic_cast<BCheckBox*>(mTabHotKey->View()->FindView(VIEW_NAME_CBOX_DELETE_SHIFT));
	mCBoxDeleteCtrl = dynamic_cast<BCheckBox*>(mTabHotKey->View()->FindView(VIEW_NAME_CBOX_DELETE_CTRL));
	mCBoxDeleteAlt = dynamic_cast<BCheckBox*>(mTabHotKey->View()->FindView(VIEW_NAME_CBOX_DELETE_ALT));
	mTxtCtrlDeleteChar = dynamic_cast<BTextControl*>(mTabHotKey->View()->FindView(VIEW_NAME_TXT_CTRL_DELETE_CHAR));
	mBoxPopUp = dynamic_cast<BBox*>(mTabHotKey->View()->FindView(VIEW_NAME_BOX_POP_UP));
	mCBoxPopUpShift = dynamic_cast<BCheckBox*>(mTabHotKey->View()->FindView(VIEW_NAME_CBOX_POP_UP_SHIFT));
	mCBoxPopUpCtrl = dynamic_cast<BCheckBox*>(mTabHotKey->View()->FindView(VIEW_NAME_CBOX_POP_UP_CTRL));
	mCBoxPopUpAlt = dynamic_cast<BCheckBox*>(mTabHotKey->View()->FindView(VIEW_NAME_CBOX_POP_UP_ALT));
	mTxtCtrlPopUpChar = dynamic_cast<BTextControl*>(mTabHotKey->View()->FindView(VIEW_NAME_TXT_CTRL_POP_UP_CHAR));
	mStrViewHintUsingHotKey = dynamic_cast<BStringView*>(mTabHotKey->View()->FindView(VIEW_NAME_STR_VIEW_HINT_USING_HOT_KEY));
		// アシスタントタブ
	mBoxAutoInstallAsst = dynamic_cast<BBox*>(mTabAsst->View()->FindView(VIEW_NAME_BOX_AUTO_INSTALL_ASST));
	mRButtonEnableAutoInstallAsst = dynamic_cast<BRadioButton*>(mTabAsst->View()->FindView(VIEW_NAME_RBUTTON_ENABLE_AUTO_INSTALL_ASST));
	mRButtonDisableAutoInstallAsst = dynamic_cast<BRadioButton*>(mTabAsst->View()->FindView(VIEW_NAME_RBUTTON_DISABLE_AUTO_INSTALL_ASST));
	mButtonInstallAsst = dynamic_cast<BButton*>(mTabAsst->View()->FindView(VIEW_NAME_BUTTON_INSTALL_ASST));
	mButtonUninstallAsst = dynamic_cast<BButton*>(mTabAsst->View()->FindView(VIEW_NAME_BUTTON_UNINSTALL_ASST));
	mStrViewAsstStatus = dynamic_cast<BStringView*>(mTabAsst->View()->FindView(VIEW_NAME_STR_VIEW_ASST_STATUS));
	// ボタン
	mButtonHelp = dynamic_cast<BButton*>(FindView(VIEW_NAME_BUTTON_HELP));
	mButtonDefaults = dynamic_cast<BButton*>(FindView(VIEW_NAME_BUTTON_DEFAULTS));
	mButtonCancel = dynamic_cast<BButton*>(FindView(VIEW_NAME_BUTTON_CANCEL));
	mButtonOK = dynamic_cast<BButton*>(FindView(VIEW_NAME_BUTTON_OK));
}
//------------------------------------------------------------------------------
void	TPrefsView::HotKeyActivateChanged()
{
	BMessage	aSendMsg(PREF_HOT_KEY_CHANGED);

//	DBEXP("TPrefsView::HotKeyActivateChanged()", "");
	gPrefs.mActivateModifiers = mCBoxActivateShift->Value()*B_SHIFT_KEY | mCBoxActivateCtrl->Value()*B_COMMAND_KEY | mCBoxActivateAlt->Value()*B_CONTROL_KEY;
	gPrefs.mActivateChar = tolower(mTxtCtrlActivateChar->Text()[0]);
	gPrefs.Archive(&aSendMsg);
	send_message_to_assistant(PORT_NAME_ASST_SPY, aSendMsg);
}
//------------------------------------------------------------------------------
void	TPrefsView::HotKeyDeleteChanged()
{
	BMessage	aSendMsg(PREF_HOT_KEY_CHANGED);

	gPrefs.mDeleteModifiers = mCBoxDeleteShift->Value()*B_SHIFT_KEY | mCBoxDeleteCtrl->Value()*B_COMMAND_KEY | mCBoxDeleteAlt->Value()*B_CONTROL_KEY;
	gPrefs.mDeleteChar = tolower(mTxtCtrlDeleteChar->Text()[0]);
	gPrefs.Archive(&aSendMsg);
	send_message_to_assistant(PORT_NAME_ASST_SPY, aSendMsg);
}
//------------------------------------------------------------------------------
void	TPrefsView::HotKeyPopUpChanged()
{
	BMessage	aSendMsg(PREF_HOT_KEY_CHANGED);

	gPrefs.mPopUpModifiers = mCBoxPopUpShift->Value()*B_SHIFT_KEY | mCBoxPopUpCtrl->Value()*B_COMMAND_KEY | mCBoxPopUpAlt->Value()*B_CONTROL_KEY;
	gPrefs.mPopUpChar = tolower(mTxtCtrlPopUpChar->Text()[0]);
	gPrefs.Archive(&aSendMsg);
	send_message_to_assistant(PORT_NAME_ASST_SPY, aSendMsg);
}
//------------------------------------------------------------------------------
void	TPrefsView::IAPChanged(BMessage* oMsg)
{
	SetViewLabels();
}
//------------------------------------------------------------------------------
void	TPrefsView::LanguageChanged()
{
	SetViewLabels();
	mTabViewPrefs->Invalidate();	// タブが更新されないため
}
//------------------------------------------------------------------------------
void	TPrefsView::MessageReceived(BMessage* oMsg)
{
	switch ( oMsg->what ) {
	case FUC_BUTTON_CANCEL_CLICKED:
		ButtonCancelClicked();
		break;
	case FUC_BUTTON_DEFAULTS_CLICKED:
		ButtonDefaultsClicked();
		break;
	case FUC_BUTTON_HELP_CLICKED:
		ButtonHelpClicked();
		break;
	case FUC_BUTTON_OK_CLICKED:
		ButtonOKClicked();
		break;
	case BUTTON_INSTALL_ASST_CLICKED:
		ButtonInstallAsstClicked();
		break;
	case BUTTON_UNINSTALL_ASST_CLICKED:
		ButtonUninstallAsstClicked();
		break;
	case CBOX_ACTIVATE_CLICKED:
	case TXT_CTRL_ACTIVATE_CHAR_CHANGED:
		HotKeyActivateChanged();
		break;
	case CBOX_DELETE_CLICKED:
	case TXT_CTRL_DELETE_CHAR_CHANGED:
		HotKeyDeleteChanged();
		break;
	case CBOX_LOCK_TEMPLATE_ORDER_CLICKED:
		CBoxLockTemplateOrderClicked();
		break;
	case CBOX_LOSE_FOCUS_CLICKED:
		CBoxLoseFocusClicked();
		break;
	case CBOX_MINIMIZED_BOOT_CLICKED:
		CBoxMinimizedBootClicked();
		break;
	case CBOX_POP_UP_CLICKED:
	case TXT_CTRL_POP_UP_CHAR_CHANGED:
		HotKeyPopUpChanged();
		break;
	case CBOX_SAVE_CLIPBOARD_CLICKED:
		CBoxSaveClipboardClicked();
		break;
	case PREF_LOCK_TEMPLATE_ORDER_CHANGED:
		PrefLockTemplateOrderChanged();
		break;
	case RBUTTON_AUTO_INSTALL_ASST_CLICKED:
		RButtonAutoInstallAsstClicked();
		break;
	case TXT_CTRL_NUM_CLIPBOARD_HISTORIES_CHANGED:
		TxtCtrlNumClipboardHistoriesChanged();
		break;
	default:
		SUPER::MessageReceived(oMsg);
	}
}
//------------------------------------------------------------------------------
/* ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
設定のキャンセルを反映するために、設定変更のメッセージを飛ばす
UpdateCompos() によって設定の変更が通知される項目については、この関数でメッセージを飛ばす必要はない
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''' */
void	TPrefsView::NotifyPrefDifferences(const TPrefs& oOldPrefs, const TPrefs& oNewPrefs)
{
	list<uint32>	aWhats;
	list<uint32>::iterator	p;
	BMessage		aSendMsg;

	// 直ちに効果を反映しなくてはならない設定を検索
	// 発効
	p = aWhats.begin();
	while ( p != aWhats.end() ) {
		aSendMsg = BMessage(*p);
		dynamic_cast<TDialogWindow*>(Window())->Target().SendMessage(&aSendMsg);
		p++;
	}
}
//------------------------------------------------------------------------------
void	TPrefsView::PrefLockTemplateOrderChanged()
{
	UpdateCBoxLockTemplateOrder();
	mPrevPrefs.mLockTemplateOrder = gPrefs.mLockTemplateOrder;
}
//------------------------------------------------------------------------------
void	TPrefsView::RButtonAutoInstallAsstClicked()
{
	DBEXP("TPrefsView::RButtonAutoInstallAsstClicked()", "");
	gPrefs.mAutoInstallAsst = mRButtonEnableAutoInstallAsst->Value();
}
//------------------------------------------------------------------------------
void	TPrefsView::SetViewLabels()
{
	// 全般タブ
	mTabGeneral->View()->SetName(_T(LKK_TAB_GENERAL));
	mBoxClipboard->SetLabel(_T(LKK_BOX_CLIPBOARD));
	mTxtCtrlNumClipboardHistories->SetLabel((acc_label("I", _T(LKK_TXT_CTRL_NUM_CLIPBOARD_HISTORIES))+_T(LKK_COLON_SPACE)).c_str());
	adjust_txt_ctrl_divider(mTxtCtrlNumClipboardHistories);
	mTxtCtrlNumClipboardHistories->SetAlignment(B_ALIGN_LEFT, B_ALIGN_RIGHT);	// これをやらないと、違うタブにフォーカスがある場合に入力位置がおかしくなる
	mCBoxSaveClipboard->SetLabel(acc_label("S", _T(LKK_CBOX_SAVE_CLIPBOARD)).c_str());
	mCBoxLoseFocus->SetLabel(acc_label("L", _T(LKK_CBOX_LOSE_FOCUS)).c_str());
	mCBoxMinimizedBoot->SetLabel(acc_label("M", _T(LKK_CBOX_MINIMIZED_BOOT)).c_str());
	mCBoxLockTemplateOrder->SetLabel(acc_label("T", _T(LKK_CBOX_LOCK_TEMPLATE_ORDER)).c_str());
	// ホットキータブ
	mTabHotKey->View()->SetName(_T(LKK_TAB_HOT_KEY));
	mBoxActivate->SetLabel(BString(_T(LKK_BOX_ACTIVATE)).ReplaceAll(LKR_APP_NAME, _T(gAppNameEnUs)).String());
	mTxtCtrlActivateChar->SetLabel((string(_T(LKK_TXT_CTRL_HOT_KEY_CHAR))+_T(LKK_COLON_SPACE)).c_str());
	adjust_txt_ctrl_divider(mTxtCtrlActivateChar);
	mBoxDelete->SetLabel(_T(LKK_BOX_DELETE));
	mTxtCtrlDeleteChar->SetLabel((string(_T(LKK_TXT_CTRL_HOT_KEY_CHAR))+_T(LKK_COLON_SPACE)).c_str());
	adjust_txt_ctrl_divider(mTxtCtrlDeleteChar);
	mBoxPopUp->SetLabel(_T(LKK_BOX_POP_UP));
	mTxtCtrlPopUpChar->SetLabel((string(_T(LKK_TXT_CTRL_HOT_KEY_CHAR))+_T(LKK_COLON_SPACE)).c_str());
	adjust_txt_ctrl_divider(mTxtCtrlPopUpChar);
	mStrViewHintUsingHotKey->SetText(_T(LKK_STR_VIEW_HINT_USING_HOT_KEY));
	// アシスタントタブ
	mTabAsst->View()->SetName(_T(LKK_TAB_ASST));
	mBoxAutoInstallAsst->SetLabel(_T(LKK_BOX_AUTO_INSTALL_ASST));
	mRButtonEnableAutoInstallAsst->SetLabel(acc_label("E", _T(LKK_RBUTTON_ENABLE_AUTO_INSTALL_ASST)).c_str());
	mRButtonDisableAutoInstallAsst->SetLabel(acc_label("S", _T(LKK_RBUTTON_DISABLE_AUTO_INSTALL_ASST)).c_str());
	mButtonInstallAsst->SetLabel(acc_label("I", _T(LKK_BUTTON_INSTALL_ASST)).c_str());
	mButtonUninstallAsst->SetLabel(acc_label("U", _T(LKK_BUTTON_UNINSTALL_ASST)).c_str());
	UpdateAsstInfo();
	// ボタン
	mButtonHelp->SetLabel(acc_label("F1", _T(LKK_BUTTON_HELP)).c_str());
	mButtonDefaults->SetLabel(acc_label("D", _T(LKK_BUTTON_DEFAULTS)).c_str());
	mButtonCancel->SetLabel(_T(LKK_BUTTON_CANCEL));
	mButtonOK->SetLabel(_T(LKK_BUTTON_OK));
}
//------------------------------------------------------------------------------
void	TPrefsView::ShortCutDown(int32 oChar, int32 oFunc)
{
	switch ( oChar ) {
	case B_FUNCTION_KEY:
		if ( oFunc == B_F1_KEY ) {
			click_button(mButtonHelp);
		}
		break;
	default:
		SUPER::ShortCutDown(oChar, oFunc);
	}
}
//------------------------------------------------------------------------------
void	TPrefsView::TxtCtrlNumClipboardHistoriesChanged()
{
	BMessage	aSendMsg(PREF_NUM_CLIPBOARD_HISTORIES_CHANGED);

	gPrefs.mNumClipboardHistories = atoi(mTxtCtrlNumClipboardHistories->Text());
	adjust_value(&gPrefs.mNumClipboardHistories, MIX_NUM_CLIPBOARD_HISTORIES, MAX_NUM_CLIPBOARD_HISTORIES);
	dynamic_cast<TDialogWindow*>(Window())->Target().SendMessage(&aSendMsg);
}
//------------------------------------------------------------------------------
void	TPrefsView::UpdateAsstCompos()
{
	mButtonInstallAsst->SetEnabled(!is_asst_installed());
	mButtonUninstallAsst->SetEnabled(is_asst_installed());
}
//------------------------------------------------------------------------------
void	TPrefsView::UpdateAsstInfo()
{
	const char*	aKey = NULL;

	switch ( is_asst_installed()*2+is_asst_running() ) {
	case 0:
		aKey = LKK_ASST_INFO_UNINST_NORUN;
		break;
	case 1:
		aKey = LKK_ASST_INFO_UNINST_RUN;
		break;
	case 2:
		aKey = LKK_ASST_INFO_INST_NORUN;
		break;
	case 3:
		aKey = LKK_ASST_INFO_INST_RUN;
		break;
	}
	mStrViewAsstStatus->SetText(_T(aKey));
}
//------------------------------------------------------------------------------
void	TPrefsView::UpdateCBoxLockTemplateOrder()
{
	mCBoxLockTemplateOrder->SetValue(gPrefs.mLockTemplateOrder);
}
//------------------------------------------------------------------------------
void	TPrefsView::UpdateCompos()
{
	// 全般タブ
	set_txt_ctrl_num(mTxtCtrlNumClipboardHistories, gPrefs.mNumClipboardHistories);
	mCBoxSaveClipboard->SetValue(gPrefs.mSaveClipboard);
	mCBoxLoseFocus->SetValue(gPrefs.mLoseFocus);
	mCBoxMinimizedBoot->SetValue(gPrefs.mMinimizedBoot);
	UpdateCBoxLockTemplateOrder();
	// ホットキータブ
	mCBoxActivateShift->SetValue((gPrefs.mActivateModifiers&B_SHIFT_KEY) != 0);
	mCBoxActivateCtrl->SetValue((gPrefs.mActivateModifiers&B_COMMAND_KEY) != 0);
	mCBoxActivateAlt->SetValue((gPrefs.mActivateModifiers&B_CONTROL_KEY) != 0);
	mTxtCtrlActivateChar->SetText(string(1, static_cast<int32>(toupper(gPrefs.mActivateChar))).c_str());
	mCBoxDeleteShift->SetValue((gPrefs.mDeleteModifiers&B_SHIFT_KEY) != 0);
	mCBoxDeleteCtrl->SetValue((gPrefs.mDeleteModifiers&B_COMMAND_KEY) != 0);
	mCBoxDeleteAlt->SetValue((gPrefs.mDeleteModifiers&B_CONTROL_KEY) != 0);
	mTxtCtrlDeleteChar->SetText(string(1, static_cast<int32>(toupper(gPrefs.mDeleteChar))).c_str());
	mCBoxPopUpShift->SetValue((gPrefs.mPopUpModifiers&B_SHIFT_KEY) != 0);
	mCBoxPopUpCtrl->SetValue((gPrefs.mPopUpModifiers&B_COMMAND_KEY) != 0);
	mCBoxPopUpAlt->SetValue((gPrefs.mPopUpModifiers&B_CONTROL_KEY) != 0);
	mTxtCtrlPopUpChar->SetText(string(1, static_cast<int32>(toupper(gPrefs.mPopUpChar))).c_str());
	// アシスタントタブ
	if ( gPrefs.mAutoInstallAsst ) {
		mRButtonEnableAutoInstallAsst->SetValue(B_CONTROL_ON);
	} else {
		mRButtonDisableAutoInstallAsst->SetValue(B_CONTROL_ON);
	}
	UpdateAsstCompos();
	UpdateAsstInfo();
}
//------------------------------------------------------------------------------
//==============================================================================

