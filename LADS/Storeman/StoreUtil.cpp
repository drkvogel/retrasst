//---------------------------------------------------------------------------

#include <sstream>
#include "StoreUtil.h"
#include "LCDbTankMap.h"
#include "LCDbObject.h"
#include "LDbBoxSize.h"
#include "LPDbBoxes.h"
#include "LDbBoxType.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

int Util::getImageIndex( const IPart* data )
{
	switch( data->getType() ) {
		case IPart::SitesType:
			return SITE_LIST;
		case IPart::SiteType:
			return SITE;
	}
	switch( data->availability() ) {
		case IPart::UNAVAILABLE:
			return OFF_LINE;
		case IPart::IS_AVAILABLE:
			return AVAILABLE;
		case IPart::IS_EMPTY:
			return NO_CHILDREN;
		case IPart::PART_FULL:
			return PART_FILLED;
		case IPart::IS_FULL:
			return ALL_FILLED;
		default:
			return UNKNOWN;
	}
}

//---------------------------------------------------------------------------

int Util::getImageIndex( TTreeNode * node )
{
	if( node != NULL && node -> Data != NULL ) {
		return getImageIndex( (IPart*)(node->Data) );
	} else {
		return UNKNOWN;
	}
}

//---------------------------------------------------------------------------

void Util::InitPropertyGrid( TStringGrid *grdProps )
{
	grdProps->Cells[0][0] = "Property";
	grdProps->Cells[1][0] = "Value";
	grdProps->ColWidths[1] = (int)(grdProps->Width * 0.7);
	int used = grdProps->ColWidths[1] + 28;
	grdProps->ColWidths[0] = grdProps->Width - used;
}

//---------------------------------------------------------------------------
/*
void Util::ShowPropertyGrid(TStringGrid *grdProps, IPart* data)
{
	std::auto_ptr<ROSETTA> r = data->getProperties();
	int row = 1;
	for( int i = 0; i < r -> count(); i ++ ) {
		const std::string value = r->getString( i );
		if( !value.empty() ) {
			const std::string name = r->getName( i );
			grdProps->Cells[ 0 ][ row ] = split( name.c_str() );
			grdProps->Cells[ 1 ][ row ] = value.c_str();
			row ++;
		}
	}
	grdProps->RowCount = grdProps->FixedRows + row;
}
*/
//---------------------------------------------------------------------------

const char * Util::split( const char * fieldName )
{
	static char buffer[ 45 ];
	int i = 0;
	if( fieldName != NULL && fieldName[ 0 ] != '\0' )
	{
		buffer[ 0 ] = toupper( fieldName[ 0 ] );
		while( ++ i < 44 && fieldName[ i ] != '\0' )
			buffer[ i ] = fieldName[ i ] == '_' ? ' ' : fieldName[ i ];
	}
	buffer[ i ] = '\0';
	return buffer;
}

//---------------------------------------------------------------------------

void Util::ClearPropertyGrid(TStringGrid *grdProps)
{
	for(int i = 1; i < grdProps->RowCount; i++ )
		grdProps->Rows[i]->Clear();
	grdProps->RowCount = 2;  
}

//---------------------------------------------------------------------------

bool Util::validateText( TEdit* txt, TLabel* controlname )
{
	String stxt = txt->Text.Trim();
	if( stxt.Length() > 0 ) {
		return true;
	}
	String buf = controlname->Caption + " not specified";
	String title = "Validation Error";
	Application->MessageBox( buf.c_str(), title.c_str(), MB_OK);
	txt->SetFocus();
	return false;
}

//---------------------------------------------------------------------------

int Util::validateInteger( TEdit* txt, int min, int max )
{
	String stxt = txt->Text.Trim();
	int val = stxt.ToIntDef( -1 );
	if( val >= min && val <= max ) {
		return val;
	}
	String buf;
	buf.sprintf( L"Enter a number between %d and %d", min, max );
	Application->MessageBox( buf.c_str(), L"Validation Error", MB_OK );
	txt->SetFocus();
	return -1;
}

//---------------------------------------------------------------------------
// 	populate a tree node from the database and update the display
//---------------------------------------------------------------------------

void Util::ShowTreeNode( TTreeView* tree, TTreeNode* parent, bool showGaps, bool update )
{
	IPart* data = (IPart*)(parent -> Data);
	if( update ) {
		Screen->Cursor = crSQLWait;
		data -> populate();
	}
	if( data -> getChildCount() > 0 ) {
		char buff[ 20 ];
		short next = 1;
		for( IPart* li : data -> getList() ) {
			if( showGaps ) {
				while( next < li->getPosition() ) {
					std::sprintf( buff, "(%d)", next );
					tree -> Items -> AddChild( parent, buff );
					next ++;
				}
			}
			String label = li->getName().c_str();
			tree -> Items -> AddChildObject( parent, label, li );
			next = li->getPosition() + 1;
		}
		if( showGaps ) {
			while( data -> getCapacity() >= next ) {
				std::sprintf( buff, "(%d)", next );
				tree -> Items -> AddChild( parent, buff );
				next ++;
			}
		}
		parent -> Expand( true );
	}
	Screen->Cursor = crDefault;
}

