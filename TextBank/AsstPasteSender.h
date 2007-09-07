#ifndef	___AsstPasteSender_H_
#define	___AsstPasteSender_H_
//------------------------------------------------------------------------------
// BeOS
#include <add-ons/input_server/InputServerDevice.h>
// C++
// Add2
// Proj
#include "AsstCommon.h"
//------------------------------------------------------------------------------
class	TPasteSenderThread;
//==============================================================================
/* ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Input Device 型
B_PASTE をアクティブなアプリケーションに送信
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''' */
class	TAsstPasteSender : public BInputServerDevice
{
public:
	TAsstPasteSender();
	virtual	~TAsstPasteSender();

	virtual status_t	Start(const char* oDevice, void* oCookie);
	virtual	status_t	Stop(const char* oDevice, void* oCookie);

private:
	typedef	BInputServerDevice	SUPER;

	TPasteSenderThread*	mThread;
};
//==============================================================================
class	TPasteSenderThread : public TAsstThread
{
public:
	TPasteSenderThread(BInputServerDevice* oOwner);
	virtual	~TPasteSenderThread();

private:
	typedef	TAsstThread	SUPER;

	virtual void	MessageReceived(BMessage* oMsg);
};
//==============================================================================
extern "C" _EXPORT BInputServerDevice*	instantiate_input_device();
//==============================================================================
#endif	// ___AsstPasteSender_H_
