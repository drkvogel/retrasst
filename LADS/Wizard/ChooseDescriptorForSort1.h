//---------------------------------------------------------------------------
#ifndef ChooseDescriptorForSort1H
#define ChooseDescriptorForSort1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class DescriptorClass;
class TChooseDescriptorForSort : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TButton *btnSort;
    TButton *Cancel;
    TListBox *ChooseDescriptorBox;
    TLabel *Label1;
    TLabel *Label2;
    TListBox *SortBox;
    void __fastcall num(TObject *Sender);
    void __fastcall CancelClick(TObject *Sender);
    void __fastcall btnSortClick(TObject *Sender);
    void __fastcall ChooseDescriptorBoxMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);

private:	// User declarations
    int selection;
    int maxSelections;
    int numDescriptors;
    DescriptorClass** theDescriptors;

public:		// User declarations
    __fastcall TChooseDescriptorForSort(TComponent* Owner);
    void SetDefaultValues();
    void SetDescriptors( int numDescrip, DescriptorClass** theDescrips );
//    void SetDDescripIndex(int index){descripIndex = index;};
    int PM_sortDescriptor[30]; // index of descriptor on which to sort
                            // relative to ProfileMap Class (and grid)
                            // (only those with tests attached)
    int D_sortDescriptor[30]; // index of descriptor on which to sort
                            // relative to Descriptor Class
                            // (all those with and without tests attached)
    int numSelections; // number of descriptors selected for sort
};
//---------------------------------------------------------------------------
extern PACKAGE TChooseDescriptorForSort *ChooseDescriptorForSort;
//---------------------------------------------------------------------------
#endif
