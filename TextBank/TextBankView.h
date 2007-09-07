#ifndef	___TextBankView_H_
#define	___TextBankView_H_
//------------------------------------------------------------------------------
// BeOS
#include <Menu.h>
#include <ScrollView.h>
#include <interface/StringView.h>
// Add2
#include <add2/GUIBase.h>
//------------------------------------------------------------------------------
class	TClipboardView;
class	TTemplateView;
//==============================================================================
class	TTextBankView : public TBasicView
{
public:
	TTextBankView(BRect oFrame);
	virtual	~TTextBankView();

private:
	typedef	TBasicView	SUPER;	// スーパークラス

	void			ActivateClipboardView();
	void			ActivateTemplateView();
	virtual void	AllAttached();
	virtual void	AttachedToWindow();
	void			ExpStatBarRequested(BMessage* oMsg);
	virtual void	LanguageChanged();
	virtual void	MessageReceived(BMessage* oMsg);
	void			ShowPopUpRequested(BMessage* oMsg);
	void			TButtonClipboardClicked();
	void			TButtonTemplateClicked();

	BScrollView*	mScrlViewClipboard;
	BScrollView*	mScrlViewTemplate;
	BStringView*	mStrViewStatBar;
	TClipboardView*	mClipboardView;
	TTemplateView*	mTemplateView;
};
//==============================================================================
#endif	// ___TextBankView_H_
