#include "TextBankShared.h"
//------------------------------------------------------------------------------
// Add2
// Proj
//==============================================================================
const char*	gAppSignature  = "application/x-vnd.SHINTA-TextBank";
//==============================================================================
TPrefs::TPrefs()
		: SUPER()
{
	// bool
	mAutoInstallAsst = true;
	mLockTemplateOrder = false;
	mLoseFocus = false;
	mMinimizedBoot = false;
	mSaveClipboard = false;
	// int32
	mActivateModifiers = mDeleteModifiers = mPopUpModifiers = B_COMMAND_KEY | B_CONTROL_KEY;
	mActivateChar = 'a';
	mDeleteChar = 'd';
	mNumClipboardHistories = 100;
	mPopUpChar = 'p';
}
//------------------------------------------------------------------------------
TPrefs::~TPrefs()
{
}
//------------------------------------------------------------------------------
status_t	TPrefs::Archive(BMessage* oArc, bool oDeep) const
{
	status_t	aResult = B_OK;

	aResult |= SUPER::Archive(oArc);
	// bool
	aResult |= oArc->AddBool(ARC_NAME_AUTO_INSTALL_ASST, mAutoInstallAsst);
	aResult |= oArc->AddBool(ARC_NAME_LOCK_TEMPLATE_ORDER, mLockTemplateOrder);
	aResult |= oArc->AddBool(ARC_NAME_LOSE_FOCUS, mLoseFocus);
	aResult |= oArc->AddBool(ARC_NAME_MINIMIZED_BOOT, mMinimizedBoot);
	aResult |= oArc->AddBool(ARC_NAME_SAVE_CLIPBOARD, mSaveClipboard);
	// int32
	aResult |= oArc->AddInt32(ARC_NAME_ACTIVATE_CHAR, mActivateChar);
	aResult |= oArc->AddInt32(ARC_NAME_ACTIVATE_MODIFIERS, mActivateModifiers);
	aResult |= oArc->AddInt32(ARC_NAME_DELETE_CHAR, mDeleteChar);
	aResult |= oArc->AddInt32(ARC_NAME_DELETE_MODIFIERS, mDeleteModifiers);
	aResult |= oArc->AddInt32(ARC_NAME_NUM_CLIPBOARD_HISTORIES, mNumClipboardHistories);
	aResult |= oArc->AddInt32(ARC_NAME_POP_UP_CHAR, mPopUpChar);
	aResult |= oArc->AddInt32(ARC_NAME_POP_UP_MODIFIERS, mPopUpModifiers);
	return aResult;
}
//------------------------------------------------------------------------------
status_t	TPrefs::LoadFromFile()
{
	return LoadByName(FOLDER_NAME_TEXT_BANK_SETTINGS, FILE_NAME_PREFS);
}
//------------------------------------------------------------------------------
status_t	TPrefs::SaveToFile()
{
	return SaveByName(FOLDER_NAME_TEXT_BANK_SETTINGS, FILE_NAME_PREFS);
}
//------------------------------------------------------------------------------
status_t	TPrefs::Unarchive(const BMessage& oMsg)
{
	status_t	aResult = B_OK;

	aResult |= SUPER::Unarchive(oMsg);
	// bool
	aResult |= FindBool(oMsg, ARC_NAME_AUTO_INSTALL_ASST, &mAutoInstallAsst);
	aResult |= FindBool(oMsg, ARC_NAME_LOCK_TEMPLATE_ORDER, &mLockTemplateOrder);
	aResult |= FindBool(oMsg, ARC_NAME_LOSE_FOCUS, &mLoseFocus);
	aResult |= FindBool(oMsg, ARC_NAME_MINIMIZED_BOOT, &mMinimizedBoot);
	aResult |= FindBool(oMsg, ARC_NAME_SAVE_CLIPBOARD, &mSaveClipboard);
	// int32
	aResult |= FindInt32(oMsg, ARC_NAME_ACTIVATE_CHAR, &mActivateChar);
	aResult |= FindInt32(oMsg, ARC_NAME_ACTIVATE_MODIFIERS, &mActivateModifiers);
	aResult |= FindInt32(oMsg, ARC_NAME_DELETE_CHAR, &mDeleteChar);
	aResult |= FindInt32(oMsg, ARC_NAME_DELETE_MODIFIERS, &mDeleteModifiers);
	aResult |= FindInt32(oMsg, ARC_NAME_NUM_CLIPBOARD_HISTORIES, &mNumClipboardHistories);
	aResult |= FindInt32(oMsg, ARC_NAME_POP_UP_CHAR, &mPopUpChar);
	aResult |= FindInt32(oMsg, ARC_NAME_POP_UP_MODIFIERS, &mPopUpModifiers);
	return aResult;
}
//------------------------------------------------------------------------------
//==============================================================================
