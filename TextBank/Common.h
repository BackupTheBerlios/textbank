#ifndef	___TextBankCommon_H_
#define	___TextBankCommon_H_
//------------------------------------------------------------------------------
#ifdef	GLOBAL_DEF
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif
//------------------------------------------------------------------------------
// Add2
#include <add2/AppPrivacy.h>
#include <add2/SHINTAArchivable.h>
#include <add2/SHINTAIAPs.h>
// Proj
#include "TextBankShared.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================
/* ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
終了時の状態
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''' */
class	TExitStates : public TSHINTAArchivable
{
public:
	TExitStates();
	~TExitStates();

	virtual status_t	Archive(BMessage* oArc, bool oDeep = true) const;
	status_t			LoadFromFile();
	status_t			SaveToFile();
	virtual status_t	Unarchive(const BMessage& oMsg);	// メッセージから情報を得る

	bool			mMinimized;								// Win: 最小化
	string			mAppVer;								// App: バージョンチェック用
	BRect			mFrame;									// Win: ウィンドウの位置

private:
	typedef	TSHINTAArchivable	SUPER;	// スーパークラス
};
//==============================================================================
class	TTextBankAppPrivacy : public TAppPrivacy
{
public:
	TTextBankAppPrivacy();
	virtual	~TTextBankAppPrivacy();

	void		UpdateAccFormat();

	string		mAccFormat;

private:
	typedef	TAppPrivacy	SUPER;
};
//==============================================================================
class	TTextData : public TSHINTAArchivable
{
public:
	TTextData();

	virtual status_t	Archive(BMessage* oArc, bool oDeep = true) const;
	virtual status_t	Unarchive(const BMessage& oArc);

	bool		mExpand;		// 子アイテムを展開しているか（定型文用）：使用するのは保存・読み込みの時のみ。
	bool		mGroup;			// グループかどうか（定型文用）
	bool		mPatternMatch;	// 特殊記号を貼り付け時に展開するかどうか（定型文用）
	int32		mID;			// ユニークな ID（定型文用）
	uint32		mOutlineLevel;	// 階層（定型文用）：使用するのは保存・読み込みの時のみ。普段は ListItem の OutlineLevel を用いる。
	string		mTitle;			// タイトル（定型文用）
	BMessage	mData;			// MIME 形式（クリップボードの形式そのまま）のテキストデータ

private:
	typedef	TSHINTAArchivable	SUPER;
};
//==============================================================================
string		acc_label(const string& oAcc, const string& oStr);
string		alert_caption();
status_t	install_asst();
bool		is_asst_installed();
bool		is_asst_running();
status_t	run_asst(const char* oConfirmMsg);
void		send_message_to_assistant(const char* oPortName, const BMessage& oMsg);
status_t	uninstall_asst(bool oSilent = false);
//==============================================================================
// グローバル変数
extern const char			AM_PM_EN[][4];
extern const char			DAY_OF_WEEK_EN_LONG[][16];
extern const char			DAY_OF_WEEK_EN_SHORT[][4];
extern const char			LKK_AM_PM_JP[][8];
extern const char			LKK_DAY_OF_WEEK_JP[][8];
extern const char			LKK_MONTH_JP[][16];
extern const char			LKK_YEAR_JP[][16];
extern const char			MONTH_EN_LONG[][16];
extern const char			MONTH_EN_SHORT[][4];
extern const char*			gAppAuthorEnUs;
extern const char*			gAppAuthorJaJp;
extern const char*			gAppNameEnUs;
extern const char*			gAppNameJaJp;
extern const char*			gAppSignature;
extern const char*			gAppVer;
extern const char*			gCright;
GLOBAL TExitStates			gExitStates;
GLOBAL TInterAppPrefs		gIAPs;
GLOBAL TPrefs				gPrefs;
GLOBAL TTextBankAppPrivacy	gAppPrivacy;
//==============================================================================
#endif	// ___TextBankCommon_H_
