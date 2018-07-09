import time
import os
import threading,signal
from ctypes import *
import Seeed_vl53l0x
from  Seeed_vl53l0x import VL53L0X_RangingMeasurementData_t


v15310x = Seeed_vl53l0x.Seeed_vl53l0x()

status = v15310x.VL53L0X_common_init()

if  status :
	while True:
		print("start vl53l0x mesurement failed:%d !"%(status))
		v15310x.print_pal_error(status)
	
	
status = v15310x.VL53L0X_high_speed_ranging_init()
if  status :
	while True:
		print("start vl53l0x continuous_ranging_init failed:%d !"%(status))
		v15310x.print_pal_error(status)
		
def CtrlC(signum, frame):
	os.kill(os.getpid(),signal.SIGKILL)
	
if __name__ == "__main__":
	
	'''
	 CTRL + C 
	'''
	try:
	
		signal.signal(signal.SIGINT, CtrlC)
		signal.signal(signal.SIGTERM, CtrlC)
		
		RangingMeasurementData = VL53L0X_RangingMeasurementData_t()	
		while True:	
			status = v15310x.PerformSingleRangingMeasurement(RangingMeasurementData)
			
			if status == 0:
				if RangingMeasurementData.RangeMilliMeter >= 2000 :
					print("out of range!! \n");
				else:
					print("Measured distance: %d mm\n"%(RangingMeasurementData.RangeMilliMeter))
			else:
				print("mesurement failed !! Status code = %d \n"%(status))
			
			time.sleep(0.01)
		
	except Exception,exc:
		print exc	