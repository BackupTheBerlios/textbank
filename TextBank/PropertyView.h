#ifndef	___PropertyView_H_
#define	___PropertyView_H_
//------------------------------------------------------------------------------
// BeOS
#include <Button.h>
#include <CheckBox.h>
#include <interface/StringView.h>
#include <TextControl.h>
// Add2
#include <add2/GUIBase.h>
// Proj
#include "Common.h"
//==============================================================================
class	TPropertyView : public TDialogView
{
public:
	TPropertyView(BMessage* oArc, const TTextData& oData);
	virtual	~TPropertyView();

	void			ButtonCancelClicked();
	TTextData		OriginalData() const;

private:
	typedef	TDialogView	SUPER;

	virtual void	AllAttached();
	void			ButtonOKClicked();
	void			GetChildPointers();
	virtual void	IAPChanged(BMessage* oMsg);
	virtual void	LanguageChanged();
	virtual void	MessageReceived(BMessage* oMsg);
	void			NotifyProperty(const TTextData& oData, bool oNeedSave) const;
	void			PropertyChanged();
	void			SetViewLabels();
	void			UpdateCompos();

	BButton*		mButtonCancel;
	BButton*		mButtonOK;
	BCheckBox*		mCBoxPatternMatch;
	BStringView*	mStrViewText;
	BStringView*	mStrViewTextCaption;
	BTextControl*	mTxtCtrlTitle;
	TTextData		mOrgData;		// 修正元検索用
};
//==============================================================================
#endif	// ___PropertyView_H_
