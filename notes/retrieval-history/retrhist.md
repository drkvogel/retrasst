# Retrieval history 

First draft – 22 July 2014

## Retrieval stages

A `c_retrieval_history` table will be created to record the stage a retrieval job is currently at and the stage it is likely to be at next, if known. The stages will be taken from `c_object_name`.  Examples could include `PlanCreated`, `JobStarted`, `InHouse`, or `InTransit` (tbc).  The Retrieval Assistant will create some of these records, since the initial stages are well defined, but the user will have to add later records on a case by case basis.

A `c_retrieval_note` table will be created to record details for each event in the retrieval history.  Each stage can have notes attached from `c_canned_text`. These, in turn, can have free text attached (this may change once Chris has created a prototype we can discuss; might `c_sample_event` + `c_sef_detail` give us the structure we need?)

## Box-specific events

Some events, e.g. “defrosted in transit” or “analysed and discarded” will result in a status update in `box_name`, `c_box_name`, `cryovial` and `cryovial_store`. This may apply to particular samples, boxes or the whole box set. Samples that are discarded should have a note attached.  This could be hard coded, canned test or taken from the description of the event.  What happened to each box can be recorded in `l_box_arrival_event_history`. This takes an event code from `c_object_name` but no explanatory text. A record should also be added to `c_sample_event` (this seems like overkill; can we rationalise the recording of discarded samples?)
