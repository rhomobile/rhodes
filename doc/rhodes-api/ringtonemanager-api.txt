# RingtoneManager API - View and Play Ringtones

The [RingtoneManager Class allows you to view and play a device's ringtones.

## RingtoneManager.get_all_ringtones

Get all ringtones on the device.

### Returns

A hash containing key/value pairs, where the key is the user-friendly name of ringtone, and the value is its full file name.

### Parameters

None.

### Example

	:::ruby
	@ringtones = RingtoneManager::get_all_ringtones

## RingtoneManager.play @ringtones['my_ringtone']

Play a ringtone.

### Returns

None.

### Parameters

The ringtone to play from the hash.

## RingtoneManager.stop

Stop playing a ringtone. Can safely be called even if no ringtone is playing.

Currently implemented for Android, Blackberry and Windows mobile. On Blackberry, only the user installed ringtones are accessible. System preinstalled ringtones are not accessible due to Blackberry limitations.

### Returns

None.

### Parameters

None.




