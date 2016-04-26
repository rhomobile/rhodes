
module AddressBar 
class << self #means static methods
		
	def left=(argument)
		RhoElementsExt.meta_proc('addressBar', 'left:' + argument.to_s, '')
	end
	
	def top=(argument)
		RhoElementsExt.meta_proc('addressBar', 'top:' + argument.to_s, '')
	end
	
	def width=(argument)
		RhoElementsExt.meta_proc('addressBar', 'width:' + argument.to_s, '')
	end
	
	def visibility=(argument)
		RhoElementsExt.meta_proc('addressBar', 'visibility:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('addressBar', argument.to_s, '')
	end
end
end
    
module Alarm 
class << self #means static methods
		
	def interval=(argument)
		RhoElementsExt.meta_proc('alarm', 'interval:' + argument.to_s, '')
	end
	
	def repeat=(argument)
		RhoElementsExt.meta_proc('alarm', 'repeat:' + argument.to_s, '')
	end
	
	def time=(argument)
		RhoElementsExt.meta_proc('alarm', 'time:' + argument.to_s, '')
	end
	
	def clear
		RhoElementsExt.meta_proc('alarm', 'clear', '')
	end
	
	def set
		RhoElementsExt.meta_proc('alarm', 'set', '')
	end
	
	def alarmTriggered=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('alarm', 'alarmTriggered:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('alarm', 'alarmTriggered:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('alarm', argument.to_s, '')
	end
end
end
    
module Application 
class << self #means static methods
		
	def quit
		RhoElementsExt.meta_proc('application', 'quit', '')
	end
	
	def minimize
		RhoElementsExt.meta_proc('application', 'minimize', '')
	end
	
	def restore
		RhoElementsExt.meta_proc('application', 'restore', '')
	end
	
	def getVersion
		RhoElementsExt.meta_proc('application', 'getVersion', '')
	end
	
	def applicationEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('application', 'applicationEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('application', 'applicationEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def versionEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('application', 'versionEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('application', 'versionEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('application', argument.to_s, '')
	end
end
end
    
module AppSwitcher 
class << self #means static methods
		
	def visibility=(argument)
		RhoElementsExt.meta_proc('appSwitcher', 'visibility:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('appSwitcher', argument.to_s, '')
	end
end
end
    
module AudioCapture 
class << self #means static methods
		
	def duration=(argument)
		RhoElementsExt.meta_proc('audioCapture', 'duration:' + argument.to_s, '')
	end
	
	def destination=(argument)
		RhoElementsExt.meta_proc('audioCapture', 'destination:' + argument.to_s, '')
	end
	
	def username=(argument)
		RhoElementsExt.meta_proc('audioCapture', 'username:' + argument.to_s, '')
	end
	
	def password=(argument)
		RhoElementsExt.meta_proc('audioCapture', 'password:' + argument.to_s, '')
	end
	
	def name=(argument)
		RhoElementsExt.meta_proc('audioCapture', 'name:' + argument.to_s, '')
	end
	
	def codec=(argument)
		RhoElementsExt.meta_proc('audioCapture', 'codec:' + argument.to_s, '')
	end
	
	def start
		RhoElementsExt.meta_proc('audioCapture', 'start', '')
	end
	
	def stop
		RhoElementsExt.meta_proc('audioCapture', 'stop', '')
	end
	
	def cancel
		RhoElementsExt.meta_proc('audioCapture', 'cancel', '')
	end
	
	def audioSaveEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('audioCapture', 'audioSaveEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('audioCapture', 'audioSaveEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('audioCapture', argument.to_s, '')
	end
end
end
    
module BackButton 
class << self #means static methods
		
	def left=(argument)
		RhoElementsExt.meta_proc('backButton', 'left:' + argument.to_s, '')
	end
	
	def top=(argument)
		RhoElementsExt.meta_proc('backButton', 'top:' + argument.to_s, '')
	end
	
	def height=(argument)
		RhoElementsExt.meta_proc('backButton', 'height:' + argument.to_s, '')
	end
	
	def width=(argument)
		RhoElementsExt.meta_proc('backButton', 'width:' + argument.to_s, '')
	end
	
	def imageUp=(argument)
		RhoElementsExt.meta_proc('backButton', 'imageUp:' + argument.to_s, '')
	end
	
	def imageDown=(argument)
		RhoElementsExt.meta_proc('backButton', 'imageDown:' + argument.to_s, '')
	end
	
	def visibility=(argument)
		RhoElementsExt.meta_proc('backButton', 'visibility:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('backButton', argument.to_s, '')
	end
end
end
    
module Backlight 
class << self #means static methods
		
	def intensity=(argument)
		RhoElementsExt.meta_proc('backlight', 'intensity:' + argument.to_s, '')
	end
	
	def on
		RhoElementsExt.meta_proc('backlight', 'on', '')
	end
	
	def off
		RhoElementsExt.meta_proc('backlight', 'off', '')
	end
	
	def getBacklightSettings
		RhoElementsExt.meta_proc('backlight', 'getBacklightSettings', '')
	end
	
	def backlightSettingsEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('backlight', 'backlightSettingsEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('backlight', 'backlightSettingsEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('backlight', argument.to_s, '')
	end
end
end
    
module Battery 
class << self #means static methods
		
	def visibility=(argument)
		RhoElementsExt.meta_proc('battery', 'visibility:' + argument.to_s, '')
	end
	
	def left=(argument)
		RhoElementsExt.meta_proc('battery', 'left:' + argument.to_s, '')
	end
	
	def top=(argument)
		RhoElementsExt.meta_proc('battery', 'top:' + argument.to_s, '')
	end
	
	def layout=(argument)
		RhoElementsExt.meta_proc('battery', 'layout:' + argument.to_s, '')
	end
	
	def color=(argument)
		RhoElementsExt.meta_proc('battery', 'color:' + argument.to_s, '')
	end
	
	def sampleInterval=(argument)
		RhoElementsExt.meta_proc('battery', 'sampleInterval:' + argument.to_s, '')
	end
	
	def setSystemNotifications=(argument)
		RhoElementsExt.meta_proc('battery', 'setSystemNotifications:' + argument.to_s, '')
	end
	
	def getSmartBatteryStatus
		RhoElementsExt.meta_proc('battery', 'getSmartBatteryStatus', '')
	end
	
	def getBatteryStatus
		RhoElementsExt.meta_proc('battery', 'getBatteryStatus', '')
	end
	
	def batteryEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('battery', 'batteryEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('battery', 'batteryEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def smartBatteryEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('battery', 'smartBatteryEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('battery', 'smartBatteryEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('battery', argument.to_s, '')
	end
end
end
    
module BottomCommandArea 
class << self #means static methods
		
	def visibility=(argument)
		RhoElementsExt.meta_proc('bottomCommandArea', 'visibility:' + argument.to_s, '')
	end
	
	def height=(argument)
		RhoElementsExt.meta_proc('bottomCommandArea', 'height:' + argument.to_s, '')
	end
	
	def color=(argument)
		RhoElementsExt.meta_proc('bottomCommandArea', 'color:' + argument.to_s, '')
	end
	
	def image=(argument)
		RhoElementsExt.meta_proc('bottomCommandArea', 'image:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('bottomCommandArea', argument.to_s, '')
	end
end
end    
 
module Comm 
class << self #means static methods
		
	def baudRate=(argument)
		RhoElementsExt.meta_proc('comm', 'baudRate:' + argument.to_s, '')
	end
	
	def dataBits=(argument)
		RhoElementsExt.meta_proc('comm', 'dataBits:' + argument.to_s, '')
	end
	
	def stopBits=(argument)
		RhoElementsExt.meta_proc('comm', 'stopBits:' + argument.to_s, '')
	end
	
	def parity=(argument)
		RhoElementsExt.meta_proc('comm', 'parity:' + argument.to_s, '')
	end
	
	def handshake=(argument)
		RhoElementsExt.meta_proc('comm', 'handshake:' + argument.to_s, '')
	end
	
	def port=(argument)
		RhoElementsExt.meta_proc('comm', 'port:' + argument.to_s, '')
	end
	
	def chars=(argument)
		RhoElementsExt.meta_proc('comm', 'chars:' + argument.to_s, '')
	end
	
	def endChar=(argument)
		RhoElementsExt.meta_proc('comm', 'endChar:' + argument.to_s, '')
	end
	
	def time=(argument)
		RhoElementsExt.meta_proc('comm', 'time:' + argument.to_s, '')
	end
	
	def writeBytes=(argument)
		RhoElementsExt.meta_proc('comm', 'writeBytes:' + argument.to_s, '')
	end
	
	def writeString=(argument)
		RhoElementsExt.meta_proc('comm', 'writeString:' + argument.to_s, '')
	end
	
	def writeFile=(argument)
		RhoElementsExt.meta_proc('comm', 'writeFile:' + argument.to_s, '')
	end
	
	def autoEnter=(argument)
		RhoElementsExt.meta_proc('comm', 'autoEnter:' + argument.to_s, '')
	end
	
	def autoTab=(argument)
		RhoElementsExt.meta_proc('comm', 'autoTab:' + argument.to_s, '')
	end
	
	def open
		RhoElementsExt.meta_proc('comm', 'open', '')
	end
	
	def close
		RhoElementsExt.meta_proc('comm', 'close', '')
	end
	
	def commEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('comm', 'commEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('comm', 'commEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('comm', argument.to_s, '')
	end
end
end
    
module Cradle 
class << self #means static methods
		
	def getCradleId
		RhoElementsExt.meta_proc('cradle', 'getCradleId', '')
	end
	
	def cradleEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('cradle', 'cradleEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('cradle', 'cradleEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('cradle', argument.to_s, '')
	end
end
end
    
module Device 
class << self #means static methods
		
	def suspend
		RhoElementsExt.meta_proc('device', 'suspend', '')
	end
	
	def powerOff
		RhoElementsExt.meta_proc('device', 'powerOff', '')
	end
	
	def wake
		RhoElementsExt.meta_proc('device', 'wake', '')
	end
	
	def calibrate
		RhoElementsExt.meta_proc('device', 'calibrate', '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('device', argument.to_s, '')
	end
end
end
    
module Eink 
class << self #means static methods
		
	def screenUpdateBpp=(argument)
		RhoElementsExt.meta_proc('eink', 'screenUpdateBpp:' + argument.to_s, '')
	end
	
	def forceFullScreenUpdate
		RhoElementsExt.meta_proc('eink', 'forceFullScreenUpdate', '')
	end
	
	def pauseFullScreenUpdate
		RhoElementsExt.meta_proc('eink', 'pauseFullScreenUpdate', '')
	end
	
	def resumeFullScreenUpdate
		RhoElementsExt.meta_proc('eink', 'resumeFullScreenUpdate', '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('eink', argument.to_s, '')
	end
end
end
    
module EmmlProfile 
class << self #means static methods
		
	def name=(argument)
		RhoElementsExt.meta_proc('emmlProfile', 'name:' + argument.to_s, '')
	end
	
	def importProfile=(argument)
		RhoElementsExt.meta_proc('emmlProfile', 'importProfile:' + argument.to_s, '')
	end
	
	def clear=(argument)
		RhoElementsExt.meta_proc('emmlProfile', 'clear:' + argument.to_s, '')
	end
	
	def apply=(argument)
		RhoElementsExt.meta_proc('emmlProfile', 'apply:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('emmlProfile', argument.to_s, '')
	end
end
end
    
module FileTransfer 
class << self #means static methods
		
	def destination=(argument)
		RhoElementsExt.meta_proc('fileTransfer', 'destination:' + argument.to_s, '')
	end
	
	def source=(argument)
		RhoElementsExt.meta_proc('fileTransfer', 'source:' + argument.to_s, '')
	end
	
	def username=(argument)
		RhoElementsExt.meta_proc('fileTransfer', 'username:' + argument.to_s, '')
	end
	
	def password=(argument)
		RhoElementsExt.meta_proc('fileTransfer', 'password:' + argument.to_s, '')
	end
	
	def createFolders=(argument)
		RhoElementsExt.meta_proc('fileTransfer', 'createFolders:' + argument.to_s, '')
	end
	
	def overWrite=(argument)
		RhoElementsExt.meta_proc('fileTransfer', 'overWrite:' + argument.to_s, '')
	end
    
    def port=(argument)
        RhoElementsExt.meta_proc('fileTransfer', 'port:' + argument.to_s, '')
    end
	
	def transfer
		RhoElementsExt.meta_proc('fileTransfer', 'transfer', '')
	end
	
	def transferEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('fileTransfer', 'transferEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('fileTransfer', 'transferEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('fileTransfer', argument.to_s, '')
	end
end
end
    
module ForwardButton 
class << self #means static methods
		
	def left=(argument)
		RhoElementsExt.meta_proc('forwardButton', 'left:' + argument.to_s, '')
	end
	
	def top=(argument)
		RhoElementsExt.meta_proc('forwardButton', 'top:' + argument.to_s, '')
	end
	
	def height=(argument)
		RhoElementsExt.meta_proc('forwardButton', 'height:' + argument.to_s, '')
	end
	
	def width=(argument)
		RhoElementsExt.meta_proc('forwardButton', 'width:' + argument.to_s, '')
	end
	
	def imageUp=(argument)
		RhoElementsExt.meta_proc('forwardButton', 'imageUp:' + argument.to_s, '')
	end
	
	def imageDown=(argument)
		RhoElementsExt.meta_proc('forwardButton', 'imageDown:' + argument.to_s, '')
	end
	
	def visibility=(argument)
		RhoElementsExt.meta_proc('forwardButton', 'visibility:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('forwardButton', argument.to_s, '')
	end
end
end
    
module Gesture 
class << self #means static methods
		
	def type=(argument)
		RhoElementsExt.meta_proc('gesture', 'type:' + argument.to_s, '')
	end
	
	def id=(argument)
		RhoElementsExt.meta_proc('gesture', 'id:' + argument.to_s, '')
	end
	
	def preset=(argument)
		RhoElementsExt.meta_proc('gesture', 'preset:' + argument.to_s, '')
	end
	
	def diagnostics=(argument)
		RhoElementsExt.meta_proc('gesture', 'diagnostics:' + argument.to_s, '')
	end
	
	def startX=(argument)
		RhoElementsExt.meta_proc('gesture', 'startX:' + argument.to_s, '')
	end
	
	def startY=(argument)
		RhoElementsExt.meta_proc('gesture', 'startY:' + argument.to_s, '')
	end
	
	def endX=(argument)
		RhoElementsExt.meta_proc('gesture', 'endX:' + argument.to_s, '')
	end
	
	def endY=(argument)
		RhoElementsExt.meta_proc('gesture', 'endY:' + argument.to_s, '')
	end
	
	def skew=(argument)
		RhoElementsExt.meta_proc('gesture', 'skew:' + argument.to_s, '')
	end
	
	def deviation=(argument)
		RhoElementsExt.meta_proc('gesture', 'deviation:' + argument.to_s, '')
	end
	
	def regionWidth=(argument)
		RhoElementsExt.meta_proc('gesture', 'regionWidth:' + argument.to_s, '')
	end
	
	def centerX=(argument)
		RhoElementsExt.meta_proc('gesture', 'centerX:' + argument.to_s, '')
	end
	
	def centerY=(argument)
		RhoElementsExt.meta_proc('gesture', 'centerY:' + argument.to_s, '')
	end
	
	def radius=(argument)
		RhoElementsExt.meta_proc('gesture', 'radius:' + argument.to_s, '')
	end
	
	def start=(argument)
		RhoElementsExt.meta_proc('gesture', 'start:' + argument.to_s, '')
	end
	
	def end=(argument)
		RhoElementsExt.meta_proc('gesture', 'end:' + argument.to_s, '')
	end
	
	def tolerance=(argument)
		RhoElementsExt.meta_proc('gesture', 'tolerance:' + argument.to_s, '')
	end
	
	def sensitivity=(argument)
		RhoElementsExt.meta_proc('gesture', 'sensitivity:' + argument.to_s, '')
	end
	
	def delay=(argument)
		RhoElementsExt.meta_proc('gesture', 'delay:' + argument.to_s, '')
	end
	
	def interval=(argument)
		RhoElementsExt.meta_proc('gesture', 'interval:' + argument.to_s, '')
	end
	
	def TargetX=(argument)
		RhoElementsExt.meta_proc('gesture', 'TargetX:' + argument.to_s, '')
	end
	
	def TargetY=(argument)
		RhoElementsExt.meta_proc('gesture', 'TargetY:' + argument.to_s, '')
	end
	
	def TargetZ=(argument)
		RhoElementsExt.meta_proc('gesture', 'TargetZ:' + argument.to_s, '')
	end
	
	def TiltTolerance=(argument)
		RhoElementsExt.meta_proc('gesture', 'TiltTolerance:' + argument.to_s, '')
	end
	
	def Hysteresis=(argument)
		RhoElementsExt.meta_proc('gesture', 'Hysteresis:' + argument.to_s, '')
	end
	
	def Threshold=(argument)
		RhoElementsExt.meta_proc('gesture', 'Threshold:' + argument.to_s, '')
	end
	
	def Quiet=(argument)
		RhoElementsExt.meta_proc('gesture', 'Quiet:' + argument.to_s, '')
	end
	
	def create
		RhoElementsExt.meta_proc('gesture', 'create', '')
	end
	
	def delete
		RhoElementsExt.meta_proc('gesture', 'delete', '')
	end
	
	def detected=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('gesture', 'detected:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('gesture', 'detected:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('gesture', argument.to_s, '')
	end
end
end
    
module GoButton 
class << self #means static methods
		
	def left=(argument)
		RhoElementsExt.meta_proc('goButton', 'left:' + argument.to_s, '')
	end
	
	def top=(argument)
		RhoElementsExt.meta_proc('goButton', 'top:' + argument.to_s, '')
	end
	
	def height=(argument)
		RhoElementsExt.meta_proc('goButton', 'height:' + argument.to_s, '')
	end
	
	def width=(argument)
		RhoElementsExt.meta_proc('goButton', 'width:' + argument.to_s, '')
	end
	
	def imageUp=(argument)
		RhoElementsExt.meta_proc('goButton', 'imageUp:' + argument.to_s, '')
	end
	
	def imageDown=(argument)
		RhoElementsExt.meta_proc('goButton', 'imageDown:' + argument.to_s, '')
	end
	
	def visibility=(argument)
		RhoElementsExt.meta_proc('goButton', 'visibility:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('goButton', argument.to_s, '')
	end
end
end
    
module History:Back 
class << self #means static methods
		
	def setEmml(argument)
		RhoElementsExt.meta_proc('History:Back', argument.to_s, '')
	end
end
end
    
module HomeButton 
class << self #means static methods
		
	def left=(argument)
		RhoElementsExt.meta_proc('homeButton', 'left:' + argument.to_s, '')
	end
	
	def top=(argument)
		RhoElementsExt.meta_proc('homeButton', 'top:' + argument.to_s, '')
	end
	
	def height=(argument)
		RhoElementsExt.meta_proc('homeButton', 'height:' + argument.to_s, '')
	end
	
	def width=(argument)
		RhoElementsExt.meta_proc('homeButton', 'width:' + argument.to_s, '')
	end
	
	def imageUp=(argument)
		RhoElementsExt.meta_proc('homeButton', 'imageUp:' + argument.to_s, '')
	end
	
	def imageDown=(argument)
		RhoElementsExt.meta_proc('homeButton', 'imageDown:' + argument.to_s, '')
	end
	
	def visibility=(argument)
		RhoElementsExt.meta_proc('homeButton', 'visibility:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('homeButton', argument.to_s, '')
	end
end
end
    
module Hourglass 
class << self #means static methods
		
	def visibility=(argument)
		RhoElementsExt.meta_proc('hourglass', 'visibility:' + argument.to_s, '')
	end
	
	def left=(argument)
		RhoElementsExt.meta_proc('hourglass', 'left:' + argument.to_s, '')
	end
	
	def top=(argument)
		RhoElementsExt.meta_proc('hourglass', 'top:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('hourglass', argument.to_s, '')
	end
end
end
    
module Imager 
class << self #means static methods
		
	def enabled=(argument)
		RhoElementsExt.meta_proc('imager', 'enabled:' + argument.to_s, '')
	end
	
	def desiredWidth=(argument)
		RhoElementsExt.meta_proc('imager', 'desiredWidth:' + argument.to_s, '')
	end
	
	def desiredHeight=(argument)
		RhoElementsExt.meta_proc('imager', 'desiredHeight:' + argument.to_s, '')
	end
	
	def left=(argument)
		RhoElementsExt.meta_proc('imager', 'left:' + argument.to_s, '')
	end
	
	def top=(argument)
		RhoElementsExt.meta_proc('imager', 'top:' + argument.to_s, '')
	end
	
	def width=(argument)
		RhoElementsExt.meta_proc('imager', 'width:' + argument.to_s, '')
	end
	
	def height=(argument)
		RhoElementsExt.meta_proc('imager', 'height:' + argument.to_s, '')
	end
	
	def lamp=(argument)
		RhoElementsExt.meta_proc('imager', 'lamp:' + argument.to_s, '')
	end
	
	def aim=(argument)
		RhoElementsExt.meta_proc('imager', 'aim:' + argument.to_s, '')
	end
	
	def username=(argument)
		RhoElementsExt.meta_proc('imager', 'username:' + argument.to_s, '')
	end
	
	def password=(argument)
		RhoElementsExt.meta_proc('imager', 'password:' + argument.to_s, '')
	end
	
	def sound=(argument)
		RhoElementsExt.meta_proc('imager', 'sound:' + argument.to_s, '')
	end
	
	def destination=(argument)
		RhoElementsExt.meta_proc('imager', 'destination:' + argument.to_s, '')
	end
	
	def deviceId=(argument)
		RhoElementsExt.meta_proc('imager', 'deviceId:' + argument.to_s, '')
	end
	
	def enable
		RhoElementsExt.meta_proc('imager', 'enable', '')
	end
	
	def disable
		RhoElementsExt.meta_proc('imager', 'disable', '')
	end
	
	def capture
		RhoElementsExt.meta_proc('imager', 'capture', '')
	end
	
	def enumerate
		RhoElementsExt.meta_proc('imager', 'enumerate', '')
	end
	
	def imagerEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('imager', 'imagerEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('imager', 'imagerEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def imagerCaptureEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('imager', 'imagerCaptureEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('imager', 'imagerCaptureEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def imagerEnumEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('imager', 'imagerEnumEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('imager', 'imagerEnumEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('imager', argument.to_s, '')
	end
end
end
    
module IoAnalog 
class << self #means static methods
		
	def generalVoltage1=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'generalVoltage1:' + argument.to_s, '')
	end
	
	def generalVoltage1MaximumThreshold=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'generalVoltage1MaximumThreshold:' + argument.to_s, '')
	end
	
	def generalVoltage1MinimumThreshold=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'generalVoltage1MinimumThreshold:' + argument.to_s, '')
	end
	
	def generalVoltage1Debounce=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'generalVoltage1Debounce:' + argument.to_s, '')
	end
	
	def generalVoltage2=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'generalVoltage2:' + argument.to_s, '')
	end
	
	def generalVoltage2MaximumThreshold=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'generalVoltage2MaximumThreshold:' + argument.to_s, '')
	end
	
	def generalVoltage2MinimumThreshold=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'generalVoltage2MinimumThreshold:' + argument.to_s, '')
	end
	
	def generalVoltage2Debounce=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'generalVoltage2Debounce:' + argument.to_s, '')
	end
	
	def BatteryVoltage=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'BatteryVoltage:' + argument.to_s, '')
	end
	
	def BatteryVoltageMaximumThreshold=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'BatteryVoltageMaximumThreshold:' + argument.to_s, '')
	end
	
	def BatteryVoltageMinimumThreshold=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'BatteryVoltageMinimumThreshold:' + argument.to_s, '')
	end
	
	def BatteryVoltageDebounce=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'BatteryVoltageDebounce:' + argument.to_s, '')
	end
	
	def BatteryCurrent=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'BatteryCurrent:' + argument.to_s, '')
	end
	
	def BatteryCurrentMaximumThreshold=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'BatteryCurrentMaximumThreshold:' + argument.to_s, '')
	end
	
	def BatteryCurrentMinimumThreshold=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'BatteryCurrentMinimumThreshold:' + argument.to_s, '')
	end
	
	def BatteryCurrentDebounce=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'BatteryCurrentDebounce:' + argument.to_s, '')
	end
	
	def AmbientTemperature=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'AmbientTemperature:' + argument.to_s, '')
	end
	
	def AmbientTemperatureMaximumThreshold=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'AmbientTemperatureMaximumThreshold:' + argument.to_s, '')
	end
	
	def AmbientTemperatureMinimumThreshold=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'AmbientTemperatureMinimumThreshold:' + argument.to_s, '')
	end
	
	def AmbientTemperatureDebounce=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'AmbientTemperatureDebounce:' + argument.to_s, '')
	end
	
	def HeaterTemperature=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'HeaterTemperature:' + argument.to_s, '')
	end
	
	def HeaterTemperatureMaximumThreshold=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'HeaterTemperatureMaximumThreshold:' + argument.to_s, '')
	end
	
	def HeaterTemperatureMinimumThreshold=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'HeaterTemperatureMinimumThreshold:' + argument.to_s, '')
	end
	
	def HeaterTemperatureDebounce=(argument)
		RhoElementsExt.meta_proc('ioAnalog', 'HeaterTemperatureDebounce:' + argument.to_s, '')
	end
	
	def invokeGeneralVoltage1
		RhoElementsExt.meta_proc('ioAnalog', 'invokeGeneralVoltage1', '')
	end
	
	def invokeGeneralVoltage2
		RhoElementsExt.meta_proc('ioAnalog', 'invokeGeneralVoltage2', '')
	end
	
	def invokeBatteryVoltage
		RhoElementsExt.meta_proc('ioAnalog', 'invokeBatteryVoltage', '')
	end
	
	def invokeBatteryCurrent
		RhoElementsExt.meta_proc('ioAnalog', 'invokeBatteryCurrent', '')
	end
	
	def invokeAmbientTemperature
		RhoElementsExt.meta_proc('ioAnalog', 'invokeAmbientTemperature', '')
	end
	
	def invokeHeaterTemperature
		RhoElementsExt.meta_proc('ioAnalog', 'invokeHeaterTemperature', '')
	end
	
	def portEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('ioAnalog', 'portEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('ioAnalog', 'portEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('ioAnalog', argument.to_s, '')
	end
end
end
    
module IoDigital 
class << self #means static methods
		
	def input1=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'input1:' + argument.to_s, '')
	end
	
	def outputState1=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputState1:' + argument.to_s, '')
	end
	
	def outputWarmBootState1=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputWarmBootState1:' + argument.to_s, '')
	end
	
	def outputColdBootState1=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputColdBootState1:' + argument.to_s, '')
	end
	
	def input2=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'input2:' + argument.to_s, '')
	end
	
	def outputState2=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputState2:' + argument.to_s, '')
	end
	
	def outputWarmBootState2=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputWarmBootState2:' + argument.to_s, '')
	end
	
	def outputColdBootState2=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputColdBootState2:' + argument.to_s, '')
	end
	
	def input3=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'input3:' + argument.to_s, '')
	end
	
	def outputState3=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputState3:' + argument.to_s, '')
	end
	
	def outputWarmBootState3=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputWarmBootState3:' + argument.to_s, '')
	end
	
	def outputColdBootState3=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputColdBootState3:' + argument.to_s, '')
	end
	
	def input4=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'input4:' + argument.to_s, '')
	end
	
	def outputState4=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputState4:' + argument.to_s, '')
	end
	
	def outputWarmBootState4=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputWarmBootState4:' + argument.to_s, '')
	end
	
	def outputColdBootState4=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputColdBootState4:' + argument.to_s, '')
	end
	
	def input5=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'input5:' + argument.to_s, '')
	end
	
	def outputState5=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputState5:' + argument.to_s, '')
	end
	
	def outputWarmBootState5=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputWarmBootState5:' + argument.to_s, '')
	end
	
	def outputColdBootState5=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputColdBootState5:' + argument.to_s, '')
	end
	
	def input6=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'input6:' + argument.to_s, '')
	end
	
	def outputState6=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputState6:' + argument.to_s, '')
	end
	
	def outputWarmBootState6=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputWarmBootState6:' + argument.to_s, '')
	end
	
	def outputColdBootState6=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputColdBootState6:' + argument.to_s, '')
	end
	
	def input7=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'input7:' + argument.to_s, '')
	end
	
	def outputState7=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputState7:' + argument.to_s, '')
	end
	
	def outputWarmBootState7=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputWarmBootState7:' + argument.to_s, '')
	end
	
	def outputColdBootState7=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputColdBootState7:' + argument.to_s, '')
	end
	
	def input8=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'input8:' + argument.to_s, '')
	end
	
	def outputState8=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputState8:' + argument.to_s, '')
	end
	
	def outputWarmBootState8=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputWarmBootState8:' + argument.to_s, '')
	end
	
	def outputColdBootState8=(argument)
		RhoElementsExt.meta_proc('ioDigital', 'outputColdBootState8:' + argument.to_s, '')
	end
	
	def InvokeDigital1
		RhoElementsExt.meta_proc('ioDigital', 'InvokeDigital1', '')
	end
	
	def InvokeDigital2
		RhoElementsExt.meta_proc('ioDigital', 'InvokeDigital2', '')
	end
	
	def InvokeDigital3
		RhoElementsExt.meta_proc('ioDigital', 'InvokeDigital3', '')
	end
	
	def InvokeDigital4
		RhoElementsExt.meta_proc('ioDigital', 'InvokeDigital4', '')
	end
	
	def InvokeDigital5
		RhoElementsExt.meta_proc('ioDigital', 'InvokeDigital5', '')
	end
	
	def InvokeDigital6
		RhoElementsExt.meta_proc('ioDigital', 'InvokeDigital6', '')
	end
	
	def InvokeDigital7
		RhoElementsExt.meta_proc('ioDigital', 'InvokeDigital7', '')
	end
	
	def InvokeDigital8
		RhoElementsExt.meta_proc('ioDigital', 'InvokeDigital8', '')
	end
	
	def PortEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('ioDigital', 'PortEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('ioDigital', 'PortEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('ioDigital', argument.to_s, '')
	end
end
end
    
module IoSystem 
class << self #means static methods
		
	def ignition=(argument)
		RhoElementsExt.meta_proc('ioSystem', 'ignition:' + argument.to_s, '')
	end
	
	def powerSource=(argument)
		RhoElementsExt.meta_proc('ioSystem', 'powerSource:' + argument.to_s, '')
	end
	
	def invokeIgnition
		RhoElementsExt.meta_proc('ioSystem', 'invokeIgnition', '')
	end
	
	def invokePowerSource
		RhoElementsExt.meta_proc('ioSystem', 'invokePowerSource', '')
	end
	
	def portEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('ioSystem', 'portEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('ioSystem', 'portEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('ioSystem', argument.to_s, '')
	end
end
end
    
module KeyCapture 
class << self #means static methods
		
	def dispatch=(argument)
		RhoElementsExt.meta_proc('keyCapture', 'dispatch:' + argument.to_s, '')
	end
	
	def keyValue=(argument)
		RhoElementsExt.meta_proc('keyCapture', 'keyValue:' + argument.to_s, '')
	end
	
	def remap=(argument)
		RhoElementsExt.meta_proc('keyCapture', 'remap:' + argument.to_s, '')
	end
	
	def accelerateKey=(argument)
		RhoElementsExt.meta_proc('keyCapture', 'accelerateKey:' + argument.to_s, '')
	end
	
	def homeKeyValue=(argument)
		RhoElementsExt.meta_proc('keyCapture', 'homeKeyValue:' + argument.to_s, '')
	end
	
	def keyEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('keyCapture', 'keyEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('keyCapture', 'keyEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def triggerEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('keyCapture', 'triggerEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('keyCapture', 'triggerEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('keyCapture', argument.to_s, '')
	end
end
end
    
module Keylight 
class << self #means static methods
		
	def intensity=(argument)
		RhoElementsExt.meta_proc('keylight', 'intensity:' + argument.to_s, '')
	end
	
	def on
		RhoElementsExt.meta_proc('keylight', 'on', '')
	end
	
	def off
		RhoElementsExt.meta_proc('keylight', 'off', '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('keylight', argument.to_s, '')
	end
end
end
    
module KeyState 
class << self #means static methods
		
	def right=(argument)
		RhoElementsExt.meta_proc('keyState', 'right:' + argument.to_s, '')
	end
	
	def left=(argument)
		RhoElementsExt.meta_proc('keyState', 'left:' + argument.to_s, '')
	end
	
	def top=(argument)
		RhoElementsExt.meta_proc('keyState', 'top:' + argument.to_s, '')
	end
	
	def height=(argument)
		RhoElementsExt.meta_proc('keyState', 'height:' + argument.to_s, '')
	end
	
	def width=(argument)
		RhoElementsExt.meta_proc('keyState', 'width:' + argument.to_s, '')
	end
	
	def visibility=(argument)
		RhoElementsExt.meta_proc('keyState', 'visibility:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('keyState', argument.to_s, '')
	end
end
end
    
module Lightsensor 
class << self #means static methods
		
	def status=(argument)
		RhoElementsExt.meta_proc('lightsensor', 'status:' + argument.to_s, '')
	end
	
	def minimumInterval=(argument)
		RhoElementsExt.meta_proc('lightsensor', 'minimumInterval:' + argument.to_s, '')
	end
	
	def getSensorData
		RhoElementsExt.meta_proc('lightsensor', 'getSensorData', '')
	end
	
	def LightSensorEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('lightsensor', 'LightSensorEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('lightsensor', 'LightSensorEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('lightsensor', argument.to_s, '')
	end
end
end
    
module MediaPlayer 
class << self #means static methods
		
	def filename=(argument)
		RhoElementsExt.meta_proc('mediaPlayer', 'filename:' + argument.to_s, '')
	end
	
	def type=(argument)
		RhoElementsExt.meta_proc('mediaPlayer', 'type:' + argument.to_s, '')
	end
	
	def start
		RhoElementsExt.meta_proc('mediaPlayer', 'start', '')
	end
	
	def stop
		RhoElementsExt.meta_proc('mediaPlayer', 'stop', '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('mediaPlayer', argument.to_s, '')
	end
end
end
    
module Memory 
class << self #means static methods
		
	def lowMemThreshold=(argument)
		RhoElementsExt.meta_proc('memory', 'lowMemThreshold:' + argument.to_s, '')
	end
	
	def getMemoryStats
		RhoElementsExt.meta_proc('memory', 'getMemoryStats', '')
	end
	
	def memoryEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('memory', 'memoryEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('memory', 'memoryEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('memory', argument.to_s, '')
	end
end
end
    
module MinimizeButton 
class << self #means static methods
		
	def left=(argument)
		RhoElementsExt.meta_proc('minimizeButton', 'left:' + argument.to_s, '')
	end
	
	def top=(argument)
		RhoElementsExt.meta_proc('minimizeButton', 'top:' + argument.to_s, '')
	end
	
	def height=(argument)
		RhoElementsExt.meta_proc('minimizeButton', 'height:' + argument.to_s, '')
	end
	
	def width=(argument)
		RhoElementsExt.meta_proc('minimizeButton', 'width:' + argument.to_s, '')
	end
	
	def imageUp=(argument)
		RhoElementsExt.meta_proc('minimizeButton', 'imageUp:' + argument.to_s, '')
	end
	
	def imageDown=(argument)
		RhoElementsExt.meta_proc('minimizeButton', 'imageDown:' + argument.to_s, '')
	end
	
	def visibility=(argument)
		RhoElementsExt.meta_proc('minimizeButton', 'visibility:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('minimizeButton', argument.to_s, '')
	end
end
end
    
module Network 
class << self #means static methods
		
	def host=(argument)
		RhoElementsExt.meta_proc('network', 'host:' + argument.to_s, '')
	end
	
	def port=(argument)
		RhoElementsExt.meta_proc('network', 'port:' + argument.to_s, '')
	end
	
	def networkPollInterval=(argument)
		RhoElementsExt.meta_proc('network', 'networkPollInterval:' + argument.to_s, '')
	end
	
	def connectionTimeout=(argument)
		RhoElementsExt.meta_proc('network', 'connectionTimeout:' + argument.to_s, '')
	end
	
	def connectWan=(argument)
		RhoElementsExt.meta_proc('network', 'connectWan:' + argument.to_s, '')
	end
	
	def start
		RhoElementsExt.meta_proc('network', 'start', '')
	end
	
	def stop
		RhoElementsExt.meta_proc('network', 'stop', '')
	end
	
	def disconnectWan
		RhoElementsExt.meta_proc('network', 'disconnectWan', '')
	end
	
	def networkEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('network', 'networkEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('network', 'networkEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def wanStatusEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('network', 'wanStatusEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('network', 'wanStatusEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('network', argument.to_s, '')
	end
end
end
    
module Notification 
class << self #means static methods
		
	def setLEDOnDuration=(argument)
		RhoElementsExt.meta_proc('notification', 'setLEDOnDuration:' + argument.to_s, '')
	end
	
	def setLEDOffDuration=(argument)
		RhoElementsExt.meta_proc('notification', 'setLEDOffDuration:' + argument.to_s, '')
	end
	
	def setLEDNumberOfCycles=(argument)
		RhoElementsExt.meta_proc('notification', 'setLEDNumberOfCycles:' + argument.to_s, '')
	end
	
	def setBeeperFrequency=(argument)
		RhoElementsExt.meta_proc('notification', 'setBeeperFrequency:' + argument.to_s, '')
	end
	
	def setBeeperVolume=(argument)
		RhoElementsExt.meta_proc('notification', 'setBeeperVolume:' + argument.to_s, '')
	end
	
	def setBeeperDuration=(argument)
		RhoElementsExt.meta_proc('notification', 'setBeeperDuration:' + argument.to_s, '')
	end
	
	def setVibrateDuration=(argument)
		RhoElementsExt.meta_proc('notification', 'setVibrateDuration:' + argument.to_s, '')
	end
	
	def stateOn=(argument)
		RhoElementsExt.meta_proc('notification', 'stateOn:' + argument.to_s, '')
	end
	
	def stateOff=(argument)
		RhoElementsExt.meta_proc('notification', 'stateOff:' + argument.to_s, '')
	end
	
	def stateCycle=(argument)
		RhoElementsExt.meta_proc('notification', 'stateCycle:' + argument.to_s, '')
	end
	
	def enumerate
		RhoElementsExt.meta_proc('notification', 'enumerate', '')
	end
	
	def enumNotificationsEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('notification', 'enumNotificationsEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('notification', 'enumNotificationsEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('notification', argument.to_s, '')
	end
end
end
    
module ZoomInPageButton 
class << self #means static methods
		
	def left=(argument)
		RhoElementsExt.meta_proc('zoomInPageButton', 'left:' + argument.to_s, '')
	end
	
	def top=(argument)
		RhoElementsExt.meta_proc('zoomInPageButton', 'top:' + argument.to_s, '')
	end
	
	def height=(argument)
		RhoElementsExt.meta_proc('zoomInPageButton', 'height:' + argument.to_s, '')
	end
	
	def width=(argument)
		RhoElementsExt.meta_proc('zoomInPageButton', 'width:' + argument.to_s, '')
	end
	
	def imageup=(argument)
		RhoElementsExt.meta_proc('zoomInPageButton', 'imageup:' + argument.to_s, '')
	end
	
	def imagedown=(argument)
		RhoElementsExt.meta_proc('zoomInPageButton', 'imagedown:' + argument.to_s, '')
	end
	
	def visibility=(argument)
		RhoElementsExt.meta_proc('zoomInPageButton', 'visibility:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('zoomInPageButton', argument.to_s, '')
	end
end
end
    
module ZoomOutPageButton 
class << self #means static methods
		
	def left=(argument)
		RhoElementsExt.meta_proc('zoomOutPageButton', 'left:' + argument.to_s, '')
	end
	
	def top=(argument)
		RhoElementsExt.meta_proc('zoomOutPageButton', 'top:' + argument.to_s, '')
	end
	
	def height=(argument)
		RhoElementsExt.meta_proc('zoomOutPageButton', 'height:' + argument.to_s, '')
	end
	
	def width=(argument)
		RhoElementsExt.meta_proc('zoomOutPageButton', 'width:' + argument.to_s, '')
	end
	
	def imageup=(argument)
		RhoElementsExt.meta_proc('zoomOutPageButton', 'imageup:' + argument.to_s, '')
	end
	
	def imagedown=(argument)
		RhoElementsExt.meta_proc('zoomOutPageButton', 'imagedown:' + argument.to_s, '')
	end
	
	def visibility=(argument)
		RhoElementsExt.meta_proc('zoomOutPageButton', 'visibility:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('zoomOutPageButton', argument.to_s, '')
	end
end
end
    
module PowerOn 
class << self #means static methods
		
	def powerOnEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('powerOn', 'powerOnEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('powerOn', 'powerOnEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('powerOn', argument.to_s, '')
	end
end
end
    
module Push 
class << self #means static methods
		
	def port=(argument)
		RhoElementsExt.meta_proc('push', 'port:' + argument.to_s, '')
	end
	
	def passKey=(argument)
		RhoElementsExt.meta_proc('push', 'passKey:' + argument.to_s, '')
	end
	
	def response=(argument)
		RhoElementsExt.meta_proc('push', 'response:' + argument.to_s, '')
	end
	
	def path=(argument)
		RhoElementsExt.meta_proc('push', 'path:' + argument.to_s, '')
	end
	
	def unattended=(argument)
		RhoElementsExt.meta_proc('push', 'unattended:' + argument.to_s, '')
	end
	
	def start
		RhoElementsExt.meta_proc('push', 'start', '')
	end
	
	def stop
		RhoElementsExt.meta_proc('push', 'stop', '')
	end
	
	def detected=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('push', 'detected:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('push', 'detected:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('push', argument.to_s, '')
	end
end
end
    
module QuitButton 
class << self #means static methods
		
	def left=(argument)
		RhoElementsExt.meta_proc('quitButton', 'left:' + argument.to_s, '')
	end
	
	def top=(argument)
		RhoElementsExt.meta_proc('quitButton', 'top:' + argument.to_s, '')
	end
	
	def height=(argument)
		RhoElementsExt.meta_proc('quitButton', 'height:' + argument.to_s, '')
	end
	
	def width=(argument)
		RhoElementsExt.meta_proc('quitButton', 'width:' + argument.to_s, '')
	end
	
	def imageUp=(argument)
		RhoElementsExt.meta_proc('quitButton', 'imageUp:' + argument.to_s, '')
	end
	
	def imageDown=(argument)
		RhoElementsExt.meta_proc('quitButton', 'imageDown:' + argument.to_s, '')
	end
	
	def visibility=(argument)
		RhoElementsExt.meta_proc('quitButton', 'visibility:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('quitButton', argument.to_s, '')
	end
end
end

module SettingsButton 
class << self #means static methods
		
	def left=(argument)
		RhoElementsExt.meta_proc('settingsButton', 'left:' + argument.to_s, '')
	end
	
	def top=(argument)
		RhoElementsExt.meta_proc('settingsButton', 'top:' + argument.to_s, '')
	end
	
	def height=(argument)
		RhoElementsExt.meta_proc('settingsButton', 'height:' + argument.to_s, '')
	end
	
	def width=(argument)
		RhoElementsExt.meta_proc('settingsButton', 'width:' + argument.to_s, '')
	end
	
	def imageUp=(argument)
		RhoElementsExt.meta_proc('settingsButton', 'imageUp:' + argument.to_s, '')
	end
	
	def imageDown=(argument)
		RhoElementsExt.meta_proc('settingsButton', 'imageDown:' + argument.to_s, '')
	end
	
	def visibility=(argument)
		RhoElementsExt.meta_proc('settingsButton', 'visibility:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('settingsButton', argument.to_s, '')
	end
end
end

module Reboot 
class << self #means static methods
		
	def bootType=(argument)
		RhoElementsExt.meta_proc('reboot', 'bootType:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('reboot', argument.to_s, '')
	end
end
end
    
module Registry 
class << self #means static methods
		
	def hive=(argument)
		RhoElementsExt.meta_proc('registry', 'hive:' + argument.to_s, '')
	end
	
	def key=(argument)
		RhoElementsExt.meta_proc('registry', 'key:' + argument.to_s, '')
	end
	
	def setting=(argument)
		RhoElementsExt.meta_proc('registry', 'setting:' + argument.to_s, '')
	end
	
	def type=(argument)
		RhoElementsExt.meta_proc('registry', 'type:' + argument.to_s, '')
	end
	
	def persistent=(argument)
		RhoElementsExt.meta_proc('registry', 'persistent:' + argument.to_s, '')
	end
	
	def value=(argument)
		RhoElementsExt.meta_proc('registry', 'value:' + argument.to_s, '')
	end
	
	def delete
		RhoElementsExt.meta_proc('registry', 'delete', '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('registry', argument.to_s, '')
	end
end
end
    
module ReloadButton 
class << self #means static methods
		
	def left=(argument)
		RhoElementsExt.meta_proc('reloadButton', 'left:' + argument.to_s, '')
	end
	
	def top=(argument)
		RhoElementsExt.meta_proc('reloadButton', 'top:' + argument.to_s, '')
	end
	
	def height=(argument)
		RhoElementsExt.meta_proc('reloadButton', 'height:' + argument.to_s, '')
	end
	
	def width=(argument)
		RhoElementsExt.meta_proc('reloadButton', 'width:' + argument.to_s, '')
	end
	
	def imageUp=(argument)
		RhoElementsExt.meta_proc('reloadButton', 'imageUp:' + argument.to_s, '')
	end
	
	def imageDown=(argument)
		RhoElementsExt.meta_proc('reloadButton', 'imageDown:' + argument.to_s, '')
	end
	
	def visibility=(argument)
		RhoElementsExt.meta_proc('reloadButton', 'visibility:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('reloadButton', argument.to_s, '')
	end
end
end
    
module Rfid 
class << self #means static methods
		
	def antennaSelected=(argument)
		RhoElementsExt.meta_proc('rfid', 'antennaSelected:' + argument.to_s, '')
	end
	
	def beepOnRead=(argument)
		RhoElementsExt.meta_proc('rfid', 'beepOnRead:' + argument.to_s, '')
	end
	
	def enableTagAccessStatus=(argument)
		RhoElementsExt.meta_proc('rfid', 'enableTagAccessStatus:' + argument.to_s, '')
	end
	
	def enableTagAntennaID=(argument)
		RhoElementsExt.meta_proc('rfid', 'enableTagAntennaID:' + argument.to_s, '')
	end
	
	def enableTagCRC=(argument)
		RhoElementsExt.meta_proc('rfid', 'enableTagCRC:' + argument.to_s, '')
	end
	
	def enableTagXPC=(argument)
		RhoElementsExt.meta_proc('rfid', 'enableTagXPC:' + argument.to_s, '')
	end
	
	def enableTagPC=(argument)
		RhoElementsExt.meta_proc('rfid', 'enableTagPC:' + argument.to_s, '')
	end
	
	def enableTagRSSI=(argument)
		RhoElementsExt.meta_proc('rfid', 'enableTagRSSI:' + argument.to_s, '')
	end
	
	def enableTagSeenCount=(argument)
		RhoElementsExt.meta_proc('rfid', 'enableTagSeenCount:' + argument.to_s, '')
	end
	
	def enableTagUTCTimeStamp=(argument)
		RhoElementsExt.meta_proc('rfid', 'enableTagUTCTimeStamp:' + argument.to_s, '')
	end
	
	def invMemBank=(argument)
		RhoElementsExt.meta_proc('rfid', 'invMemBank:' + argument.to_s, '')
	end
	
	def lockField=(argument)
		RhoElementsExt.meta_proc('rfid', 'lockField:' + argument.to_s, '')
	end
	
	def lockPrivilege=(argument)
		RhoElementsExt.meta_proc('rfid', 'lockPrivilege:' + argument.to_s, '')
	end
	
	def matchPattern=(argument)
		RhoElementsExt.meta_proc('rfid', 'matchPattern:' + argument.to_s, '')
	end
	
	def newTagID=(argument)
		RhoElementsExt.meta_proc('rfid', 'newTagID:' + argument.to_s, '')
	end
	
	def newAccessPassword=(argument)
		RhoElementsExt.meta_proc('rfid', 'newAccessPassword:' + argument.to_s, '')
	end
	
	def newKillPassword=(argument)
		RhoElementsExt.meta_proc('rfid', 'newKillPassword:' + argument.to_s, '')
	end
	
	def performStateAwareSingulation=(argument)
		RhoElementsExt.meta_proc('rfid', 'performStateAwareSingulation:' + argument.to_s, '')
	end
	
	def preFilterStateAwareAction=(argument)
		RhoElementsExt.meta_proc('rfid', 'preFilterStateAwareAction:' + argument.to_s, '')
	end
	
	def preFilterTarget=(argument)
		RhoElementsExt.meta_proc('rfid', 'preFilterTarget:' + argument.to_s, '')
	end
	
	def preFilterStateUnawareAction=(argument)
		RhoElementsExt.meta_proc('rfid', 'preFilterStateUnawareAction:' + argument.to_s, '')
	end
	
	def preFilterBitCount=(argument)
		RhoElementsExt.meta_proc('rfid', 'preFilterBitCount:' + argument.to_s, '')
	end
	
	def preFilterBitOffset=(argument)
		RhoElementsExt.meta_proc('rfid', 'preFilterBitOffset:' + argument.to_s, '')
	end
	
	def preFilterHexPattern=(argument)
		RhoElementsExt.meta_proc('rfid', 'preFilterHexPattern:' + argument.to_s, '')
	end
	
	def preFilterID=(argument)
		RhoElementsExt.meta_proc('rfid', 'preFilterID:' + argument.to_s, '')
	end
	
	def preFilterMemBank=(argument)
		RhoElementsExt.meta_proc('rfid', 'preFilterMemBank:' + argument.to_s, '')
	end
	
	def startPeriod=(argument)
		RhoElementsExt.meta_proc('rfid', 'startPeriod:' + argument.to_s, '')
	end
	
	def reportTrigger=(argument)
		RhoElementsExt.meta_proc('rfid', 'reportTrigger:' + argument.to_s, '')
	end
	
	def reportUniqueTags=(argument)
		RhoElementsExt.meta_proc('rfid', 'reportUniqueTags:' + argument.to_s, '')
	end
	
	def RFMode=(argument)
		RhoElementsExt.meta_proc('rfid', 'RFMode:' + argument.to_s, '')
	end
	
	def singulationSession=(argument)
		RhoElementsExt.meta_proc('rfid', 'singulationSession:' + argument.to_s, '')
	end
	
	def singulationTagPopulation=(argument)
		RhoElementsExt.meta_proc('rfid', 'singulationTagPopulation:' + argument.to_s, '')
	end
	
	def singulationInventoryState=(argument)
		RhoElementsExt.meta_proc('rfid', 'singulationInventoryState:' + argument.to_s, '')
	end
	
	def singulationSLFlag=(argument)
		RhoElementsExt.meta_proc('rfid', 'singulationSLFlag:' + argument.to_s, '')
	end
	
	def startTriggerType=(argument)
		RhoElementsExt.meta_proc('rfid', 'startTriggerType:' + argument.to_s, '')
	end
	
	def stopDuration=(argument)
		RhoElementsExt.meta_proc('rfid', 'stopDuration:' + argument.to_s, '')
	end
	
	def stopObservationCount=(argument)
		RhoElementsExt.meta_proc('rfid', 'stopObservationCount:' + argument.to_s, '')
	end
	
	def stopTriggerType=(argument)
		RhoElementsExt.meta_proc('rfid', 'stopTriggerType:' + argument.to_s, '')
	end
	
	def tagByteOffset=(argument)
		RhoElementsExt.meta_proc('rfid', 'tagByteOffset:' + argument.to_s, '')
	end
	
	def tagID=(argument)
		RhoElementsExt.meta_proc('rfid', 'tagID:' + argument.to_s, '')
	end
	
	def tagKillPassword=(argument)
		RhoElementsExt.meta_proc('rfid', 'tagKillPassword:' + argument.to_s, '')
	end
	
	def tagMemBank=(argument)
		RhoElementsExt.meta_proc('rfid', 'tagMemBank:' + argument.to_s, '')
	end
	
	def tagPassword=(argument)
		RhoElementsExt.meta_proc('rfid', 'tagPassword:' + argument.to_s, '')
	end
	
	def tagPatternAByteOffset=(argument)
		RhoElementsExt.meta_proc('rfid', 'tagPatternAByteOffset:' + argument.to_s, '')
	end
	
	def tagPatternAHexMask=(argument)
		RhoElementsExt.meta_proc('rfid', 'tagPatternAHexMask:' + argument.to_s, '')
	end
	
	def tagPatternAHexPattern=(argument)
		RhoElementsExt.meta_proc('rfid', 'tagPatternAHexPattern:' + argument.to_s, '')
	end
	
	def tagPatternAMemBank=(argument)
		RhoElementsExt.meta_proc('rfid', 'tagPatternAMemBank:' + argument.to_s, '')
	end
	
	def tagPatternBByteOffset=(argument)
		RhoElementsExt.meta_proc('rfid', 'tagPatternBByteOffset:' + argument.to_s, '')
	end
	
	def tagPatternBHexMask=(argument)
		RhoElementsExt.meta_proc('rfid', 'tagPatternBHexMask:' + argument.to_s, '')
	end
	
	def tagPatternBHexPattern=(argument)
		RhoElementsExt.meta_proc('rfid', 'tagPatternBHexPattern:' + argument.to_s, '')
	end
	
	def tagPatternBMemBank=(argument)
		RhoElementsExt.meta_proc('rfid', 'tagPatternBMemBank:' + argument.to_s, '')
	end
	
	def tagReadSize=(argument)
		RhoElementsExt.meta_proc('rfid', 'tagReadSize:' + argument.to_s, '')
	end
	
	def tagWriteData=(argument)
		RhoElementsExt.meta_proc('rfid', 'tagWriteData:' + argument.to_s, '')
	end
	
	def transmitPower=(argument)
		RhoElementsExt.meta_proc('rfid', 'transmitPower:' + argument.to_s, '')
	end
	
	def useAccessFilter=(argument)
		RhoElementsExt.meta_proc('rfid', 'useAccessFilter:' + argument.to_s, '')
	end
	
	def enumerate
		RhoElementsExt.meta_proc('rfid', 'enumerate', '')
	end
	
	def connect
		RhoElementsExt.meta_proc('rfid', 'connect', '')
	end
	
	def disconnect
		RhoElementsExt.meta_proc('rfid', 'disconnect', '')
	end
	
	def stop
		RhoElementsExt.meta_proc('rfid', 'stop', '')
	end
	
	def performInventory
		RhoElementsExt.meta_proc('rfid', 'performInventory', '')
	end
	
	def locateTag
		RhoElementsExt.meta_proc('rfid', 'locateTag', '')
	end
	
	def killTag
		RhoElementsExt.meta_proc('rfid', 'killTag', '')
	end
	
	def getRadioPowerState
		RhoElementsExt.meta_proc('rfid', 'getRadioPowerState', '')
	end
	
	def addPreFilter
		RhoElementsExt.meta_proc('rfid', 'addPreFilter', '')
	end
	
	def deletePreFilter
		RhoElementsExt.meta_proc('rfid', 'deletePreFilter', '')
	end
	
	def clearPreFilters
		RhoElementsExt.meta_proc('rfid', 'clearPreFilters', '')
	end
	
	def radioPowerStateOn
		RhoElementsExt.meta_proc('rfid', 'radioPowerStateOn', '')
	end
	
	def radioPowerStateOff
		RhoElementsExt.meta_proc('rfid', 'radioPowerStateOff', '')
	end
	
	def tagRead
		RhoElementsExt.meta_proc('rfid', 'tagRead', '')
	end
	
	def tagWrite
		RhoElementsExt.meta_proc('rfid', 'tagWrite', '')
	end
	
	def setTagID
		RhoElementsExt.meta_proc('rfid', 'setTagID', '')
	end
	
	def setTagAccessPassword
		RhoElementsExt.meta_proc('rfid', 'setTagAccessPassword', '')
	end
	
	def setTagKillPassword
		RhoElementsExt.meta_proc('rfid', 'setTagKillPassword', '')
	end
	
	def setSingulation
		RhoElementsExt.meta_proc('rfid', 'setSingulation', '')
	end
	
	def getSingulation
		RhoElementsExt.meta_proc('rfid', 'getSingulation', '')
	end
	
	def getRFParams
		RhoElementsExt.meta_proc('rfid', 'getRFParams', '')
	end
	
	def tagLock
		RhoElementsExt.meta_proc('rfid', 'tagLock', '')
	end
	
	def getRFModeInfo
		RhoElementsExt.meta_proc('rfid', 'getRFModeInfo', '')
	end
	
	def setRFParams
		RhoElementsExt.meta_proc('rfid', 'setRFParams', '')
	end
	
	def getLastAccessResult
		RhoElementsExt.meta_proc('rfid', 'getLastAccessResult', '')
	end
	
	def enumRFIDEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('rfid', 'enumRFIDEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('rfid', 'enumRFIDEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def rfParamsEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('rfid', 'rfParamsEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('rfid', 'rfParamsEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def rfModeInfoEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('rfid', 'rfModeInfoEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('rfid', 'rfModeInfoEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def tagEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('rfid', 'tagEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('rfid', 'tagEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def radioPowerStateEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('rfid', 'radioPowerStateEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('rfid', 'radioPowerStateEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def statusEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('rfid', 'statusEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('rfid', 'statusEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def singulationEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('rfid', 'singulationEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('rfid', 'singulationEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def operationCompleteEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('rfid', 'operationCompleteEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('rfid', 'operationCompleteEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def lastAccessResultEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('rfid', 'lastAccessResultEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('rfid', 'lastAccessResultEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('rfid', argument.to_s, '')
	end
end
end
    
module Rsm 
class << self #means static methods
		
	def bluetoothAuthentication=(argument)
		RhoElementsExt.meta_proc('rsm', 'bluetoothAuthentication:' + argument.to_s, '')
	end
	
	def bluetoothEncryption=(argument)
		RhoElementsExt.meta_proc('rsm', 'bluetoothEncryption:' + argument.to_s, '')
	end
	
	def bluetoothPinCode=(argument)
		RhoElementsExt.meta_proc('rsm', 'bluetoothPinCode:' + argument.to_s, '')
	end
	
	def bluetoothPinCodeType=(argument)
		RhoElementsExt.meta_proc('rsm', 'bluetoothPinCodeType:' + argument.to_s, '')
	end
	
	def bluetoothReconnectAttempts=(argument)
		RhoElementsExt.meta_proc('rsm', 'bluetoothReconnectAttempts:' + argument.to_s, '')
	end
	
	def bluetoothBeepOnReconnectAttempt=(argument)
		RhoElementsExt.meta_proc('rsm', 'bluetoothBeepOnReconnectAttempt:' + argument.to_s, '')
	end
	
	def bluetoothHidAutoReconnect=(argument)
		RhoElementsExt.meta_proc('rsm', 'bluetoothHidAutoReconnect:' + argument.to_s, '')
	end
	
	def bluetoothFriendlyName=(argument)
		RhoElementsExt.meta_proc('rsm', 'bluetoothFriendlyName:' + argument.to_s, '')
	end
	
	def bluetoothInquiryMode=(argument)
		RhoElementsExt.meta_proc('rsm', 'bluetoothInquiryMode:' + argument.to_s, '')
	end
	
	def bluetoothAutoReconnect=(argument)
		RhoElementsExt.meta_proc('rsm', 'bluetoothAutoReconnect:' + argument.to_s, '')
	end
	
	def forceSavePairingBarcode=(argument)
		RhoElementsExt.meta_proc('rsm', 'forceSavePairingBarcode:' + argument.to_s, '')
	end
	
	def lowBatteryIndication=(argument)
		RhoElementsExt.meta_proc('rsm', 'lowBatteryIndication:' + argument.to_s, '')
	end
	
	def lowBatteryIndicationCycle=(argument)
		RhoElementsExt.meta_proc('rsm', 'lowBatteryIndicationCycle:' + argument.to_s, '')
	end
	
	def scanLineWidth=(argument)
		RhoElementsExt.meta_proc('rsm', 'scanLineWidth:' + argument.to_s, '')
	end
	
	def goodScansDelay=(argument)
		RhoElementsExt.meta_proc('rsm', 'goodScansDelay:' + argument.to_s, '')
	end
	
	def decodeFeedback=(argument)
		RhoElementsExt.meta_proc('rsm', 'decodeFeedback:' + argument.to_s, '')
	end
	
	def ignoreCode128Usps=(argument)
		RhoElementsExt.meta_proc('rsm', 'ignoreCode128Usps:' + argument.to_s, '')
	end
	
	def scanTriggerWakeup=(argument)
		RhoElementsExt.meta_proc('rsm', 'scanTriggerWakeup:' + argument.to_s, '')
	end
	
	def mems=(argument)
		RhoElementsExt.meta_proc('rsm', 'mems:' + argument.to_s, '')
	end
	
	def proximityEnable=(argument)
		RhoElementsExt.meta_proc('rsm', 'proximityEnable:' + argument.to_s, '')
	end
	
	def proximityContinuous=(argument)
		RhoElementsExt.meta_proc('rsm', 'proximityContinuous:' + argument.to_s, '')
	end
	
	def proximityDistance=(argument)
		RhoElementsExt.meta_proc('rsm', 'proximityDistance:' + argument.to_s, '')
	end
	
	def pagingEnable=(argument)
		RhoElementsExt.meta_proc('rsm', 'pagingEnable:' + argument.to_s, '')
	end
	
	def pagingBeepSequence=(argument)
		RhoElementsExt.meta_proc('rsm', 'pagingBeepSequence:' + argument.to_s, '')
	end
	
	def pagingActivate=(argument)
		RhoElementsExt.meta_proc('rsm', 'pagingActivate:' + argument.to_s, '')
	end
	
	def markForRetrievalModelNumber
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalModelNumber', '')
	end
	
	def markForRetrievalSerialNumber
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalSerialNumber', '')
	end
	
	def markForRetrievalDateOfManufacture
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalDateOfManufacture', '')
	end
	
	def markForRetrievalDateOfService
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalDateOfService', '')
	end
	
	def markForRetrievalBluetoothAddress
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalBluetoothAddress', '')
	end
	
	def markForRetrievalFirmwareVersion
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalFirmwareVersion', '')
	end
	
	def markForRetrievalDeviceClass
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalDeviceClass', '')
	end
	
	def markForRetrievalBatteryStatus
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalBatteryStatus', '')
	end
	
	def markForRetrievalBatteryCapacity
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalBatteryCapacity', '')
	end
	
	def markForRetrievalBatteryId
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalBatteryId', '')
	end
	
	def markForRetrievalBluetoothAuthentication
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalBluetoothAuthentication', '')
	end
	
	def markForRetrievalBluetoothEncryption
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalBluetoothEncryption', '')
	end
	
	def markForRetrievalBluetoothPinCode
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalBluetoothPinCode', '')
	end
	
	def markForRetrievalBluetoothPinCodeType
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalBluetoothPinCodeType', '')
	end
	
	def markForRetrievalBluetoothReconnectAttempts
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalBluetoothReconnectAttempts', '')
	end
	
	def markForRetrievalBluetoothBeepOnReconnectAttempt
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalBluetoothBeepOnReconnectAttempt', '')
	end
	
	def markForRetrievalBluetoothHidAutoReconnect
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalBluetoothHidAutoReconnect', '')
	end
	
	def markForRetrievalBluetoothFriendlyName
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalBluetoothFriendlyName', '')
	end
	
	def markForRetrievalBluetoothInquiryMode
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalBluetoothInquiryMode', '')
	end
	
	def markForRetrievalBluetoothAutoReconnect
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalBluetoothAutoReconnect', '')
	end
	
	def markForRetrievalForceSavePairingBarcode
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalForceSavePairingBarcode', '')
	end
	
	def markForRetrievalLowBatteryIndication
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalLowBatteryIndication', '')
	end
	
	def markForRetrievalLowBatteryIndicationCycle
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalLowBatteryIndicationCycle', '')
	end
	
	def markForRetrievalScanLineWidth
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalScanLineWidth', '')
	end
	
	def markForRetrievalGoodScansDelay
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalGoodScansDelay', '')
	end
	
	def markForRetrievalDecodeFeedback
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalDecodeFeedback', '')
	end
	
	def markForRetrievalIgnoreCode128Usps
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalIgnoreCode128Usps', '')
	end
	
	def markForRetrievalScanTriggerWakeup
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalScanTriggerWakeup', '')
	end
	
	def markForRetrievalMems
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalMems', '')
	end
	
	def markForRetrievalProximityEnable
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalProximityEnable', '')
	end
	
	def markForRetrievalProximityContinuous
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalProximityContinuous', '')
	end
	
	def markForRetrievalProximityDistance
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalProximityDistance', '')
	end
	
	def markForRetrievalPagingEnable
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalPagingEnable', '')
	end
	
	def markForRetrievalPagingBeepSequence
		RhoElementsExt.meta_proc('rsm', 'markForRetrievalPagingBeepSequence', '')
	end
	
	def bluetoothDisconnect
		RhoElementsExt.meta_proc('rsm', 'bluetoothDisconnect', '')
	end
	
	def bluetoothUnpair
		RhoElementsExt.meta_proc('rsm', 'bluetoothUnpair', '')
	end
	
	def rsmGetEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('rsm', 'rsmGetEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('rsm', 'rsmGetEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('rsm', argument.to_s, '')
	end
end
end
    
module ScreenOrientation 
class << self #means static methods
		
	def autoRotate=(argument)
		RhoElementsExt.meta_proc('screenOrientation', 'autoRotate:' + argument.to_s, '')
	end
	
	def normal
		RhoElementsExt.meta_proc('screenOrientation', 'normal', '')
	end
	
	def rightHanded
		RhoElementsExt.meta_proc('screenOrientation', 'rightHanded', '')
	end
	
	def leftHanded
		RhoElementsExt.meta_proc('screenOrientation', 'leftHanded', '')
	end
	
	def upsideDown
		RhoElementsExt.meta_proc('screenOrientation', 'upsideDown', '')
	end
	
	def screenOrientationEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('screenOrientation', 'screenOrientationEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('screenOrientation', 'screenOrientationEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('screenOrientation', argument.to_s, '')
	end
end
end
    
module Signal 
class << self #means static methods
		
	def visibility=(argument)
		RhoElementsExt.meta_proc('signal', 'visibility:' + argument.to_s, '')
	end
	
	def left=(argument)
		RhoElementsExt.meta_proc('signal', 'left:' + argument.to_s, '')
	end
	
	def top=(argument)
		RhoElementsExt.meta_proc('signal', 'top:' + argument.to_s, '')
	end
	
	def layout=(argument)
		RhoElementsExt.meta_proc('signal', 'layout:' + argument.to_s, '')
	end
	
	def color=(argument)
		RhoElementsExt.meta_proc('signal', 'color:' + argument.to_s, '')
	end
	
	def signalEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('signal', 'signalEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('signal', 'signalEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('signal', argument.to_s, '')
	end
end
end
    
module SignatureCapture 
class << self #means static methods
		
	def visibility=(argument)
		RhoElementsExt.meta_proc('signatureCapture', 'visibility:' + argument.to_s, '')
	end
	
	def border=(argument)
		RhoElementsExt.meta_proc('signatureCapture', 'border:' + argument.to_s, '')
	end
	
	def width=(argument)
		RhoElementsExt.meta_proc('signatureCapture', 'width:' + argument.to_s, '')
	end
	
	def height=(argument)
		RhoElementsExt.meta_proc('signatureCapture', 'height:' + argument.to_s, '')
	end
	
	def left=(argument)
		RhoElementsExt.meta_proc('signatureCapture', 'left:' + argument.to_s, '')
	end
	
	def top=(argument)
		RhoElementsExt.meta_proc('signatureCapture', 'top:' + argument.to_s, '')
	end
	
	def penWidth=(argument)
		RhoElementsExt.meta_proc('signatureCapture', 'penWidth:' + argument.to_s, '')
	end
	
	def penColor=(argument)
		RhoElementsExt.meta_proc('signatureCapture', 'penColor:' + argument.to_s, '')
	end
	
	def bgColor=(argument)
		RhoElementsExt.meta_proc('signatureCapture', 'bgColor:' + argument.to_s, '')
	end
	
	def destination=(argument)
		RhoElementsExt.meta_proc('signatureCapture', 'destination:' + argument.to_s, '')
	end
	
	def username=(argument)
		RhoElementsExt.meta_proc('signatureCapture', 'username:' + argument.to_s, '')
	end
	
	def password=(argument)
		RhoElementsExt.meta_proc('signatureCapture', 'password:' + argument.to_s, '')
	end
	
	def name=(argument)
		RhoElementsExt.meta_proc('signatureCapture', 'name:' + argument.to_s, '')
	end
	
	def clear
		RhoElementsExt.meta_proc('signatureCapture', 'clear', '')
	end
	
	def capture
		RhoElementsExt.meta_proc('signatureCapture', 'capture', '')
	end
	
	def signatureSaveEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('signatureCapture', 'signatureSaveEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('signatureCapture', 'signatureSaveEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def signatureCaptureEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('signatureCapture', 'signatureCaptureEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('signatureCapture', 'signatureCaptureEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def vectorEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('signatureCapture', 'vectorEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('signatureCapture', 'vectorEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('signatureCapture', argument.to_s, '')
	end
end
end
    
module Sip 
class << self #means static methods
		
	def left=(argument)
		RhoElementsExt.meta_proc('sip', 'left:' + argument.to_s, '')
	end
	
	def top=(argument)
		RhoElementsExt.meta_proc('sip', 'top:' + argument.to_s, '')
	end
	
	def manual
		RhoElementsExt.meta_proc('sip', 'manual', '')
	end
	
	def automatic
		RhoElementsExt.meta_proc('sip', 'automatic', '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('sip', argument.to_s, '')
	end
end
end
    
module SipButton 
class << self #means static methods
		
	def left=(argument)
		RhoElementsExt.meta_proc('sipButton', 'left:' + argument.to_s, '')
	end
	
	def top=(argument)
		RhoElementsExt.meta_proc('sipButton', 'top:' + argument.to_s, '')
	end
	
	def height=(argument)
		RhoElementsExt.meta_proc('sipButton', 'height:' + argument.to_s, '')
	end
	
	def width=(argument)
		RhoElementsExt.meta_proc('sipButton', 'width:' + argument.to_s, '')
	end
	
	def imageUp=(argument)
		RhoElementsExt.meta_proc('sipButton', 'imageUp:' + argument.to_s, '')
	end
	
	def imageDown=(argument)
		RhoElementsExt.meta_proc('sipButton', 'imageDown:' + argument.to_s, '')
	end
	
	def visibility=(argument)
		RhoElementsExt.meta_proc('sipButton', 'visibility:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('sipButton', argument.to_s, '')
	end
end
end
    
module Sntp 
class << self #means static methods
		
	def serverIp=(argument)
		RhoElementsExt.meta_proc('sntp', 'serverIp:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('sntp', argument.to_s, '')
	end
end
end
    
module StopButton 
class << self #means static methods
		
	def left=(argument)
		RhoElementsExt.meta_proc('stopButton', 'left:' + argument.to_s, '')
	end
	
	def top=(argument)
		RhoElementsExt.meta_proc('stopButton', 'top:' + argument.to_s, '')
	end
	
	def height=(argument)
		RhoElementsExt.meta_proc('stopButton', 'height:' + argument.to_s, '')
	end
	
	def width=(argument)
		RhoElementsExt.meta_proc('stopButton', 'width:' + argument.to_s, '')
	end
	
	def imageUp=(argument)
		RhoElementsExt.meta_proc('stopButton', 'imageUp:' + argument.to_s, '')
	end
	
	def imageDown=(argument)
		RhoElementsExt.meta_proc('stopButton', 'imageDown:' + argument.to_s, '')
	end
	
	def visibility=(argument)
		RhoElementsExt.meta_proc('stopButton', 'visibility:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('stopButton', argument.to_s, '')
	end
end
end
    
module Stylus 
class << self #means static methods
		
	def enable
		RhoElementsExt.meta_proc('stylus', 'enable', '')
	end
	
	def disable
		RhoElementsExt.meta_proc('stylus', 'disable', '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('stylus', argument.to_s, '')
	end
end
end
    
module SystemTime 
class << self #means static methods
		
	def setLocal=(argument)
		RhoElementsExt.meta_proc('systemTime', 'setLocal:' + argument.to_s, '')
	end
	
	def setUtc=(argument)
		RhoElementsExt.meta_proc('systemTime', 'setUtc:' + argument.to_s, '')
	end
	
	def sntpServerIp=(argument)
		RhoElementsExt.meta_proc('systemTime', 'sntpServerIp:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('systemTime', argument.to_s, '')
	end
end
end
    
module Telemetry 
class << self #means static methods
		
	def queryFrequency=(argument)
		RhoElementsExt.meta_proc('telemetry', 'queryFrequency:' + argument.to_s, '')
	end
	
	def enabled=(argument)
		RhoElementsExt.meta_proc('telemetry', 'enabled:' + argument.to_s, '')
	end
	
	def disabled=(argument)
		RhoElementsExt.meta_proc('telemetry', 'disabled:' + argument.to_s, '')
	end
	
	def invoke
		RhoElementsExt.meta_proc('telemetry', 'invoke', '')
	end
	
	def telemetryDataEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('telemetry', 'telemetryDataEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('telemetry', 'telemetryDataEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def telemetryArrayEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('telemetry', 'telemetryArrayEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('telemetry', 'telemetryArrayEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('telemetry', argument.to_s, '')
	end
end
end
    
module Timer 
class << self #means static methods
		
	def interval=(argument)
		RhoElementsExt.meta_proc('timer', 'interval:' + argument.to_s, '')
	end
	
	def start
		RhoElementsExt.meta_proc('timer', 'start', '')
	end
	
	def stop
		RhoElementsExt.meta_proc('timer', 'stop', '')
	end
	
	def timeout=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('timer', 'timeout:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('timer', 'timeout:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('timer', argument.to_s, '')
	end
end
end
    
module TopCommandArea 
class << self #means static methods
		
	def visibility=(argument)
		RhoElementsExt.meta_proc('topCommandArea', 'visibility:' + argument.to_s, '')
	end
	
	def height=(argument)
		RhoElementsExt.meta_proc('topCommandArea', 'height:' + argument.to_s, '')
	end
	
	def color=(argument)
		RhoElementsExt.meta_proc('topCommandArea', 'color:' + argument.to_s, '')
	end
	
	def image=(argument)
		RhoElementsExt.meta_proc('topCommandArea', 'image:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('topCommandArea', argument.to_s, '')
	end
end
end
    
module Volume 
class << self #means static methods
		
	def setVolume=(argument)
		RhoElementsExt.meta_proc('volume', 'setVolume:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('volume', argument.to_s, '')
	end
end
end
    
module Wake 
class << self #means static methods
		
	def wakeLock=(argument)
		RhoElementsExt.meta_proc('wake', 'wakeLock:' + argument.to_s, '')
	end
	
	def wifiLock=(argument)
		RhoElementsExt.meta_proc('wake', 'wifiLock:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('wake', argument.to_s, '')
	end
end
end
    
module Wlan 
class << self #means static methods
		
	def parameter=(argument)
		RhoElementsExt.meta_proc('wlan', 'parameter:' + argument.to_s, '')
	end
	
	def profileGuid=(argument)
		RhoElementsExt.meta_proc('wlan', 'profileGuid:' + argument.to_s, '')
	end
	
	def networkUsername=(argument)
		RhoElementsExt.meta_proc('wlan', 'networkUsername:' + argument.to_s, '')
	end
	
	def networkPassword=(argument)
		RhoElementsExt.meta_proc('wlan', 'networkPassword:' + argument.to_s, '')
	end
	
	def networkDomain=(argument)
		RhoElementsExt.meta_proc('wlan', 'networkDomain:' + argument.to_s, '')
	end
	
	def selectLoginProfile=(argument)
		RhoElementsExt.meta_proc('wlan', 'selectLoginProfile:' + argument.to_s, '')
	end
	
	def profileEssid=(argument)
		RhoElementsExt.meta_proc('wlan', 'profileEssid:' + argument.to_s, '')
	end
	
	def profileName=(argument)
		RhoElementsExt.meta_proc('wlan', 'profileName:' + argument.to_s, '')
	end
	
	def profilePassphrase=(argument)
		RhoElementsExt.meta_proc('wlan', 'profilePassphrase:' + argument.to_s, '')
	end
	
	def profileEncryption=(argument)
		RhoElementsExt.meta_proc('wlan', 'profileEncryption:' + argument.to_s, '')
	end
	
	def profileSecurityMode=(argument)
		RhoElementsExt.meta_proc('wlan', 'profileSecurityMode:' + argument.to_s, '')
	end
	
	def deleteAfterImport=(argument)
		RhoElementsExt.meta_proc('wlan', 'deleteAfterImport:' + argument.to_s, '')
	end
	
	def importConfigFromLocation=(argument)
		RhoElementsExt.meta_proc('wlan', 'importConfigFromLocation:' + argument.to_s, '')
	end
	
	def exportType=(argument)
		RhoElementsExt.meta_proc('wlan', 'exportType:' + argument.to_s, '')
	end
	
	def exportConfigToLocation=(argument)
		RhoElementsExt.meta_proc('wlan', 'exportConfigToLocation:' + argument.to_s, '')
	end
	
	def configFilePassword=(argument)
		RhoElementsExt.meta_proc('wlan', 'configFilePassword:' + argument.to_s, '')
	end
	
	def profileGuids=(argument)
		RhoElementsExt.meta_proc('wlan', 'profileGuids:' + argument.to_s, '')
	end
	
	def countryCode=(argument)
		RhoElementsExt.meta_proc('wlan', 'countryCode:' + argument.to_s, '')
	end
	
	def optionsPassword=(argument)
		RhoElementsExt.meta_proc('wlan', 'optionsPassword:' + argument.to_s, '')
	end
	
	def regulatory11dMode=(argument)
		RhoElementsExt.meta_proc('wlan', 'regulatory11dMode:' + argument.to_s, '')
	end
	
	def regulatoryCountryCode=(argument)
		RhoElementsExt.meta_proc('wlan', 'regulatoryCountryCode:' + argument.to_s, '')
	end
	
	def enableAdapter
		RhoElementsExt.meta_proc('wlan', 'enableAdapter', '')
	end
	
	def disableAdapter
		RhoElementsExt.meta_proc('wlan', 'disableAdapter', '')
	end
	
	def getAdapterPowerState
		RhoElementsExt.meta_proc('wlan', 'getAdapterPowerState', '')
	end
	
	def resetToDefault
		RhoElementsExt.meta_proc('wlan', 'resetToDefault', '')
	end
	
	def deleteProfile
		RhoElementsExt.meta_proc('wlan', 'deleteProfile', '')
	end
	
	def networkLogin
		RhoElementsExt.meta_proc('wlan', 'networkLogin', '')
	end
	
	def networkLogout
		RhoElementsExt.meta_proc('wlan', 'networkLogout', '')
	end
	
	def cancelNetworkLogin
		RhoElementsExt.meta_proc('wlan', 'cancelNetworkLogin', '')
	end
	
	def addProfile
		RhoElementsExt.meta_proc('wlan', 'addProfile', '')
	end
	
	def connectProfile
		RhoElementsExt.meta_proc('wlan', 'connectProfile', '')
	end
	
	def importConfig
		RhoElementsExt.meta_proc('wlan', 'importConfig', '')
	end
	
	def getAllProfiles
		RhoElementsExt.meta_proc('wlan', 'getAllProfiles', '')
	end
	
	def getCurrentProfile
		RhoElementsExt.meta_proc('wlan', 'getCurrentProfile', '')
	end
	
	def getConnectionState
		RhoElementsExt.meta_proc('wlan', 'getConnectionState', '')
	end
	
	def exportConfig
		RhoElementsExt.meta_proc('wlan', 'exportConfig', '')
	end
	
	def enableDot11d
		RhoElementsExt.meta_proc('wlan', 'enableDot11d', '')
	end
	
	def disableDot11d
		RhoElementsExt.meta_proc('wlan', 'disableDot11d', '')
	end
	
	def getDot11dMode
		RhoElementsExt.meta_proc('wlan', 'getDot11dMode', '')
	end
	
	def getCountryCode
		RhoElementsExt.meta_proc('wlan', 'getCountryCode', '')
	end
	
	def getCountryList
		RhoElementsExt.meta_proc('wlan', 'getCountryList', '')
	end
	
	def setCountryCode
		RhoElementsExt.meta_proc('wlan', 'setCountryCode', '')
	end
	
	def disableProfile
		RhoElementsExt.meta_proc('wlan', 'disableProfile', '')
	end
	
	def enableProfile
		RhoElementsExt.meta_proc('wlan', 'enableProfile', '')
	end
	
	def validateOptionsPassword
		RhoElementsExt.meta_proc('wlan', 'validateOptionsPassword', '')
	end
	
	def getOptionsPasswordState
		RhoElementsExt.meta_proc('wlan', 'getOptionsPasswordState', '')
	end
	
	def getRegulatorySettings
		RhoElementsExt.meta_proc('wlan', 'getRegulatorySettings', '')
	end
	
	def adapterPowerStateEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('wlan', 'adapterPowerStateEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('wlan', 'adapterPowerStateEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def loginResponseEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('wlan', 'loginResponseEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('wlan', 'loginResponseEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def logoutResponseEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('wlan', 'logoutResponseEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('wlan', 'logoutResponseEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def cancelLoginResponseEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('wlan', 'cancelLoginResponseEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('wlan', 'cancelLoginResponseEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def loginCandidateProfilesEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('wlan', 'loginCandidateProfilesEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('wlan', 'loginCandidateProfilesEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def networkLoginEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('wlan', 'networkLoginEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('wlan', 'networkLoginEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def networkLoginCloseEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('wlan', 'networkLoginCloseEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('wlan', 'networkLoginCloseEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def importConfigEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('wlan', 'importConfigEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('wlan', 'importConfigEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def addedProfileEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('wlan', 'addedProfileEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('wlan', 'addedProfileEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def allProfilesEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('wlan', 'allProfilesEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('wlan', 'allProfilesEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def currentProfileEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('wlan', 'currentProfileEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('wlan', 'currentProfileEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def connectionStateEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('wlan', 'connectionStateEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('wlan', 'connectionStateEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def exportConfigEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('wlan', 'exportConfigEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('wlan', 'exportConfigEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def dot11dModeEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('wlan', 'dot11dModeEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('wlan', 'dot11dModeEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def countryCodeEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('wlan', 'countryCodeEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('wlan', 'countryCodeEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def countryListEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('wlan', 'countryListEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('wlan', 'countryListEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def profileStateEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('wlan', 'profileStateEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('wlan', 'profileStateEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def validateOptionsPasswordEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('wlan', 'validateOptionsPasswordEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('wlan', 'validateOptionsPasswordEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def optionsPasswordStateEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('wlan', 'optionsPasswordStateEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('wlan', 'optionsPasswordStateEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def regulatorySettingsEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('wlan', 'regulatorySettingsEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('wlan', 'regulatorySettingsEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('wlan', argument.to_s, '')
	end
end
end
    
module Zoom 
class << self #means static methods
		
	def text=(argument)
		RhoElementsExt.meta_proc('zoom', 'text:' + argument.to_s, '')
	end
	
	def page=(argument)
		RhoElementsExt.meta_proc('zoom', 'page:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('zoom', argument.to_s, '')
	end
end
end
    
module ZoomTextButton 
class << self #means static methods
		
	def left=(argument)
		RhoElementsExt.meta_proc('zoomTextButton', 'left:' + argument.to_s, '')
	end
	
	def top=(argument)
		RhoElementsExt.meta_proc('zoomTextButton', 'top:' + argument.to_s, '')
	end
	
	def height=(argument)
		RhoElementsExt.meta_proc('zoomTextButton', 'height:' + argument.to_s, '')
	end
	
	def width=(argument)
		RhoElementsExt.meta_proc('zoomTextButton', 'width:' + argument.to_s, '')
	end
	
	def imageUp=(argument)
		RhoElementsExt.meta_proc('zoomTextButton', 'imageUp:' + argument.to_s, '')
	end
	
	def imageDown=(argument)
		RhoElementsExt.meta_proc('zoomTextButton', 'imageDown:' + argument.to_s, '')
	end
	
	def visibility=(argument)
		RhoElementsExt.meta_proc('zoomTextButton', 'visibility:' + argument.to_s, '')
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('zoomTextButton', argument.to_s, '')
	end
end
end
    
module Scanner 
class << self #means static methods
		
	def allDecoders=(argument)
		RhoElementsExt.meta_proc('scanner', 'allDecoders:' + argument.to_s, '')
	end
	
	def ausPostal=(argument)
		RhoElementsExt.meta_proc('scanner', 'ausPostal:' + argument.to_s, '')
	end
	
	def aztec=(argument)
		RhoElementsExt.meta_proc('scanner', 'aztec:' + argument.to_s, '')
	end
	
	def canPostal=(argument)
		RhoElementsExt.meta_proc('scanner', 'canPostal:' + argument.to_s, '')
	end
	
	def chinese2of5=(argument)
		RhoElementsExt.meta_proc('scanner', 'chinese2of5:' + argument.to_s, '')
	end
	
	def codabar=(argument)
		RhoElementsExt.meta_proc('scanner', 'codabar:' + argument.to_s, '')
	end
	
	def codabarClsiEditing=(argument)
		RhoElementsExt.meta_proc('scanner', 'codabarClsiEditing:' + argument.to_s, '')
	end
	
	def codabarMaxLength=(argument)
		RhoElementsExt.meta_proc('scanner', 'codabarMaxLength:' + argument.to_s, '')
	end
	
	def codabarMinLength=(argument)
		RhoElementsExt.meta_proc('scanner', 'codabarMinLength:' + argument.to_s, '')
	end
	
	def codabarNotisEditing=(argument)
		RhoElementsExt.meta_proc('scanner', 'codabarNotisEditing:' + argument.to_s, '')
	end
	
	def codabarRedundancy=(argument)
		RhoElementsExt.meta_proc('scanner', 'codabarRedundancy:' + argument.to_s, '')
	end
	
	def code11=(argument)
		RhoElementsExt.meta_proc('scanner', 'code11:' + argument.to_s, '')
	end
	
	def code11checkDigitCount=(argument)
		RhoElementsExt.meta_proc('scanner', 'code11checkDigitCount:' + argument.to_s, '')
	end
	
	def code11maxLength=(argument)
		RhoElementsExt.meta_proc('scanner', 'code11maxLength:' + argument.to_s, '')
	end
	
	def code11minLength=(argument)
		RhoElementsExt.meta_proc('scanner', 'code11minLength:' + argument.to_s, '')
	end
	
	def code11redundancy=(argument)
		RhoElementsExt.meta_proc('scanner', 'code11redundancy:' + argument.to_s, '')
	end
	
	def code11reportCheckDigit=(argument)
		RhoElementsExt.meta_proc('scanner', 'code11reportCheckDigit:' + argument.to_s, '')
	end
	
	def code128=(argument)
		RhoElementsExt.meta_proc('scanner', 'code128:' + argument.to_s, '')
	end
	
	def code128checkIsBtTable=(argument)
		RhoElementsExt.meta_proc('scanner', 'code128checkIsBtTable:' + argument.to_s, '')
	end
	
	def code128Ean128=(argument)
		RhoElementsExt.meta_proc('scanner', 'code128Ean128:' + argument.to_s, '')
	end
	
	def code128isBt128=(argument)
		RhoElementsExt.meta_proc('scanner', 'code128isBt128:' + argument.to_s, '')
	end
	
	def code128isBt128ConcatMode=(argument)
		RhoElementsExt.meta_proc('scanner', 'code128isBt128ConcatMode:' + argument.to_s, '')
	end
	
	def code128maxLength=(argument)
		RhoElementsExt.meta_proc('scanner', 'code128maxLength:' + argument.to_s, '')
	end
	
	def code128minLength=(argument)
		RhoElementsExt.meta_proc('scanner', 'code128minLength:' + argument.to_s, '')
	end
	
	def code128other128=(argument)
		RhoElementsExt.meta_proc('scanner', 'code128other128:' + argument.to_s, '')
	end
	
	def code128redundancy=(argument)
		RhoElementsExt.meta_proc('scanner', 'code128redundancy:' + argument.to_s, '')
	end
	
	def code128securityLevel=(argument)
		RhoElementsExt.meta_proc('scanner', 'code128securityLevel:' + argument.to_s, '')
	end
	
	def code39=(argument)
		RhoElementsExt.meta_proc('scanner', 'code39:' + argument.to_s, '')
	end
	
	def code39code32prefix=(argument)
		RhoElementsExt.meta_proc('scanner', 'code39code32prefix:' + argument.to_s, '')
	end
	
	def code39concatenation=(argument)
		RhoElementsExt.meta_proc('scanner', 'code39concatenation:' + argument.to_s, '')
	end
	
	def code39convertToCode32=(argument)
		RhoElementsExt.meta_proc('scanner', 'code39convertToCode32:' + argument.to_s, '')
	end
	
	def code39FullAscii=(argument)
		RhoElementsExt.meta_proc('scanner', 'code39FullAscii:' + argument.to_s, '')
	end
	
	def code39maxLength=(argument)
		RhoElementsExt.meta_proc('scanner', 'code39maxLength:' + argument.to_s, '')
	end
	
	def code39minLength=(argument)
		RhoElementsExt.meta_proc('scanner', 'code39minLength:' + argument.to_s, '')
	end
	
	def code39redundancy=(argument)
		RhoElementsExt.meta_proc('scanner', 'code39redundancy:' + argument.to_s, '')
	end
	
	def code39reportCheckDigit=(argument)
		RhoElementsExt.meta_proc('scanner', 'code39reportCheckDigit:' + argument.to_s, '')
	end
	
	def code39securityLevel=(argument)
		RhoElementsExt.meta_proc('scanner', 'code39securityLevel:' + argument.to_s, '')
	end
	
	def code39verifyCheckDigit=(argument)
		RhoElementsExt.meta_proc('scanner', 'code39verifyCheckDigit:' + argument.to_s, '')
	end
	
	def code93=(argument)
		RhoElementsExt.meta_proc('scanner', 'code93:' + argument.to_s, '')
	end
	
	def code93maxLength=(argument)
		RhoElementsExt.meta_proc('scanner', 'code93maxLength:' + argument.to_s, '')
	end
	
	def code93minLength=(argument)
		RhoElementsExt.meta_proc('scanner', 'code93minLength:' + argument.to_s, '')
	end
	
	def code93redundancy=(argument)
		RhoElementsExt.meta_proc('scanner', 'code93redundancy:' + argument.to_s, '')
	end
	
	def compositeAbUccLinkMode=(argument)
		RhoElementsExt.meta_proc('scanner', 'compositeAbUccLinkMode:' + argument.to_s, '')
	end
	
	def compositeAbUseUpcPreambleCheckDigitRules=(argument)
		RhoElementsExt.meta_proc('scanner', 'compositeAbUseUpcPreambleCheckDigitRules:' + argument.to_s, '')
	end
	
	def compositeAb=(argument)
		RhoElementsExt.meta_proc('scanner', 'compositeAb:' + argument.to_s, '')
	end
	
	def compositeC=(argument)
		RhoElementsExt.meta_proc('scanner', 'compositeC:' + argument.to_s, '')
	end
	
	def d2of5=(argument)
		RhoElementsExt.meta_proc('scanner', 'd2of5:' + argument.to_s, '')
	end
	
	def d2of5maxLength=(argument)
		RhoElementsExt.meta_proc('scanner', 'd2of5maxLength:' + argument.to_s, '')
	end
	
	def d2of5minLength=(argument)
		RhoElementsExt.meta_proc('scanner', 'd2of5minLength:' + argument.to_s, '')
	end
	
	def d2of5redundancy=(argument)
		RhoElementsExt.meta_proc('scanner', 'd2of5redundancy:' + argument.to_s, '')
	end
	
	def dataMatrix=(argument)
		RhoElementsExt.meta_proc('scanner', 'dataMatrix:' + argument.to_s, '')
	end
	
	def dutchPostal=(argument)
		RhoElementsExt.meta_proc('scanner', 'dutchPostal:' + argument.to_s, '')
	end
	
	def ean13=(argument)
		RhoElementsExt.meta_proc('scanner', 'ean13:' + argument.to_s, '')
	end
	
	def ean8=(argument)
		RhoElementsExt.meta_proc('scanner', 'ean8:' + argument.to_s, '')
	end
	
	def ean8convertToEan13=(argument)
		RhoElementsExt.meta_proc('scanner', 'ean8convertToEan13:' + argument.to_s, '')
	end
	
	def gs1dataBar=(argument)
		RhoElementsExt.meta_proc('scanner', 'gs1dataBar:' + argument.to_s, '')
	end
	
	def gs1dataBarExpanded=(argument)
		RhoElementsExt.meta_proc('scanner', 'gs1dataBarExpanded:' + argument.to_s, '')
	end
	
	def gs1dataBarLimited=(argument)
		RhoElementsExt.meta_proc('scanner', 'gs1dataBarLimited:' + argument.to_s, '')
	end
	
	def i2of5=(argument)
		RhoElementsExt.meta_proc('scanner', 'i2of5:' + argument.to_s, '')
	end
	
	def i2of5convertToEan13=(argument)
		RhoElementsExt.meta_proc('scanner', 'i2of5convertToEan13:' + argument.to_s, '')
	end
	
	def i2of5maxLength=(argument)
		RhoElementsExt.meta_proc('scanner', 'i2of5maxLength:' + argument.to_s, '')
	end
	
	def i2of5minLength=(argument)
		RhoElementsExt.meta_proc('scanner', 'i2of5minLength:' + argument.to_s, '')
	end
	
	def i2of5redundancy=(argument)
		RhoElementsExt.meta_proc('scanner', 'i2of5redundancy:' + argument.to_s, '')
	end
	
	def i2of5reportCheckDigit=(argument)
		RhoElementsExt.meta_proc('scanner', 'i2of5reportCheckDigit:' + argument.to_s, '')
	end
	
	def i2Of5verifyCheckDigit=(argument)
		RhoElementsExt.meta_proc('scanner', 'i2Of5verifyCheckDigit:' + argument.to_s, '')
	end
	
	def japPostal=(argument)
		RhoElementsExt.meta_proc('scanner', 'japPostal:' + argument.to_s, '')
	end
	
	def korean3of5maxLength=(argument)
		RhoElementsExt.meta_proc('scanner', 'korean3of5maxLength:' + argument.to_s, '')
	end
	
	def korean3of5minLength=(argument)
		RhoElementsExt.meta_proc('scanner', 'korean3of5minLength:' + argument.to_s, '')
	end
	
	def korean3of5redundancy=(argument)
		RhoElementsExt.meta_proc('scanner', 'korean3of5redundancy:' + argument.to_s, '')
	end
	
	def korean3of5=(argument)
		RhoElementsExt.meta_proc('scanner', 'korean3of5:' + argument.to_s, '')
	end
	
	def macroMicroPdf=(argument)
		RhoElementsExt.meta_proc('scanner', 'macroMicroPdf:' + argument.to_s, '')
	end
	
	def macroMicroPdfBufferLabels=(argument)
		RhoElementsExt.meta_proc('scanner', 'macroMicroPdfBufferLabels:' + argument.to_s, '')
	end
	
	def macroMicroPdfConvertToMicroPdf=(argument)
		RhoElementsExt.meta_proc('scanner', 'macroMicroPdfConvertToMicroPdf:' + argument.to_s, '')
	end
	
	def macroMicroPdfExclusive=(argument)
		RhoElementsExt.meta_proc('scanner', 'macroMicroPdfExclusive:' + argument.to_s, '')
	end
	
	def macroMicroPdfReportAppendInfo=(argument)
		RhoElementsExt.meta_proc('scanner', 'macroMicroPdfReportAppendInfo:' + argument.to_s, '')
	end
	
	def macroPdf=(argument)
		RhoElementsExt.meta_proc('scanner', 'macroPdf:' + argument.to_s, '')
	end
	
	def macroPdfBufferLabels=(argument)
		RhoElementsExt.meta_proc('scanner', 'macroPdfBufferLabels:' + argument.to_s, '')
	end
	
	def macroPdfConvertToPdf417=(argument)
		RhoElementsExt.meta_proc('scanner', 'macroPdfConvertToPdf417:' + argument.to_s, '')
	end
	
	def macroPdfExclusive=(argument)
		RhoElementsExt.meta_proc('scanner', 'macroPdfExclusive:' + argument.to_s, '')
	end
	
	def matrix2of5=(argument)
		RhoElementsExt.meta_proc('scanner', 'matrix2of5:' + argument.to_s, '')
	end
	
	def matrix2of5maxLength=(argument)
		RhoElementsExt.meta_proc('scanner', 'matrix2of5maxLength:' + argument.to_s, '')
	end
	
	def matrix2of5minLength=(argument)
		RhoElementsExt.meta_proc('scanner', 'matrix2of5minLength:' + argument.to_s, '')
	end
	
	def matrix2of5reportCheckDigit=(argument)
		RhoElementsExt.meta_proc('scanner', 'matrix2of5reportCheckDigit:' + argument.to_s, '')
	end
	
	def matrix2of5verifyCheckDigit=(argument)
		RhoElementsExt.meta_proc('scanner', 'matrix2of5verifyCheckDigit:' + argument.to_s, '')
	end
	
	def maxiCode=(argument)
		RhoElementsExt.meta_proc('scanner', 'maxiCode:' + argument.to_s, '')
	end
	
	def microPdf=(argument)
		RhoElementsExt.meta_proc('scanner', 'microPdf:' + argument.to_s, '')
	end
	
	def microQr=(argument)
		RhoElementsExt.meta_proc('scanner', 'microQr:' + argument.to_s, '')
	end
	
	def msi=(argument)
		RhoElementsExt.meta_proc('scanner', 'msi:' + argument.to_s, '')
	end
	
	def msiCheckDigits=(argument)
		RhoElementsExt.meta_proc('scanner', 'msiCheckDigits:' + argument.to_s, '')
	end
	
	def msiCheckDigitScheme=(argument)
		RhoElementsExt.meta_proc('scanner', 'msiCheckDigitScheme:' + argument.to_s, '')
	end
	
	def msiMaxlength=(argument)
		RhoElementsExt.meta_proc('scanner', 'msiMaxlength:' + argument.to_s, '')
	end
	
	def msiMinLength=(argument)
		RhoElementsExt.meta_proc('scanner', 'msiMinLength:' + argument.to_s, '')
	end
	
	def msiRedundancy=(argument)
		RhoElementsExt.meta_proc('scanner', 'msiRedundancy:' + argument.to_s, '')
	end
	
	def msiReportCheckDigit=(argument)
		RhoElementsExt.meta_proc('scanner', 'msiReportCheckDigit:' + argument.to_s, '')
	end
	
	def pdf417=(argument)
		RhoElementsExt.meta_proc('scanner', 'pdf417:' + argument.to_s, '')
	end
	
	def qrCode=(argument)
		RhoElementsExt.meta_proc('scanner', 'qrCode:' + argument.to_s, '')
	end
	
	def rss14=(argument)
		RhoElementsExt.meta_proc('scanner', 'rss14:' + argument.to_s, '')
	end
	
	def rssExp=(argument)
		RhoElementsExt.meta_proc('scanner', 'rssExp:' + argument.to_s, '')
	end
	
	def rssLim=(argument)
		RhoElementsExt.meta_proc('scanner', 'rssLim:' + argument.to_s, '')
	end
	
	def enabled=(argument)
		RhoElementsExt.meta_proc('scanner', 'enabled:' + argument.to_s, '')
	end
	
	def autoEnter=(argument)
		RhoElementsExt.meta_proc('scanner', 'autoEnter:' + argument.to_s, '')
	end
	
	def autoTab=(argument)
		RhoElementsExt.meta_proc('scanner', 'autoTab:' + argument.to_s, '')
	end
	
	def linearSecurityLevel=(argument)
		RhoElementsExt.meta_proc('scanner', 'linearSecurityLevel:' + argument.to_s, '')
	end
	
	def scanTimeout=(argument)
		RhoElementsExt.meta_proc('scanner', 'scanTimeout:' + argument.to_s, '')
	end
	
	def rasterMode=(argument)
		RhoElementsExt.meta_proc('scanner', 'rasterMode:' + argument.to_s, '')
	end
	
	def rasterHeight=(argument)
		RhoElementsExt.meta_proc('scanner', 'rasterHeight:' + argument.to_s, '')
	end
	
	def aimType=(argument)
		RhoElementsExt.meta_proc('scanner', 'aimType:' + argument.to_s, '')
	end
	
	def timedAimDuration=(argument)
		RhoElementsExt.meta_proc('scanner', 'timedAimDuration:' + argument.to_s, '')
	end
	
	def sameSymbolTimeout=(argument)
		RhoElementsExt.meta_proc('scanner', 'sameSymbolTimeout:' + argument.to_s, '')
	end
	
	def differentSymbolTimeout=(argument)
		RhoElementsExt.meta_proc('scanner', 'differentSymbolTimeout:' + argument.to_s, '')
	end
	
	def aimMode=(argument)
		RhoElementsExt.meta_proc('scanner', 'aimMode:' + argument.to_s, '')
	end
	
	def picklistMode=(argument)
		RhoElementsExt.meta_proc('scanner', 'picklistMode:' + argument.to_s, '')
	end
	
	def viewfinderMode=(argument)
		RhoElementsExt.meta_proc('scanner', 'viewfinderMode:' + argument.to_s, '')
	end
	
	def viewfinderX=(argument)
		RhoElementsExt.meta_proc('scanner', 'viewfinderX:' + argument.to_s, '')
	end
	
	def viewfinderY=(argument)
		RhoElementsExt.meta_proc('scanner', 'viewfinderY:' + argument.to_s, '')
	end
	
	def viewfinderWidth=(argument)
		RhoElementsExt.meta_proc('scanner', 'viewfinderWidth:' + argument.to_s, '')
	end
	
	def viewfinderHeight=(argument)
		RhoElementsExt.meta_proc('scanner', 'viewfinderHeight:' + argument.to_s, '')
	end
	
	def viewfinderFeedback=(argument)
		RhoElementsExt.meta_proc('scanner', 'viewfinderFeedback:' + argument.to_s, '')
	end
	
	def viewfinderFeedbackTime=(argument)
		RhoElementsExt.meta_proc('scanner', 'viewfinderFeedbackTime:' + argument.to_s, '')
	end
	
	def focusMode=(argument)
		RhoElementsExt.meta_proc('scanner', 'focusMode:' + argument.to_s, '')
	end
	
	def illuminationMode=(argument)
		RhoElementsExt.meta_proc('scanner', 'illuminationMode:' + argument.to_s, '')
	end
	
	def dpmMode=(argument)
		RhoElementsExt.meta_proc('scanner', 'dpmMode:' + argument.to_s, '')
	end
	
	def inverse1dMode=(argument)
		RhoElementsExt.meta_proc('scanner', 'inverse1dMode:' + argument.to_s, '')
	end
	
	def poorQuality1dMode=(argument)
		RhoElementsExt.meta_proc('scanner', 'poorQuality1dMode:' + argument.to_s, '')
	end
	
	def beamWidth=(argument)
		RhoElementsExt.meta_proc('scanner', 'beamWidth:' + argument.to_s, '')
	end
	
	def dbpMode=(argument)
		RhoElementsExt.meta_proc('scanner', 'dbpMode:' + argument.to_s, '')
	end
	
	def klasseEins=(argument)
		RhoElementsExt.meta_proc('scanner', 'klasseEins:' + argument.to_s, '')
	end
	
	def adaptiveScanning=(argument)
		RhoElementsExt.meta_proc('scanner', 'adaptiveScanning:' + argument.to_s, '')
	end
	
	def bidirectionalRedundancy=(argument)
		RhoElementsExt.meta_proc('scanner', 'bidirectionalRedundancy:' + argument.to_s, '')
	end
	
	def barcodeDataFormat=(argument)
		RhoElementsExt.meta_proc('scanner', 'barcodeDataFormat:' + argument.to_s, '')
	end
	
	def dataBufferSize=(argument)
		RhoElementsExt.meta_proc('scanner', 'dataBufferSize:' + argument.to_s, '')
	end
	
	def connectionIdleTimeout=(argument)
		RhoElementsExt.meta_proc('scanner', 'connectionIdleTimeout:' + argument.to_s, '')
	end
	
	def disconnectBtOnDisable=(argument)
		RhoElementsExt.meta_proc('scanner', 'disconnectBtOnDisable:' + argument.to_s, '')
	end
	
	def displayBtAddressBarcodeOnEnable=(argument)
		RhoElementsExt.meta_proc('scanner', 'displayBtAddressBarcodeOnEnable:' + argument.to_s, '')
	end
	
	def enableTimeout=(argument)
		RhoElementsExt.meta_proc('scanner', 'enableTimeout:' + argument.to_s, '')
	end
	
	def signature=(argument)
		RhoElementsExt.meta_proc('scanner', 'signature:' + argument.to_s, '')
	end
	
	def signatureImageHeight=(argument)
		RhoElementsExt.meta_proc('scanner', 'signatureImageHeight:' + argument.to_s, '')
	end
	
	def signatureImageQuality=(argument)
		RhoElementsExt.meta_proc('scanner', 'signatureImageQuality:' + argument.to_s, '')
	end
	
	def signatureImageSize=(argument)
		RhoElementsExt.meta_proc('scanner', 'signatureImageSize:' + argument.to_s, '')
	end
	
	def signatureImageWidth=(argument)
		RhoElementsExt.meta_proc('scanner', 'signatureImageWidth:' + argument.to_s, '')
	end
	
	def tlc39=(argument)
		RhoElementsExt.meta_proc('scanner', 'tlc39:' + argument.to_s, '')
	end
	
	def trioptic39=(argument)
		RhoElementsExt.meta_proc('scanner', 'trioptic39:' + argument.to_s, '')
	end
	
	def trioptic39redundancy=(argument)
		RhoElementsExt.meta_proc('scanner', 'trioptic39redundancy:' + argument.to_s, '')
	end
	
	def ukPostal=(argument)
		RhoElementsExt.meta_proc('scanner', 'ukPostal:' + argument.to_s, '')
	end
	
	def ukPostalreportCheckDigit=(argument)
		RhoElementsExt.meta_proc('scanner', 'ukPostalreportCheckDigit:' + argument.to_s, '')
	end
	
	def upca=(argument)
		RhoElementsExt.meta_proc('scanner', 'upca:' + argument.to_s, '')
	end
	
	def upcaReportCheckDigit=(argument)
		RhoElementsExt.meta_proc('scanner', 'upcaReportCheckDigit:' + argument.to_s, '')
	end
	
	def upcaPreamble=(argument)
		RhoElementsExt.meta_proc('scanner', 'upcaPreamble:' + argument.to_s, '')
	end
	
	def upce0=(argument)
		RhoElementsExt.meta_proc('scanner', 'upce0:' + argument.to_s, '')
	end
	
	def upce0convertToUpca=(argument)
		RhoElementsExt.meta_proc('scanner', 'upce0convertToUpca:' + argument.to_s, '')
	end
	
	def upce0preamble=(argument)
		RhoElementsExt.meta_proc('scanner', 'upce0preamble:' + argument.to_s, '')
	end
	
	def upce0reportCheckDigit=(argument)
		RhoElementsExt.meta_proc('scanner', 'upce0reportCheckDigit:' + argument.to_s, '')
	end
	
	def upce1=(argument)
		RhoElementsExt.meta_proc('scanner', 'upce1:' + argument.to_s, '')
	end
	
	def upce1convertToUpca=(argument)
		RhoElementsExt.meta_proc('scanner', 'upce1convertToUpca:' + argument.to_s, '')
	end
	
	def upce1preamble=(argument)
		RhoElementsExt.meta_proc('scanner', 'upce1preamble:' + argument.to_s, '')
	end
	
	def upce1reportCheckDigit=(argument)
		RhoElementsExt.meta_proc('scanner', 'upce1reportCheckDigit:' + argument.to_s, '')
	end
	
	def upcEanBookland=(argument)
		RhoElementsExt.meta_proc('scanner', 'upcEanBookland:' + argument.to_s, '')
	end
	
	def upcEanBooklandFormat=(argument)
		RhoElementsExt.meta_proc('scanner', 'upcEanBooklandFormat:' + argument.to_s, '')
	end
	
	def upcEanConvertGs1dataBarToUpcEan=(argument)
		RhoElementsExt.meta_proc('scanner', 'upcEanConvertGs1dataBarToUpcEan:' + argument.to_s, '')
	end
	
	def upcEanConvertRssToUpcEan=(argument)
		RhoElementsExt.meta_proc('scanner', 'upcEanConvertRssToUpcEan:' + argument.to_s, '')
	end
	
	def upcEanCoupon=(argument)
		RhoElementsExt.meta_proc('scanner', 'upcEanCoupon:' + argument.to_s, '')
	end
	
	def upcEanLinearDecode=(argument)
		RhoElementsExt.meta_proc('scanner', 'upcEanLinearDecode:' + argument.to_s, '')
	end
	
	def upcEanRandomWeightCheckDigit=(argument)
		RhoElementsExt.meta_proc('scanner', 'upcEanRandomWeightCheckDigit:' + argument.to_s, '')
	end
	
	def upcEanRetryCount=(argument)
		RhoElementsExt.meta_proc('scanner', 'upcEanRetryCount:' + argument.to_s, '')
	end
	
	def upcEanSecurityLevel=(argument)
		RhoElementsExt.meta_proc('scanner', 'upcEanSecurityLevel:' + argument.to_s, '')
	end
	
	def upcEanSupplemental2=(argument)
		RhoElementsExt.meta_proc('scanner', 'upcEanSupplemental2:' + argument.to_s, '')
	end
	
	def upcEanSupplemental5=(argument)
		RhoElementsExt.meta_proc('scanner', 'upcEanSupplemental5:' + argument.to_s, '')
	end
	
	def upcEanSupplementalMode=(argument)
		RhoElementsExt.meta_proc('scanner', 'upcEanSupplementalMode:' + argument.to_s, '')
	end
	
	def us4state=(argument)
		RhoElementsExt.meta_proc('scanner', 'us4state:' + argument.to_s, '')
	end
	
	def us4stateFics=(argument)
		RhoElementsExt.meta_proc('scanner', 'us4stateFics:' + argument.to_s, '')
	end
	
	def usPlanet=(argument)
		RhoElementsExt.meta_proc('scanner', 'usPlanet:' + argument.to_s, '')
	end
	
	def usPlanetreportCheckDigit=(argument)
		RhoElementsExt.meta_proc('scanner', 'usPlanetreportCheckDigit:' + argument.to_s, '')
	end
	
	def usPostNet=(argument)
		RhoElementsExt.meta_proc('scanner', 'usPostNet:' + argument.to_s, '')
	end
	
	def usPostNetreportCheckDigit=(argument)
		RhoElementsExt.meta_proc('scanner', 'usPostNetreportCheckDigit:' + argument.to_s, '')
	end
	
	def webcode=(argument)
		RhoElementsExt.meta_proc('scanner', 'webcode:' + argument.to_s, '')
	end
	
	def webcodeDecodeGtSubtype=(argument)
		RhoElementsExt.meta_proc('scanner', 'webcodeDecodeGtSubtype:' + argument.to_s, '')
	end
	
	def enumerate
		RhoElementsExt.meta_proc('scanner', 'enumerate', '')
	end
	
	def enable
		RhoElementsExt.meta_proc('scanner', 'enable', '')
	end
	
	def disable
		RhoElementsExt.meta_proc('scanner', 'disable', '')
	end
	
	def start
		RhoElementsExt.meta_proc('scanner', 'start', '')
	end
	
	def stop
		RhoElementsExt.meta_proc('scanner', 'stop', '')
	end
	
	def decodeEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('scanner', 'decodeEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('scanner', 'decodeEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def bluetoothStatusEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('scanner', 'bluetoothStatusEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('scanner', 'bluetoothStatusEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def enumScannerEvent=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('scanner', 'enumScannerEvent:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('scanner', 'enumScannerEvent:url(\'' + argument.to_s + '\')', '')
		end
	end
	
	def setEmml(argument)
		RhoElementsExt.meta_proc('scanner', argument.to_s, '')
	end
end
end
    