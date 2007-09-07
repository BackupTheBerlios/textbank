#ifndef	___AsstSpy_H_
#define	___AsstSpy_H_
//------------------------------------------------------------------------------
// BeOS
#include <add-ons/input_server/InputServerFilter.h>
// C++
// Add2
// Proj
#include "AsstCommon.h"
#include "Common.h"
//------------------------------------------------------------------------------
class	TSpyThread;
//==============================================================================
/* ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
ホットキーを検知して Text Bank に送る。
ホットキーの環境設定変更時、input_server にメッセージを送っても InputServerFilter にはメッセージが届かない。
ポートを経由して TAsstSpy にメッセージを送る。
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''' */
class	TAsstSpy : public BInputServerFilter
{
public:
	TAsstSpy();
	virtual	~TAsstSpy();

	status_t				SetTextBankPrefs(const BMessage& oArc);

private:
	typedef	BInputServerFilter	SUPER;

	virtual filter_result	Filter(BMessage* oMsg, BList* oOutList);
	filter_result			KeyDown(BMessage* oMsg, BList* oOutList);
	status_t				LoadTextBankPrefs();

	TPrefs		mPrefs;
	TSpyThread*	mThread;
};
//==============================================================================
class	TSpyThread : public TAsstThread
{
public:
	TSpyThread(BInputServerFilter* oOwner);
	virtual	~TSpyThread();

private:
	typedef	TAsstThread	SUPER;

	virtual void	MessageReceived(BMessage* oMsg);
};
//==============================================================================
extern "C" _EXPORT BInputServerFilter*	instantiate_input_filter();
//==============================================================================
#endif	// ___AsstSpy_H_
