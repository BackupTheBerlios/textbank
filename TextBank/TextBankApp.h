#ifndef ___TextBankApp_H_
#define ___TextBankApp_H_
//------------------------------------------------------------------------------
// BeOS
#include <Application.h>
#include <SupportDefs.h>
// Add2
#include <add2/SHINTAAbout.h>
// Proj
//------------------------------------------------------------------------------
class	TPrefsWindow;
class	TTextBankWindow;
//==============================================================================
class	TTextBankApp : public BApplication
{
public:
	TTextBankApp();
	virtual	~TTextBankApp();

protected:
private:
	typedef	BApplication	SUPER;						// スーパークラス

	virtual void	AboutRequested();
	void			ActivateWindowRequested();
	void			IAPChanged(BMessage* oMsg);
	virtual void	MessageReceived(BMessage* oMsg);
	void			PopMItemAuthorWebClicked();
	void			PopMItemHelpClicked();
	void			PopMItemPrefsClicked();
	void			PopMItemSHINTACommonPrefsClicked();
	virtual void	Quit();
	virtual bool	QuitRequested();
	virtual void	ReadyToRun();
	void			SendMessageToPropertyWindows(BMessage* oMsg);

	TPrefsWindow*			mPrefsWin;
	TTextBankWindow*		mTextBankWin;
	TSHINTAAboutWindow*		mAboutWin;			// バージョン情報ダイアログ
};
//==============================================================================
int		main(int oArgC, char* oArgV[]);			// メインルーチン
//==============================================================================
#endif	// ___TextBankApp_H_
