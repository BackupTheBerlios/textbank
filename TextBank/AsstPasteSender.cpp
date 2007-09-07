#include "AsstPasteSender.h"
//------------------------------------------------------------------------------
// BeOS
// Add2
#include <add2/FUConstants.h>
#include <add2/MiscDebug.h>
// Proj
#include "AsstConstants.h"
#include "TextBankShared.h"
//==============================================================================
TAsstPasteSender::TAsstPasteSender()
		: SUPER()
{
	input_device_ref	aKeyboard = { DEVICE_NAME_ASST_SENDER, B_KEYBOARD_DEVICE, NULL };
	input_device_ref*	aDevRef[2];

//	DBEXP("TAsstPasteSender::TAsstPasteSender()", "");
	// 初期化
	mThread = NULL;
	// 登録
	aDevRef[0] = &aKeyboard;
	aDevRef[1] = NULL;
	RegisterDevices(aDevRef);
}
//------------------------------------------------------------------------------
TAsstPasteSender::~TAsstPasteSender()
{
}
//------------------------------------------------------------------------------
status_t	TAsstPasteSender::Start(const char *oDevice, void* oCookie)
{
	// ??:サンプルでは、Start/Stop の度にスレッドを作り直していた。でもその必要はあるの？
//	DBEXP("TAsstPasteSender::Start()", "");
	mThread = new TPasteSenderThread(this);
	mThread->Resume();
	return B_OK;
}
//------------------------------------------------------------------------------
status_t	TAsstPasteSender::Stop(const char *oDevice, void* oCookie)
{
//	DBEXP("TAsstPasteSender::Stop()", "a");
	mThread->Terminate();
	mThread->PreventBlocking();
	mThread->Wait();
//	DBEXP("TAsstPasteSender::Stop()", "b");
	delete mThread;
	mThread = NULL;
	return B_OK;
}
//------------------------------------------------------------------------------
//==============================================================================
TPasteSenderThread::TPasteSenderThread(BInputServerDevice* oOwner)
		: SUPER(THREAD_NAME_ASST_PASTE_SENDER, PORT_NAME_ASST_PASTE_SENDER, oOwner)
{
}
//------------------------------------------------------------------------------
TPasteSenderThread::~TPasteSenderThread()
{
}
//------------------------------------------------------------------------------
void	TPasteSenderThread::MessageReceived(BMessage* oMsg)
{
	BMessage	aNotifyMsg(PASTE_MESSAGE_ENQUEUED);
	BMessage*	aEnqueueMsg;
	BMessenger	aEnqueueNotifyMessenger;

	switch ( oMsg->what ) {
	case SEND_PASTE_MESSAGE_REQUESTED:
//		DBEXP("TPasteSenderThread::MessageReceived()", "a");
		aEnqueueMsg = new BMessage(B_PASTE);
//		DBEXP("TPasteSenderThread::MessageReceived()", "b");
		reinterpret_cast<TAsstPasteSender*>(mOwner)->EnqueueMessage(aEnqueueMsg);
//		DBEXP("TPasteSenderThread::MessageReceived()", "c");
		oMsg->FindMessenger(MSG_NAME_ENQUEUE_NOTIFY_ADDRESS, &aEnqueueNotifyMessenger);
		aEnqueueNotifyMessenger.SendMessage(&aNotifyMsg);
		break;
	default:
		DBEXP("TPasteSenderThread::MessageReceived", "知らないメッセージ");
	}
}
//==============================================================================
BInputServerDevice* instantiate_input_device()
{
	return new TAsstPasteSender();
}
//==============================================================================