//---------------------------------------------------------------------------

bool Util::isVesselInUse( const std::string & srlno )
{
	String error;
	const LCDbObject * store = LCDbObjects::records().findByName( srlno );
	if( store != NULL ) {
		if( store->getObjectType() != LCDbObject::STORAGE_VESSEL ) {
			error = "Wrong object type for " + String( srlno.c_str() );
		}
		for( Range< LCDbTankMap > tmi = LCDbTankMaps::records(); tmi.isValid(); ++ tmi ) {
			if( tmi->isActive() && tmi->getStorageCID() == store->getID() ) {
                    //??? store->getId() is a population, tmi->getStorageCID() is a vessel?
				error = String( srlno.c_str() ) + " is already in use";
			}
		}
	}
	if( error.IsEmpty() ) {
		return false;
	} else {
		Application->MessageBox( error.c_str(), L"Validation Error", MB_ICONWARNING );
		return true;
	}
}

//---------------------------------------------------------------------------
//	check if given name is in use for a storage population (hive)
//	return rack layout CID if it does or an error code if not
//---------------------------------------------------------------------------

int Util::getCurrentLayout( const std::string & name )
{
	const LCDbObject * obj = LCDbObjects::records().findByName( name );
	if( obj == NULL ) {
		return NOT_FOUND;
    }
	else if( obj->getObjectType() != LCDbObject::STORAGE_POPULATION ) {
		return WRONG_TYPE;
	} else {
		return Layouts::getLayoutId( obj->getID() );
	}
}

//---------------------------------------------------------------------------
//	check if given site/position (and shelf) is in use; warn if it is
//---------------------------------------------------------------------------

