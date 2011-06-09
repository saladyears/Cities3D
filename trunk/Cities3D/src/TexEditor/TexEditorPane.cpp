/*
 *  Cities3D - Copyright (C) 2001-2009 Jason Fugate (saladyears@gmail.com)
 * 
 *  This program is free software; you can redistribute it and/or modify it 
 *  under the terms of the GNU General Public License as published by the Free 
 *  Software Foundation; either version 2 of the License, or (at your option) 
 *  any later version.
 * 
 *  This program is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 *  for more details.
 */
#include "stdwx.h"
#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "TexEditorPane.h"
#include "SkinManager.h"
#include "BaseButton.h"
#include "BaseTextCtrl.h"
#include "BaseStaticText.h"
#include "TexDropListCtrl.h"

#include "define/defineFile.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	enum
	{
		ID_IMPORT_BUTTON			,
		ID_EXPORT_BUTTON			,
		ID_EXTRACT_BUTTON			,
		ID_PREVIEW_LIST				,
	};

	class wxScrolledBitmap : public wxScrolledWindow
	{
	public:
		wxScrolledBitmap(wxWindow *parent, wxWindowID id, 
			const wxBitmap& bitmap, const wxPoint& pos, const wxSize& size)
			: wxScrolledWindow(parent, id, pos, size)
		{
			this->SetBackgroundColour(parent->GetBackgroundColour());
			SetScrollRate(1, 1);
			mBitmap = new wxStaticBitmap(this, id, bitmap, pos, size);
		}

		void SetBitmap(const wxBitmap& bitmap)
		{
			SetVirtualSize(bitmap.GetWidth(), bitmap.GetHeight());
			mBitmap->SetSize(bitmap.GetWidth(), bitmap.GetHeight());
			mBitmap->SetBitmap(bitmap);
		}

	private:
		wxStaticBitmap* mBitmap;
	};

	//controls
	wxBaseButton *spImportButton;
	wxBaseButton *spExportButton;
	wxBaseButton *spExtractButton;
	wxTextCtrl *spImportText;
	wxTextCtrl *spExportText;
	wxStaticText *spPreviewText;
	wxTexDropListCtrl *spPreviewList;
	wxScrolledBitmap *spPreviewWnd;
}

//---------------------------- PUBLIC           -----------------------------//
wxTexEditorPane::wxTexEditorPane(wxWindow *parent)
: wxPanel(parent, wxID_ANY, wxPoint(parent->GetClientSize().x, 
	parent->GetClientSize().y))
, PaneMixin(this)
{
	wxSizer* pSizer = new wxBoxSizer(wxHORIZONTAL);

	//import row
	spImportButton = new wxBaseButton(this, ID_IMPORT_BUTTON,
		stImport);
	spImportText = new wxBaseTextCtrl(this, -1, wxDefaultPosition,
		wxSize(200, 20), wxTE_READONLY, shInfoText);
	wxBoxSizer *horz1 = new wxBoxSizer(wxHORIZONTAL);
	horz1->Add(spImportButton, 0, wxALL, 3);
	horz1->Add(spImportText, 1, wxALL | wxEXPAND, 3);

	//preview row
	wxBoxSizer *vert1 = new wxBoxSizer(wxVERTICAL);
	spPreviewList = new wxTexDropListCtrl(this, ID_PREVIEW_LIST, 
		wxDefaultPosition, wxSize(200, 200));
	vert1->Add(spPreviewList, 1, wxEXPAND);

	wxBoxSizer *vert2 = new wxBoxSizer(wxVERTICAL);
	spPreviewText = new wxBaseStaticText(this, -1, swEmpty, 
		shDefaultText, shInfoFont, wxSize(200, 20));
	spPreviewWnd = new wxScrolledBitmap(this, -1, 
		wxBitmap(wxImage(200, 200)), wxDefaultPosition, wxSize(200, 200));
	vert2->Add(spPreviewText, 0, wxBOTTOM, 5);
	vert2->Add(spPreviewWnd, 1, wxEXPAND | wxALIGN_TOP | wxALIGN_LEFT);
	
	wxBoxSizer *horz2 = new wxBoxSizer(wxHORIZONTAL);
	horz2->Add(vert1, 0, wxALL | wxEXPAND | wxALIGN_LEFT, 3);
	horz2->Add(vert2, 1, wxALL | wxEXPAND | wxALIGN_RIGHT, 3);

	//export row
	spExportButton = new wxBaseButton(this, ID_EXPORT_BUTTON,
		stExport);
	spExportButton->Enable(false);
	spExportText = new wxBaseTextCtrl(this, -1, wxDefaultPosition, 
		wxSize(200, 20), wxTE_READONLY, shInfoText);
	wxBoxSizer *horz3 = new wxBoxSizer(wxHORIZONTAL);
	horz3->Add(spExportButton, 0, wxALL, 3);
	horz3->Add(spExportText, 1, wxALL | wxEXPAND, 3);

	wxBoxSizer *vertical1 = new wxBoxSizer(wxVERTICAL);
	vertical1->Add(horz1, 0, wxALL | wxEXPAND, 3);
	vertical1->Add(horz2, 1, wxALL | wxEXPAND, 3);
	vertical1->Add(horz3, 0, wxALL | wxEXPAND | wxALIGN_BOTTOM, 3);

	spExtractButton = new wxBaseButton(this, ID_EXTRACT_BUTTON,
		stExtract);
	spExtractButton->Enable(false);
	
	wxBoxSizer *vertical2 = new wxBoxSizer(wxVERTICAL);
	vertical2->Add(spExtractButton, 0, wxALL, 6);

	pSizer->Add(vertical1, 1, wxEXPAND);
	pSizer->Add(vertical2, 0, wxEXPAND);

	SetSizer(pSizer);
	pSizer->SetSizeHints(this);
}

