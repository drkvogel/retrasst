#include <vcl.h>
#pragma hdrstop

#include <fstream>
#include "TfrmConfirm.h"
#include "DiscardSamples.h"
#include "DiscardSearch.h"
#include "DiscardReason.h"
#include "DiscardMethod.h"
#include "DiscardNote.h"
#include "DiscardUtil.h"
#include "DiscardSef.h"
#include "StoreUtil.h"
#include "SMLogin.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmDiscardSamples *frmDiscardSamples;


void
TfrmDiscardSamples::init( Discard::Context * context )
{
    if (context == 0)
    {
		std::string message = "";
		message += "null context";
		message += " at ";
        message += HERE;
		throw Exception(message.c_str());
    }
    m_context = context;
    m_context->setSaved(false);
    m_context->configPersonFname();

	m_samples.init(context);

	reset();

	return;
}

void
TfrmDiscardSamples::reset( )
{
//	using Discard::Util;
	using Discard::Cryovial;

	m_cells.reset();
	this->grdResults->Hide();
	this->grdResults->RowCount = 0;
	this->grdResults->ColCount = 0;
	this->pnlResults->Caption = "";

    this->bitDiscard->Visible = true;
    this->bitNote->Visible = false;

	std::string caption = "";
    const int nextCrstatus = m_context->getNextCrstatus();

    // was FIXME 66 begin
    if (nextCrstatus == Cryovial::CONFIRMED)
    {
		caption += "Create discard job";
    }
//    if (nextCrstatus == Cryovial::REMOVED)
//    {
//		caption += "Create discard job";
//    }
    // was FIXME 66 end

    else if (nextCrstatus == Cryovial::DESTROYED)
    {
		caption += "Destroy samples";
    }
    else
    {
		std::string message = "";
		message += "bad crstatus ";
		message += Discard::Util::asString(nextCrstatus);
		message += " at ";
        message += HERE;
		throw Exception(message.c_str());
    }

    caption += " - ";
    caption += m_context->getProjectName();

	this->Caption = caption.c_str();

	updateUI();

	return;
}

void
TfrmDiscardSamples::showSamples( )
{
    using Discard::Sample;
    using Discard::Cell;

	TCursor cursor = Screen->Cursor;
	Screen->Cursor = crHourGlass;

	m_cells.reset();
    m_barcode = "";

	do
	{
        Discard::GridStuff gstuff(m_samples);
        Discard::SCComparator * scc = m_samples.getSCComparator();
        scc->setSamples(&m_samples);
        gstuff.analyze(scc);
        gstuff.setup();

        m_firstSampleColno = gstuff.getFirstSampleColno();
        m_firstSampleRowno = gstuff.getFirstSampleRowno();

        m_ncols = gstuff.getNcols();
        m_nrows = gstuff.getNrows();

        this->grdResults->RowCount = gstuff.getNrows();
        this->grdResults->ColCount = gstuff.getNcols();

        for (int rowno=0; rowno<m_nrows; rowno++)
        {
            for (int colno=0; colno<m_ncols; colno++)
            {
                const std::string text = gstuff.getText(Cell(colno, rowno));
                this->grdResults->Cells[colno][rowno] = text.c_str();
            }
        }

        resizeColumns(this->grdResults);

        {
            std::set<Cell>::const_iterator begin =
                gstuff.getHeaderBegin();
            std::set<Cell>::const_iterator end =
                gstuff.getHeaderEnd();
            for (std::set<Cell>::const_iterator it = begin; it != end; it++)
            {
                const Cell cell = *it;
                m_cells.setHeader(cell);
            }
        }

        {
            std::map<Cell,int>::const_iterator begin =
                gstuff.getCellSamplenoBegin();
            std::map<Cell,int>::const_iterator end =
                gstuff.getCellSamplenoEnd();
            for (std::map<Cell,int>::const_iterator it = begin; it != end; it++)
            {
                const Cell cell = it->first;
                const int sampleno = it->second;
                m_cells.setSampleno(cell, sampleno);
            }
        }

	} while (false);

	this->pnlResults->Show();

	updateUI();

	Screen->Cursor = cursor;

	return;
}

