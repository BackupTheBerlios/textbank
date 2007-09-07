#define	GLOBAL_DEF

//------------------------------------------------------------------------------
#include "Common.h"
//------------------------------------------------------------------------------
// BeOS
#include <Clipboard.h>
#include <FindDirectory.h>
#include <Locale.h>
#include <Message.h>
#include <Path.h>
#include <Roster.h>
// Add2
#include <add2/FUConstants.h>
#include <add2/MiscDebug.h>
#include <add2/MiscUtils.h>
#include <add2/SHINTAUtils.h>
// Proj
#include "Constants.h"
#include "LKKConstants.h"
//------------------------------------------------------------------------------
const char	AM_PM_EN[][4] = { "AM", "PM" };
const char	DAY_OF_WEEK_EN_LONG[][16] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
const char	DAY_OF_WEEK_EN_SHORT[][4] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
const char	MONTH_EN_LONG[][16] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
const char	MONTH_EN_SHORT[][4] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
//------------------------------------------------------------------------------
const char	LKK_AM_PM_JP[][8] = { "Gozen", "Gogo" };
const char	LKK_DAY_OF_WEEK_JP[][8] = { "Nichi", "Getsu", "Ka", "Sui", "Moku", "Kin", "Do" };
const char	LKK_MONTH_JP[][16] = { "Mutsuki", "Kisaragi", "Yayoi", "Uzuki", "Satsuki", "Minazuki", "Fumizuki", "Hazuki", "Nagatsuki", "Kannazuki", "Shimotsuki", "Shiwasu" };
const char	LKK_YEAR_JP[][16] = { "Meiji", "Taisho", "Showa", "Heisei" };
//==============================================================================
TExitStates::TExitStates()
		: SUPER()
{
	// bool
	mMinimized = false;
	// その他
	mFrame.Set(50, 50, 350, 300);
}
//------------------------------------------------------------------------------
TExitStates::~TExitStates()
{
}
//------------------------------------------------------------------------------
status_t	TExitStates::Archive(BMessage* oArc, bool oDeep) const
{
	status_t	aResult = B_OK;

	aResult |= SUPER::Archive(oArc);
	// bool
	aResult |= oArc->AddBool(ARC_NAME_MINIMIZED, mMinimized);
	// string
	aResult |= oArc->AddString(FUC_ARC_NAME_APP_VER, gAppVer);
	// その他
	aResult |= oArc->AddRect(FUC_ARC_NAME_FRAME, mFrame);
	return aResult;
}
//------------------------------------------------------------------------------
status_t	TExitStates::LoadFromFile()
{
	return LoadByName(FOLDER_NAME_TEXT_BANK_SETTINGS, FILE_NAME_EXIT_STATES);
}
//------------------------------------------------------------------------------
status_t	TExitStates::SaveToFile()
{
	return SaveByName(FOLDER_NAME_TEXT_BANK_SETTINGS, FILE_NAME_EXIT_STATES);
}
//------------------------------------------------------------------------------
status_t	TExitStates::Unarchive(const BMessage& oMsg)
{
	status_t	aResult = B_OK;

	aResult |= SUPER::Unarchive(oMsg);
	// bool
	aResult |= FindBool(oMsg, ARC_NAME_MINIMIZED, &mMinimized);
	// string
	aResult |= FindString(oMsg, FUC_ARC_NAME_APP_VER, &mAppVer);
	// その他
	aResult |= FindRect(oMsg, FUC_ARC_NAME_FRAME, &mFrame);
	return aResult;
}
//==============================================================================
TTextBankAppPrivacy::TTextBankAppPrivacy()
{
}
//------------------------------------------------------------------------------
TTextBankAppPrivacy::~TTextBankAppPrivacy()
{
}
//------------------------------------------------------------------------------
void	TTextBankAppPrivacy::UpdateAccFormat()
{
	gIAPs.GetString(IAP_NAME_ACC_KEY_FORMAT, &mAccFormat);
}
//------------------------------------------------------------------------------
//==============================================================================
TTextData::TTextData()
		: SUPER()
{
	mExpand = true;
	mGroup = false;
	mPatternMatch = false;
	mID = 0;
	mOutlineLevel = 0;
}
//------------------------------------------------------------------------------
status_t	TTextData::Archive(BMessage* oArc, bool oDeep) const
{
	status_t	aResult = B_OK;

	aResult |= SUPER::Archive(oArc);
	// bool
	aResult |= oArc->AddBool(ARC_NAME_EXPAND, mExpand);
	aResult |= oArc->AddBool(ARC_NAME_GROUP, mGroup);
	aResult |= oArc->AddBool(ARC_NAME_PATTERN_MATCH, mPatternMatch);
	// int32
	aResult |= oArc->AddInt32(ARC_NAME_ID, mID);
	// uint32
	aResult |= oArc->AddInt32(ARC_NAME_OUTLINE_LEVEL, mOutlineLevel);
	// string
	aResult |= oArc->AddString(ARC_NAME_TITLE, mTitle.c_str());
	// BMessage
	aResult |= oArc->AddMessage(ARC_NAME_DATA, &mData);
	return aResult;
}
//------------------------------------------------------------------------------
status_t	TTextData::Unarchive(const BMessage& oArc)
{
	status_t	aResult = B_OK;

	aResult |= SUPER::Unarchive(oArc);
	// bool
	aResult |= FindBool(oArc, ARC_NAME_EXPAND, &mExpand);
	aResult |= FindBool(oArc, ARC_NAME_GROUP, &mGroup);
	aResult |= FindBool(oArc, ARC_NAME_PATTERN_MATCH, &mPatternMatch);
	// int32
	aResult |= FindInt32(oArc, ARC_NAME_ID, &mID);
	// uint32
	aResult |= FindInt32(oArc, ARC_NAME_OUTLINE_LEVEL, reinterpret_cast<int32*>(&mOutlineLevel));
	// string
	aResult |= FindString(oArc, ARC_NAME_TITLE, &mTitle);
	// Message
	aResult |= FindMessage(oArc, ARC_NAME_DATA, &mData);
	return aResult;
}
//------------------------------------------------------------------------------
//==============================================================================
string	acc_label(const string& oAcc, const string& oStr)
{
	return merge_accelerator_key(gAppPrivacy.mAccFormat, oAcc, oStr);
}
//------------------------------------------------------------------------------
string	alert_caption()
{
	return BString(_T(LKK_ALERT_CAPTION)).ReplaceAll(LKR_APP_NAME, _T(gAppNameEnUs)).String();
}
//------------------------------------------------------------------------------
string	asst_install_path(const char* oFolder)
{
	BPath	aPath;

	find_directory(B_USER_ADDONS_DIRECTORY, &aPath);
	aPath.Append(FOLDER_NAME_INPUT_SERVER);
	aPath.Append(oFolder);
	aPath.Append(FILE_NAME_ASST);
	return aPath.Path();
}
//------------------------------------------------------------------------------
status_t	install_asst()
{
	string		aAsstRealPath;	// 実体のパス
	BDirectory	aDir;
	BEntry		aEntry;

	uninstall_asst(true);
	aAsstRealPath = gAppPrivacy.ExePath()+FILE_NAME_ASST;
	// Hot Key Spy
	aDir.CreateSymLink(asst_install_path(FOLDER_NAME_ASST_SPY).c_str(), aAsstRealPath.c_str(), NULL);
	// Paste Sender
	aDir.CreateSymLink(asst_install_path(FOLDER_NAME_ASST_PASTE_SENDER).c_str(), aAsstRealPath.c_str(), NULL);
	if ( !is_asst_installed() )
		return B_ERROR;
	return B_OK;
}
//------------------------------------------------------------------------------
bool	is_asst_installed()
{
	bool	aInstalled = true;
	BEntry	aEntry;

	try {
		aEntry.SetTo(asst_install_path(FOLDER_NAME_ASST_SPY).c_str(), true);
		if ( !aEntry.Exists() )
			throw B_ERROR;
		aEntry.SetTo(asst_install_path(FOLDER_NAME_ASST_PASTE_SENDER).c_str(), true);
		if ( !aEntry.Exists() )
			throw B_ERROR;
	} catch (...) {
		aInstalled = false;
	}
	return aInstalled;	
}
//------------------------------------------------------------------------------
bool	is_asst_running()
{
	bool	aRunning = true;
	BEntry	aEntry;

	try {
		if ( find_port(PORT_NAME_ASST_SPY) < 0 )
			throw B_ERROR;
		if ( find_port(PORT_NAME_ASST_PASTE_SENDER) < 0 )
			throw B_ERROR;
	} catch (...) {
		aRunning = false;
	}
//	DBEXP("is_asst_running",aRunning);
	return aRunning;	
}
//------------------------------------------------------------------------------
status_t	run_asst(const char* oConfirmMsg)
{
	bigtime_t	aWaitBeginTime;
	app_info	aInputServerAppInfo;
	app_info	aTextBankAppInfo;

	if ( show_alert3(alert_caption(), oConfirmMsg, acc_label("N", _T(LKK_BUTTON_NO)),
			acc_label("Y", _T(LKK_BUTTON_YES))) == 0 )
		return B_ERROR;
	// input_server 終了
	be_roster->GetAppInfo(FUC_SIGNATURE_INPUT_SERVER, &aInputServerAppInfo);
	kill_team(aInputServerAppInfo.team);
	// input_server の復帰を待つ
	aWaitBeginTime = system_time();
	while ( !is_asst_running() && system_time()-aWaitBeginTime < RESTART_INPUT_SERVER_WAIT_LIMIT ) {
		snooze(FUC_TIME_SNOOZE_MIN);
	}
	// アクティブ化
	be_roster->GetAppInfo(gAppSignature, &aTextBankAppInfo);
	be_roster->ActivateApp(aTextBankAppInfo.team);
	// チェック
	if ( !is_asst_running() ) {
		show_alert3(alert_caption(), _T(LKK_ERR_CANNOT_RUN_ASST), acc_label("U", _T(LKK_BUTTON_UNDERSTAND)),
				"", "", -1, B_WARNING_ALERT);
		return B_ERROR;
	}
	show_alert3(alert_caption(), _T(LKK_INFO_RAN_ASST), _T(LKK_BUTTON_OK));
	return B_OK;
}
//------------------------------------------------------------------------------
void	send_message_to_assistant(const char* oPortName, const BMessage& oMsg)
{
	char*		aFlattenBuf;
	port_id		aAsstPort;
	ssize_t		aFlattenedSize;

	aAsstPort = find_port(oPortName);
	if ( aAsstPort < 0 ) {
		DBEXP("SendMessageToAssistant()", "port not found");
		return;
	}
	// フラット化
	aFlattenedSize = oMsg.FlattenedSize();
	aFlattenBuf = new char [aFlattenedSize];
	oMsg.Flatten(aFlattenBuf, aFlattenedSize);
	// 書き込み
	write_port(aAsstPort, 0, aFlattenBuf, aFlattenedSize);
	delete [] aFlattenBuf;
}
//------------------------------------------------------------------------------
status_t	uninstall_asst(bool oSilent)
{
	BEntry(static_cast<const char*>(asst_install_path(FOLDER_NAME_ASST_SPY).c_str())).Remove();
	BEntry(static_cast<const char*>(asst_install_path(FOLDER_NAME_ASST_PASTE_SENDER).c_str())).Remove();
	if ( !oSilent ) {
		show_alert3(alert_caption(), _T(LKK_INFO_UNINSTALLED_ASST) , _T(LKK_BUTTON_OK));
	}
	if ( is_asst_installed() )
		return B_ERROR;
	return B_OK;
}
//------------------------------------------------------------------------------
//==============================================================================