wxTexEditorPane::~wxTexEditorPane()
{
}

void wxTexEditorPane::SetPath(wxInt32 index, const wxString &path, bool newTex)
{
	//see if they are appending a new item
	if(mPaths.size() <= (wxUint32) index)
	{
		//add it to the end of the path list
		mPaths.push_back(PathPair());

		//append a new blank space to the list control
		spPreviewList->AppendItem(swEmpty);
	}

	mPaths[index].first = newTex;
	mPaths[index].second = path;

	wxFileName name(path);

	spPreviewList->SetItemText(index, swFile, name.GetFullName());
	spPreviewList->SetItemColor(index, newTex ? 
		SKIN.Element(shUserChangeableText) : 
		SKIN.Element(shDefaultText));
}

void wxTexEditorPane::DeleteItem(wxInt32 index)
{
	//sanity check
	if(-1 != index)
	{
		//remove this item from the list
		spPreviewList->DeleteItem(index);

		//remove it internally
		mPaths.erase(mPaths.begin() + index);

		//TODO: other stuff like undo
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxTexEditorPane, wxPanel)
	EVT_BUTTON(ID_IMPORT_BUTTON, wxTexEditorPane::OnImportButton)
	EVT_BUTTON(ID_EXPORT_BUTTON, wxTexEditorPane::OnExportButton)
	EVT_BUTTON(ID_EXTRACT_BUTTON, wxTexEditorPane::OnExtractButton)
	EVT_LIST_ITEM_SELECTED(ID_PREVIEW_LIST, wxTexEditorPane::OnItemSelected)
END_EVENT_TABLE()

void wxTexEditorPane::OnImportButton(wxCommandEvent &)
{
	wxString path;

	path = ::wxFileSelector(stSelectATexFileToImport,
							swEmpty,
							swEmpty,
							swTex,
							swTexFiles,
							wxFD_OPEN,
							this);

	if(true == path.IsEmpty())
	{
		return;
	}

	importFile(path);
}

void wxTexEditorPane::OnExportButton(wxCommandEvent &)
{
	wxString path;
	wxString original;

	wxFileName name(spImportText->GetValue());
	original = name.GetFullName();
	
	path = ::wxDirSelector(wxString::Format(swStringStringColon.c_str(), 
		stSelectADirectoryToExport.c_str(), original.c_str()),
		name.GetPath(wxPATH_GET_VOLUME));

	if(false == path.IsEmpty())
	{
		//append the original name
		path += (swForwardSlash + original);

		exportFile(path);
	}
}

void wxTexEditorPane::OnExtractButton(wxCommandEvent &)
{
	wxFileName name(spImportText->GetValue());
	wxString original = name.GetFullName();
	
	wxString path = ::wxDirSelector(wxString::Format(swStringStringColon.c_str(),
		stSelectADirectoryToExtract.c_str(), original.c_str()));

	if(false == path.IsEmpty())
	{
		extractFile(path);
	}
}

void wxTexEditorPane::OnItemSelected(wxListEvent &)
{
	wxInt32 index = spPreviewList->GetSelectedItem();

	//and now, we preview it
	if(index != mLastSelection)
	{
		previewItem(index);
		mLastSelection = index;
	}
}

void wxTexEditorPane::importFile(const wxString &path)
{
	wxFileName file(path);

	//sanity
	if(false == file.FileExists())
	{
		return;
	}

	//first thing to do is to look up the file info for this tex file
	wxString name = file.GetFullName();

	//read the images off the disk
	readFromDisk(path);
	populateTree();

	spImportText->SetValue(path);

	mLastSelection = -1;
	spExportButton->Enable();

	// Now that we've got something in, allow extraction
	spExtractButton->Enable(true);
}

void wxTexEditorPane::exportFile(const wxString &path)
{
	//check for an overwrite
	wxFileName file(path);

	if(true == file.FileExists())
	{
		//TODO: ask about overwrite
	}

	//create a temp file to do this as automically as possible
	wxString temp = wxFileName::CreateTempFileName(swImg);

	wxFile out;
	out.Open(temp, wxFile::write);
	wxFileOutputStream x(out);
	wxDataOutputStream data(x);

	//dump the flag immediately
	out.Write(TEX_FLAG.c_str(), TEX_FLAG.size());

	//iterate through the paths, reading the file and dumping it to the .tex 
	//file
	for(wxUint32 i = 0; i < mPaths.size(); ++i)
	{
		//read the file into memory
		wxFile in;
		in.Open(mPaths[i].second);

		//sanity
		if(false == in.IsOpened())
		{
			//TODO: log error message
			return;
		}

		wxUint32 dwLength = in.Length();
		boost::scoped_array<wxUint8> bytes(new wxUint8[dwLength]);

		//read the data
		in.Read(bytes.get(), dwLength);

		//now dump it
		wxFileName temp(mPaths[i].second);
		data << temp.GetFullName();
		data << dwLength;
		out.Write(bytes.get(), dwLength);

		in.Close();
	}

	//now copy to the destination file
	wxRemoveFile(path);
	wxRenameFile(temp, path);

	//set the output path
	wxFileName name(path);
	spExportText->SetValue(name.GetFullPath());
}

void wxTexEditorPane::extractFile(const wxString &path)
{
	// Iterate through the paths, copying the files to the selected directory.
	for(size_t i = 0; i < mPaths.size(); ++i)
	{
		wxFileName temp(mPaths[i].second);
		wxString name = temp.GetFullName();

		wxString dirPath = path + swForwardSlash + name;

		::wxCopyFile(mPaths[i].second, dirPath);
	}
}

void wxTexEditorPane::readFromDisk(const wxString &path)
{
	wxFile in;
	in.Open(path);
	wxFileInputStream x(in);
	wxDataInputStream fileIn(x);

	//open the right texture set
	if(false == in.IsOpened())
	{
		wxLogError(wxString::Format(wxT("Could not open %s."), path.c_str()));
		
		return;
	}

	//immediate sanity check
	boost::scoped_array<char> flag(new char[TEX_FLAG.size()]);
	in.Read(flag.get(), TEX_FLAG.size());

	//sanity
	if(TEX_FLAG != HashString(flag.get(), TEX_FLAG.size()))
	{
		//TODO: log error message
		in.Seek(0);
		//return;
	}

	mPaths.clear();
	mPaths.reserve(50);

	//read through the files and dump them into temp files
	while(false == in.Eof())
	{
		wxString strIn;
		wxUint32 dwLength;

		//read the file name and length
		fileIn >> strIn;
		fileIn >> dwLength;

		boost::scoped_array<wxUint8> bytes(new wxUint8[dwLength]);
		in.Read(bytes.get(), dwLength);

		//dump to a temp file
		wxString temp = wxFileName::CreateTempFileName(swImg);
		wxFileName name(temp);
		wxFileName orig(strIn);
		name.SetFullName(orig.GetFullName());
		
		//remove original temp file
		wxRemoveFile(temp);

		//dump to new temp file
		wxFile out;
		out.Open(name.GetFullPath(), wxFile::write);
		wxASSERT(out.IsOpened());

		out.Write(bytes.get(), dwLength);
		out.Close();

		//set the path pair
		mPaths.push_back(std::make_pair(false, name.GetFullPath()));
	}
}

void wxTexEditorPane::populateTree()
{
	// reset the preview picture and info
	spPreviewText->SetLabel(swEmpty);
	spPreviewWnd->SetBitmap(wxBitmap(wxImage(200,200)));

	spPreviewList->DeleteAllItems();

	//populate the tree
	for(wxUint32 i = 0; i < mPaths.size(); ++i)
	{
		wxFileName name(mPaths[i].second);
		spPreviewList->AppendItem(name.GetFullName());
	}

	//always add one extra "blank" spot for appending new items
	spPreviewList->AppendItem(swEmpty);

	Refresh();
}

void wxTexEditorPane::previewItem(wxInt32 index)
{
	//sanity check
	if(mPaths.size() <= (wxUint32) index)
	{
		spPreviewText->SetLabel(swEmpty);
		spPreviewWnd->SetBitmap(wxBitmap(wxImage(200, 200)));
		return;
	}

	//open the file and read it in
	wxFile in;
	in.Open(mPaths[index].second);
	wxFileInputStream file(in);

	//read in the data
	wxImage image(file);

	//see if we were successful
	if(FALSE == image.Ok())
	{
		wxLogMessage(wxString::Format(wxT("Unable to open file %s."), 
			mPaths[index].second.c_str()));
		return;
	}

	//set the preview picture and info
	spPreviewText->SetLabel(wxString::Format(swByteFormat.c_str(), 
		stDimensions.c_str(), image.GetWidth(), image.GetHeight(), 
		stSize.c_str(), in.Length()));
	spPreviewWnd->SetBitmap(wxBitmap(image));

	Refresh();
}
