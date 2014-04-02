# RPKI_Downgrade_Detector


## Overview

detector <old file> <new file> <outputfile>

optional flags:

--U2I - enables unknown to invalid downgrade reporting, beware generates large files.




## Getting Setup


### Step 1: Download the Downgrade Detector source and compile it.

```
git clone git@github.com:BUSEC/RPKI_Downgrade_Detector.git
make all
```


### Step 2: Compile Modified scan_roas.

The downgrade detector ingests the output of the scan_roas utility. Unfortunately scan_roas as distributed by the rcynic project is missing a field that we require. Therefore the next step to getting the downgrade detector setup is compiling a modified a version of scan_roas. 

First we need to download rcynic and configure rcynic.

```
svn checkout http://subvert-rpki.hactrn.net/trunk/
cd trunk
svn up -r5064
./configure --disable-ca-tools
make all
```

Next we copy our modified scan_roas c file into the rcynic directory for scan_roas and compile it.

```
cp RPKI_Downgrade_Detector/scan_roas.c trunk/utils/scan_roas/
cd trunk/utils/scan_roas/
make all
```

#### Important! 
For the rest of this documentation when we use the command './scan_roas', we will be refering to the modified scan_roas command in this directory. You may have to reference this command by exact path to avoid confusion the default scan_roas.

### Step 3: Setup Rcynic.

Follow the directions on the [rcynic webpage](http://rpki.net/wiki/doc/RPKI/Installation) to setup rcynic.

Running
---------

Once rcynic is up and running ([a guide to getting rcynic running](http://rpki.net/wiki/doc/RPKI/RP/rcynic)) it should create an authenicated directory with the current timestamp. 
After two runs you should have two timestamped authenticated directories, one directory for each run. 
We can now use the detector to notice any downgrades between these two states of rcynic.
We will refer to the older authenticated directory as 'authenticated-old' and the most recent authenticated directory as 'authenticated-new' in place of timestamps.

### Step 1. Parse the ROA files.

We use the modified scan_roas files to create two files 'old.sr' and 'new.sr' from the two authenticated directories.

```
cd trunk/utils/scan_roas/
./scan_roas /path/to/authenticated-old/ > old.sr
./scan_roas /path/to/authenticated-new/ > new.sr
```

### Step 2. Run the detector.

The detect takes the file old.sr, new.sr, and an output filename and outputs the downgrades between old and new in the output file.

```
./detector.out old.sr new.sr output.json
```

Note that due to the large number of unknown to invalid downgrades we ignore these downgrades unless the flag --U2I is supplied.



