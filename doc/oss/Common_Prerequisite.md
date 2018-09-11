#Common Prerequisite Guidelines

These are the most common steps required for setting up a development host for the RhoMobile Suite.  

1. Download and install the latest version of **[Git](https://git-scm.com/download/win)** 

2. Download and install the latest **[JDK and JRE](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)** 

3. Download and install **[Ruby-1.9.3-p194](http://dl.bintray.com/oneclick/rubyinstaller/rubyinstaller-1.9.3-p194.exe)** version from [Ruby.org]( http://rubyinstaller.org/downloads/archives)

4. **Ruby Gem Installation**:
	* Open command prompt and type ```gem list```
	* Install any missing gems from the list below: 
		* At the command prompt, type **```gem install <gem_name>``` (i.e. ```rest_client```, ```rest-client```, ```listen```, etc.)** 
		* Set the Ruby environment variables prior to use of the ```gem``` command:
		- ```gem install rest_client``` 	<i>- Must be installed</i>
		- ```gem install listen --version 3.0.6```			<i>- Must be installed</i>
		- ```gem install templater``` 		<i>- Must be installed</i>
		- ```gem install uuid```			<i>- Must be installed</i>
		- ```gem install net-scp```			<i>- Must be installed</i>		
		- ```gem install CFPropertyList```	<i>- Install only if required and not present</i>
		- ```gem install rubyzip``` <i>- Install only if required and not present</i>
		- ```gem install rhomobile-debug``` <i>- Install only if required and not present</i>
		- ```gem install rhodes-translator``` <i>- Must be installed if ***rhodes_translator*** extension is added in ```build.yml``` file</i>
		- ```gem install rake``` 			<i>- Install only if required and not present, this is actually present by default</i>
		- ```gem install systemu```			<i>- Present by default; install only if required and not present</i>
		- ```gem install json``` 			<i>- Present by default; install only if required and not present</i>
		- ```gem install celluloid```		<i>- Present by default; install only if required and not present. Note: Internally, this installs **hitimes** and **timers** **gems** as well.</i>

5. **Environment variables** - Append the variables below to the System **`Path`** environment variable: 

* C:\project\rhodes\bin;
* C:\Ruby193\bin;
