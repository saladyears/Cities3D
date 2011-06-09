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
#include "StatsPane.h"
#include "Controller.h"
#include "HtmlWindowEx.h"
#include "GameEngine.h"
#include "IStatEngine.h"
#include "json_spirit_reader.h"

//---------------------------- USER INCLUDES    -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxHtmlWindowEx* spHtmlWindow = NULL;

	const wxString sH2open = wxT("<font size=3 face=Verdana><b>");
	const wxString sH2close = wxT("</b></font>");

    const wxString sH3open = wxT("<font size=4 face=Verdana><b>");
	const wxString sH3close = wxT("</b></font>");

	const wxString fontopen = wxT("<font size=2 face=Verdana>");
	const wxString fontclose = wxT("</font>");

    const wxString sHeaderFormat = wxT("&nbsp;<font size=3 face=Verdana><b>%s:</b></font> <font size=3 face=Verdana>%s</font><br>");

    wxInt32 sId = -1;
    wxString sHeaderBlock;

    void CreateGameHeaderBlock(wxInt32 id, wxString& html)
    {
        // Don't do this unless we have to.
        if( (sId != id) &&
            (id != -1))
        {
            sId = id;
            sHeaderBlock.Empty();

            // Get all the loaded rulesets and options.
            const Map::StringPairArray& rulesets = MAP.rulesets();

            size_t count = 0;
            wxString sets;

            StringPairArray::const_iterator it, itEnd = rulesets.end();
            for(it = rulesets.begin(); it != itEnd; ++it)
            {
                const StringPair& pair = *it;
                const wxString& name = PLUGIN.GetRulesetName(pair.first, pair.second);
                
                if(false == name.IsEmpty())
                {
                    // Get the user friendly name.
                    if(0 < count)
                    {
                        sets += swCommaSpace;
                    }

                    sets += name;
                    ++count;
                }
            }

            wxString opts;
            StringArray options;
            OPTION.GetOptionStrings(options);

            for(size_t i = 0; i < options.size(); ++i)
            {
                if(0 < i)
                {
                    opts += swCommaSpace;
                }

                opts += options[i];
            }

            // Write out a header block that we can get with the current data.
            sHeaderBlock += sH3open + wxT("&nbsp;Stat Report") + sH3close + wxT("<br><br>");

            sHeaderBlock += wxString::Format(sHeaderFormat.c_str(), _("Map"), MAP.name().c_str());
            sHeaderBlock += wxString::Format(sHeaderFormat.c_str(), _("Map points"), 
                wxString::Format(swIntFormat.c_str(), MAP.read<wxInt32>(shPoints)).c_str());
            
            sHeaderBlock += wxString::Format(sHeaderFormat.c_str(), _("Rulesets"), sets.c_str());
            sHeaderBlock += wxString::Format(sHeaderFormat.c_str(), _("Options"), opts.c_str());
        }

        html += sHeaderBlock;
    }

	void ConvertJsonToHtml(const json_spirit::wObject& obj, wxUint32 id, wxString& html)
	{
		using namespace json_spirit;

		const wObject& top = obj;

		bool inBlock = false;

		html = wxT("<html><head>");
		html += wxT("</head><body>");

        CreateGameHeaderBlock(id, html);

		for(wObject::const_iterator it = top.begin(); it != top.end(); ++it)
        {
            const wPair& pair = *it;
            
			if(false == inBlock)
			{
				wxASSERT(wxT("stat") == pair.name_);
				const wxString& name = pair.value_.get_str();

				html += wxT("<p>");
				html += sH2open + wxT("&nbsp;") + name + sH2close;

				inBlock = true;
			}
			else
			{
				wxASSERT(wxT("block") == pair.name_);

				html += fontopen;
				html += wxT("<table>");

				const wArray& statBlock = pair.value_.get_array();
				wArray::const_iterator itA, itAEnd = statBlock.end();
				for(itA = statBlock.begin(); itA != itAEnd; ++itA)
				{
					const wArray& statLine = itA->get_array();
					html += wxT("<tr>");

					wxUint32 count = 0;

					wArray::const_iterator itL, itLEnd = statLine.end();
					for(itL = statLine.begin(); itL != itLEnd; ++itL)
					{
						const wxString& val = itL->get_str();

						if(0 < count)
						{
							html += wxT("<td align=\"center\">");
						}
						else
						{
							html += wxT("<td>");
						}
						html += val;
						html += wxT("</td>");

						++count;
					}

					html += wxT("</tr>");
				}

				html += wxT("</td></td></table>");
				html += fontclose;
				inBlock = false;
			}
		}

		html += wxT("</body></html>");
	}
}

//---------------------------- PUBLIC           -----------------------------//
wxStatsPane::wxStatsPane(wxWindow *parent, wxInt32 id)
: wxPanel(parent)
, PaneMixin(this, id)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	spHtmlWindow = new wxHtmlWindowEx(this, wxID_ANY, 
		wxDefaultPosition, wxSize(640, 480));
	sizer->Add(spHtmlWindow, 1, wxEXPAND);

	SetSizer(sizer);
	sizer->SetSizeHints(this);

	Controller::get().AddReceiver(shEventStats, &wxStatsPane::OnStats, this);
}

wxStatsPane::~wxStatsPane()
{
	Controller::get().RemoveReceiver(shEventStats, &wxStatsPane::OnStats, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxStatsPane, wxPanel)
END_EVENT_TABLE()

void wxStatsPane::OnStats(wxInt32 id)
{
	json_spirit::wObject obj;
	
	STAT.GetJson(obj);

	wxString html;
	ConvertJsonToHtml(obj, id, html);

	wxASSERT(NULL != spHtmlWindow);

    // Save off the current scroll position
    wxInt32 x, y;
    spHtmlWindow->Freeze();
    spHtmlWindow->GetViewStart(&x, &y);
	spHtmlWindow->SetPage(html);
    spHtmlWindow->Scroll(x, y);
    spHtmlWindow->Thaw();
}