#ifndef	___TextBankLKKConstants_H_
#define	___TextBankLKKConstants_H_
//==============================================================================

// ### Section ### General Button

const char	LKK_BUTTON_CANCEL[] = "Cancel";
const char	LKK_BUTTON_DEFAULTS[] = "Defaults";
const char	LKK_BUTTON_HELP[] = "Help";
const char	LKK_BUTTON_NO[] = "No";
const char	LKK_BUTTON_OK[] = "OK";
const char	LKK_BUTTON_UNDERSTAND[] = "Understand";
const char	LKK_BUTTON_YES[] = "Yes";

// ### Section ### Tool Bar

const char	LKK_TBUTTON_CLIPBOARD[] = "Clipboard";
const char	LKK_TBUTTON_TEMPLATE[] = "Template";

// ### Section ### Pop Up Menu

const char	LKK_POP_MITEM_SEND_TO_CLIPBOARD[] = "Send to Clipboard";
const char	LKK_POP_MITEM_SAVE_AS_TEMPLATE[] = "Save as Template";
const char	LKK_POP_MITEM_PROPERTY[] = "Property";

const char	LKK_POP_MITEM_NEW_GROUP[] = "New Group";

const char	LKK_POP_MITEM_DELETE[] = "Delete";

const char	LKK_POP_MITEM_MOVE_UP[] = "Move Up";
const char	LKK_POP_MITEM_MOVE_DOWN[] = "Move Down";
const char	LKK_POP_MITEM_LOCK_TEMPLATE_ORDER[] = "Lock Template Order";

const char	LKK_POP_MITEM_PREFS[] = "Preferences";
const char	LKK_POP_MITEM_SHINTA_COMMON_PREFS[] = "SHINTA Common Preferences";

const char	LKK_POP_MITEM_HELP[] = "Help";
const char	LKK_POP_MITEM_AUTHOR_WEB[] = "Open %AppAuthor%'s Web";
const char	LKK_POP_MITEM_ABOUT[] = "About";

const char	LKK_POP_MITEM_CLIPBOARD[] = "Clipboard";
const char	LKK_POP_MITEM_TEMPLATE[] = "Template";

// ### Section ### Status Bar

const char	LKK_STAT_BAR_ADD_HISTORY[] = "Add history  Total: %Num%";
const char	LKK_STAT_BAR_ADD_TEMPLATE[] = "Add template  Total: %Num%";
const char	LKK_STAT_BAR_DELETE_HISTORY[] = "Delete history  Total: %Num%";
const char	LKK_STAT_BAR_DELETE_TEMPLATE[] = "Delete template  Total: %Num%";
const char	LKK_STAT_BAR_SEND_TO_CLIPBOARD[] = "Send to clipboard";

// ### Section ### Preferences Window

const char	LKK_TITLE_PREFS_WIN[] = "Preferences";

const char	LKK_TAB_GENERAL[] = "General";
const char	LKK_BOX_CLIPBOARD[] = "Clipboard History";
const char	LKK_TXT_CTRL_NUM_CLIPBOARD_HISTORIES[] = "Number of clipboard histories";
const char	LKK_CBOX_SAVE_CLIPBOARD[] = "Save to disk";
const char	LKK_CBOX_LOSE_FOCUS[] = "Lose focus after double-clicking";
const char	LKK_CBOX_MINIMIZED_BOOT[] = "Boot with main window minimized";
const char	LKK_CBOX_LOCK_TEMPLATE_ORDER[] = "Lock template order";

const char	LKK_TAB_HOT_KEY[] = "Hot Key";
const char	LKK_BOX_ACTIVATE[] = "Activate %AppName%";
const char	LKK_TXT_CTRL_HOT_KEY_CHAR[] = "Char";
const char	LKK_BOX_DELETE[] = "Delete Latest History";
const char	LKK_BOX_POP_UP[] = "Show PopUp";
const char	LKK_STR_VIEW_HINT_USING_HOT_KEY[] = "Installing assistant is needed to use hot key.";

const char	LKK_TAB_ASST[] = "Assistant";
const char	LKK_BOX_AUTO_INSTALL_ASST[] = "Auto Install Assistant";
const char	LKK_RBUTTON_ENABLE_AUTO_INSTALL_ASST[] = "Enable";
const char	LKK_RBUTTON_DISABLE_AUTO_INSTALL_ASST[] = "Disable";
const char	LKK_BUTTON_INSTALL_ASST[] = "Install Assistant Now";
const char	LKK_BUTTON_UNINSTALL_ASST[] = "Uninstall Assistant Now";

