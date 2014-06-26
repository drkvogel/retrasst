# refboxes todo

## general

need way of recreating test cases, probably, and reset button

## dummy run

Why? Is this needed? For me for debugging, or required by spec or from meeting?

Improvements suggested:

1) Could be made more visually explicit - a popup for example

2) Automatically do live run after dummy run, on error:

  a) stop
  b) colour differently and skip

e.g. `vlab::test_ldbc STICS SERUM_2`

## looking through all projects slow

Checking all projects for clashing storage takes a while, less on subsequent boxes (due to Ingres caching?), could be very time-consuming when there are a lot of referred boxes. 

Could now possibly use `c_slot_allocation` to search central - discuss with Nick.

## group actions

multiple rows could be actioned together to save time.