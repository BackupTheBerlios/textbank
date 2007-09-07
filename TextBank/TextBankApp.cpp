#include <InterfaceDefs.h>

/*============================================================================*/
const char*	gAppNameJaJp   = "Text Bank";
const char*	gAppNameEnUs   = "Text Bank";
const char*	gAppVer        = "Ver 5.10";
const char*	gCright        = "Copyright "B_UTF8_COPYRIGHT" 1999-2007 by %AppAuthor%";
const char*	gAppAuthorJaJp = "ＳＨＩＮＴＡ";
const char*	gAppAuthorEnUs = "SHINTA";
/*============================================================================*/

/*==============================================================================
 Ver. |					主な改良点
--------------------------------------------------------------------------------
 ---- |  開発着手。
==============================================================================*/

/*=== Memo =====================================================================
NEXT:

デスクトレイに入れるように
ツールバーより先にメインビューを AddChild() しないと LanguageChanged が呼び出されないなどの弊害がある

OK アイコンの変更
OK ヘルプコマンド用のファイル準備
OK Live にする
OK ポップアップメニューで LockTemplateOrder を設定できるように
OK ポップアップメニューのショートカットキー
OK C, T キーでページを切り替えられるように
OK クリーンラウンチ
OK Templates.defaults の自動インストール
OK Locale Kit 対応
OK 特殊記号
OK キーボード操作
OK 環境設定ウィンドウのヘルプとデフォルト
OK 環境設定ウィンドウの Del, Show hot key
OK ポップアップメニューを上から再実装
OK Property ウィンドウ
不要 ZETA のポップアップ機能からポップアップメニューを呼び出せるようにしてはどうか？ → Paste のために結局はアシスタントが必要
==============================================================================*/

