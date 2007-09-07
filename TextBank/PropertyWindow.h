#ifndef	___PropertyWindow_H_
#define	___PropertyWindow_H_
//------------------------------------------------------------------------------
// Add2
#include <add2/GUIBase.h>
// Proj
#include "Common.h"
//------------------------------------------------------------------------------
class	TPropertyView;
//==============================================================================
class	TPropertyWindow : public TDialogWindow
{
public:
	TPropertyWindow(BRect oFrame, const TTextData& oData);
	~TPropertyWindow();

	status_t		InitCheck() const;				// きちんと生成できたか
	TTextData		OriginalData() const;

protected:
private:
	typedef	TDialogWindow	SUPER;

	virtual void	LanguageChanged();
	virtual void	MessageReceived(BMessage* oMsg);
	virtual bool	QuitRequested();
	void			SetWindowTitle();				// ウィンドウタイトル設定

	status_t		mInitCheck;		// きちんと生成できたか
	TPropertyView*	mPropertyView;	//メインビュー
};
//==============================================================================
#endif	// ___PropertyWindow_H_