// ### Section ### Property Window

const char	LKK_TITLE_PROPERTY_WIN[] = "Property";

const char	LKK_TXT_CTRL_TITLE[] = "Title";
const char	LKK_STR_VIEW_TEXT_CAPTION[] = "Text";
const char	LKK_CBOX_PATTERN_MATCH[] = "Replace special patterns on pasting";

const char	LKK_STR_VIEW_TEXT_GROUP[] = "(Group)";

// ### Section ### SHINTA Common Preferences Window

// # Include # /boot/beos/etc/develop/zeta-r1-gcc2-x86/headers/add2/SHINTAIAPs.h

// ### Section ### About Window

// # Include # /boot/beos/etc/develop/zeta-r1-gcc2-x86/headers/add2/SHINTAAbout.h

// ### Section ### Message (Error)

const char	LKK_ERR_CANNOT_RUN_ASST[] = "@Err_Cannot_Run_Asst@";
const char	LKK_ERR_CANNOT_SHOW_HELP[] = "@Err_Cannot_Show_Help %FileName%@";
const char	LKK_ERR_CANNOT_SHOW_PREFS_WIN[] = "@Err_Cannot_Show_Preferences_Window@";
const char	LKK_ERR_CANNOT_SHOW_PROPERTY_WIN[] = "@Err_Cannot_Show_Property_Win@";

// ### Section ### Message (Ask)

const char	LKK_ASK_RUN_ASST_AUTO[] = "@Ask_Run_Asst_Auto@";
const char	LKK_ASK_RUN_ASST_INSTALL_NOW[] = "@Ask_Run_Asst_Install_Now@";

// ### Section ### Message (Info)

const char	LKK_INFO_RAN_ASST[] = "@Info_Ran_Asst@";
const char	LKK_ASST_INFO_UNINST_NORUN[] = "@Asst_Info_Uninst_Norun@";
const char	LKK_ASST_INFO_UNINST_RUN[] = "@Asst_Info_Uninst_Run@";
const char	LKK_ASST_INFO_INST_NORUN[] = "@Asst_Info_Inst_Norun@";
const char	LKK_ASST_INFO_INST_RUN[] = "@Asst_Info_Inst_Run@";
const char	LKK_INFO_UNINSTALLED_ASST[] = "@Info_Uninstalled_Asst@";

// ### Section ### Message (Hint)

const char	LKK_HINT_EXCUTABLE_MAYBE_BROKEN[] = "@Hint_Excutable_Maybe_Broken@";

// ### Section ### Pattern Match (JP)

// # Add # "Gozen"
// # Add # "Gogo"

// # Add # "Nichi"
// # Add # "Getsu"
// # Add # "Ka"
// # Add # "Sui"
// # Add # "Moku"
// # Add # "Kin"
// # Add # "Do"

// # Add # "Mutsuki"
// # Add # "Kisaragi"
// # Add # "Yayoi"
// # Add # "Uzuki"
// # Add # "Satsuki"
// # Add # "Minazuki"
// # Add # "Fumizuki"
// # Add # "Hazuki"
// # Add # "Nagatsuki"
// # Add # "Kannazuki"
// # Add # "Shimotsuki"
// # Add # "Shiwasu"

// # Add # "Meiji"
// # Add # "Taisho"
// # Add # "Showa"
// # Add # "Heisei"

// ### Section ### Etc.

const char	LKK_ALERT_CAPTION[] = "* %AppName% *";
const char	LKK_COLON_SPACE[] = ": ";
const char	LKK_GROUP_TITLE[] = "< %String% >";
const char	LKK_NEW_GROUP[] = "New Group";
// # Add # "SHINTA"

//------------------------------------------------------------------------------

const char	LKR_APP_AUTHOR[] = "%AppAuthor%";
const char	LKR_APP_NAME[] = "%AppName%";
const char	LKR_FILE_NAME[] = "%FileName%";
const char	LKR_NUM[] = "%Num%";
const char	LKR_STRING[] = "%String%";

//------------------------------------------------------------------------------

const char	LKV_FILE_NAME_HELP[] = "$FileNameHelp$";

//==============================================================================
#endif	// ___TextBankLKKConstants_H_