//------------------------------------------------------------------------------
#include "TextBankApp.h"
//------------------------------------------------------------------------------
// BeOS
#include <Locale.h>
// C++
// Add2
#include <add2/FUConstants.h>
#include <add2/MiscDebug.h>
#include <add2/MiscUtils.h>
#include <add2/SHINTAConstants.h>
// Proj
#include "Common.h"
#include "Constants.h"
#include "LKKConstants.h"
#include "PrefsWindow.h"
#include "PropertyWindow.h"
#include "TextBankWindow.h"
//==============================================================================
TTextBankApp::TTextBankApp()
		: SUPER(gAppSignature)
{
	BRect	aWinFrame = gExitStates.mFrame;

	// メンバ変数初期化
	mAboutWin = NULL;
	mPrefsWin = NULL;
	// アシスタントのインストール
	if ( !is_asst_running() && gPrefs.mAutoInstallAsst ) {
		install_asst();
		run_asst(_T(LKK_ASK_RUN_ASST_AUTO));
	}
	// ウィンドウ作成・実行
	aWinFrame.OffsetTo(B_ORIGIN-aWinFrame.RightBottom());	// 最初は画面外に表示
	mTextBankWin = new TTextBankWindow(aWinFrame);
	mTextBankWin->Show();
	if ( gPrefs.mMinimizedBoot ) {
		gExitStates.mMinimized = true;
	}
}
//------------------------------------------------------------------------------
TTextBankApp::~TTextBankApp()
{
}
//------------------------------------------------------------------------------
void	TTextBankApp::AboutRequested()
{
	BRect		aWinFrame;

	if ( mAboutWin == NULL ) {
		aWinFrame = mTextBankWin->Frame();
		aWinFrame.top += FUC_HEIGHT_WIN_TAB;
		mAboutWin = new TSHINTAAboutWindow(aWinFrame);
		mAboutWin->IntelligentMoveTo(aWinFrame.LeftTop());
		mAboutWin->SetAppInfo(gAppNameEnUs, gAppVer, gCright);
		mAboutWin->SetTranslatorInfo(LKK_ABOUT_TRANSLATOR_INFO);
		mAboutWin->SetIAPRef(&gIAPs);
	}
	mAboutWin->Appear();
}
//------------------------------------------------------------------------------
void	TTextBankApp::ActivateWindowRequested()
{
	mTextBankWin->Appear();
}
//------------------------------------------------------------------------------
void	TTextBankApp::IAPChanged(BMessage* oMsg)
{
	BString	aItemName;

	if ( oMsg->FindString(MSG_NAME_IAP_SOURCE_IAP_ITEM, &aItemName) != B_OK )
		return;
	gIAPs.SetValue(*oMsg);
	if ( aItemName == IAP_NAME_ACC_KEY_FORMAT ) {
		gAppPrivacy.UpdateAccFormat();
	}
}
//------------------------------------------------------------------------------
void	TTextBankApp::MessageReceived(BMessage* oMsg)
{
	switch ( oMsg->what ) {
	// 転送
	case DELETE_TOP_HISTORY_REQUESTED:
	case SHOW_POP_UP_REQUESTED:
		BMessenger(mTextBankWin).SendMessage(oMsg);
		break;
	case PREF_LOCK_TEMPLATE_ORDER_CHANGED:
		BMessenger(mPrefsWin).SendMessage(oMsg);
		break;
	// 転送＆処理
	// 自前処理
	case FUC_IAP_CHANGED:
		IAPChanged(oMsg);
		BMessenger(mTextBankWin).SendMessage(oMsg);
		BMessenger(mPrefsWin).SendMessage(oMsg);
		SendMessageToPropertyWindows(oMsg);
		break;
	case ACTIVATE_WINDOW_REQUESTED:
		ActivateWindowRequested();
		break;
	case POP_MITEM_AUTHOR_WEB_CLICKED:
		PopMItemAuthorWebClicked();
		break;
	case POP_MITEM_HELP_CLICKED:
		PopMItemHelpClicked();
		break;
	case POP_MITEM_PREFS_CLICKED:
		PopMItemPrefsClicked();
		break;
	case POP_MITEM_SHINTA_COMMON_PREFS_CLICKED:
		PopMItemSHINTACommonPrefsClicked();
		break;
	default:
		SUPER::MessageReceived(oMsg);
	}
}
//------------------------------------------------------------------------------
void	TTextBankApp::PopMItemAuthorWebClicked()
{
	show_url(SHINTA_URL_AUTHOR_WEB);
}
//------------------------------------------------------------------------------
void	TTextBankApp::PopMItemHelpClicked()
{
	string	aHelpFileName;

	aHelpFileName = locale_text_with_default(LKV_FILE_NAME_HELP, FILE_NAME_DEFAULT_HELP);
	if ( show_help((gAppPrivacy.ExePath()+aHelpFileName).c_str()) != B_OK ) {
		show_alert3(alert_caption(), BString(_T(LKK_ERR_CANNOT_SHOW_HELP)).ReplaceAll(LKR_FILE_NAME, aHelpFileName.c_str()).String(),
				acc_label("U", _T(LKK_BUTTON_OK)), "", "", -1, B_WARNING_ALERT);
	}
}
//------------------------------------------------------------------------------
void	TTextBankApp::PopMItemPrefsClicked()
{
	BRect	aWinFrame;

	if ( mPrefsWin == NULL ) {
		aWinFrame = mTextBankWin->Frame();
		aWinFrame.top += FUC_HEIGHT_WIN_TAB;
		mPrefsWin = new TPrefsWindow(aWinFrame);
		if ( mPrefsWin->InitCheck() == B_OK ) {
			mPrefsWin->IntelligentMoveTo(aWinFrame.LeftTop());
			mPrefsWin->SetTarget(mTextBankWin);
		}
	}
	if ( mPrefsWin->InitCheck() == B_OK ) {
		mPrefsWin->Appear();
	} else {
		show_alert3(alert_caption(),
				string(_T(LKK_ERR_CANNOT_SHOW_PREFS_WIN))+"\n"+_T(LKK_HINT_EXCUTABLE_MAYBE_BROKEN),
				acc_label("U", _T(LKK_BUTTON_UNDERSTAND)), "", "", -1, B_WARNING_ALERT);
	}
}
//------------------------------------------------------------------------------
void	TTextBankApp::PopMItemSHINTACommonPrefsClicked()
{
	BRect	aWinFrame;

	aWinFrame = mTextBankWin->Frame();
	aWinFrame.top += FUC_HEIGHT_WIN_TAB;
	gIAPs.ShowWindow(aWinFrame);
}
//------------------------------------------------------------------------------
void	TTextBankApp::Quit()
{
	// 見かけの終了を速めるため、先に隠す……と IsMinimized() が働かなくなるのでやらない
//	mTextBankWin->Hide();
	// ダイアログを終了
	close_window(reinterpret_cast<BWindow**>(&mAboutWin));
	// メインウィンドウを終了
	close_window(reinterpret_cast<BWindow**>(&mTextBankWin));
	// 自身を終了
	SUPER::Quit();
}
//------------------------------------------------------------------------------
bool	TTextBankApp::QuitRequested()
{
	return mTextBankWin->QuitRequested();
}
//------------------------------------------------------------------------------
void	TTextBankApp::ReadyToRun()
{
//	DBEXP("TTextBankApp::ReadyToRun()", "");
	mTextBankWin->Minimize(gExitStates.mMinimized);
	while ( mTextBankWin->IsMinimized() != gExitStates.mMinimized ) {
		snooze(FUC_TIME_SNOOZE_MIN);
	}
	mTextBankWin->MoveTo(gExitStates.mFrame.LeftTop());
}
//------------------------------------------------------------------------------
void	TTextBankApp::SendMessageToPropertyWindows(BMessage* oMsg)
{
	int32				aWinIndex = 0;
	TPropertyWindow*	aPropWin;

	while ( WindowAt(aWinIndex) != NULL ) {
		aPropWin = dynamic_cast<TPropertyWindow*>(WindowAt(aWinIndex));
		if ( aPropWin != NULL ) {
			BMessenger(aPropWin).SendMessage(oMsg);
		}
		aWinIndex++;
	}
}
//------------------------------------------------------------------------------
//==============================================================================
int		main(int oArgC,char* oArgV[])
{
	TTextBankApp*	aTextBankApp;

	gAppPrivacy.AnalyzeExePath(oArgV[0]);		// 最初にやる
	gExitStates.LoadFromFile();					// 2 番目にやる
	gPrefs.LoadFromFile();
	gIAPs.SetAppSignatureRef(gAppSignature);	// IAP: 3 番目にやる
	gIAPs.AddItem(new TIAPItemAccKeyFormat());
	gIAPs.AddItem(new TIAPItemFlatToolBar());
	gIAPs.AddItem(new TIAPItemShouseiUserName());
	gIAPs.AddItem(new TIAPItemShouseiUserNumber());
	gIAPs.GetString(IAP_NAME_ACC_KEY_FORMAT, &gAppPrivacy.mAccFormat);
	aTextBankApp = new TTextBankApp();
	aTextBankApp->Run();
	delete aTextBankApp;
	gExitStates.SaveToFile();
	return B_OK;
}
//==============================================================================
#ifdef	DEBUG
extern "C" _EXPORT	const char	gDebugMark[] = "DEBUG/debug";
#endif
//==============================================================================
