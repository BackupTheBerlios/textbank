#ifndef	___VarPackTextBankView_H_
#define	___VarPackTextBankView_H_
//------------------------------------------------------------------------------
// BeOS
#include <OutlineListView.h>
#include <PopUpMenu.h>
// C++
#include <deque>
#include <string>
#include <vector>
// Add2
#include <add2/MouseDownManager.h>
// Proj
#include "Common.h"
//------------------------------------------------------------------------------
//==============================================================================
/* ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
TClipboardView と TTemplateView の共通スーパークラス（インスタンス作成禁止）
移動は 1 アイテムずつに制限（当面の措置）
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''' */
class	TTextBankListView : public BOutlineListView, public TMouseDownManager
{
public:
	BMenu*				CreateHoldDataMenu(const char* oLabel) const;		// メニューの所有権は移動する
	void				SetMousePointShowPopUp(BPoint oPoint);

protected:
	TTextBankListView(BRect oFrame, const char* oName, BView* oOwner);
	virtual	~TTextBankListView();

	void				AddPopUpMItems(BPopUpMenu* oPopMenu, bool oSelected) const;	// クリップボードビューとテンプレートビューで共通のポップアップメニューアイテムを追加
	status_t			AddData(const TTextData& oNewData, int32 oIndex);	// mHoldDatas とリストビューの両方に追加
	void				AddDeletePopUpMItems(BPopUpMenu* oPopMenu) const;	// 削除ポップアップメニューアイテムを追加
	void				AddHelpPopUpMItems(BPopUpMenu* oPopMenu) const;		// ヘルプポップアップメニューアイテムを追加
	int32				AddHoldDataMItem(BMenu* oMenu, int32 oIndex, int32 oLabelIndex) const;
	int32				AddHoldDataSubMenu(BMenu* oMenu, int32 oIndex, int32 oLabelIndex) const;
	void				AddToolPopUpMItems(BPopUpMenu* oPopMenu) const;		// ツールポップアップメニューアイテムを追加
	virtual void		AttachedToWindow();
	void				DeleteData(int32 oIndex);							// データを削除
	void				DeleteSelections();									// 選択されているアイテムを削除
	status_t			DetectAndAddData(const BMessage* oNewData);			// 文字列かパスなら先頭に追加
	virtual bool		DoDataPatternMatch() const;
	virtual int32		GenerateDataID() const;
	string				HoldDataLabel(const TTextData& oData) const;
	string				HoldDataMItemAccelerator(int32 oSubIndex) const;
	string				HoldDataMItemLabel(int32 oSubIndex, const string& oLabel) const;
	virtual void		KeyDown(const char* oBytes, int32 oNumBytes);
	status_t			LoadData(const BEntry& oEntry);						// ディスクからデータを読み込む
	virtual void		MessageReceived(BMessage* oMsg);
	virtual void		MouseUp(BPoint oPoint);
	void				MoveData(int32 oBefore, int32 oAfter);				// mHoldDatas とリストビューの両方を移動
	virtual void		OpenPopUpMenu(BPoint oPoint) const = 0;
	void				PasteMessageEnqueued(BMessage* oMsg);
	void				PushSelectionsToClipboard(bool oMoveToTop);			// 選択されているアイテムをクリップボードへコピー
	void				PushToClipboard(int32 oIndex) const;				// クリップボードへコピー
	status_t			ReplaceData(const TTextData& oNewData, int32 oIndex);		// mHoldDatas とリストビューの両方を置換
	status_t			SaveData(const BEntry& oEntry);						// ディスクにデータを保存
	void				SendExpStatBarRequested(const string& oText) const;
	virtual status_t	SequenceCheck(const TTextData& oNewData) const = 0;	// 連続して同じデータを追加しても良いかどうか

//	int32				mLastSelection;										// 最後に選択されたアイテムのインデックス
//	float				mItemHeight;
	deque<TTextData>	mHoldDatas;
	BView*				mOwner;

private:
	typedef	BOutlineListView	SUPER;

