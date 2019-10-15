#include "FileDialogUtils.h"
#include <iostream>
#include <tchar.h>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

namespace vc {
	std::optional<std::string> openLevelExportDialog() {
		wchar_t path[MAX_PATH] = L"";

		OPENFILENAME ofn;
		ZeroMemory(&path, sizeof(path));
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;  
		ofn.lpstrFilter = _T(".zip-Files\0*.zip\0Any File\0*.*\0");
		ofn.lpstrFile = path;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = _T("Export world");
		ofn.Flags = OFN_DONTADDTORECENT;

		if(GetSaveFileName(&ofn)) {
			std::wstring str(path);
			return {std::string(str.begin(), str.end())};	
		} else {
			std::optional<std::string> errorCode = std::nullopt;			

			switch(CommDlgExtendedError()) {
				case CDERR_DIALOGFAILURE: errorCode = "CDERR_DIALOGFAILURE";   break;
				case CDERR_FINDRESFAILURE: errorCode = "CDERR_FINDRESFAILURE";  break;
				case CDERR_INITIALIZATION: errorCode = "CDERR_INITIALIZATION";  break;
				case CDERR_LOADRESFAILURE: errorCode = "CDERR_LOADRESFAILURE";  break;
				case CDERR_LOADSTRFAILURE: errorCode = "CDERR_LOADSTRFAILURE";  break;
				case CDERR_LOCKRESFAILURE: errorCode = "CDERR_LOCKRESFAILURE";  break;
				case CDERR_MEMALLOCFAILURE: errorCode = "CDERR_MEMALLOCFAILURE"; break;
				case CDERR_MEMLOCKFAILURE: errorCode = "CDERR_MEMLOCKFAILURE";  break;
				case CDERR_NOHINSTANCE: errorCode = "CDERR_NOHINSTANCE";     break;
				case CDERR_NOHOOK: errorCode = "CDERR_NOHOOK";          break;
				case CDERR_NOTEMPLATE: errorCode = "CDERR_NOTEMPLATE";      break;
				case CDERR_STRUCTSIZE: errorCode = "CDERR_STRUCTSIZE";      break;
				case FNERR_BUFFERTOOSMALL: errorCode = "FNERR_BUFFERTOOSMALL";  break;
				case FNERR_INVALIDFILENAME: errorCode = "FNERR_INVALIDFILENAME"; break;
				case FNERR_SUBCLASSFAILURE: errorCode = "FNERR_SUBCLASSFAILURE"; break;					
			}
			
			if(errorCode.has_value()) {
				std::cerr << "An error occured! " << errorCode.value() << std::endl;
			}
			return std::nullopt;
		}
	}
	std::optional<std::string> openLevelImportDialog() {
		wchar_t path[MAX_PATH] = L"";

		OPENFILENAME ofn;
		ZeroMemory(&path, sizeof(path));
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = _T(".zip-Files\0*.zip\0Any File\0*.*\0");
		ofn.lpstrFile = path;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = _T("Import world");
		ofn.Flags = OFN_DONTADDTORECENT;

		if(GetOpenFileName(&ofn)) {
			std::wstring str(path);
			return { std::string(str.begin(), str.end()) };
		} else {
			std::optional<std::string> errorCode = std::nullopt;

			switch(CommDlgExtendedError()) {
				case CDERR_DIALOGFAILURE: errorCode = "CDERR_DIALOGFAILURE";   break;
				case CDERR_FINDRESFAILURE: errorCode = "CDERR_FINDRESFAILURE";  break;
				case CDERR_INITIALIZATION: errorCode = "CDERR_INITIALIZATION";  break;
				case CDERR_LOADRESFAILURE: errorCode = "CDERR_LOADRESFAILURE";  break;
				case CDERR_LOADSTRFAILURE: errorCode = "CDERR_LOADSTRFAILURE";  break;
				case CDERR_LOCKRESFAILURE: errorCode = "CDERR_LOCKRESFAILURE";  break;
				case CDERR_MEMALLOCFAILURE: errorCode = "CDERR_MEMALLOCFAILURE"; break;
				case CDERR_MEMLOCKFAILURE: errorCode = "CDERR_MEMLOCKFAILURE";  break;
				case CDERR_NOHINSTANCE: errorCode = "CDERR_NOHINSTANCE";     break;
				case CDERR_NOHOOK: errorCode = "CDERR_NOHOOK";          break;
				case CDERR_NOTEMPLATE: errorCode = "CDERR_NOTEMPLATE";      break;
				case CDERR_STRUCTSIZE: errorCode = "CDERR_STRUCTSIZE";      break;
				case FNERR_BUFFERTOOSMALL: errorCode = "FNERR_BUFFERTOOSMALL";  break;
				case FNERR_INVALIDFILENAME: errorCode = "FNERR_INVALIDFILENAME"; break;
				case FNERR_SUBCLASSFAILURE: errorCode = "FNERR_SUBCLASSFAILURE"; break;
			}

			if(errorCode.has_value()) {
				std::cerr << "An error occured! " << errorCode.value() << std::endl;
			}
			return std::nullopt;
		}
	}
}
