# Hand Safety Kit 
### Release info
Version 1.0.0
Date: 30th Jan 2023

## How to run
To **start**, open Terminal and run the following commands: (may need to set permission with `chmod +x ./run.sh`) 

    cd Safety_Kit
    ./run.sh

To **stop**, press the **Q** button.


## How to record 
To **start**, open Terminal and run the following commands: 
### Record video while processing frame

    cd Safety_Kit
    ./run.sh -r

### Record processed video 

    cd Safety_Kit
    ./run.sh -rp


To **stop**, press the **Q** button
The video will be saved outside `/Safety_Kit` folder (`/home/mano/_subfolder_`) by default . To change the location of the saved folder, edit `file_output_path =  "../"` in `Main_safety.h`. The name of the video is `video_<time_stamp>.avi`.


## How to remove previous marker information
### Remove zone makers

    cd Safety_Kit
    ./remove.sh -z

### Remove status makers

    cd Safety_Kit
    ./remove.sh -s

### Remove both zone and status makers

    cd Safety_Kit
    ./remove.sh -a



## How to build
### Create directory structure
- Create a /build folder inside Safety_Kit
- The directory structure should look like this:

```
Safety_Kit
|____build
|    readme.md 
|    ...
```

### Using cmake-gui
- Fill in "Where is the source code": (Path to `Safety_Kit` folder above)
- Fill in "Where to build the binaries": (Path to `Safety_Kit/build` folder above)
- Run `Configure`

```
Specify the generator for this project         Unix Makefiles
Use default native compilers		       (Check)
```

- Run `Generate`


### Compile the project
- Run the following command in terminal (may need to set permission with `chmod +x ./build.sh`) 

```   
    cd Safety_Kit
    ./build.sh
```

## How to get the updates
The kit is configure with git to handle the updates. Any new updates with be in a new git branch (*_branch_name_*). It can be obtained by running the following commands in the Terminal. Replace the *_branch_name_* with the corresponding branch name.

    cd Safety_Kit
    git fetch origin _branch_name_
    git checkout _branch_name_


### Notes: 
For a new Jetson NX board, install curl and openssl development headers:

```   
  sudo apt install libcurl4-openssl-dev libssl-dev
```