void
TfrmDiscardSamples::resizeColumns( TStringGrid * grid )
{
    using Discard::SCComparator;

    const int nrows = grid->RowCount;
    const int ncols = grid->ColCount;

    const bool hidePerson = ! Discard::Person::canSearch();

    for (int colno=0; colno<ncols; colno++)
    {
        bool isHidden = false;
        isHidden |= (colno == SCComparator::HIDDEN);
        isHidden |= (hidePerson && (colno == SCComparator::PERSONID));
        if (isHidden)
        {
            grid->ColWidths[colno] = 0;
            continue;
        }
        int maxwidth = -1;
        for (int rowno=0; rowno<nrows; rowno++)
        {
            const int width = grid->Canvas->TextWidth(
                grid->Cells[colno][rowno]);
            if (width > maxwidth) maxwidth = width;
        }
        grid->ColWidths[colno] = maxwidth + 5;
    }

    return;
}

std::string
TfrmDiscardSamples::addSamples( )
{
   // using Discard::Util;

    std::string diagnosis = "";

	TCursor cursor = Screen->Cursor;
	Screen->Cursor = crHourGlass;

    do
    {
        const std::string type = m_context->getSearchType();
        const StringSet & texts = m_context->getSearchTexts();

        const bool isJob = (type == "Job");
        const bool isTube = (type == "Tube");
        const bool isCryovial = (type == "Cryovial");
        const bool isBox = (type == "Box");
        const bool isPerson = (type == "Person");

        if ((! isJob) && (! isTube) && (! isCryovial) && (! isBox) &&
            (! isPerson))
        {
			diagnosis = "unexpected type " + Discard::Util::quote(type);
            break;
        }

        for (StringSet::const_iterator it = texts.begin();
            it != texts.end(); it++)
        {
            const std::string text = *it;
            IntPair counts;
            if (isJob)
            {
                const int jobno = atoi(text.c_str());
                if (jobno == 0)
                {
                    diagnosis = "bad " + type + " " +
						Discard::Util::quote(text);
                    break;
                }
                m_context->setJobno(jobno);
                counts = m_samples.add(Discard::Job(jobno));
            }
            else if (isTube)
            {
                counts = m_samples.add(Discard::Tube(text));
            }
            else if (isCryovial)
            {
                counts = m_samples.add(Discard::Cryovial(text));
            }
            else if (isBox)
            {
                counts = m_samples.add(Discard::Box(text));
            }
            else if (isPerson)
            {
                counts = m_samples.add(Discard::Person(text));
            }

            if (counts.first == 0)
            {
                diagnosis = "failed to match " + type + " " +
					Discard::Util::quote(text);
                break;
            }
            if (counts.second == 0)
            {
                diagnosis = "no samples added";
                break;
            }
        }

    } while (false);

	Screen->Cursor = cursor;

    return diagnosis;
}


void
TfrmDiscardSamples::doSelect( const bool isNoting )
{
    using Discard::Cell;

    bool isIgnored = true;

    do
    {
		if ((m_clickedColno < 0) || (m_clickedRowno < 0)) break; // outside grid
		if (m_clickedColno == 0) break; // hidden column

        // if (m_context->isSelectJobStage()) break;

        isIgnored = false;

        int nSelectedCols = 1;
        int nSelectedRows = 1;

        int firstSelectedColno = m_clickedColno;
        int firstSelectedRowno = m_clickedRowno;

        const bool isWholeRow = isClickWholeRow();
        if (isWholeRow)
        {
            // select all cols
            firstSelectedColno = m_firstSampleColno;
            nSelectedCols = m_ncols - firstSelectedColno;
        }
        const bool isWholeCol = isClickWholeCol();
        if (isWholeCol)
        {
            // select all rows
            firstSelectedRowno = m_firstSampleRowno;
            nSelectedRows = m_nrows - firstSelectedRowno;
        }

        int nchanged = 0;
        {
            const Discard::SampleHandler * handler = 0;
            if (isNoting)
            {
                handler = &(m_context->getNoter());
            }
            else if (isWholeRow || isWholeCol)
            {
                handler = &(m_context->getMarker());
            }
            else
            {
                handler = &(m_context->getToggleMarker());
            }

            for (int selectedRowno=firstSelectedRowno;
                selectedRowno<firstSelectedRowno+nSelectedRows; selectedRowno++)
            {
                for (int selectedColno=firstSelectedColno;
                    selectedColno<firstSelectedColno+nSelectedCols; selectedColno++)
                {
                    const Cell cell(selectedColno, selectedRowno);
                    const int sampleno = m_cells.getSampleno(cell);
                    nchanged += (*handler)(&m_samples, sampleno);
                }
            }
        }

        if (isNoting) break;

        if (nchanged > 0) break;

        {
            const Discard::SampleHandler * handler = &(m_context->getToggleMarker());

            for (int selectedRowno=firstSelectedRowno;
                selectedRowno<firstSelectedRowno+nSelectedRows; selectedRowno++)
            {
                for (int selectedColno=firstSelectedColno;
                    selectedColno<firstSelectedColno+nSelectedCols; selectedColno++)
                {
                    const Cell cell(selectedColno, selectedRowno);
                    const int sampleno = m_cells.getSampleno(cell);
                    nchanged += (*handler)(&m_samples, sampleno);
                }
            }
        }

    } while (false);

    if (! isIgnored)
    {
		this->grdResults->Invalidate();
		updateUI();
    }

	return;
}

