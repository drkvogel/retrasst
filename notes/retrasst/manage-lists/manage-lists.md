
## manage lists - new screen 

During meeting on 2014-07-15 (RIP John 'Dickie' Bird - 4th anniversary), it was decided that a new screen, similar to the main screen of Retrieval Assistant (RetrAssMain), was required, for labs staff at CTSU to deal with jobs that had been retrieved - the next stage might be Analyse Samples, or Discard, or transfer to somewhere else - in chunks or even parts of chunks - so needs a more fine-grained way of recording the progress of this stage - hence a new table will be added to the database which will relate to `c_retrieval_job`. The initial screen, however, will be a list of retrieval jobs, i.e. very similar to the main screen of retrieval assistant. 

I suggested copy-pasting RetrAsstMain.cpp/h to a new form, and Nick suggested subclassing it. Then I thought of making the existing form an abstract superclass, and subclassing both the main Retrieval Assistant screen and the new ('Manage Lists') screen from that.

Tried making `TfrmRetrievalAssistant` a subclass of `TfrmRetrievalJobList`. OK, that worked. Then tried making `ManageLists.cpp/h/dfm` and making it a subclass also of `TfrmRetrievalJobList`. OK, until you run it - you get errors about components already having been instantiated. Alternatively tried to make `ManageLists` a non-form Unit - either by creating again as a Unit, or by removing/commenting:

    //#pragma resource "*.dfm"

setting the `.cpp` file to use the RetrAsst form:

    #pragma resource "RetrAsstMain.dfm"

but then you get access violations. Then we remembered that the official way to do it is to do File -> New -> Other -> Inheritable Items and choose the existing form to subclass (in the same was as SMLogin was created). This creates items in the `.dfm` that are `inherited` - these cannot be removed from the resulting subclass, but can of course be made invisible. New controls can be added to the subclass. 

Rewound to just after I did the first subclass:

    $ git checkout 5792f83
    Note: checking out '5792f83'.
    
    You are in 'detached HEAD' state. You can look around, make experimental
    changes and commit them, and you can discard any commits you make in this
    state without impacting any branches by performing another checkout.
    
    If you want to create a new branch to retain commits you create, you may
    do so (now or later) by using -b with the checkout command again. Example:
    
      git checkout -b new_branch_name
    
    HEAD is now at 5792f83... made TfrmRetrievalAssistant a subclass of TfrmRetrievalJobList


TfrmRetrieveMain - is Create List

## Subclass form using Inheritable Items dialog

Subclassed
    * RetrAsstMain from new RetrievalListBase
    * ManageLists, from RetrievalListBase

In the proper way (using File -> New -> Inheritable Items).

i.e. created:

    RetrievalListBase
     |- RetrAsstMain
     L- ManageLists

Now to decide what methods to move into subclasses, and/or which to override, and what extra controls required.

