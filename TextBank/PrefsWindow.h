#ifndef	___TextBankPrefsWindow_H_
#define	___TextBankPrefsWindow_H_
//------------------------------------------------------------------------------
// Add2
#include <add2/GUIBase.h>
// Proj
//------------------------------------------------------------------------------
class	TPrefsView;
//==============================================================================
class	TPrefsWindow : public TDialogWindow
{
public:
	TPrefsWindow(BRect oFrame);
	~TPrefsWindow();

	status_t		InitCheck() const;				// きちんと生成できたか

protected:
private:
	typedef	TDialogWindow	SUPER;

	virtual void	LanguageChanged();
	virtual void	MessageReceived(BMessage* oMsg);
	virtual bool	QuitRequested();
	void			SetWindowTitle();				// ウィンドウタイトル設定

	status_t	mInitCheck;		// きちんと生成できたか
	TPrefsView*	mPrefsView;		//メインビュー
};
//==============================================================================
#endif	// ___TextBankPrefsWindow_H_
