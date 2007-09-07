#ifndef	___TextBankAsstCommon_H_
#define	___TextBankAsstCommon_H_
//------------------------------------------------------------------------------
// BeOS
// C++
#include <string>
// Add2
#include <add2/Thread.h>
//------------------------------------------------------------------------------
//==============================================================================
class	TAsstThread : public TThread
{
public:
	TAsstThread(const char* oThreadName, const char* oPortName, void* oOwner);
	virtual	~TAsstThread();

	status_t	PreventBlocking();

protected:
	virtual void	MessageReceived(BMessage* oMsg) = 0;

	void*		mOwner;
	port_id		mPort;

private:
	typedef	TThread	SUPER;

	virtual int32	Execute();

	string		mPortName;
};
//==============================================================================
#endif	// ___TextBankAsstCommon_H_
