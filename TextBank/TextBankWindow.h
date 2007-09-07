#ifndef	___TextBankWindow_H_
#define	___TextBankWindow_H_
//------------------------------------------------------------------------------
// BeOS
#include <MenuBar.h>
// Add
#include <add2/GUIBase.h>
#include <add2/ToolBar.h>
// Proj
//------------------------------------------------------------------------------
class	TTextBankView;
//==============================================================================
class	TTextBankWindow : public TBasicWindow
{
public:
	TTextBankWindow(BRect oFrame);
	virtual	~TTextBankWindow();

	virtual void	Quit();
	virtual bool	QuitRequested();
protected:
private:
	typedef	TBasicWindow	SUPER;	// スーパークラス

	void			ActivateClipboardViewRequested();
	void			ActivateTemplateViewRequested();
	void			CreateToolBar();
	void			DestroyToolBar();
	virtual void	IAPChanged(BMessage* oMsg);
	virtual void	LanguageChanged();
	virtual void	MessageReceived(BMessage* oMsg);
	void			SetToolBarLabels();
	void			SetToolBarStyle();
	void			SetWinTitle();				// タイトルの設定

	TTextBankView*	mTextBankView;		// メインビュー
	TToolBar*		mToolBar;
	TToolButton*	mTButtonClipboard;
	TToolButton*	mTButtonTemplate;
};
//==============================================================================
#endif	// ___TextBankWindow_H_
