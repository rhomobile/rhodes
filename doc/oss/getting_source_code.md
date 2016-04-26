#Step to Download Source Code From GITHUB

## Setting Up Rhodes Source Code

Rhodes source code is available on github.com, and you can use Git to get it. 

* For the command-line tool, [visit git-scm](http://git-scm.com) and download and install the latest. 

	<b>-- or --</b> 

* [Visit GitHub.com](https://desktop.github.com) and download GitHub Desktop for the GUI. 


1. After installing git, clone Rhodes to your local computer with the git clone command:
	
		:::term
		$ git clone git://github.com/rhomobile/rhodes.git
		$ cd rhodes

2. Then checkout the OSS branch (Note that the branch name is case-sensitive): 

		:::term
		$ git checkout OSS

3. To keep your branch up to date, use the git pull command:

		:::term	
		$ git pull

4. Add the bin folder of this clone to your path. On Linux/Mac it is typically done in the .profile file. On Windows, it is done using the Control Panel.

5. Configure your environment by running the rhodes-setup script. This will attempt to auto-detect installed SDKs. It will prompt you to verify the SDK paths or enter paths for SDKs that cannot be detected.

		:::term
		$ rhodes-setup

The rhodes-setup command also creates ```rhobuild.yml``` file where rhodes code checked out.

To do it manually: 

* go to rhodes folder
* rename rhobuild.yml.example to rhobuild.yml
* Add the required sdk paths as done earlier

## Setting Up Rhoconnect-Client Source Code

Rhoconnect-client source code is available on github.com, and you can use Git to get it. 

* For the command-line tool, [visit git-scm](http://git-scm.com) and download and install the latest. 

	<b>-- or --</b> 

* [Visit GitHub.com](https://desktop.github.com) and download GitHub Desktop for the GUI. 


To clone rhoconnect-client to your local computer, execute git clone from the command line:
	
		:::term
		$ git clone git://github.com/rhomobile/rhoconnect-client.git
	
Provide rhoconnect-client path in your build.yml: 

		:::term
	    paths:
	      extensions:
	      - /Users/home/GitHub/rhoconnect-client/ext
