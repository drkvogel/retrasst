## id: 979124, name: "Retrieval_1", desc: "A contrived example mixing THRIVE and REVEAL", type: 4, status: 1, project: 0, primary: -31781, secondary: 0, reason: "order 66"

throws "cannot handle multiple projects" error - correct, at the moment - but exit was ugly

LoadPlanThread::Execute() 
    ...
    if (0 == project_cid) // fixme handle multiple projects
        msgbox("cannot handle multiple projects at the moment - contact core programming"); return;
...
showChunks()
    if (0 == chunks.size()) throw runtime_error("No chunks")        


## id: 979369, name: "Retrieval_3", desc: "Primary and secondary, no location", type: 4, status: 0, project: -1212564, primary: -31781, secondary: -31782, reason: "order 66"

crashes - freezes - on close, see <../misc/freeze-on-plan-save.md>

## "Retrieval_4", "A simple Friday afternoon retrieval", id: 979372, type: 4, status: 1, project: -1212564, primary: -31783, secondary: -833348, reason: "order 66"


# general

## plan

when it saves a plan, it says "saving primary:" but it isn't always the primary you are saving, they could be loose secondaries or neither

## collect

jerky scrolling - could turn off painting while doing this?

basically, accept, not found, defer all seems to work - it's coming to the end of a chunk that is not sure
so need test plans split into small chunks

## "Retrieval_4", "A simple Friday afternoon retrieval", id: 979372, type: 4, status: 1, project: -1212564, primary: -31783, secondary: -833348, reason: "order 66"

Exit in middle of chunk:

> There are unactioned samples in this retrieval plan; not closing job

[OK]

> Handle disposal of empty boxes

[OK]

Collect Empty Boxes window pops up (empty)

[Close]

> handle close

[OK]