std::pair<TColor,TColor>
TfrmDiscardSamples::getColours( const Discard::Cell & cell )
{
    using Discard::Cryovial;
	// using Discard::Util;

	TColor background = BACKGROUND_OOPS;
	TColor foreground = FOREGROUND_NORMAL;

	do
	{
		const int isHeader = m_cells.isHeader(cell);
		if (isHeader == -1) break;
		if (isHeader == 1)
		{
            background = BACKGROUND_HEADER;

            if (cell.getRowno() != 0) break;
            Discard::SCComparator * scc = m_samples.getSCComparator();
            const int sortPosition = scc->getSortPosition(cell.getColno());
            if (sortPosition == 0) break;
            if (sortPosition == 1)
            {
                foreground = FOREGROUND_ASCENDING;
                break;
            }
            if (sortPosition == -1)
            {
                foreground = FOREGROUND_DESCENDING;
                break;
            }
			break;
		}

        const int sampleno = m_cells.getSampleno(cell);
        const Discard::Sample * sample = m_samples.getSample(sampleno);
        if (sample == 0)
        {
			background = BACKGROUND_CANTSELECT;
            break;
        }

		if (! m_samples.isSampleMarkable(*sample))
		{
			background = BACKGROUND_CANTSELECT;
			break;
		}
        
		const int isMarked = m_samples.isMarked(sampleno);
		if (isMarked == -1) break;

		const int crst = sample->getCryovialStatus();

		if (isMarked == 0)
		{
            switch (crst)
            {
            case Cryovial::CONFIRMED:
                background = BACKGROUND_CONFIRMED; break;
            case Cryovial::REMOVED:
                background = BACKGROUND_REMOVED;   break;
            }
            break;
		}

		if (isMarked == 1)
		{
            switch (crst)
            {
            case Cryovial::CONFIRMED:
                background = BACKGROUND_REMOVED; break;
            case Cryovial::REMOVED:
                background = BACKGROUND_DESTROYED;   break;
            }
            break;
		}

	} while (false);

	return std::make_pair(foreground, background);
}

void
TfrmDiscardSamples::updateUI( )
{
    using Discard::Context;
	// using Discard::Util;

	this->grdResults->Visible = hasResults();

	this->btnClear->Enabled = hasResults();

	this->btnSearch->Enabled = m_context->isCreateJobStage() || ! hasResults();

	const int nmarked = m_samples.getNMarked();
	const int nnoted = m_samples.getNNoted();
    const bool isDirty = (nmarked > 0) || (nnoted > 0);
	this->btnConfirm->Enabled = isDirty;

    this->btnAbort->Enabled =
        m_context->isSelectJobStage() &&
        (! isDirty) &&
        m_context->canAbort();

    const bool isNoting = this->bitNote->Visible;
    this->btnNote->Enabled = true;

    m_context->setSelectMode(isNoting ? Context::NOTE : Context::MARK);

    this->btnNote->ShowHint = true;
    std::string hint = m_context->getNote();
    if (hint == "") hint = "removing draft note";
	else hint = Discard::Util::quote(hint);
    this->btnNote->Hint = hint.c_str();

	return;
}

bool
TfrmDiscardSamples::hasResults( ) const
{
    const int nresults = this->grdResults->RowCount - 1; // bit grubby
    return (nresults > 0);
}

void
TfrmDiscardSamples::sortSamples( const int sortColno, const int direction )
{
    Discard::SCComparator * scc = m_samples.getSCComparator();
    scc->insertSortcolno(sortColno, direction);
    showSamples();

    this->grdResults->Invalidate();

    updateUI();

    return;
}

