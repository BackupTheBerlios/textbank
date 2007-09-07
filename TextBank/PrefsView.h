#ifndef	___TextBankPrefsView_H_
#define	___TextBankPrefsView_H_
//------------------------------------------------------------------------------
// BeOS
#include <Box.h>
#include <Button.h>
#include <CheckBox.h>
#include <RadioButton.h>
#include <interface/StringView.h>
#include <interface/TabView.h>
#include <TextControl.h>
// Add2
#include <add2/GUIBase.h>
// Proj
#include "Common.h"
//==============================================================================
class	TPrefsView : public TDialogView
{
public:
	TPrefsView(BMessage* oArc);
	virtual	~TPrefsView();

	void			ButtonCancelClicked();

private:
	typedef	TDialogView	SUPER;

	virtual void	AllAttached();
	void			ButtonDefaultsClicked();
	void			ButtonHelpClicked();
	void			ButtonInstallAsstClicked();
	void			ButtonOKClicked();
	void			ButtonUninstallAsstClicked();
	void			CBoxLockTemplateOrderClicked();
	void			CBoxLoseFocusClicked();
	void			CBoxMinimizedBootClicked();
	void			CBoxSaveClipboardClicked();
	void			GetChildPointers();
	void			HotKeyActivateChanged();
	void			HotKeyDeleteChanged();
	void			HotKeyPopUpChanged();
	virtual void	IAPChanged(BMessage* oMsg);
	virtual void	LanguageChanged();
	virtual void	MessageReceived(BMessage* oMsg);
	void			NotifyPrefDifferences(const TPrefs& oOldPrefs, const TPrefs& oNewPrefs);
	void			PrefLockTemplateOrderChanged();
	void			RButtonAutoInstallAsstClicked();
	void			SetViewLabels();
	virtual void	ShortCutDown(int32 oChar, int32 oFunc);
	void			TxtCtrlNumClipboardHistoriesChanged();
	void			UpdateAsstCompos();
	void			UpdateAsstInfo();
	void			UpdateCBoxLockTemplateOrder();
	void			UpdateCompos();

	BBox*			mBoxActivate;
	BBox*			mBoxAutoInstallAsst;
	BBox*			mBoxClipboard;
	BBox*			mBoxDelete;
	BBox*			mBoxPopUp;
	BButton*		mButtonCancel;
	BButton*		mButtonDefaults;
	BButton*		mButtonHelp;
	BButton*		mButtonOK;
	BButton*		mButtonInstallAsst;
	BButton*		mButtonUninstallAsst;
	BCheckBox*		mCBoxActivateAlt;
	BCheckBox*		mCBoxActivateCtrl;
	BCheckBox*		mCBoxActivateShift;
	BCheckBox*		mCBoxDeleteAlt;
	BCheckBox*		mCBoxDeleteCtrl;
	BCheckBox*		mCBoxDeleteShift;
	BCheckBox*		mCBoxPopUpAlt;
	BCheckBox*		mCBoxPopUpCtrl;
	BCheckBox*		mCBoxPopUpShift;
	BCheckBox*		mCBoxLockTemplateOrder;
	BCheckBox*		mCBoxLoseFocus;
	BCheckBox*		mCBoxMinimizedBoot;
	BCheckBox*		mCBoxSaveClipboard;
	BRadioButton*	mRButtonDisableAutoInstallAsst;
	BRadioButton*	mRButtonEnableAutoInstallAsst;
	BStringView*	mStrViewAsstStatus;
	BStringView*	mStrViewHintUsingHotKey;
	BTab*			mTabAsst;
	BTab*			mTabGeneral;
	BTab*			mTabHotKey;
	BTabView*		mTabViewPrefs;
	BTextControl*	mTxtCtrlActivateChar;
	BTextControl*	mTxtCtrlDeleteChar;
	BTextControl*	mTxtCtrlPopUpChar;
	BTextControl*	mTxtCtrlNumClipboardHistories;
	TPrefs			mPrevPrefs;					// キャンセル用
};
//==============================================================================
#endif	// ___TextBankPrefsView_H_
