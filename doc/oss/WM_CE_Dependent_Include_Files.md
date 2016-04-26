#WM/CE-Dependent Include Files

For successful operation, the files listed below must be copied either from **Windows Mobile 6 SDK** or **Windows Mobile 6.5.3 DTK** into the **```rhodes```** repository as indicated below. 

**Note**: Copying of the dependent header files is automated; there is no need to manually copy the files. When executing ```build_wmbinaries.bat```, an internal call is made to ```ms_include.bat```, which copies the dependent include files required in ```rhodes``` repository. 

During the process, the user will be prompted to verify the location of ```Include\Armv4i``` for **Windows Mobile 6 SDK** and **Windows Mobile 6.5.3 DTK**. The files will be copied only if the right path is set for ```Include\Armv4i``` of **Windows Mobile 6 SDK** and **Windows Mobile 6.5.3 DTK**.

1. 	**```gpsapi.h```** should be copied into the below location in ```rhodes``` repository.
   - ```rhodes\platform\wm\rhodes\stubs\``` - Copy ```gpsapi.h``` to this location from **Windows Mobile 6.5.3 DTK** (i.e. C:\Program Files (x86)\Windows Mobile 6.5.3 DTK\PocketPC\Include\Armv4i)
 
   - ```rhodes\neon\Helium\HeliumForWindows\Implementation\PBCore\PBCore\gpswrapper\``` - Copy ```gpsapi.h```  to this location from **Windows Mobile 6 SDK** (i.e. C:\Program Files (x86)\Windows Mobile 6 SDK\PocketPC\Include\Armv4i)
	
2.  **```getdeviceuniqueid.h```** should be copied into the location below in ```rhodes``` repository.
   - ```rhodes\platform\wm\rhodes\stubs\``` - Copy ```getdeviceuniqueid.h``` to this location from **Windows Mobile 6 SDK** (i.e. C:\Program Files (x86)\Windows Mobile 6 SDK\PocketPC\Include\Armv4i)

3. 	**```connmgr.h```** should be copied into the location below in ```rhodes``` repository.
   - ```rhodes\platform\wm\rhodes\stubs\``` - Copy ```connmgr.h``` to this location from **Windows Mobile 6 SDK** (i.e. C:\Program Files (x86)\Windows Mobile 6 SDK\PocketPC\Include\Armv4i)
	 
4. 	**```cfgmgrapi.h```** should be copied into the location below in ```rhodes``` repository.
   - ```rhodes\platform\wm\rhodes\stubs\``` - Copy ```cfgmgrapi.h```  to this location from **Windows Mobile 6 SDK** (i.e. C:\Program Files (x86)\Windows Mobile 6 SDK\PocketPC\Include\Armv4i)

5. 	**```bthutil.h```** should be copied into the location below in ```rhodes``` repository.
   - ```rhodes\platform\wm\rhodes\stubs\``` - Copy ```bthutil.h```  to this location from **Windows Mobile 6 SDK** (i.e. C:\Program Files (x86)\Windows Mobile 6 SDK\PocketPC\Include\Armv4i)

6. 	**```soundfile.h```** should be copied into the location below in ```rhodes``` repository.
   - ```rhodes\platform\wm\rhodes\stubs\``` - Copy ```soundfile.h```  to this location from **Windows Mobile 6.5.3 DTK** (i.e. C:\Program Files (x86)\Windows Mobile 6.5.3 DTK\PocketPC\Include\Armv4i)
		
7. 	**```snapi.h```** should be copied into the location below in ```rhodes``` repository.
   - ```rhodes\platform\wm\rhodes\stubs\``` - Copy ```snapi.h```  to this location from **Windows Mobile 6.5.3 DTK** (i.e. C:\Program Files (x86)\Windows Mobile 6.5.3 DTK\PocketPC\Include\Armv4i)

8. 	**```simmgr.h```** should be copied into the location below in ```rhodes``` repository.
   - ```rhodes\platform\wm\rhodes\stubs\``` - Copy ```simmgr.h```  to this location from **Windows Mobile 6.5.3 DTK** (i.e. C:\Program Files (x86)\Windows Mobile 6.5.3 DTK\PocketPC\Include\Armv4i)

9. 	**```regext.h```** should be copied into the location below in ```rhodes``` repository.
   - ```rhodes\platform\wm\rhodes\stubs\``` - Copy ```regext.h```  to this location from **Windows Mobile 6 SDK** (i.e. C:\Program Files (x86)\Windows Mobile 6 SDK\PocketPC\Include\Armv4i)
		
10. **```bthapi.h```** should be copied into the location below in ```rhodes``` repository.
   - ```rhodes\platform\wm\rhodes\stubs\``` - Copy ```bthapi.h```  to this location from **Windows Mobile 6.5.3 DTK** (i.e. C:\Program Files (x86)\Windows Mobile 6.5.3 DTK\PocketPC\Include\Armv4i)

11. **```htmlctrl.h```** should be copied into the location below in ```rhodes``` repository.
   - ```rhodes\platform\wm\rhodes\browser\``` - Copy ```htmlctrl.h```  to this location from **Windows Mobile 6 SDK**(i.e. C:\Program Files (x86)\Windows Mobile 6 SDK\PocketPC\Include\Armv4i)