	status_t		AddDataToList(const TTextData& oNewData, int32 oIndex);
	status_t		AddDataToView(const TTextData& oNewData, int32 oIndex);
	virtual void	MouseClickRightSingle(BPoint oPoint);
	virtual void	MouseDown(BPoint oPoint);
	virtual void	MouseDragBeginLeft(BPoint oBeginPoint, BPoint oPoint);
	virtual void	MouseMoved(BPoint oPoint, uint32 oTransit, const BMessage* oMsg);
	void			MoveDataOfList(const vector<TTextData>& oBeforeDatas, int32 oAfter);
	void			MoveDataOfView(const vector<BStringItem*>& oBeforeItems, int32 oAfter);	// アイテムの所有権は移動
	BString			PaddingString(int32 oNum) const;						// 先頭に 0 を付加して 2 桁にする
	void			PopMItemHoldDataClicked(BMessage* oMsg);
	void			PushToClipboard(const BMessage& oStrMsg, bool oPatternMatch) const;
	BMessage		ReplacePatterns(BMessage oStrMsg) const;
	void			SetHoldDataMenu(BMenu* oMenu) const;

	list<int32>		mMouseUpSelections;			// 右クリックのポップアップメニュー表示前に選択状態が解除されないように
	BPoint			mMousePointClickPopUp;		// for FFM
	BPoint			mMousePointShowPopUp;		// for FFM
};
//==============================================================================
/* ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
クリップボード履歴を保持するリスト（階層化しない）
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''' */
class	TClipboardView : public TTextBankListView
{
public:
	TClipboardView(BRect oFrame, BView* oOwner);
	virtual	~TClipboardView();

private:
	typedef	TTextBankListView	SUPER;

	virtual void		AllAttached();
	virtual void		AllDetached();
	void				ClipboardChanged();
	BEntry				ClipboardSaveEntry() const;
	void				DeleteTopHistoryRequested();
	virtual void		MessageReceived(BMessage* oMsg);
	virtual void		MouseClickLeftDouble(BPoint oPoint);
	virtual void		OpenPopUpMenu(BPoint oPoint) const;
	void				PopMItemDeleteClicked();
	void				PopMItemSaveAsTemplateClicked();
	void				PopMItemSendToClipboardClicked();
	void				PrefNumClipboardHistoriesChanged();
	virtual status_t	SequenceCheck(const TTextData& oNewData) const;
};
//==============================================================================
/* ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
定型文を保持するリスト
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''' */
class	TTemplateView : public TTextBankListView
{
public:
	TTemplateView(BRect oFrame, BView* oOwner);
	virtual	~TTemplateView();

private:
	typedef	TTextBankListView	SUPER;

	virtual void		AllAttached();
	void				ChangeOrderRequested(BMessage* oMsg);
	void				DataPropertyChanged(BMessage* oMsg);
	virtual bool		DoDataPatternMatch() const;
	int32				FindID(int32 oID) const;
	virtual int32		GenerateDataID() const;
	virtual void		MessageReceived(BMessage* oMsg);
	virtual void		MouseClickLeftDouble(BPoint oPoint);
	virtual void		OpenPopUpMenu(BPoint oPoint) const;
	void				PopMItemDeleteClicked();
	void				PopMItemLockTemplateOrderClicked();
	void				PopMItemMoveDownClicked();
	void				PopMItemMoveUpClicked();
	void				PopMItemNewGroupClicked();
	void				PopMItemPropertyClicked();
	void				PopMItemSendToClipboardClicked();
	void				SaveAsTemplateRequested(BMessage* oMsg);
	virtual status_t	SequenceCheck(const TTextData& oNewData) const;
	BEntry				TemplateSaveEntry() const;
};
//==============================================================================
#endif	// ___VarPackTextBankView_H_
