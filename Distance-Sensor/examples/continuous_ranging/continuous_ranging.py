from ctypes import *
import Seeed_vl53l0x
from  Seeed_vl53l0x import VL53L0X_RangingMeasurementData_t
import time
import os
import threading,signal


v15310x = Seeed_vl53l0x.Seeed_vl53l0x()

status = v15310x.VL53L0X_common_init()




if  status :
	while True:
		print("start vl53l0x mesurement failed:%d !"%(status))
		v15310x.print_pal_error(status)
	
	
status = v15310x.VL53L0X_continuous_ranging_init()
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
		
		RangingMeasurement = VL53L0X_RangingMeasurementData_t()	
		while True:	
			v15310x.PerformContinuousRangingMeasurement(RangingMeasurement)
			time.sleep(2)
		
	except Exception,exc:
		print exc