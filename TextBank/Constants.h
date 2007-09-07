#ifndef	___TextBankConstants_H_
#define	___TextBankConstants_H_
//------------------------------------------------------------------------------
// add2
#include <add2/FUConstants.h>
#include <add2/SHINTAConstants.h>
//==============================================================================
enum	{	// Preferences Tab
		PT_GENERAL, PT_HOT_KEY, PT_ASST, PT_END };
//------------------------------------------------------------------------------
// メニュー定数
const uint32	ACTIVATE_CLIPBOARD_VIEW_REQUESTED = 'acvr';
const uint32	ACTIVATE_TEMPLATE_VIEW_REQUESTED = 'atvr';
const uint32	CBOX_ACTIVATE_CLICKED = 'cact';
const uint32	CBOX_DELETE_CLICKED = 'cdel';
const uint32	CHANGE_ORDER_REQUESTED = 'chor';
const uint32	CBOX_LOSE_FOCUS_CLICKED = 'clsf';
const uint32	CBOX_LOCK_TEMPLATE_ORDER_CLICKED = 'clto';
const uint32	CBOX_MINIMIZED_BOOT_CLICKED = 'cmmb';
const uint32	CBOX_POP_UP_CLICKED = 'cpop';
const uint32	CBOX_PATTERN_MATCH_CLICKED = 'cptm';
const uint32	CBOX_SAVE_CLIPBOARD_CLICKED = 'csvc';
const uint32	DATA_PROPERTY_CHANGED = 'dtpc';
const uint32	EXP_STAT_BAR_REQUESTED = 'esbr';
const uint32	BUTTON_INSTALL_ASST_CLICKED = 'inas';
const uint32	POP_MITEM_DELETE_CLICKED = 'pdel';
const uint32	POP_MITEM_HOLD_DATA_CLICKED = 'phld';
const uint32	PREF_LOCK_TEMPLATE_ORDER_CHANGED = 'pltc';
const uint32	POP_MITEM_LOCK_TEMPLATE_ORDER_CLICKED = 'plto';
const uint32	POP_MITEM_MOVE_DOWN_CLICKED = 'pmvd';
const uint32	POP_MITEM_MOVE_UP_CLICKED = 'pmvu';
const uint32	PREF_NUM_CLIPBOARD_HISTORIES_CHANGED = 'pncc';
const uint32	POP_MITEM_NEW_GROUP_CLICKED = 'pngr';
const uint32	POP_MITEM_PROPERTY_CLICKED = 'pprp';
const uint32	POP_MITEM_SAVE_AS_TEMPLATE_CLICKED = 'psat';
const uint32	POP_MITEM_SEND_TO_CLIPBOARD_CLICKED = 'pstc';
const uint32	RBUTTON_AUTO_INSTALL_ASST_CLICKED = 'raia';
const uint32	SAVE_AS_TEMPLATE_REQUESTED = 'satr';
const uint32	TXT_CTRL_ACTIVATE_CHAR_CHANGED = 'tacc';
const uint32	TBUTTON_CLIPBOARD_CLICKED = 'tclp';
const uint32	TXT_CTRL_DELETE_CHAR_CHANGED = 'tdcc';
const uint32	TXT_CTRL_NUM_CLIPBOARD_HISTORIES_CHANGED = 'tncc';
const uint32	TXT_CTRL_POP_UP_CHAR_CHANGED = 'tpcc';
const uint32	TBUTTON_TEMPLATE_CLICKED = 'ttpl';
const uint32	TXT_CTRL_TITLE_CHANGED = 'tttc';
const uint32	BUTTON_UNINSTALL_ASST_CLICKED = 'unas';
//------------------------------------------------------------------------------
const uint32	POP_MITEM_AUTHOR_WEB_CLICKED = FUC_MITEM_AUTHOR_WEB_CLICKED;
const uint32	POP_MITEM_HELP_CLICKED = FUC_MITEM_HELP_CLICKED;
const uint32	POP_MITEM_PREFS_CLICKED = FUC_MITEM_PREFERENCES_CLICKED;
const uint32	POP_MITEM_SHINTA_COMMON_PREFS_CLICKED = SHINTA_MITEM_SHINTA_COMMON_PREFS_CLICKED;
//------------------------------------------------------------------------------
// 終了時の状態
const char		ARC_NAME_MINIMIZED[] = "Mmzd";
//------------------------------------------------------------------------------
// 履歴・定型文保存
const char		ARC_NAME_DATA[] = "Data";
const char		ARC_NAME_EXPAND[] = "Expd";
const char		ARC_NAME_GROUP[] = "Grup";
const char		ARC_NAME_ID[] = "ID  ";
const char		ARC_NAME_OUTLINE_LEVEL[] = "OlLv";
const char		ARC_NAME_PATTERN_MATCH[] = "PtMt";
const char		ARC_NAME_TITLE[] = "Titl";
//------------------------------------------------------------------------------
// メッセージ
const char		MSG_NAME_DATA[] = "Data";
const char		MSG_NAME_INDEX[] = "Index";
const char		MSG_NAME_NEED_SAVE[] = "NeedSave";
const char		MSG_NAME_PATTERN_MATCH[] = "PatternMatch";
const char		MSG_NAME_STAT_BAR[] = "StatBar";
//------------------------------------------------------------------------------
// ビュー
const char		VIEW_NAME_BOX_ACTIVATE[] = "BoxActivate";
const char		VIEW_NAME_BOX_AUTO_INSTALL_ASST[] = "BoxAutoInstallAsst";
const char		VIEW_NAME_BOX_CLIPBOARD[] = "BoxClipboard";
const char		VIEW_NAME_BOX_DELETE[] = "BoxDelete";
const char		VIEW_NAME_BOX_POP_UP[] = "BoxPopUp";
const char		VIEW_NAME_BUTTON_CANCEL[] = "ButtonCancel";
const char		VIEW_NAME_BUTTON_DEFAULTS[] = "ButtonDefaults";
const char		VIEW_NAME_BUTTON_HELP[] = "ButtonHelp";
const char		VIEW_NAME_BUTTON_INSTALL_ASST[] = "ButtonInstallAsst";
const char		VIEW_NAME_BUTTON_OK[] = "ButtonOK";
const char		VIEW_NAME_BUTTON_UNINSTALL_ASST[] = "ButtonUninstallAsst";
const char		VIEW_NAME_CBOX_ACTIVATE_ALT[] = "CBoxActivateAlt";
const char		VIEW_NAME_CBOX_ACTIVATE_CTRL[] = "CBoxActivateCtrl";
const char		VIEW_NAME_CBOX_ACTIVATE_SHIFT[] = "CBoxActivateShift";
const char		VIEW_NAME_CBOX_DELETE_ALT[] = "CBoxDeleteAlt";
const char		VIEW_NAME_CBOX_DELETE_CTRL[] = "CBoxDeleteCtrl";
const char		VIEW_NAME_CBOX_DELETE_SHIFT[] = "CBoxDeleteShift";
const char		VIEW_NAME_CBOX_LOCK_TEMPLATE_ORDER[] = "CBoxLockTemplateOrder";
const char		VIEW_NAME_CBOX_LOSE_FOCUS[] = "CBoxLoseFocus";
const char		VIEW_NAME_CBOX_MINIMIZED_BOOT[] = "CBoxMinimizedBoot";
const char		VIEW_NAME_CBOX_PATTERN_MATCH[] = "CBoxPatternMatch";
const char		VIEW_NAME_CBOX_POP_UP_ALT[] = "CBoxPopUpAlt";
const char		VIEW_NAME_CBOX_POP_UP_CTRL[] = "CBoxPopUpCtrl";
const char		VIEW_NAME_CBOX_POP_UP_SHIFT[] = "CBoxPopUpShift";
const char		VIEW_NAME_CBOX_SAVE_CLIPBOARD[] = "CBoxSaveClipboard";
const char		VIEW_NAME_RBUTTON_DISABLE_AUTO_INSTALL_ASST[] = "RButtonDisableAutoInstallAsst";
const char		VIEW_NAME_RBUTTON_ENABLE_AUTO_INSTALL_ASST[] = "RButtonEnableAutoInstallAsst";
const char		VIEW_NAME_STR_VIEW_ASST_STATUS[] = "StrViewAsstStatus";
const char		VIEW_NAME_STR_VIEW_HINT_USING_HOT_KEY[] = "StrViewHintUsingHotKey";
const char		VIEW_NAME_TAB_VIEW_PREFS[] = "TabViewPrefs";
const char		VIEW_NAME_STR_VIEW_TEXT[] = "StrViewText";
const char		VIEW_NAME_STR_VIEW_TEXT_CAPTION[] = "StrViewTextCaption";
const char		VIEW_NAME_TXT_CTRL_ACTIVATE_CHAR[] = "TxtCtrlActivateChar";
const char		VIEW_NAME_TXT_CTRL_DELETE_CHAR[] = "TxtCtrlDeleteChar";
const char		VIEW_NAME_TXT_CTRL_NUM_CLIPBOARD_HISTORIES[] = "TxtCtrlNumClipboardHistories";
const char		VIEW_NAME_TXT_CTRL_POP_UP_CHAR[] = "TxtCtrlPopUpChar";
const char		VIEW_NAME_TXT_CTRL_TITLE[] = "TxtCtrlTitle";
//------------------------------------------------------------------------------
// パターン
const char		PATTERN_AVOID_PERCENT[] = "\1";
const char		PATTERN_DEST_LF[] = "\n";
const char		PATTERN_DEST_PERCENT[] = "%";
const char		PATTERN_DEST_TAB[] = "\t";
const char		PATTERN_PREFIX[] = "%";
const char		PATTERN_SRC_AM_PM[] = "%ampm";
const char		PATTERN_SRC_AM_PM_HOUR_1[] = "%ampmh";
const char		PATTERN_SRC_AM_PM_HOUR_2[] = "%ampmhh";
const char		PATTERN_SRC_AM_PM_JP[] = "%jampm";
const char		PATTERN_SRC_DAY_OF_WEEK_1[] = "%w";
const char		PATTERN_SRC_DAY_OF_WEEK_2[] = "%ww";
const char		PATTERN_SRC_DAY_OF_WEEK_JP[] = "%jw";
const char		PATTERN_SRC_DAY_1[] = "%d";
const char		PATTERN_SRC_DAY_2[] = "%dd";
const char		PATTERN_SRC_HOUR_1[] = "%h";
const char		PATTERN_SRC_HOUR_2[] = "%hh";
const char		PATTERN_SRC_LF[] = "%lf";
const char		PATTERN_SRC_MINUTE_1[] = "%n";
const char		PATTERN_SRC_MINUTE_2[] = "%nn";
const char		PATTERN_SRC_MONTH_1[] = "%m";
const char		PATTERN_SRC_MONTH_2[] = "%mm";
const char		PATTERN_SRC_MONTH_3[] = "%mmm";
const char		PATTERN_SRC_MONTH_4[] = "%mmmm";
const char		PATTERN_SRC_MONTH_JP[] = "%jm";
const char		PATTERN_SRC_PERCENT[] = "%%";
const char		PATTERN_SRC_SECOND_1[] = "%s";
const char		PATTERN_SRC_SECOND_2[] = "%ss";
const char		PATTERN_SRC_TAB[] = "%t";
const char		PATTERN_SRC_YEAR_1[] = "%y";
const char		PATTERN_SRC_YEAR_2[] = "%yy";
const char		PATTERN_SRC_YEAR_4[] = "%yyyy";
const char		PATTERN_SRC_YEAR_JP_1[] = "%jy";
const char		PATTERN_SRC_YEAR_JP_2[] = "%jyy";
// char[]
const char		ESCAPE_NEW_LINE_FROM[] = "\n";								// リスト表示時の文字列変換用
const char		ESCAPE_NEW_LINE_TO[] = "/";									// リスト表示時の文字列変換用
const char		FILE_NAME_ASST[] = "TextBankAssistant";
const char		FILE_NAME_CLIPBOARDS[] = "TextBank_Clipboards.arc";
const char		FILE_NAME_DEFAULT_HELP[] = "ReadMe_enUS.txt";
const char		FILE_NAME_DEFAULT_TEMPLATES[] = "Templates.defaults";
const char		FILE_NAME_EXIT_STATES[] = "TextBank_ExitStates.arc";		// 終了時の状態記録ファイル名
const char		FILE_NAME_TEMPLATES[] = "TextBank_Templates.arc";
const char		FOLDER_NAME_ASST_PASTE_SENDER[] = "devices";
const char		FOLDER_NAME_ASST_SPY[] = "filters";
const char		FOLDER_NAME_INPUT_SERVER[] = "input_server";
const char		RSRC_NAME_PREFS_VIEW[] = "PrefsViewRsrc";
const char		RSRC_NAME_PROPERTY_VIEW[] = "PropertyViewRsrc";
// int32
const int32		DATA_ID_BASE = 100;
const int32		MAX_NUM_CLIPBOARD_HISTORIES = 500;
const int32		MIX_NUM_CLIPBOARD_HISTORIES = 5;
// float
const float		WIDTH_HOLD_DATA_POP_UP = 220.0;
// bigtime_t
const bigtime_t	RESTART_INPUT_SERVER_WAIT_LIMIT = 7*1000*1000;
//==============================================================================
#endif	// ___TextBankConstants_H_
