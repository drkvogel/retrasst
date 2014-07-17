
## new screen 

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

## detached HEAD

In my first attempt at subclassing the form, I labelled the preceding commit: 

    Commit: 5792f83ca846bd2da0d6af426b533d46f1763e8c [5792f83]
    Parents: c4191cb917
    Author: Chris Bird <chris.bird@ctsu.ox.ac.uk>
    Date: 15 July 2014 17:55:02
    made TfrmRetrievalAssistant a subclass of TfrmRetrievalJobList

and then made a further two commits before realising this wasn't the best way to do it, and did a `git reset` to the above point.

Then I carried on committing from that point, seemingly with no problem, but when I tried to pull from home, I got "everything up to date", and if you 

    $ git status
    # Not currently on any branch.

    $ git branch
    * (no branch)
      master
      unfuddle

No branch means we have a [detached HEAD](http://stackoverflow.com/questions/5772192/git-how-can-i-reconcile-detached-head-with-master-origin).

i.e. the current branch does not have a name, and `master` is back where we left it on the old branch where I gave up trying to subclass the form manually.

    $ git symbolic-ref HEAD
    fatal: ref HEAD is not a symbolic ref
    $ git rev-parse refs/heads/master
    dcddcb603e61fb5fbbb343565e28d0bb2f9d87d9
    $ git rev-parse HEAD
    f1e1b8c0e046e30eb6787cc5f5107fb4a1136cff # not same as master

So:

    $ git branch temp           # call the current branch 'temp'
    $ git branch
    * (no branch)               # still on 'no branch' as haven't checked out
      master
      temp
      unfuddle
    $ git checkout temp         # checkout temp
    Switched to branch 'temp'
    $ git status
    # On branch temp            # now HEAD is on branch 'temp'
    nothing to commit (working directory clean)

Now force master to temp with `branch -f`:

    $ git branch -f master temp
    $ git status
    # On branch temp
    no changes added to commit (use "git add" and/or "git commit -a")

But we're still on temp, so checkout master:

    $ git checkout master
    M       notes/retrasst/manage-lists/new-screen.md
    Switched to branch 'master'
    Your branch and 'unfuddle/master' have diverged,
    and have 8 and 2 different commit(s) each, respectively.
    $ git status
    # On branch master
    # Your branch and 'unfuddle/master' have diverged,
    # and have 8 and 2 different commit(s) each, respectively.

OK, so it's telling me that `unfuddle/master` (still on the old, aborted branch) is different to what is now the current local `master` - that's fine.

Check in these notes:

    $ git add -u
    $ git commit -m "notes about branching and detached HEADs"
    [master 57b1e91] notes about branching and detached HEADs
     1 files changed, 34 insertions(+), 0 deletions(-)
    $ git status
    # On branch master
    # Your branch and 'unfuddle/master' have diverged,
    # and have 9 and 2 different commit(s) each, respectively.
    #
    nothing to commit (working directory clean)

Attempt to push to unfuddle:

    $ gpum
    To git@drkvogel.unfuddle.com:drkvogel/retrieval-assistant.git
     ! [rejected]        master -> master (non-fast-forward)
    error: failed to push some refs to 'git@drkvogel.unfuddle.com:drkvogel/retrieval-assistant.git'
    To prevent you from losing history, non-fast-forward updates were rejected
    Merge the remote changes (e.g. 'git pull') before pushing again.  See the
    'Note about fast-forwards' section of 'git push --help' for details.
    $ git push --help

Force push:

    $ git push -f unfuddle master
    Counting objects: 151, done.
    Delta compression using up to 8 threads.
    Compressing objects: 100% (114/114), done.
    Writing objects: 100% (115/115), 60.89 KiB, done.
    Total 115 (delta 86), reused 0 (delta 0)
    To git@drkvogel.unfuddle.com:drkvogel/retrieval-assistant.git
     + dcddcb6...57b1e91 master -> master (forced update)

Ah, I've lost the abandoned branch, this way - locally and now remotely. Should perhaps have renamed `master` to something else to preserve it.

## the proper workflow

All of this is because I didn't do the right thing at the point of branching: knowing I was doing something experimental, I should have created a branch at that point and checked it out, committed on that, then when I realised I needed to go back, gone back to `master` and carried on.

Having made the mistake of staying on `master` for something I knew was an experiment, I should have done `checkout master` so that subsequent commits went onto master rather than an un-named 'no branch' - i.e. with a detached HEAD. It would be nice if git warned you when you were about to do this...



