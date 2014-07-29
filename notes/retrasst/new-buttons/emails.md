Hi Martin 
 
We continued the discussion after you left and hope we've come up with something that fits the requirements. It has roughly six buttons: Enter, Not Found, Skip, Bad, Note and Sign Off. 
 
Working through my four scenarios (ignoring the complication of secondaries): 
 
1) Look in the box, find the right sample, scan it in or type the barcode and hit Enter: 
 If that entry is Expected, it would be marked as Collected; 
 If Deferred, it would be marked as Found Later (the distinction may be of little relevance to the user). 
 
2) Look in the box, find the wrong sample, scan it in or type the barcode and hit Enter: 
 The user is asked to check the sample. If they confirm the barcode: 
  if the entry is Expected, it would be marked as Deferred; 
  if already Deferred, it would be marked as Not Found. 
 
3) Look in the box and find nothing there. Hit Not Found: 
 If that entry is Expected, it would be marked as Deferred; 
 If it's already Deferred, it would be marked as Not Found (maybe with an "are you sure?" dialogue). 
 
4) Decide not to look (e.g. for a secondary). Hit Skip: 
 Entry remains Expected or Deferred (but allows user to move to the next one). 
 
The Bad and Note buttons add a flag and/or note to the expected sample when you scan the barcode (or modify the one you've just dealt with? We couldn't be sure; they can't do both). 
 
The Sign Off button is disabled until they reach the end of a chunk. It is not enabled if any entries are still Expected. It gives a strong warning if any entries are still Deferred; if the user decides to ignore the warning, Deferred entries will be marked Not Found, or it may be simpler not to enable Sign Off until everything is marked as Collected, Found Later or Not Found. 
 
I hope that makes sense. Your comments would be welcome 
-- 
Nick 

---

## after 6-hour mega meeting, Fri 2014-07-25

Hi Martin 
 
The discussion following your departure was long, detailed and not always harmonious but I think we're a lot closer to coming up with a suggestion that gives you most of what you want without too many major changes. I still have a few nagging questions, though. 
 
There are various ideas floating about: 
 
1) An exercise: one or more retrievals. It has a name and description but very little else. The other day we agreed that each retrieval must belong to an exercise. 
 
2) A retrieval. Each retrieval has a number. If samples are being retrieved into new boxes, the barcode will usually include the retrieval number as a prefix, creating a box set. At present we also record a name, description and reason for each retrieval. Question: do we need all three or do some relate to the exercise? 
 
3) A retrieval list: the samples to put into the boxes and where you ought to be able to find them. It might suggest more than one sample for a given position, in case you can't find their first choice. 
 
4) A retrieval plan: created from a retrieval list by the Retrieval Assistant. Whereas the list is often in destination box/position order (e.g. for analysis), the plan dictates the order in which the samples must be collected (for ease of retrieval). A plan may by divided up into chunks. 
 
5) A retrieval task: working through a retrieval plan to collect the samples, guided by the Retrieval Assistant. 
 
6) Retrieval stages, e.g. List created, plan created, samples being retrieved, task complete, sent to RDB, shipped... This has a new table in the database and a new screen in StoreMan. 
 
Until today I assumed you created a list, created a plan from it, retrieved the samples, signed it off and that was it. The current system therefore assumes a 1:1 relationship between list, plan and task; plans cannot be modified once they've been created so, currently, nor can a retrieval list. Question: if we can't come up with a suitable solution, is this a limitation you can live with? As a work around for missing cryovials you'd have to create a new retrieval list to work with an existing box set, giving your two retrievals with the same retrieval number. 
 
If you do need to edit (in some sense) a retrieval list, it would make our lives a lot easier if you couldn't modify an existing plan. KEY QUESTION: can you add to a list that's being acted on or vice versa? Thinking about the spreadsheet model, Martin sets it up, Tim might create the plan and Richard may act on it but can Martin edit it before Richard and Tim have finished with it. I'd like to hope not. In which case we can still create tasks from plans and plans from lists but what you can do is governed by the stage you are at. 
 