std::string
TfrmDiscardSamples::getDraftNote( const Discard::Cell & cell )
{
    std::string note = "";

    do
    {
        const int colno = cell.getColno();
        const int rowno = cell.getRowno();

		if ((colno < 0) || (rowno < 0)) break; // maybe outside grid

		if (colno == 0) break; // hidden column

        if (colno < m_firstSampleColno) break; // row heading
        if (rowno < m_firstSampleRowno) break; // column heading

        const int sampleno = m_cells.getSampleno(cell);
        if (sampleno < 0) break;
        note = m_samples.getNote(sampleno);

    } while (false);

    return note;
}

bool
TfrmDiscardSamples::isClickWholeRow( )
{
    return (m_clickedColno < m_firstSampleColno);
}

bool
TfrmDiscardSamples::isClickWholeCol( )
{
    return (m_clickedRowno < m_firstSampleRowno);
}

bool
TfrmDiscardSamples::isClickWholeTab( )
{
    return isClickWholeRow() && isClickWholeCol();
}


// start of TfrmDiscardSamples callbacks

__fastcall TfrmDiscardSamples::TfrmDiscardSamples(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::grdResultsDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
    using Discard::Cell;
	// using Discard::Util;
    using Discard::Sample;

	TStringGrid * g = (TStringGrid *) Sender;
	TRect r = Rect;
	int colno = ACol;
	int rowno = ARow;

	do
	{
		g->Canvas->Brush->Color = g->Color;
        const Cell cell(colno, rowno);
        const std::pair<TColor,TColor> colours = getColours(cell);
		g->Canvas->Brush->Color = colours.second;
		g->Canvas->Font->Color = colours.first;
		g->Canvas->FillRect(r);
		InflateRect(&r, -2, -2);
		AnsiString text = g->Cells[colno][rowno];

        const int sampleno = m_cells.getSampleno(cell);
        const Sample * sample = m_samples.getSample(sampleno);
        const bool hasNote = ((sample != 0) && (sample->getNote() != ""));
        if (hasNote)
        {
            if (text != "") text += " ";
            text += "N";
        }

        const bool hasDraftNote = getDraftNote(cell) != "";
        if (hasDraftNote)
        {
            if (text != "") text += " ";
            text += "*";
        }

		g->Canvas->TextRect(r, r.left, r.top, text.c_str());

	} while (false);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::grdResultsMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    using Discard::Cell;
    
	do
	{
        if (Button != mbLeft) break;

        TStringGrid * g = (TStringGrid *) Sender;

        int rowno = -1;
        int colno = -1;
        g->MouseToCell(X, Y, colno, rowno);

        m_clickedColno = colno;
        m_clickedRowno = rowno;

        doSelect(m_context->isNoting());

	} while (false);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::btnConfirmClick(TObject *Sender)
{
	using Discard::Cryovial;
	// using Discard::Util;

	do
	{
        if (m_context->isCreateJobStage())
        {
            frmDiscardReason->init(m_context);
    		if (frmDiscardReason->ShowModal() != mrOk) break;
        }
        if (m_context->isSelectJobStage())
        {
            frmDiscardMethod->init(m_context);
    		if (frmDiscardMethod->ShowModal() != mrOk) break;
        }

		std::string summary = "";
		const int nmarked = m_samples.getNMarked();
		const int nnoted = m_samples.getNNoted();
        if (m_context->isSelectJobStage())
        {
            summary += "Mark " + Discard::Util::asString(nmarked) + " cryovial";
            if (nmarked != 1) summary += "s";
            summary += " for disposal";
        }
        else
        {
			summary += "Mark " + Discard::Util::asString(nmarked) + " cryovial";
            if (nmarked != 1) summary += "s";
            summary += " as " + m_context->getNextCrstatusName();
        }
        if (nnoted > 0)
        {
			summary += " (also " + Discard::Util::asString(nnoted) + " note";
            if (nnoted != 1) summary += "s";
            summary += ")";
        }
		summary += " ... ";

		frmConfirm->initialise(TfrmSMLogin::DISCARD, summary.c_str());

		if (frmConfirm->ShowModal() != mrOk) break;

		AnsiString userid =  frmConfirm->cbUserNames->Text ;

        std::string error = "";

        {
            TCursor cursor = Screen->Cursor;
            Screen->Cursor = crHourGlass;

            error = m_samples.update(m_context->getNextDbCrstatus(),
                m_context->getDescription(), m_context->getReason());

        	Screen->Cursor = cursor;
        }

		if (error != "")
		{
			String message = error.c_str();
			Application->MessageBox( message.c_str(), L"", MB_ICONWARNING);
			break;
		}

        if (m_context->isSelectJobStage())
        {
            using Discard::Sef;
            using Discard::SefBatch;

            TCursor cursor = Screen->Cursor;
            Screen->Cursor = crHourGlass;

            const int nsamples = m_samples.count();

            SefBatch sefbatch(m_context);
            for (int sampleno = 0; sampleno<nsamples; sampleno++)
            {
                const Discard::Sample * s = m_samples.getSample(sampleno);
                if (s == 0) continue;
                if (! m_samples.isMarked(sampleno)) continue;
                sefbatch.addSample(s);
            }
            const size_t nsefs = sefbatch.size();

            if (nsefs > 0)
            {
                sefbatch.publish(-(m_context->allocCids(nsefs)));
            }

        	Screen->Cursor = cursor;
        }

        m_context->setSaved(true);

		reset();

		this->ModalResult = mrOk;

	} while (false);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::btnClearClick(TObject *Sender)
{
    do
    {
	    if (this->btnConfirm->Enabled)
		{
			String title = "Unsaved changes";
			String message = "Clear search results without saving ?";
            if (Application->MessageBox(message.c_str(), title.c_str(),
				MB_OKCANCEL | MB_ICONWARNING) != IDOK) break;
        }

        m_samples.clear();
        reset();

    } while (false);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::btnSearchClick(TObject *Sender)
{
    do
    {
        frmSearch->init(m_context);
        if (frmSearch->ShowModal() != mrOk) break;

        const std::string diagnosis = addSamples();
        if (diagnosis != "")
		{
			String message = diagnosis.c_str();
			Application->MessageBox(message.c_str(), L"", MB_OK);
            break;
        }

        showSamples();

    } while (false);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::grdResultsKeyPress(TObject *Sender,
      char &Key)
{
	// using Discard::Util;

    const char c = Key;

    do
    {
        if (m_context->isSelectJobStage()) break;

        if (c == '\r')
        {
			String message = "barcode ";
			message += Discard::Util::quote(m_barcode).c_str();
			Application->MessageBox(message.c_str(), L"", MB_OK);
            m_barcode = "";
            break;
        }

        m_barcode += c;

    } while (false);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::miSortAscClick(TObject *Sender)
{
    sortSamples(m_clickedColno, 1);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::miSortDescClick(TObject *Sender)
{
    sortSamples(m_clickedColno, -1);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::grdResultsContextPopup(TObject *Sender,
      TPoint &MousePos, bool &Handled)
{
    using Discard::Cell;
    using Discard::Sample;

    do
    {
        TStringGrid * g = (TStringGrid *) Sender;

        int rowno = -1;
        int colno = -1;
        g->MouseToCell(MousePos.x, MousePos.y, colno, rowno);

        m_clickedColno = colno;
        m_clickedRowno = rowno;

		if ((m_clickedColno < 0) || (m_clickedColno < 0)) break; // outside grid

		if (m_clickedColno == 0) break; // hidden column

        TPoint screenPos = ((TControl *) Sender)->ClientToScreen(MousePos);

        TPopupMenu * menu = 0;
        if (isClickWholeTab())
        {
            menu = this->popTab;
        }
        else
        {
            if (isClickWholeRow())
            {
                menu = this->popRow;
            }
            else
            {
                if (isClickWholeCol())
                {
                    menu = this->popCol;
                }
                else
                {
                    menu = this->popCell;

                    const Cell cell(m_clickedColno, m_clickedRowno);
                    const int sampleno = m_cells.getSampleno(cell);
                    const Sample * sample = m_samples.getSample(sampleno);

                    this->miDiscardCell->Enabled = (sample != 0)
                        && (m_samples.isSampleMarkable(*sample));
                    this->miNoteCell->Enabled = (sample != 0);

                    this->miViewNote->Enabled = (sample != 0)
                        && (sample->getNote() != "");

                    this->miViewDnote->Enabled = getDraftNote(cell) != "";
                }
            }
        }

        menu->Popup(screenPos.x, screenPos.y);

    } while (false);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::btnNoteClick(TObject *Sender)
{
    do
    {
        frmDiscardNote->init(m_context);
        if (frmDiscardNote->ShowModal() != mrOk) break;

        updateUI();
        
    } while (false);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::cmbModeChange(TObject *Sender)
{
    updateUI();
}
//---------------------------------------------------------------------------


void __fastcall TfrmDiscardSamples::grdResultsMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    using Discard::Cell;

    TStringGrid * g = (TStringGrid *) Sender;

    std::string dnote = "";

	do
	{
        int rowno = -1;
        int colno = -1;
        g->MouseToCell(X, Y, colno, rowno);

        dnote = getDraftNote(Cell(colno, rowno));

	} while (false);

    if (dnote == "")
    {
        g->ShowHint = false;
    }
    else
    {
        g->ShowHint = true;
        g->Hint = dnote.c_str();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::miDiscardCellClick(TObject *Sender)
{
    doSelect(false);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::miViewNoteClick(TObject *Sender)
{
    using Discard::Cell;
    using Discard::Sample;
	// using Discard::Util;

    const Cell cell(m_clickedColno, m_clickedRowno);

    const int sampleno = m_cells.getSampleno(cell);
    const Sample * sample = m_samples.getSample(sampleno);
    std::string note = "";
    if (sample != 0) note = sample->getNote();

	String title = "";
	String text = "";
    if (note == "")
    {
        title = "No Note";
        text = "";
    }
    else
    {
        title = "Note";
		text = note.c_str();
    }
	Application->MessageBox(text.c_str(), title.c_str(), MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::miDiscardAllClick(TObject *Sender)
{
    doSelect(false);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::miDiscardColClick(TObject *Sender)
{
    doSelect(false);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::miDiscardRowClick(TObject *Sender)
{
    doSelect(false);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::miViewDnoteClick(TObject *Sender)
{
    using Discard::Cell;
    // using Discard::Util;

    const Cell cell(m_clickedColno, m_clickedRowno);

    const std::string dnote = getDraftNote(cell);

	String title = "";
	String text = "";
    if (dnote == "")
    {
        title = "No Draft Note";
        text = "";
    }
    else
    {
        title = "Draft Note";
		text = dnote.c_str();
    }
	Application->MessageBox(text.c_str(), title.c_str(), MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::bitDiscardClick(TObject *Sender)
{
    this->bitDiscard->Visible = false;
    this->bitNote->Visible = true;
    updateUI();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::bitNoteClick(TObject *Sender)
{
    this->bitDiscard->Visible = true;
    this->bitNote->Visible = false;
    updateUI();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::miSortAsc2Click(TObject *Sender)
{
    sortSamples(m_clickedColno, 1);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::miSortDesc2Click(TObject *Sender)
{
    sortSamples(m_clickedColno, -1);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::miNoteColClick(TObject *Sender)
{
    doSelect(true);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::miNoteCellClick(TObject *Sender)
{
    doSelect(true);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::miNoteRowClick(TObject *Sender)
{
    doSelect(true);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::miNoteAllClick(TObject *Sender)
{
    doSelect(true);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Action = caHide;

    do
    {
        if (m_context->isSaved()) break;
        
	    if (! this->btnConfirm->Enabled) break;

		String title = "Unsaved changes";
        String message = "Return to main menu without saving ?";
		if (Application->MessageBox(message.c_str(), title.c_str(),
			MB_OKCANCEL | MB_ICONWARNING) == IDOK) break;

        Action = caNone;

    } while (false);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardSamples::btnAbortClick(TObject *Sender)
{
    do
    {
		String title = "Abort job";
		String message = "Abort job ?";
        if (Application->MessageBox(message.c_str(), title.c_str(),
			MB_OKCANCEL) != IDOK) break;

        std::string summary = "Abort job " + m_context->calcJobDescription();

		frmConfirm->initialise(TfrmSMLogin::DISCARD, summary.c_str());

		if (frmConfirm->ShowModal() != mrOk) break;

		AnsiString userid =  frmConfirm->cbUserNames->Text;

        std::string error = "";

        {
            TCursor cursor = Screen->Cursor;
            Screen->Cursor = crHourGlass;

            error = m_samples.reset();

        	Screen->Cursor = cursor;
        }

		if (error != "")
		{
			String message = error.c_str();
			Application->MessageBox(message.c_str(), L"", MB_ICONWARNING);
			break;
		}

		reset();

		this->ModalResult = mrOk;

    } while (false);
}
//---------------------------------------------------------------------------

