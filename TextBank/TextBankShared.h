#ifndef	___TextBankShared_H_
#define	___TextBankShared_H_
//------------------------------------------------------------------------------
// BeOS
// C++
// Add2
#include <add2/SHINTAArchivable.h>
// Proj
//------------------------------------------------------------------------------
//==============================================================================
const uint32	ACTIVATE_WINDOW_REQUESTED = '_act';
const uint32	DELETE_TOP_HISTORY_REQUESTED = '_del';
const uint32	PREF_HOT_KEY_CHANGED = '_phc';
const uint32	SHOW_POP_UP_REQUESTED = '_pop';
const uint32	PASTE_MESSAGE_ENQUEUED = '_pse';
const uint32	SEND_PASTE_MESSAGE_REQUESTED = '_spr';
//------------------------------------------------------------------------------
// 環境設定
const char		ARC_NAME_ACTIVATE_CHAR[] = "ActC";
const char		ARC_NAME_ACTIVATE_MODIFIERS[] = "ActM";
const char		ARC_NAME_AUTO_INSTALL_ASST[] = "AIAs";
const char		ARC_NAME_DELETE_CHAR[] = "DelC";
const char		ARC_NAME_DELETE_MODIFIERS[] = "DelM";
const char		ARC_NAME_LOCK_TEMPLATE_ORDER[] = "LkTO";
const char		ARC_NAME_LOSE_FOCUS[] = "LsFc";
const char		ARC_NAME_MINIMIZED_BOOT[] = "MmBt";
const char		ARC_NAME_NUM_CLIPBOARD_HISTORIES[] = "NCHs";
const char		ARC_NAME_POP_UP_CHAR[] = "PopC";
const char		ARC_NAME_POP_UP_MODIFIERS[] = "PopM";
const char		ARC_NAME_SAVE_CLIPBOARD[] = "SvCb";
//------------------------------------------------------------------------------
// メッセージ
const char		MSG_NAME_ENQUEUE_NOTIFY_ADDRESS[] = "EnqNotifyAddr";
//------------------------------------------------------------------------------
// char[]
const char		FILE_NAME_PREFS[] = "TextBank_Prefs.arc";					// 環境設定記録ファイル名
const char		FOLDER_NAME_TEXT_BANK_SETTINGS[] = "TextBank";				// 設定保存パス
const char		PORT_NAME_ASST_PASTE_SENDER[] = "AsstPasteSenderPort";
const char		PORT_NAME_ASST_SPY[] = "AsstSpyPort";
//==============================================================================
/* ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
ユーザーが設定できる項目
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''' */
class	TPrefs : public TSHINTAArchivable
{
public:
	TPrefs();
	~TPrefs();
	virtual status_t	Archive(BMessage* oArc, bool oDeep = true) const;
	status_t			LoadFromFile();
	status_t			SaveToFile();
	virtual status_t	Unarchive(const BMessage& oMsg);	// メッセージから情報を得る

	bool		mAutoInstallAsst;		// アシスタントを自動インストールする
	bool		mLockTemplateOrder;		// テンプレートの順序を固定
	bool		mLoseFocus;				// ダブルクリック後にフォーカスを失う
	bool		mMinimizedBoot;			// 最小化起動
	bool		mSaveClipboard;			// クリップボード履歴をディスクに保存する
	int32		mActivateChar;			// Text Bank をアクティブにするキー
	int32		mActivateModifiers;		// Text Bank をアクティブにするキー
	int32		mDeleteChar;			// 先頭の履歴を削除するキー
	int32		mDeleteModifiers;		// 先頭の履歴を削除するキー
	int32		mNumClipboardHistories;	// 保持する履歴の数
	int32		mPopUpChar;				// ポップアップメニューを表示するキー
	int32		mPopUpModifiers;		// ポップアップメニューを表示するキー

private:
	typedef	TSHINTAArchivable	SUPER;	// スーパークラス
};
//==============================================================================
#endif	// ___TextBankShared_H_
