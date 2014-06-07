
## xe5 install

<http://thepiratebay.se/torrent/9154906/Embarcadero_Delphi_XE5_with_Update_1>
`3A3FD2DF031ADA71A7AC21AE7C0B637AA794C0E4`

Mount `delphicbuilder_xe5_upd1_win.iso` with MagicDisc. `G: RADStudioXE5`.

    [AutoRun]
    open=Install_RADStudio.exe
    icon=delphi.ico, 0
     
    [CONFIG]
    SKU=RADSTUDIO

Run `Install_RADStudio.exe` - splash install screen appears. Click "Install Delphi XE5 and/or C++ Builder XE5" - nothing happens. The [torrent](http://thepiratebay.se/torrent/9154906/Embarcadero_Delphi_XE5_with_Update_1) is supposedly just the original installer [from Embarcadero](http://altd.embarcadero.com/download/radstudio/xe5/delphicbuilder_xe5_upd1_win.iso). Is it? No:

    4.7G May 30 06:50 delphicbuilder_xe5_upd1_win (1).iso*
    2.6G Apr 11 21:40 delphicbuilder_xe5_upd1_win.iso*

("(1)" is downloaded from Embarcadero, the other must be the torrent). 

Downloaded installer from Embarcadero. Mounted, installed, OK.

[Installation notes](http://docwiki.embarcadero.com/RADStudio/XE5/en/Installation_Notes_for_XE5)

### crack

<http://thepiratebay.se/torrent/9541778/Embarcadero_RAD_Studio_XE5__crack_only>
`68D4AE8009E572D6B408AAE6B3607C888EF756A1`

Following instructions in `_installation instructions.txt`:

Turn off Sophos on-access scanning before unpacking keygen and activator.

Start installer.

Ran `keygen.exe`:

Serial:   PNA4-BPTM7R-8WCLMS-HYNS
Reg ID:   10417151

After clearing some space (emptying recycle bin, and deleting torrent iso (and quo.tgz)), whole thing installed including Boost. 

Ran `activator.exe`: "BDS version not support!"

Tried generation the activation (`.slip`) file, which gets put in `C:\Program Files (x86)\Embarcadero\RAD Studio\12.0\License`, but that didn't work either.

Arg. Picked up the XE2 install, or is the installer I downloaded from Embarcadero not the one this crack was made for? Well the Embarcadero one is 5,019,125,760b... and so is the torrent.

Looking at the details for the [original torrent](http://thepiratebay.se/torrent/9154906/Embarcadero_Delphi_XE5_with_Update_1)
(`3A3FD2DF031ADA71A7AC21AE7C0B637AA794C0E4`), it's supposed to be 4.67 GiB (5019125760 Bytes), not 2.6 (as when first downloaded). Try torrent again.

It's 5,019,125,760b, same as the one straight from Embarcadero. Tried to run it - clicking Install from the splash screen doesn't seem to do anything again.

Or at least... it didn't do anything until I re-enabled on-access scanning in Sophos...

Started install again. Chose to install Delphi as well this time (which gives you the Android bit which isn't available if you uncheck Delphi - installer bug?)


3NAP-2KSZFS-53MB6D-MBPN
10417151

VNFS-SQB4KP-PDJBCS-KPQP
10417151



(Note: DO NOT click on "Generate Activation File", it is not needed for this installation ) - oops, I did

BDS not support!

reply on thread

tried to back up `delphicbuilder_xe5_upd1_win (1).iso` on phone SD but it wouldn't - presume FAT32 filesystem or similar.

copied radstudio_xe4_esd from 

    \\sanback\Software Installers\RAD Studio\radstudio_xe4_esd





xe5 
 install-
 don't need
  interbase
  codesite
 to install
  boost


Get XE4 from work?