bool Util::isPosDuplicate( int locationCID, short pos, short shelf )
{
	bool duplicate = false;
	for( Range< LCDbTankMap > store = LCDbTankMaps::records(); store.isValid(); ++ store ) {
		if( store->getStatus() < LCDbTankMap::OFFLINE ) {
			if( store->getLocationCID() == locationCID && store->getPosition() == pos ) {
				if( shelf == 0 || store->getPopulation() == 0 || store->getPopulation() == shelf ) {
					duplicate = true;
				}
			}
		}
	}
	if( duplicate ) {
		std::stringstream error;
		const LCDbObject & loc = LCDbObjects::records().get( locationCID );
		error << loc.getName() << " position " << pos;
		if( shelf > 0 ) {
			error << '[' << shelf << ']';
		}
		error  << " is already occupied";
		String message = error.str().c_str();
		Application->MessageBox( message.c_str(), NULL, MB_ICONWARNING );
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------

void Util::ShowSelectedSubTree( TTreeView* tree, unsigned minBoxCount )
{
	Screen->Cursor = crSQLWait;
	TreeParts nodes( minBoxCount );
	if( tree != NULL && tree -> Selected != NULL )
		nodes.add( tree -> Selected );

	while( !nodes.finished() )
	{
		TTreeNode * next = nodes.getNext();
		if( next -> HasChildren )
		{
			next = next -> getFirstChild();
			do
			{	nodes.add( next );
				next = next -> getNextSibling();
			} while( next != NULL );
		}
		else
		{	IPart * data = (IPart *)(next -> Data);
			data -> populate();
			const std::vector<IPart*> & list = data -> getList();
			std::vector<IPart*>::const_iterator li = list.begin();
			while( li != list.end() )
			{
				data = *li;
				String name = data -> getName().c_str();
				TTreeNode * n = tree->Items->AddChildObject( next, name, data );
				nodes.add( n );
				++ li;
			}
		}
	}
	Screen->Cursor = crDefault;
}

//---------------------------------------------------------------------------

LQuery Util::projectQuery( int projID, bool ddb ) {
	LCDbProjects & projList = LCDbProjects::records();
	int current = projList.getCurrentID();
	if( projID == 0 ) {
		projID = current;
	}
	const LCDbProject & proj = projList.get( projID );
	if( projID != current ) {
		projList.setCurrent( proj );
	}
	return LQuery( LIMSDatabase::getProjectDb( proj.getDbName(), ddb ) );
}

//---------------------------------------------------------------------------

/** Sort function: compare non-numeric characters of two strings.
    If identical, strip out numeric chars, concatenate, and compare as ints.
    Will sort like this:
        Box 1
        Box 2
        Box 10
        Box 11
        Box 20
        etc.       */
bool Util::numericCompare(const std::string a, const std::string b) {
    struct {
        static unsigned int justNumerics(std::string a) {
            std::ostringstream numerics;
            for (unsigned int i=0; i<a.length(); i++) {
                char ch = a.at(i); // at() throws exception if out of range
                if (ch >= 0x30 && ch < 0x3A) {
                    numerics << ch; // pull out the numeric characters
                }
            }
            return atoi(numerics.str().c_str());
        }
        static std::string nonNumerics(std::string a) {
            std::ostringstream others;
            for (unsigned int i=0; i<a.length(); i++) {
                char ch = a.at(i);
                if (ch < 0x30 || ch >= 0x3A) {
                    others << ch; // pull out the non-numeric characters
                }
            }
            return others.str();
        }
    } local;
    // fixme return immediately if strings are equal
    if (a == b)
        return false; //???
#ifdef _DEBUG
    std::string     a_non, b_non;
    int             a_just, b_just;

    a_non           = local.nonNumerics(a);
    b_non           = local.nonNumerics(b);
    int diff_non    = a_non.compare(b_non);

    a_just          = local.justNumerics(a);
    b_just          = local.justNumerics(b);
    int diff_just   = a_just < b_just;

    if (0 == diff_non) {
        return diff_just;
    } else {
        return diff_non;
    }
#else
    int diff = local.nonNumerics(a).compare(local.nonNumerics(b));
    return diff == 0 ? local.justNumerics(a) < local.justNumerics(b) : diff < 0;
#endif
}

std::string Util::getAliquotDescription(int aliquot_type_cid) { // c_object_name 6: aliquot type
    static std::map<int, std::string> cache;
    std::ostringstream oss;
    std::string description;

    if (0 == aliquot_type_cid) return "Not specified";
    try {
        std::map<int, std::string>::iterator found = cache.find(aliquot_type_cid);
        if (found != cache.end()) { // found
            return (found->second);
        } else {
			const LCDbObject * aliquot = LCDbObjects::records().findByID(aliquot_type_cid);
			description = aliquot ? aliquot->getName().c_str() : "unknown";
            cache[aliquot_type_cid] = description; // cache result
            return description;
        }
    } catch (...) {
        oss << "ID "<<aliquot_type_cid<<" not found";
    }
    return oss.str();
}

bool Util::secondaryIndexExists(int project_cid, std::string indexName, bool exactMatch) {
    const char * sql;
    if (exactMatch) {
		sql  = "select count(*) from iitables where table_owner=dbmsinfo('dba') and table_type='I' and index_name = ':index_name'";
    } else {
        sql  = "select count(*) from iitables where table_owner=dbmsinfo('dba') and table_type='I' and index_name like '%:index_name%'";
    }
	LQuery qt(Util::projectQuery(project_cid));
    qt.setSQL(sql);
    qt.setParam("index_name", indexName);
    qt.open();
    int count = qt.readInt(0);
    return count >= 1;
}

bool Util::statsOnColumn(int project_cid, std::string tableName, std::string colName) {
/** determine whether stats are set on cryovial_store table */
    int stat_count;
    const char * select_stats =
        "SELECT COUNT(*) FROM iistatistics s JOIN iirelation r"
        " ON s.stabbase = r.reltid AND s.stabindex = r.reltidx"
        " JOIN iiattribute a ON a.attrelid = r.reltid"
        " AND a.attrelidx = r.reltidx AND a.attid = s.satno"
        " WHERE r.relid = :table_name"
        " AND a.attname = :column_name";
	LQuery qt(Util::projectQuery(project_cid));
    qt.setSQL(select_stats);
    qt.setParam("table_name", tableName);
    qt.setParam("column_name", colName);
    qt.open();
    stat_count = qt.readInt(0);
    return stat_count == 1;
}

std::string Util::boxTubeTypeName(int project_cid, int box_cid) {
/** box_name.box_type_cid -> c_box_content.box_size_cid -> c_box_size.tube_type -> c_object_name.object_cid */
    struct ProjBox {
        int project_cid;
        int box_cid;
        ProjBox(int proj, int box) : project_cid(proj), box_cid(box) {}
        operator<(const ProjBox & rhs) const { return project_cid < rhs.project_cid || box_cid < rhs.box_cid; }

    };
    typedef std::map< ProjBox, std::string > BoxToTypeMap;

    std::string tubeType;
    static BoxToTypeMap map;
    BoxToTypeMap::iterator found;
    ProjBox projBox(project_cid, box_cid);
    found = map.find(projBox);
    if (found == map.end()) { // not added yet
        LQuery q(Util::projectQuery(project_cid, false));
        LPDbBoxNames boxes; // no LCDbBoxName(s) (c_box_name)
        try {
            const LPDbBoxName * box     = boxes.readRecord(q, box_cid);
            const LCDbBoxSize * size    = box->getLayout(); // might be null as not right project
            const LCDbObject * tube     = LCDbObjects::records().findByID(size->getTubeType());
            tubeType = tube->getName();
        } catch (...) {
            tubeType = "not found";
        }
        map[projBox] = tubeType;
    } else { // already in map
        tubeType = found->second; //std::string temp = found->second; return temp;
    }
    return tubeType;
}

//if (NULL == box) throw runtime_error(__FUNC__ +" null LPDbBoxName");
//if (NULL == size) throw runtime_error(__FUNC__ +" null LCDbBoxSize");
//if (NULL == tube) throw runtime_error(__FUNC__ +" null LCDbObject");
//operator() (const ProjBox lhs, const ProjBox rhs) { return lhs.project_cid == rhs.project_cid && lhs.box_cid == rhs.box_cid; }
