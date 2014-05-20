 
# New Test jobs following 2.7.2 database rebuild

## 978253 "Retrieval_978253", "Retrieving THRIVE samples (with secondary)"

    primary: [-31781] EDTA_1 secondary: [-31782] EDTA_2

No storage details found.

## 978238 "Retrieval_978238", "Several REVEAL boxes for Chris"

    primary: [-31781] EDTA_1 secondary: [-31782] EDTA_2

unknown aliquot type -211291 for this job


## 978243 "Retrieval_978243", "Reveal boxes of three types"

    primary: [-31781] EDTA_1 secondary: [-31782] EDTA_2

unknown aliquot type -211291 for this job


---

## StoreDAO

Anything useful? Should be using?

---

    select retrieval_cid, external_name, description, primary_aliquot, secondary_aliquot, project_cid, status, reason  from c_retrieval_job

