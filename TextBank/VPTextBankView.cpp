#include "VPTextBankView.h"
//------------------------------------------------------------------------------
// BeOS
#include <Application.h>
#include <Clipboard.h>
#include <File.h>
#include <ListItem.h>
#include <Locale.h>
#include <MenuItem.h>
#include <Path.h>
#include <WindowScreen.h>
// Add2
#include <add2/BeConstants.h>
#include <add2/FUConstants.h>
#include <add2/MiscDebug.h>
#include <add2/MiscUtils.h>
#include <add2/SHINTAUtils.h>
// Proj
#include "Constants.h"
#include "LKKConstants.h"
#include "PropertyWindow.h"
//==============================================================================
TClipboardView::TClipboardView(BRect oFrame, BView* oOwner)
		: SUPER(oFrame, "ClipboardView", oOwner)
{
}
//------------------------------------------------------------------------------
TClipboardView::~TClipboardView()
{
}
//------------------------------------------------------------------------------
void	TClipboardView::AllAttached()
{
	SUPER::AllAttached();
	if ( (modifiers() & FUC_CLEAN_LAUNCH_KEY) == FUC_CLEAN_LAUNCH_KEY ) {
		ClipboardSaveEntry().Remove();
	} else if ( gPrefs.mSaveClipboard ) {
		LoadData(ClipboardSaveEntry());
	}
	if ( be_clipboard->StartWatching(BMessenger(this)) != B_OK ) {
		DBEXP("StartWatching", "failed");
	}
}
//------------------------------------------------------------------------------
void	TClipboardView::AllDetached()
{
	be_clipboard->StopWatching(BMessenger(this));
	if ( gPrefs.mSaveClipboard ) {
		SaveData(ClipboardSaveEntry());
	}
}
//------------------------------------------------------------------------------
void	TClipboardView::ClipboardChanged()
{
	BMessage*	aClipData;

	if ( be_clipboard->Lock() ) {
		aClipData = be_clipboard->Data();
		if ( DetectAndAddData(aClipData) == B_OK ) {
			if ( static_cast<int32>(mHoldDatas.size()) > gPrefs.mNumClipboardHistories ) {
				DeleteData(mHoldDatas.size()-1);
			}
			SendExpStatBarRequested(BString(_T(LKK_STAT_BAR_ADD_HISTORY)).
					ReplaceAll(LKR_NUM, (BString() << mHoldDatas.size()).String()).String());
		}
		be_clipboard->Unlock();
	}
}
//------------------------------------------------------------------------------
BEntry	TClipboardView::ClipboardSaveEntry() const
{
	BEntry	aEntry;

	get_settings_entry(FOLDER_NAME_TEXT_BANK_SETTINGS, FILE_NAME_CLIPBOARDS, &aEntry);
	return aEntry;
}
//------------------------------------------------------------------------------
void	TClipboardView::DeleteTopHistoryRequested()
{
	if ( mHoldDatas.empty() )
		return;
	DeleteData(0);
	if ( !mHoldDatas.empty() ) {
		PushToClipboard(0);
	}
	SendExpStatBarRequested(BString(_T(LKK_STAT_BAR_DELETE_HISTORY)).
			ReplaceAll(LKR_NUM, (BString() << mHoldDatas.size()).String()).String());
}
//------------------------------------------------------------------------------
void	TClipboardView::MessageReceived(BMessage* oMsg)
{
	// ドロップされたメッセージの処理
	if ( oMsg->WasDropped() && oMsg->IsSourceRemote() ) {
		if ( DetectAndAddData(oMsg) == B_OK ) {
			PushToClipboard(0);
			SendExpStatBarRequested(BString(_T(LKK_STAT_BAR_ADD_HISTORY)).
					ReplaceAll(LKR_NUM, (BString() << mHoldDatas.size()).String()).String());
		}
		return;
	}
	// その他のメッセージの処理
	switch ( oMsg->what ) {
	case B_CLIPBOARD_CHANGED:
		ClipboardChanged();
		break;
	case DELETE_TOP_HISTORY_REQUESTED:
		DeleteTopHistoryRequested();
		break;
	case POP_MITEM_DELETE_CLICKED:
		PopMItemDeleteClicked();
		break;
	case POP_MITEM_SAVE_AS_TEMPLATE_CLICKED:
		PopMItemSaveAsTemplateClicked();
		break;
	case POP_MITEM_SEND_TO_CLIPBOARD_CLICKED:
		PopMItemSendToClipboardClicked();
		break;
	case PREF_NUM_CLIPBOARD_HISTORIES_CHANGED:
		PrefNumClipboardHistoriesChanged();
		break;
	default:
		SUPER::MessageReceived(oMsg);
	}
}
//------------------------------------------------------------------------------
void	TClipboardView::MouseClickLeftDouble(BPoint oPoint)
{
	if ( CurrentSelection() >= 0 ) {
		PushSelectionsToClipboard(true);
		SendExpStatBarRequested(_T(LKK_STAT_BAR_SEND_TO_CLIPBOARD));
//		DBEXP("TClipboardView::MouseClickLeftDouble()", gPrefs.mLoseFocus);
		if ( gPrefs.mLoseFocus && Window()->IsActive() ) {
			Window()->Activate(false);
		}
	}
}
//------------------------------------------------------------------------------
void	TClipboardView::OpenPopUpMenu(BPoint oPoint) const
{
	BMenuItem*	aMItem;
	BPopUpMenu*	aPopMenu;

	aPopMenu = new BPopUpMenu("ClipboardView PopUpMenu", false, false);
	// メニュー作成
	if ( CurrentSelection() >= 0 ) {
		if ( CurrentSelection(1) < 0 ) {
			// クリップボードに送る
			aMItem = new BMenuItem(acc_label("C", _T(LKK_POP_MITEM_SEND_TO_CLIPBOARD)).c_str(),
					new BMessage(POP_MITEM_SEND_TO_CLIPBOARD_CLICKED), 'C');
			aMItem->SetTarget(this);
			aPopMenu->AddItem(aMItem);
		}
		// 定型文として保存
		aMItem = new BMenuItem(acc_label("S", _T(LKK_POP_MITEM_SAVE_AS_TEMPLATE)).c_str(),
				new BMessage(POP_MITEM_SAVE_AS_TEMPLATE_CLICKED), 'S');
		aMItem->SetTarget(this);
		aPopMenu->AddItem(aMItem);
		aPopMenu->AddSeparatorItem();
		// 削除
		AddDeletePopUpMItems(aPopMenu);
	}
	// その他
	AddToolPopUpMItems(aPopMenu);
	AddHelpPopUpMItems(aPopMenu);
	// 表示
	aPopMenu->Go(ConvertToScreen(oPoint), true, true);
	delete aPopMenu;
}
//------------------------------------------------------------------------------
void	TClipboardView::PopMItemDeleteClicked()
{
	bool	aPush;

//	DBEXP("TClipboardView::PopMItemDeleteClicked()", "");
	aPush = IsItemSelected(0);
	DeleteSelections();
	if ( aPush && CountItems() > 0 ) {
		PushToClipboard(0);
	}
	SendExpStatBarRequested(BString(_T(LKK_STAT_BAR_DELETE_HISTORY)).
			ReplaceAll(LKR_NUM, (BString() << mHoldDatas.size()).String()).String());
}
//------------------------------------------------------------------------------
void	TClipboardView::PopMItemSaveAsTemplateClicked()
{
	BMessage	aSendMsg(SAVE_AS_TEMPLATE_REQUESTED);

	for ( int32 i = 0 ; i < CountItems() ; i++ ) {
		if ( IsItemSelected(i) ) {
			aSendMsg.AddMessage(MSG_NAME_DATA, &mHoldDatas[i].mData);
		}
	}
	BMessenger(mOwner).SendMessage(&aSendMsg);
}
//------------------------------------------------------------------------------
void	TClipboardView::PopMItemSendToClipboardClicked()
{
	PushSelectionsToClipboard(true);
	SendExpStatBarRequested(_T(LKK_STAT_BAR_SEND_TO_CLIPBOARD));
}
//------------------------------------------------------------------------------
void	TClipboardView::PrefNumClipboardHistoriesChanged()
{
//	DBEXP("TClipboardView::PrefNumClipboardHistoriesChanged()", "");
	for ( deque<TTextData>::size_type i = mHoldDatas.size()-1 ; static_cast<int32>(i) >= gPrefs.mNumClipboardHistories ; i-- ) {
		DeleteData(i);
	}
	SendExpStatBarRequested(string());
}
//------------------------------------------------------------------------------
status_t	TClipboardView::SequenceCheck(const TTextData& oNewData) const
{
	const char*	aCurText;
	const char*	aNewText;
	ssize_t		aCurSize;
	ssize_t		aNewSize;
	TTextData	aNewData;

	if ( oNewData.mData.FindData(FUC_MIME_TYPE_PLAIN_TEXT, B_MIME_TYPE, (const void**)&aNewText, &aNewSize) != B_OK )
		return B_ERROR;
	// 直前の文字列との重複チェック
	if ( !mHoldDatas.empty() ) {
		if ( mHoldDatas[0].mData.FindData(FUC_MIME_TYPE_PLAIN_TEXT, B_MIME_TYPE, (const void**)&aCurText, &aCurSize) == B_OK ) {
			if ( aCurSize == aNewSize && strncmp(aCurText, aNewText, aNewSize) == 0 )
				return B_ERROR;
		}
	}
	return B_OK;
}
//------------------------------------------------------------------------------
//==============================================================================
TTemplateView::TTemplateView(BRect oFrame, BView* oOwner)
		: SUPER(oFrame, "TemplateView", oOwner)
{
}
//------------------------------------------------------------------------------
TTemplateView::~TTemplateView()
{
}
//------------------------------------------------------------------------------
void	TTemplateView::AllAttached()
{
	BEntry	aEntry;

	if ( (modifiers() & FUC_CLEAN_LAUNCH_KEY) == FUC_CLEAN_LAUNCH_KEY ) {
		TemplateSaveEntry().Remove();	// ここで aEntry に代入すると Remove() 後も有効なのかわからないからやめておく
	}
	if ( !TemplateSaveEntry().Exists() ) {
		aEntry = TemplateSaveEntry();
		copy_file((gAppPrivacy.ExePath()+FILE_NAME_DEFAULT_TEMPLATES).c_str(), BPath(&aEntry).Path());
	}
	LoadData(TemplateSaveEntry());
}
//------------------------------------------------------------------------------
void	TTemplateView::ChangeOrderRequested(BMessage* oMsg)
{
	int32	aAfter;
	int32	aBefore;
	uint32	aButtons;
	BPoint	aCursor;

//	DBEXP("TTemplateView::ChangeOrderRequested()", "");
	// 移動元
	if ( oMsg->FindInt32(MSG_NAME_INDEX, &aBefore) != B_OK )
		return;
//	DBEXP("TTemplateView::ChangeOrderRequested() - 移動元", aBefore);
	// 移動先
	GetMouse(&aCursor, &aButtons);
	MouseUp(aCursor);
//	aAfter = static_cast<int32>(aCursor.y/mItemHeight);
	aAfter = FullListIndexOf(aCursor);
	if ( aAfter < 0 ) {
		aAfter = 0;
	}
	if ( aAfter >= static_cast<int32>(mHoldDatas.size()) ) {
		aAfter = mHoldDatas.size()-1;
	}
//	DBEXP("TTemplateView::ChangeOrderRequested() - 移動先", aAfter);
	// 移動
	DeselectAll();
	MoveData(aBefore, aAfter);
	SaveData(TemplateSaveEntry());
}
//------------------------------------------------------------------------------
void	TTemplateView::DataPropertyChanged(BMessage* oMsg)
{
	bool		aNeedSave;
	int32		aIndex;
	BMessage	aArc;
	TTextData	aNewData;

	if ( oMsg->FindMessage(MSG_NAME_DATA, &aArc) != B_OK )
		return;
	if ( oMsg->FindBool(MSG_NAME_NEED_SAVE, &aNeedSave) != B_OK )
		return;
	aNewData.Unarchive(aArc);
	aIndex = FindID(aNewData.mID);
	if ( aIndex < 0 )
		return;
	ReplaceData(aNewData, aIndex);
	if ( aNeedSave ) {
		SaveData(TemplateSaveEntry());
	}
}
//------------------------------------------------------------------------------
bool	TTemplateView::DoDataPatternMatch() const
{
	return true;
}
//------------------------------------------------------------------------------
int32	TTemplateView::FindID(int32 oID) const
{
	for ( deque<TTextData>::size_type i = 0 ; i < mHoldDatas.size() ; i++ ) {
		if ( mHoldDatas[i].mID == oID ) {
			return i;
		}
	}
	return -1;
}
//------------------------------------------------------------------------------
int32	TTemplateView::GenerateDataID() const
{
	int32	aID = DATA_ID_BASE;

	while ( FindID(aID) >= 0 ) {
		aID++;
	}
	return aID;
}
//------------------------------------------------------------------------------
void	TTemplateView::MessageReceived(BMessage* oMsg)
{
	// ドロップされたメッセージの処理
	if ( oMsg->WasDropped() && oMsg->IsSourceRemote() ) {
		if ( DetectAndAddData(oMsg) == B_OK ) {
			SendExpStatBarRequested(BString(_T(LKK_STAT_BAR_ADD_TEMPLATE)).
					ReplaceAll(LKR_NUM, (BString() << mHoldDatas.size()).String()).String());
			SaveData(TemplateSaveEntry());
		}
		return;
	}
	// その他のメッセージの処理
	switch ( oMsg->what ) {
	case CHANGE_ORDER_REQUESTED:
		ChangeOrderRequested(oMsg);
		break;
	case DATA_PROPERTY_CHANGED:
		DataPropertyChanged(oMsg);
		break;
	case POP_MITEM_DELETE_CLICKED:
		PopMItemDeleteClicked();
		break;
	case POP_MITEM_LOCK_TEMPLATE_ORDER_CLICKED:
		PopMItemLockTemplateOrderClicked();
		break;
	case POP_MITEM_MOVE_DOWN_CLICKED:
		PopMItemMoveDownClicked();
		break;
	case POP_MITEM_MOVE_UP_CLICKED:
		PopMItemMoveUpClicked();
		break;
	case POP_MITEM_NEW_GROUP_CLICKED:
		PopMItemNewGroupClicked();
		break;
	case POP_MITEM_PROPERTY_CLICKED:
		PopMItemPropertyClicked();
		break;
	case POP_MITEM_SEND_TO_CLIPBOARD_CLICKED:
		PopMItemSendToClipboardClicked();
		break;
	case SAVE_AS_TEMPLATE_REQUESTED:
		SaveAsTemplateRequested(oMsg);
		break;
	default:
		SUPER::MessageReceived(oMsg);
	}
}
//------------------------------------------------------------------------------
void	TTemplateView::MouseClickLeftDouble(BPoint oPoint)
{
	if ( CurrentSelection() >= 0 ) {
		PushSelectionsToClipboard(false);
		SendExpStatBarRequested(_T(LKK_STAT_BAR_SEND_TO_CLIPBOARD));
//		DBEXP("TClipboardView::MouseClickLeftDouble()", gPrefs.mLoseFocus);
		if ( gPrefs.mLoseFocus && Window()->IsActive() ) {
			Window()->Activate(false);
		}
	}
}
//------------------------------------------------------------------------------
void	TTemplateView::OpenPopUpMenu(BPoint oPoint) const
{
	BMenuItem*	aMItem;
	BPopUpMenu*	aPopMenu;

	aPopMenu = new BPopUpMenu("TemplateView PopUpMenu", false, false);
	// メニュー作成
	if ( CurrentSelection() >= 0 ) {
		if ( CurrentSelection(1) < 0 ) {
			// クリップボードに送る
			aMItem = new BMenuItem(acc_label("C", _T(LKK_POP_MITEM_SEND_TO_CLIPBOARD)).c_str(),
					new BMessage(POP_MITEM_SEND_TO_CLIPBOARD_CLICKED), 'C');
			aMItem->SetTarget(this);
			aPopMenu->AddItem(aMItem);
			// プロパティ
			aMItem = new BMenuItem((acc_label("T", _T(LKK_POP_MITEM_PROPERTY))+FUC_ELLIPSIS).c_str(),
					new BMessage(POP_MITEM_PROPERTY_CLICKED), 'T');
			aMItem->SetTarget(this);
			aPopMenu->AddItem(aMItem);
			aPopMenu->AddSeparatorItem();
			// 1 つ上に移動
			aMItem = new BMenuItem(acc_label("U", _T(LKK_POP_MITEM_MOVE_UP)).c_str(),
					new BMessage(POP_MITEM_MOVE_UP_CLICKED), B_UP_ARROW);
			aMItem->SetEnabled(!gPrefs.mLockTemplateOrder);
			aMItem->SetTarget(this);
			aPopMenu->AddItem(aMItem);
			// 1 つ下に移動
			aMItem = new BMenuItem(acc_label("O", _T(LKK_POP_MITEM_MOVE_DOWN)).c_str(),
					new BMessage(POP_MITEM_MOVE_DOWN_CLICKED), B_DOWN_ARROW);
			aMItem->SetEnabled(!gPrefs.mLockTemplateOrder);
			aMItem->SetTarget(this);
			aPopMenu->AddItem(aMItem);
			// 定型文の順序変更を禁止
			aMItem = new BMenuItem(acc_label("L", _T(LKK_POP_MITEM_LOCK_TEMPLATE_ORDER)).c_str(),
					new BMessage(POP_MITEM_LOCK_TEMPLATE_ORDER_CLICKED));
			aMItem->SetMarked(gPrefs.mLockTemplateOrder);
			aMItem->SetTarget(this);
			aPopMenu->AddItem(aMItem);
			aPopMenu->AddSeparatorItem();
		}
		AddDeletePopUpMItems(aPopMenu);
	}
	// 新規グループ
	aMItem = new BMenuItem(acc_label("N", _T(LKK_POP_MITEM_NEW_GROUP)).c_str(),
			new BMessage(POP_MITEM_NEW_GROUP_CLICKED), 'N');
	aMItem->SetTarget(this);
	aPopMenu->AddItem(aMItem);
	aPopMenu->AddSeparatorItem();
	// その他
	AddToolPopUpMItems(aPopMenu);
	AddHelpPopUpMItems(aPopMenu);
	// 表示
	aPopMenu->Go(ConvertToScreen(oPoint), true, true);
	delete aPopMenu;
}
//------------------------------------------------------------------------------
void	TTemplateView::PopMItemDeleteClicked()
{
	DeleteSelections();
	SendExpStatBarRequested(BString(_T(LKK_STAT_BAR_DELETE_TEMPLATE)).
			ReplaceAll(LKR_NUM, (BString() << mHoldDatas.size()).String()).String());
	SaveData(TemplateSaveEntry());
}
//------------------------------------------------------------------------------
void	TTemplateView::PopMItemLockTemplateOrderClicked()
{
	BMessage	aSendMsg(PREF_LOCK_TEMPLATE_ORDER_CHANGED);

	gPrefs.mLockTemplateOrder = !gPrefs.mLockTemplateOrder;
	gPrefs.SaveToFile();
	be_app_messenger.SendMessage(&aSendMsg);
}
//------------------------------------------------------------------------------
void	TTemplateView::PopMItemMoveDownClicked()
{
	int32	aAfter;
	int32	aBefore = FullListCurrentSelection(0);
	uint32	aBeforeLevel;

	if ( mHoldDatas.size() < 2 || IsItemSelected(FullListIndexOf(ItemAt(mHoldDatas.size()-1))) || aBefore < 0 || CurrentSelection(1) >= 0 )
		return;
	// 移動先インデックスの決定（グループの時は子供を飛び越えてインデックスを指定）
	if ( mHoldDatas[aBefore].mGroup ) {
		aBeforeLevel = FullListItemAt(aBefore)->OutlineLevel();
		aAfter = aBefore+1;
		while ( aAfter < static_cast<int32>(mHoldDatas.size()) && FullListItemAt(aAfter)->OutlineLevel() > aBeforeLevel ) {
			aAfter++;
		}
	} else {
		aAfter = aBefore+1;
	}
	// 移動
	MoveData(aBefore, aAfter);
	SaveData(TemplateSaveEntry());
}
//------------------------------------------------------------------------------
void	TTemplateView::PopMItemMoveUpClicked()
{
	int32	aBefore = FullListCurrentSelection(0);

	if ( mHoldDatas.size() < 2 || IsItemSelected(0) || aBefore < 0 || CurrentSelection(1) >= 0 )
		return;
	// 移動
	MoveData(aBefore, aBefore-1);
	SaveData(TemplateSaveEntry());
}
//------------------------------------------------------------------------------
void	TTemplateView::PopMItemNewGroupClicked()
{
	TTextData	aNewData;

	aNewData.mGroup = true;
	aNewData.mTitle = _T(LKK_NEW_GROUP);
	AddData(aNewData, 0);
	SaveData(TemplateSaveEntry());
}
//------------------------------------------------------------------------------
void	TTemplateView::PopMItemPropertyClicked()
{
	int32				aWinIndex = 0;
	BRect				aWinFrame;
	TPropertyWindow*	aPropWin = NULL;
	TPropertyWindow*	aPropWinTmp;
	TTextData			aTextData;

	if ( CurrentSelection() < 0 )
		return;
	aTextData = mHoldDatas[FullListCurrentSelection()];
	// 既にウィンドウを開いているか検索
	while ( be_app->WindowAt(aWinIndex) != NULL ) {
		aPropWinTmp = dynamic_cast<TPropertyWindow*>(be_app->WindowAt(aWinIndex));
		if ( aPropWinTmp != NULL && aPropWinTmp->OriginalData().mID == aTextData.mID ) {
			aPropWin = aPropWinTmp;
			break;
		}
		aWinIndex++;
	}
	// ウィンドウ作成
	if ( aPropWin == NULL ) {
		aWinFrame = be_app->WindowAt(0)->Frame();
		aWinFrame.top += FUC_HEIGHT_WIN_TAB;
		aPropWin = new TPropertyWindow(aWinFrame, aTextData);
		if ( aPropWin->InitCheck() == B_OK ) {
			aPropWin->IntelligentMoveTo(aWinFrame.LeftTop());
			aPropWin->SetTarget(this);
		}
	}
	if ( aPropWin->InitCheck() == B_OK ) {
		aPropWin->Appear();
	} else {
		show_alert3(alert_caption(),
				string(_T(LKK_ERR_CANNOT_SHOW_PROPERTY_WIN))+"\n"+_T(LKK_HINT_EXCUTABLE_MAYBE_BROKEN),
				acc_label("U", _T(LKK_BUTTON_UNDERSTAND)), "", "", -1, B_WARNING_ALERT);
	}
}
//------------------------------------------------------------------------------
void	TTemplateView::PopMItemSendToClipboardClicked()
{
	PushSelectionsToClipboard(false);
	SendExpStatBarRequested(_T(LKK_STAT_BAR_SEND_TO_CLIPBOARD));
}
//------------------------------------------------------------------------------
void	TTemplateView::SaveAsTemplateRequested(BMessage* oMsg)
{
	int32		aNumDatas;
	status_t	aResult = B_OK;
	type_code	aType;
	BMessage	aData;

	oMsg->GetInfo(MSG_NAME_DATA, &aType, &aNumDatas);
	for ( int32 i = 0 ; i < aNumDatas ; i++ ) {
		oMsg->FindMessage(MSG_NAME_DATA, i, &aData);
		aResult &= DetectAndAddData(&aData);
	}
	if ( aResult == B_OK ) {
		SendExpStatBarRequested(BString(_T(LKK_STAT_BAR_ADD_TEMPLATE)).
				ReplaceAll(LKR_NUM, (BString() << mHoldDatas.size()).String()).String());
		SaveData(TemplateSaveEntry());
	}
}
//------------------------------------------------------------------------------
status_t	TTemplateView::SequenceCheck(const TTextData& oNewData) const
{
	return B_OK;
}
//------------------------------------------------------------------------------
BEntry	TTemplateView::TemplateSaveEntry() const
{
	BEntry	aEntry;

	get_settings_entry(FOLDER_NAME_TEXT_BANK_SETTINGS, FILE_NAME_TEMPLATES, &aEntry);
	return aEntry;
}
//------------------------------------------------------------------------------
//==============================================================================
TTextBankListView::TTextBankListView(BRect oFrame, const char* oName, BView* oOwner)
		: SUPER(oFrame, oName, B_MULTIPLE_SELECTION_LIST, B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_NAVIGABLE),
		TMouseDownManager(this)
{
	mOwner = oOwner;
}
//------------------------------------------------------------------------------
TTextBankListView::~TTextBankListView()
{
}
//------------------------------------------------------------------------------
status_t	TTextBankListView::AddData(const TTextData& oNewData, int32 oIndex)
{
	AddDataToList(oNewData, oIndex);
	AddDataToView(oNewData, oIndex);
	return B_OK;
}
//------------------------------------------------------------------------------
status_t	TTextBankListView::AddDataToList(const TTextData& oNewData, int32 oIndex)
{
	deque<TTextData>::iterator	p;

	p = mHoldDatas.insert(mHoldDatas.begin()+oIndex, oNewData);
	p->mID = GenerateDataID();
	p->mPatternMatch = DoDataPatternMatch();
	return B_OK;
}
//------------------------------------------------------------------------------
status_t	TTextBankListView::AddDataToView(const TTextData& oNewData, int32 oIndex)
{
	AddItem(new BStringItem(HoldDataLabel(oNewData).c_str(), oNewData.mOutlineLevel), oIndex);
	return B_OK;
}
//------------------------------------------------------------------------------
void	TTextBankListView::AddDeletePopUpMItems(BPopUpMenu* oPopMenu) const
{
	BMenuItem*	aMItem;

	// 削除
	aMItem = new BMenuItem(acc_label("D", _T(LKK_POP_MITEM_DELETE)).c_str(),
			new BMessage(POP_MITEM_DELETE_CLICKED), 'D');
	aMItem->SetTarget(this);
	oPopMenu->AddItem(aMItem);
	oPopMenu->AddSeparatorItem();
}
//------------------------------------------------------------------------------
void	TTextBankListView::AddHelpPopUpMItems(BPopUpMenu* oPopMenu) const
{
	BMenuItem*	aMItem;

	// ヘルプ
	aMItem = new BMenuItem(acc_label("H", _T(LKK_POP_MITEM_HELP)).c_str(),
			new BMessage(POP_MITEM_HELP_CLICKED));
	aMItem->SetTarget(be_app);
	oPopMenu->AddItem(aMItem);
	// ＳＨＩＮＴＡのホームページを開く
	aMItem = new BMenuItem(acc_label("J", BString(_T(LKK_POP_MITEM_AUTHOR_WEB)).ReplaceAll(LKR_APP_AUTHOR, _T(gAppAuthorEnUs)).String()).c_str(),
			new BMessage(POP_MITEM_AUTHOR_WEB_CLICKED));
	aMItem->SetTarget(be_app);
	oPopMenu->AddItem(aMItem);
	// バージョン情報
	aMItem = new BMenuItem((acc_label("A", _T(LKK_POP_MITEM_ABOUT))+FUC_ELLIPSIS).c_str(),
			new BMessage(B_ABOUT_REQUESTED));
	aMItem->SetTarget(be_app);
	oPopMenu->AddItem(aMItem);
}
//------------------------------------------------------------------------------
int32	TTextBankListView::AddHoldDataMItem(BMenu* oMenu, int32 oIndex, int32 oLabelIndex) const
{
	string		aLabel;
	BMenuItem*	aMItem;
	BMessage*	aMsg;

	aMsg = new BMessage(POP_MITEM_HOLD_DATA_CLICKED);
	aMsg->AddMessage(MSG_NAME_DATA, &mHoldDatas[oIndex].mData);
	aMsg->AddBool(MSG_NAME_PATTERN_MATCH, mHoldDatas[oIndex].mPatternMatch);
	aMItem = new BMenuItem(HoldDataMItemLabel(oLabelIndex, HoldDataLabel(mHoldDatas[oIndex])).c_str(), aMsg);
	aMItem->SetTrigger(tolower(HoldDataMItemAccelerator(oLabelIndex)[0]));
	oMenu->AddItem(aMItem);
	return oIndex+1;
}
//------------------------------------------------------------------------------
int32	TTextBankListView::AddHoldDataSubMenu(BMenu* oMenu, int32 oIndex, int32 oLabelIndex) const
{
	deque<TTextData>::size_type	aSubIndex = oIndex+1;
	BMenu*	aSubMenu;

	aSubMenu = new BMenu(HoldDataMItemLabel(oLabelIndex, mHoldDatas[oIndex].mTitle).c_str());
	oLabelIndex = 0;
	while ( aSubIndex < mHoldDatas.size() ) {
		if ( FullListItemAt(aSubIndex)->OutlineLevel() <= FullListItemAt(oIndex)->OutlineLevel() )
			break;
		if ( mHoldDatas[aSubIndex].mGroup ) {
			aSubIndex = AddHoldDataSubMenu(aSubMenu, aSubIndex, oLabelIndex);
		} else {
			aSubIndex = AddHoldDataMItem(aSubMenu, aSubIndex, oLabelIndex);
		}
		oLabelIndex++;
	}
	oMenu->AddItem(aSubMenu);
	return aSubIndex;
}
//------------------------------------------------------------------------------
void	TTextBankListView::AddToolPopUpMItems(BPopUpMenu* oPopMenu) const
{
	BMenuItem*	aMItem;

	// 環境設定
	aMItem = new BMenuItem((acc_label("P", _T(LKK_POP_MITEM_PREFS))+FUC_ELLIPSIS).c_str(),
			new BMessage(POP_MITEM_PREFS_CLICKED));
	aMItem->SetTarget(be_app);
	oPopMenu->AddItem(aMItem);
	// ＳＨＩＮＴＡ共通環境設定
	aMItem = new BMenuItem((acc_label("I", _T(LKK_POP_MITEM_SHINTA_COMMON_PREFS))+FUC_ELLIPSIS).c_str(),
			new BMessage(POP_MITEM_SHINTA_COMMON_PREFS_CLICKED));
	aMItem->SetTarget(be_app);
	oPopMenu->AddItem(aMItem);
	oPopMenu->AddSeparatorItem();
}
//------------------------------------------------------------------------------
void	TTextBankListView::AttachedToWindow()
{
//	AddItem(new BStringItem(" "), 0);
//	mItemHeight = ItemFrame(0).bottom-ItemFrame(0).top+1;
//	delete RemoveItem(static_cast<int32>(0));
}
//------------------------------------------------------------------------------
BMenu*	TTextBankListView::CreateHoldDataMenu(const char* oLabel) const
{
	int32		aLabelIndex = 0;
	deque<TTextData>::size_type	aIndex = 0;
	BMenu*		aMenu;

	aMenu = new BMenu(oLabel);
	// アイテムの追加
	while ( aIndex < mHoldDatas.size() ) {
		if ( mHoldDatas[aIndex].mGroup ) {
			aIndex = AddHoldDataSubMenu(aMenu, aIndex, aLabelIndex);
		} else {
			aIndex = AddHoldDataMItem(aMenu, aIndex, aLabelIndex);
		}
		aLabelIndex++;
	}
	// 設定
	SetHoldDataMenu(aMenu);
	return aMenu;
}
//------------------------------------------------------------------------------
void	TTextBankListView::DeleteData(int32 oIndex)
{
	int32	aNumDeleteDatas = 1;
	uint32	aLevel;
	deque<TTextData>::size_type	aCheckIndex;

	// 削除すべきデータの数を数える（グループの時は子供を全部削除する）
	if ( mHoldDatas[oIndex].mGroup ) {
		aLevel = FullListItemAt(oIndex)->OutlineLevel();
		aCheckIndex = oIndex+1;
		while ( aCheckIndex < mHoldDatas.size() && FullListItemAt(aCheckIndex)->OutlineLevel() > aLevel ) {
			aNumDeleteDatas++;
			aCheckIndex++;
		}
	}
//	DBEXP("TTextBankListView::MoveData() - aNumMoveDatas", aNumMoveDatas);
	// データ群を削除
		// データ
	mHoldDatas.erase(mHoldDatas.begin()+oIndex, mHoldDatas.begin()+oIndex+aNumDeleteDatas);
		// アイテム
	for ( int32 i = aNumDeleteDatas-1 ; i >= 0 ; i-- ) {
		delete RemoveItem(oIndex+i);
	}
}
//------------------------------------------------------------------------------
void	TTextBankListView::DeleteSelections()
{
	for ( int32 i = CountItems() -1 ; i >= 0 ; i-- ) {
		if ( IsItemSelected(i) ) {
			DeleteData(FullListIndexOf(ItemAt(i)));
		}
	}
}
//------------------------------------------------------------------------------
status_t	TTextBankListView::DetectAndAddData(const BMessage* oNewData)
{
	const char*	aText;
	int32		aNumERefs;
	entry_ref	aERef;
	ssize_t		aSize;
	status_t	aResult = B_OK;
	type_code	aType;
	BMessage	aPathData;
	TTextData	aNewData;

	if ( oNewData->FindData(FUC_MIME_TYPE_PLAIN_TEXT, B_MIME_TYPE, (const void**)&aText, &aSize) == B_OK ) {	// データは文字列を含む
		aNewData.mData = *oNewData;
		if ( SequenceCheck(aNewData) == B_OK ) {
			aResult = AddData(aNewData, 0);
		} else {
			aResult = B_ERROR;
		}
	} else if ( oNewData->FindRef(B_MSG_NAME_REFS, &aERef) == B_OK ) {	// データは entry_ref
		// パス文字列に変換して追加
		oNewData->GetInfo(B_MSG_NAME_REFS, &aType, &aNumERefs);
		for ( int32 i = 0 ; i < aNumERefs ; i++ ) {
			if ( oNewData->FindRef(B_MSG_NAME_REFS, i, &aERef) != B_OK )
				continue;
			aPathData.MakeEmpty();
			aPathData.AddData(FUC_MIME_TYPE_PLAIN_TEXT, B_MIME_TYPE, BPath(&aERef).Path(), strlen(BPath(&aERef).Path()));
			aNewData.mData = aPathData;
			if ( SequenceCheck(aNewData) == B_OK ) {
				aResult &= AddData(aNewData, 0);
			} else {
				aResult &= B_ERROR;
			}
		}
	} else {
		aResult = B_ERROR;
	}
	return aResult;
}
//------------------------------------------------------------------------------
bool	TTextBankListView::DoDataPatternMatch() const
{
	return false;
}
//------------------------------------------------------------------------------
int32	TTextBankListView::GenerateDataID() const
{
	return 0;
}
//------------------------------------------------------------------------------
string	TTextBankListView::HoldDataLabel(const TTextData& oData) const
{
	const char*	aText;
	ssize_t		aSize;
	BString		aWorkString;

	if ( !oData.mTitle.empty() ) {
		aWorkString = oData.mTitle.c_str();
	} else {
		// テキストデータをタイトルとする
		if ( oData.mData.FindData(FUC_MIME_TYPE_PLAIN_TEXT, B_MIME_TYPE, (const void**)&aText, &aSize) == B_OK ) {
			aWorkString.SetTo(aText, aSize);
			// エスケープ
			aWorkString.ReplaceAll(ESCAPE_NEW_LINE_FROM, ESCAPE_NEW_LINE_TO);
		}
	}
	if ( oData.mGroup ) {
		aWorkString = BString(_T(LKK_GROUP_TITLE)).ReplaceAll(LKR_STRING, aWorkString.String());
	}
	return aWorkString.String();
}
//------------------------------------------------------------------------------
string	TTextBankListView::HoldDataMItemAccelerator(int32 oSubIndex) const
{
	if ( 0 <= oSubIndex && oSubIndex <= 9 )
		return string(1, '0'+oSubIndex);
	if ( 10 <= oSubIndex && oSubIndex <= 35 )
		return string(1, 'A'+oSubIndex-10);
	return string();
}
//------------------------------------------------------------------------------
string	TTextBankListView::HoldDataMItemLabel(int32 oSubIndex, const string& oLabel) const
{
	string	aAcc;

	aAcc = HoldDataMItemAccelerator(oSubIndex);
	if ( aAcc.empty() )
		return oLabel;
	return acc_label(aAcc, oLabel);
}
//------------------------------------------------------------------------------
void	TTextBankListView::KeyDown(const char* oBytes, int32 oNumBytes)
{
	int32		aFunctionKey;
	BMessage	aSendMsg;

	switch ( *oBytes ) {
	case B_DELETE:	// 削除
		aSendMsg.what = POP_MITEM_DELETE_CLICKED;
		BMessenger(this).SendMessage(&aSendMsg);
		break;
	case B_ENTER:	// クリップボードにコピー
		MouseClickLeftDouble(ItemFrame(CurrentSelection()).LeftTop());
		break;
	case B_FUNCTION_KEY:	// ヘルプ
		Window()->CurrentMessage()->FindInt32(B_MSG_NAME_KEY, &aFunctionKey);
		if ( aFunctionKey == B_F1_KEY ) {
			be_app_messenger.SendMessage(FUC_MITEM_HELP_CLICKED);
		}
		break;
	case '/':	// メニュー
		MouseClickRightSingle(BPoint(Bounds().right/2, 0));
		break;
	case 'c':	// クリップボード
		aSendMsg.what = ACTIVATE_CLIPBOARD_VIEW_REQUESTED;
		BMessenger(static_cast<BWindow*>(Window())).SendMessage(&aSendMsg);
		break;
	case 't':	// 定型文
		aSendMsg.what = ACTIVATE_TEMPLATE_VIEW_REQUESTED;
		BMessenger(static_cast<BWindow*>(Window())).SendMessage(&aSendMsg);
		break;
	default:
		SUPER::KeyDown(oBytes, oNumBytes);
	}
}
//------------------------------------------------------------------------------
status_t	TTextBankListView::LoadData(const BEntry& oEntry)
{
	int32		aNumDatas;
	type_code	aType;
	BFile		aFile;
	BMessage	aDataArc;
	BMessage	aTotalArc;
	TTextData	aData;

	if ( aFile.SetTo(&oEntry, B_READ_ONLY) != B_OK )
		return B_ERROR;
	if ( aTotalArc.Unflatten(&aFile) != B_OK )
		return B_ERROR;
	aTotalArc.GetInfo(ARC_NAME_DATA, &aType, &aNumDatas);
	for ( int32 i = 0 ; i < aNumDatas ; i++ ) {
		aTotalArc.FindMessage(ARC_NAME_DATA, i, &aDataArc);
		aData.Unarchive(aDataArc);
		AddData(aData, mHoldDatas.size());
	}
	return B_OK;
}
//------------------------------------------------------------------------------
void	TTextBankListView::MessageReceived(BMessage* oMsg)
{
	switch ( oMsg->what ) {
	case PASTE_MESSAGE_ENQUEUED:
		PasteMessageEnqueued(oMsg);
		break;
	case POP_MITEM_HOLD_DATA_CLICKED:
		PopMItemHoldDataClicked(oMsg);
		break;
	default:
		SUPER::MessageReceived(oMsg);
	}
}
//------------------------------------------------------------------------------
void	TTextBankListView::MouseClickRightSingle(BPoint oPoint)
{
	list<int32>::iterator	p;

	// 選択状態を復帰
	p = mMouseUpSelections.begin();
	while ( p != mMouseUpSelections.end() ) {
		Select(*p, true);
		p++;
	}
	OpenPopUpMenu(oPoint);
}
//------------------------------------------------------------------------------
void	TTextBankListView::MouseDown(BPoint oPoint)
{
	SUPER::MouseDown(oPoint);
	ManageMouseDown(oPoint);
}
//------------------------------------------------------------------------------
/* ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
折り畳んだアイテムがある場合、aIndex と aPseudoIndex の値は異なる
aIndex、aPseudoIndex 共に、スクロールによる変化は自動処理してくれるようだ
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''' */
void	TTextBankListView::MouseDragBeginLeft(BPoint oBeginPoint, BPoint oPoint)
{
	int32		aIndex = FullListIndexOf(oBeginPoint);
	int32		aPseudoIndex = IndexOf(oBeginPoint);
	BMessage	aDragMsg;

	if ( aIndex < 0 || aIndex >= static_cast<int32>(mHoldDatas.size()) )
		return;
	if ( mHoldDatas[aIndex].mPatternMatch ) {
		aDragMsg = ReplacePatterns(mHoldDatas[aIndex].mData);
	} else {
		aDragMsg = mHoldDatas[aIndex].mData;
	}
	if ( !gPrefs.mLockTemplateOrder ) {
		aDragMsg.what = CHANGE_ORDER_REQUESTED;
		aDragMsg.AddInt32(MSG_NAME_INDEX, aIndex);
//		aDragMsg.AddMessage(MSG_NAME_DATA, &mHoldDatas[aIndex].mData);
	}
	Select(aPseudoIndex);
	DragMessage(&aDragMsg, ItemFrame(aPseudoIndex));
}
//------------------------------------------------------------------------------
void	TTextBankListView::MouseMoved(BPoint oPoint, uint32 oTransit, const BMessage* oMsg)
{
	SUPER::MouseMoved(oPoint, oTransit, oMsg);
	ManageMouseMoved(oPoint, oTransit, oMsg);
}
//------------------------------------------------------------------------------
void	TTextBankListView::MouseUp(BPoint oPoint)
{
	int32		i = 0;

	// 複数のアイテムが選択されていても、SUPER::MouseUp() で 1 つのみの選択になってしまうので、選択状態を保存
	mMouseUpSelections.clear();
	while ( CurrentSelection(i) >= 0 ) {
		mMouseUpSelections.push_back(CurrentSelection(i));
		i++;
	}
	SUPER::MouseUp(oPoint);
	ManageMouseUp(oPoint);
}
//------------------------------------------------------------------------------
void	TTextBankListView::MoveData(int32 oBefore, int32 oAfter)
{
	int32	aNumMoveDatas = 1;
	uint32	aBeforeLevel;
	deque<TTextData>::size_type	aCheckIndex;
	vector<TTextData>			aBeforeDatas;
	vector<BStringItem*>		aBeforeItems;

//	DBEXP("TTextBankListView::MoveData() - Total", mHoldDatas.size());
//	DBEXP("TTextBankListView::MoveData() - After", oAfter);
	if ( oBefore == oAfter )
		return;
	// 移動すべきデータの数を数える（グループの時は子供を全部移動させる）
	if ( mHoldDatas[oBefore].mGroup ) {
		aBeforeLevel = FullListItemAt(oBefore)->OutlineLevel();
		aCheckIndex = oBefore+1;
		while ( aCheckIndex < mHoldDatas.size() && FullListItemAt(aCheckIndex)->OutlineLevel() > aBeforeLevel ) {
			aNumMoveDatas++;
			aCheckIndex++;
		}
	}
//	DBEXP("TTextBankListView::MoveData() - aNumMoveDatas", aNumMoveDatas);
	// 移動先のインデックスが移動元データ群の中に埋もれている場合は移動しない
	if ( oBefore <= oAfter && oAfter < oBefore+aNumMoveDatas )
		return;
	// 移動元データ群を退避
		// データ
	aBeforeDatas.resize(aNumMoveDatas);
	for ( int32 i = 0 ; i < aNumMoveDatas ; i++ ) {
		aBeforeDatas[i] = mHoldDatas[oBefore+i];
	}
	mHoldDatas.erase(mHoldDatas.begin()+oBefore, mHoldDatas.begin()+oBefore+aNumMoveDatas);
		// アイテム（所有権は aBeforeItems に移行）
	aBeforeItems.resize(aNumMoveDatas);
	for ( int32 i = 0 ; i < aNumMoveDatas ; i++ ) {
		aBeforeItems[i] = dynamic_cast<BStringItem*>(FullListItemAt(oBefore+i));
	}
	RemoveItem(oBefore);	// 子供は自動的に除去される
	// 移動先インデックスの補正
	if ( oBefore < oAfter ) {
		oAfter = oAfter-aNumMoveDatas+1;
		ASSERT(oAfter < 0, "TTextBankListView::MoveData() - Negative index");
	}
	MoveDataOfList(aBeforeDatas, oAfter);
	MoveDataOfView(aBeforeItems, oAfter);
//	mLastSelection = oAfter;
	if ( oBefore == 0 || oAfter == 0 ) {
		PushToClipboard(0);	// VU: 倉庫の文字列を移動した時もクリボにいってしまう
	}
}
//------------------------------------------------------------------------------
void	TTextBankListView::MoveDataOfList(const vector<TTextData>& oBeforeDatas, int32 oAfter)
{
	mHoldDatas.insert(mHoldDatas.begin()+oAfter, oBeforeDatas.begin(), oBeforeDatas.end());
}
//------------------------------------------------------------------------------
void	TTextBankListView::MoveDataOfView(const vector<BStringItem*>& oBeforeItems, int32 oAfter)
{
	int32			aNewLevel;
	int32			aPseudoIndex;
	BStringItem*	aNewItem;

//	DBEXP("TTextBankListView::MoveDataOfView() - Num Moves", oBeforeItems.size());
	// 移動先における階層の決定
	if ( oAfter == 0 ) {
		aNewLevel = 0;
	} else {
		aNewLevel = FullListItemAt(oAfter-1)->OutlineLevel()+mHoldDatas[oAfter-1].mGroup;
	}
	// 移動
	for ( vector<BStringItem*>::size_type i = 0 ; i < oBeforeItems.size() ; i++ ) {
		aNewItem = new BStringItem(oBeforeItems[i]->Text(), oBeforeItems[i]->OutlineLevel()+aNewLevel-oBeforeItems[0]->OutlineLevel());
//		DBEXP(aNewItem->Text(), aNewItem->OutlineLevel());
		AddItem(aNewItem, oAfter+i);
	}
	// 選択
	aPseudoIndex = IndexOf(FullListItemAt(oAfter));
//	DBEXP("TTextBankListView::MoveDataOfView - aPseudoIndex", aPseudoIndex);
	if ( aPseudoIndex >= 0 ) {
		Select(aPseudoIndex);
	}
	// 解放
	for ( vector<BStringItem*>::size_type i = 0 ; i < oBeforeItems.size() ; i++ ) {
		delete oBeforeItems[i];
	}
}
//------------------------------------------------------------------------------
BString	TTextBankListView::PaddingString(int32 oNum) const
{
	if ( oNum < 10 )
		return BString() << "0" << oNum;
	return BString() << oNum;
}
//------------------------------------------------------------------------------
void	TTextBankListView::PasteMessageEnqueued(BMessage* oMsg)
{
//	DBEXP("TTextBankListView::PasteMessageEnqueued()", "");
	snooze(FUC_TIME_SNOOZE_MIN);	// B_PASTE がエンキューされてから実際に貼り付けられるまで時間がかかるはず
//	set_mouse_position(static_cast<int32>(mMousePointClickList.x),static_cast<int32>(mMousePointClickList.y));
}
//------------------------------------------------------------------------------
void	TTextBankListView::PopMItemHoldDataClicked(BMessage* oMsg)
{
	bool		aPatternMatch;
	uint32		aButtons;
	BMessage	aData;
	BMessage	aSendMsg(SEND_PASTE_MESSAGE_REQUESTED);

	if ( oMsg->FindMessage(MSG_NAME_DATA, &aData) != B_OK )
		return;
	oMsg->FindBool(MSG_NAME_PATTERN_MATCH, &aPatternMatch);
//	DBEXP("TTextBankListView::PopMItemHoldDataClicked()", "");
	// FFM でなるべく確実にアクティブウィンドウが切り替わるように、早いとこマウスを動かしておく
	GetMouse(&mMousePointClickPopUp, &aButtons);
	ConvertToScreen(&mMousePointClickPopUp);
//	set_mouse_position(static_cast<int32>(mMousePointShowPopUp.x), static_cast<int32>(mMousePointShowPopUp.y));	// ZETA がフリーズしてしまう
	// QTClip の書式文字列を反映
//	if ( oMsg->FindBool(MSGNAME_IS_QT_CLIP_TXT,&aQTClip) == B_OK && aQTClip ) {
//		EditQTClipTxt(&aClipData);
//	}
	// クリップボードへ
	PushToClipboard(aData, aPatternMatch);
	aSendMsg.AddMessenger(MSG_NAME_ENQUEUE_NOTIFY_ADDRESS, BMessenger(this));
	snooze(FUC_TIME_SNOOZE_MIN);	// Focus Follow Mouse でアクティブなウィンドウが切り替わるまでには時間が必要
	send_message_to_assistant(PORT_NAME_ASST_PASTE_SENDER, aSendMsg);
}
//------------------------------------------------------------------------------
void	TTextBankListView::PushSelectionsToClipboard(bool oMoveToTop)
{
	for ( deque<BMessage>::size_type i = 0 ; i < mHoldDatas.size() ; i++ ) {
		if ( IsItemSelected(i) ) {
			PushToClipboard(FullListIndexOf(ItemAt(i)));
			if ( oMoveToTop ) {
				MoveData(FullListIndexOf(ItemAt(i)), 0);
			}
		}
	}
}
//------------------------------------------------------------------------------
void	TTextBankListView::PushToClipboard(int32 oIndex) const
{
	PushToClipboard(mHoldDatas[oIndex].mData, mHoldDatas[oIndex].mPatternMatch);
}
//------------------------------------------------------------------------------
void	TTextBankListView::PushToClipboard(const BMessage& oStrMsg, bool oPatternMatch) const
{
	BMessage*	aClipData;

	if ( be_clipboard->Lock() ) {
		be_clipboard->Clear();
		aClipData = be_clipboard->Data();
		if ( oPatternMatch ) {
			*aClipData = ReplacePatterns(oStrMsg);
		} else {
			*aClipData = oStrMsg;
		}
		be_clipboard->Commit();
		be_clipboard->Unlock();
	}
}
//------------------------------------------------------------------------------
status_t	TTextBankListView::ReplaceData(const TTextData& oNewData, int32 oIndex)
{
	// リスト
	mHoldDatas[oIndex] = oNewData;
	// ビュー
	dynamic_cast<BStringItem*>(FullListItemAt(oIndex))->SetText(HoldDataLabel(oNewData).c_str());
	InvalidateItem(IndexOf(FullListItemAt(oIndex)));
	return B_OK;
}
//------------------------------------------------------------------------------
BMessage	TTextBankListView::ReplacePatterns(BMessage oStrMsg) const
{
	const char*	aText;
	ssize_t		aSize;
	time_t		aTime = time(NULL);
	struct tm*	aTMRef;
	BString		aStr;

	if ( oStrMsg.FindData(FUC_MIME_TYPE_PLAIN_TEXT, B_MIME_TYPE, (const void**)&aText, &aSize) != B_OK )
		return oStrMsg;
	aStr.SetTo(aText, aSize);
	if ( aStr.FindFirst(PATTERN_PREFIX) == B_ERROR )
		return oStrMsg;
	// 置換準備
	aTMRef = localtime(&aTime);
	// パターン置換
	aStr.ReplaceAll(PATTERN_SRC_PERCENT, PATTERN_AVOID_PERCENT).		// まず %% を特殊記号に退避
			ReplaceAll(PATTERN_SRC_LF, PATTERN_DEST_LF).				// \n
			ReplaceAll(PATTERN_SRC_TAB, PATTERN_DEST_TAB).				// TAB
			ReplaceAll(PATTERN_SRC_YEAR_4, (BString() << aTMRef->tm_year-100+2000).String()).	// %yyyy
			ReplaceAll(PATTERN_SRC_YEAR_2, PaddingString(aTMRef->tm_year-100).String()).		// %yy
			ReplaceAll(PATTERN_SRC_YEAR_1, (BString() << aTMRef->tm_year-100).String()).		// %y
			ReplaceAll(PATTERN_SRC_YEAR_JP_2, (BString() << _T(LKK_YEAR_JP[3]) << PaddingString(aTMRef->tm_year-100+2000-1988)).String()).	// %jyy
			ReplaceAll(PATTERN_SRC_YEAR_JP_1, (BString() << _T(LKK_YEAR_JP[3]) << aTMRef->tm_year-100+2000-1988).String()).	// %jy
			ReplaceAll(PATTERN_SRC_MONTH_4, MONTH_EN_LONG[aTMRef->tm_mon]).	// %mmmm……Locale Kit をかますと日本語に変換されてしまうので、かまさない
			ReplaceAll(PATTERN_SRC_MONTH_3, MONTH_EN_SHORT[aTMRef->tm_mon]).	// %mmm
			ReplaceAll(PATTERN_SRC_MONTH_2, PaddingString(aTMRef->tm_mon+1).String()).	// %mm
			ReplaceAll(PATTERN_SRC_MONTH_1, (BString() << aTMRef->tm_mon+1).String()).	// %m
			ReplaceAll(PATTERN_SRC_MONTH_JP, _T(LKK_MONTH_JP[aTMRef->tm_mon])).	// %jm
			ReplaceAll(PATTERN_SRC_DAY_2, PaddingString(aTMRef->tm_mday).String()).	// %dd
			ReplaceAll(PATTERN_SRC_DAY_1, (BString() << aTMRef->tm_mday).String()).	// %d
			ReplaceAll(PATTERN_SRC_DAY_OF_WEEK_2, DAY_OF_WEEK_EN_LONG[aTMRef->tm_wday]).	// %ww
			ReplaceAll(PATTERN_SRC_DAY_OF_WEEK_1, DAY_OF_WEEK_EN_SHORT[aTMRef->tm_wday]).	// %w
			ReplaceAll(PATTERN_SRC_DAY_OF_WEEK_JP, _T(LKK_DAY_OF_WEEK_JP[aTMRef->tm_wday])).	// %jw
			ReplaceAll(PATTERN_SRC_AM_PM_HOUR_2, PaddingString(aTMRef->tm_hour%12).String()).	// %ampmhh
			ReplaceAll(PATTERN_SRC_AM_PM_HOUR_1, (BString() << aTMRef->tm_hour%12).String()).	// %ampmh
			ReplaceAll(PATTERN_SRC_AM_PM, AM_PM_EN[aTMRef->tm_hour >= 12]).	// %ampm
			ReplaceAll(PATTERN_SRC_AM_PM_JP, _T(LKK_AM_PM_JP[aTMRef->tm_hour >= 12])).	// %jampm
			ReplaceAll(PATTERN_SRC_HOUR_2, PaddingString(aTMRef->tm_hour).String()).	// %hh
			ReplaceAll(PATTERN_SRC_HOUR_1, (BString() << aTMRef->tm_hour).String()).	// %h
			ReplaceAll(PATTERN_SRC_MINUTE_2, PaddingString(aTMRef->tm_min).String()).	// %nn
			ReplaceAll(PATTERN_SRC_MINUTE_1, (BString() << aTMRef->tm_min).String()).	// %n
			ReplaceAll(PATTERN_SRC_SECOND_2, PaddingString(aTMRef->tm_sec).String()).	// %ss
			ReplaceAll(PATTERN_SRC_SECOND_1, (BString() << aTMRef->tm_sec).String()).	// %s
			ReplaceAll(PATTERN_AVOID_PERCENT, PATTERN_DEST_PERCENT);	// %% の特殊記号を % に置換
	// 置換
	oStrMsg.ReplaceData(FUC_MIME_TYPE_PLAIN_TEXT, B_MIME_TYPE, aStr.String(), aStr.Length());
	return oStrMsg;
}
//------------------------------------------------------------------------------
status_t	TTextBankListView::SaveData(const BEntry& oEntry)
{
	BFile		aFile;
	BMessage	aDataArc;
	BMessage	aTotalArc;
	TTextData	aData;

	// アーカイブ
	for ( deque<TTextData>::size_type i = 0 ; i < mHoldDatas.size() ; i++ ) {
		aData = mHoldDatas[i];
		aData.mExpand = FullListItemAt(i)->IsExpanded();
		aData.mOutlineLevel = FullListItemAt(i)->OutlineLevel();
		aDataArc.MakeEmpty();
		aData.Archive(&aDataArc);
		aTotalArc.AddMessage(ARC_NAME_DATA, &aDataArc);
	}
	// 保存
	if ( aFile.SetTo(&oEntry, B_WRITE_ONLY|B_CREATE_FILE|B_ERASE_FILE) != B_OK )
		return B_ERROR;
	if ( aTotalArc.Flatten(&aFile) != B_OK )
		return B_ERROR;
	return B_OK;
}
//------------------------------------------------------------------------------
void	TTextBankListView::SendExpStatBarRequested(const string& oText) const
{
	BMessage	aSendMsg(EXP_STAT_BAR_REQUESTED);

	aSendMsg.AddString(MSG_NAME_STAT_BAR, oText.c_str());
	BMessenger(mOwner).SendMessage(&aSendMsg);
}
//------------------------------------------------------------------------------
void	TTextBankListView::SetHoldDataMenu(BMenu* oMenu) const
{
	BMenuItem*	aMItem;
	int32		i = 0;

	while ( (aMItem = oMenu->ItemAt(i)) != NULL ) {
		if ( aMItem->Submenu() != NULL ) {
			SetHoldDataMenu(aMItem->Submenu());
		}
		i++;
	}
	oMenu->SetTargetForItems(this);
	oMenu->SetMaxContentWidth(WIDTH_HOLD_DATA_POP_UP);
}
//------------------------------------------------------------------------------
void	TTextBankListView::SetMousePointShowPopUp(BPoint oPoint)
{
	mMousePointShowPopUp = oPoint;
}
//------------------------------------------------------------------------------
//==============================================================================
