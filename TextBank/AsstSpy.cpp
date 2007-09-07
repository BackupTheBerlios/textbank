#include "AsstSpy.h"
//------------------------------------------------------------------------------
// BeOS
// Add2
#include <add2/BeConstants.h>
#include <add2/MiscDebug.h>
// Proj
#include "AsstConstants.h"
//==============================================================================
TAsstSpy::TAsstSpy()
		: SUPER()
{
//	DBEXP("TAsstSpy::TAsstSpy()", "");
	LoadTextBankPrefs();
	// スレッド開始
	mThread = new TSpyThread(this);
	mThread->Resume();
}
//------------------------------------------------------------------------------
TAsstSpy::~TAsstSpy()
{
	// スレッド終了
	mThread->Terminate();
	mThread->PreventBlocking();
	mThread->Wait();
	delete mThread;
	mThread = NULL;
}
//------------------------------------------------------------------------------
filter_result	TAsstSpy::Filter(BMessage* oMsg, BList* oOutList)
{
	filter_result	aResult;

	switch ( oMsg->what ) {
	case B_KEY_DOWN:
		aResult = KeyDown(oMsg, oOutList);
		break;
	default:
		aResult = B_DISPATCH_MESSAGE;
	}
	return aResult;
}
//------------------------------------------------------------------------------
filter_result	TAsstSpy::KeyDown(BMessage* oMsg, BList* oOutList)
{
	int32		aModifiers;
	int32		aRawChar;
	uint32		aSendWhat = 0;
	BMessage	aSendMsg;

	if ( oMsg->FindInt32(B_MSG_NAME_MODIFIERS, &aModifiers) != B_OK )
		return B_DISPATCH_MESSAGE;
	if ( oMsg->FindInt32(B_MSG_NAME_RAW_CHAR, &aRawChar) != B_OK )
		return B_DISPATCH_MESSAGE;
	// アクティブ化
	if ( (aModifiers & MASK_CHECK_MODIFIERS) == mPrefs.mActivateModifiers && aRawChar == mPrefs.mActivateChar ) {
		aSendWhat = ACTIVATE_WINDOW_REQUESTED;
	}
	// 先頭の履歴を削除
	if ( (aModifiers & MASK_CHECK_MODIFIERS) == mPrefs.mDeleteModifiers && aRawChar == mPrefs.mDeleteChar ) {
		aSendWhat = DELETE_TOP_HISTORY_REQUESTED;
	}
	// ポップアップメニューを表示
	if ( (aModifiers & MASK_CHECK_MODIFIERS) == mPrefs.mPopUpModifiers && aRawChar == mPrefs.mPopUpChar ) {
		aSendWhat = SHOW_POP_UP_REQUESTED;
	}
	if ( aSendWhat == 0 )
		return B_DISPATCH_MESSAGE;
	// Text Bank 本体に通知
	aSendMsg.what = aSendWhat;
	BMessenger(gAppSignature).SendMessage(&aSendMsg);
	return B_SKIP_MESSAGE;
}
//------------------------------------------------------------------------------
status_t	TAsstSpy::LoadTextBankPrefs()
{
//	DBEXP("TAsstSpy::LoadTextBankPrefs()", "");
	return mPrefs.LoadFromFile();
}
//------------------------------------------------------------------------------
status_t	TAsstSpy::SetTextBankPrefs(const BMessage& oArc)
{
//	DBEXP("TAsstSpy::SetTextBankPrefs()", "");
	return mPrefs.Unarchive(oArc);
}
//------------------------------------------------------------------------------
//==============================================================================
TSpyThread::TSpyThread(BInputServerFilter* oOwner)
		: SUPER(THREAD_NAME_ASST_SPY, PORT_NAME_ASST_SPY, oOwner)
{
}
//------------------------------------------------------------------------------
TSpyThread::~TSpyThread()
{
}
//------------------------------------------------------------------------------
void	TSpyThread::MessageReceived(BMessage* oMsg)
{
	switch ( oMsg->what ) {
	case PREF_HOT_KEY_CHANGED:
//		DBEXP("TSpyThread::MessageReceived", "PREF_HOT_KEY_CHANGED");
		reinterpret_cast<TAsstSpy*>(mOwner)->SetTextBankPrefs(*oMsg);
		break;
	default:
		;
//		DBEXP("TSpyThread::MessageReceived", "知らないメッセージ");
	}
}
//------------------------------------------------------------------------------
//==============================================================================
BInputServerFilter*	instantiate_input_filter()
{
	return new TAsstSpy();
}
//==============================================================================
