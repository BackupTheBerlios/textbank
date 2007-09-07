//------------------------------------------------------------------------------
#include "AsstCommon.h"
//------------------------------------------------------------------------------
// Add2
#include <add2/MiscDebug.h>
//==============================================================================
TAsstThread::TAsstThread(const char* oThreadName, const char* oPortName, void* oOwner)
		: SUPER(oThreadName/*, B_REAL_TIME_DISPLAY_PRIORITY+4*/), mOwner(oOwner), mPortName(oPortName)
{
}
//------------------------------------------------------------------------------
TAsstThread::~TAsstThread()
{
}
//------------------------------------------------------------------------------
int32	TAsstThread::Execute()
{
	char*		aBuf;
	int32		aMsgCode;
	ssize_t		aBufLen,aReadLen;
	BMessage	aMsg;

	// ポート作成
	mPort = create_port(100, mPortName.c_str());
	if ( mPort < 0 ) {
		DBEXP("TAsstThread::Execute - ポート作成失敗", mPort);
		return mPort;
	}
	// 本体からメッセージを受け取る
	while ( !mTerminateRqtd ) {
		aBufLen = port_buffer_size(mPort);
		aBuf = new char [aBufLen];
		aReadLen = read_port(mPort, &aMsgCode, aBuf, aBufLen);
		aMsg.Unflatten(aBuf);
		delete [] aBuf;
		MessageReceived(&aMsg);
	}
	// ポート破棄
	delete_port(mPort);
	return B_OK;
}
//------------------------------------------------------------------------------
status_t	TAsstThread::PreventBlocking()
{
	write_port(mPort, 0, NULL, 0);
	return B_OK;
}
//==============================================================================

