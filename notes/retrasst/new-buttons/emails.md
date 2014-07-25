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
 