I'll put together a draft table design on Monday. I don't know if we'll have a chance to discuss it next week if Thursday's the only day we're all in but Chris has also had some ideas about the user interface 
-- 
Martin (????) [it's from Nick]

---

Hi Nick 


I think I detected two questions in there:

Question: do we need all three or do some relate to the exercise?

There does appear to be some duplication but I don't think we ever got round to formalising how we'd use these fields so I'm not sure we want to change these too much until we've agreed how these fields should be used and what each represents.

KEY QUESTION: can you add to a list that's being acted on or vice versa?

In your discussion you suggest that an in progress retrieval might be edited once started. I think it's fairly safe to say this is highly unlikely. It's best to think of a retrieval as fairly static, replacement samples would only be required if the project suggests suitable replacement samples once it has been confirmed that a tube in the original list is missing, ordinarily this would be done once the first attempt at retrieval had been made for all samples in that retrieval.

Hope that makes sense and doesn't complicate things.

Martin

---

Thanks

That establishes the ground rules.  I’ll send you alternative suggestions for the database design in case you want to comment on Thursday

Nick

---

Meeting: Deferred retrievals
Start: 31/07/2014 11:00:00  
End: 31/07/2014 12:00:00  
Location: Tulip Treee  
Organizer: Nicholas Goodwin <nicholas.goodwin@ctsu.ox.ac.uk>  
Attendees: Martin Radley <martin.radley@ctsu.ox.ac.uk>participant)
Chris Bird <chris.bird@ctsu.ox.ac.uk>

---

Hi Martin 
 
I've put something in your diary for 11am. The meeting room is booked so it's back to the Tulip Tree. Hopefully we can keep the discussion short this time 
-- 
Nick 

---

Hi Martin 
 
I have fixed the problem you had on Friday and updated the version of StoreMan on the K: drive. 
 
(Chris: there are also changes to the project file in CVS, including a new version number, and updates to the xlib code) 
-- 
Nick 

---

Hello both 
 
In the light of Friday's discussions, we might need further changes to the database structure to allow users to "edit" a retrieval list. This shouldn't result in big changes to anyone's code since it is "highly unlikely" to mean anything more adding samples to the end of the list to fill in any gaps after the initial retrieval has been completed. 
 
Three options for your consideration: 
 
1) Allow multiple records in `c_retrieval_job` for the same box set 
 
When the user wants to edit an existing retrieval, Create List copies relevant fields into a new record, leaving box_set unchanged but creating a new `retrieval_cid`. In effect, `c_retrieval_job` becomes a list of attempts (tasks). The `c_retrieval_history` table will need to be modified slightly to link to the box set, rather than the attempt. 
 
`Start_date` and `finish_date` probably become obsolete (use `c_retrieval_history` instead). I think `external_name` does too, leaving description, reason, `project_cid` and `primary_aliquot` for each attempt. 
 
2) Link each retrieval plan to a stage in the retrieval history 
 
In this version, editing a retrieval list would add new `cryovial_store` records with the existing `retrieval_cid`, but only after the earlier records had been dealt with. The retrieval assistant would see it as a new retrieval and create a new plan. The `c_retrieval_history` table would link on `c_retrieval_job`, as now, but `c_box_retrieval` would link to the relevant record in `c_retrieval_history`, keeping the plans separate. Only the most recent would be active. 
 
`Start_date` and `finish_date` would definitely become obsolete. Others could move to `c_retrieval_history` or even `c_retrieval_note`. As Martin points out, we need to agree how each of these fields are used. This may help us decide between (1) and (2) 
 
3) Create a new table: `c_retrieval_task`
 
Create list would create one or more records in `c_retrieval_task`, linking to `c_retrieval_job`. Each task would include a priority. `Cryovial_store` (and box_store) would link to `c_retrieval_task`, as would the plans in `c_box_retrieval`. Editing an existing list would simply mean adding another task with a lower priority. The Retrieval Assistant would then create another plan. 
 
The key difference between this proposal and (2) is that it allows overlapping tasks. Perhaps, with hindsight, this is the most flexible design of the three. The confusion about aliquot types goes away: you'd just have two tasks, one for the primaries, one for the secondaries. You'd probably have different tasks for different projects, too. However, since it could mean significant changes to StoreMan code and existing data, I'm reluctant to recommend it. 
 
By all means feel free to comment, or not. There may be time to discuss it on Thursday, otherwise I'll ask Marty to implement (1) or (2) 
-- 
Nick 
 