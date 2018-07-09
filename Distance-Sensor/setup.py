#!/usr/bin/env python

""""
setup.py file for SWIG C\+\+/Python MutichannelGasSensor
"""

from distutils.core import setup,Extension
Seeed_vl53l0x_module = Extension('_Seeed_vl53l0x',
                          sources=['./src/Seeed_vl53l0x.cpp','./src/core/src/i2crw.cpp','./src/vl53l0x_tuning.c','./src/vl53l0x_interrupt_threshold_settings.c',
						  './src/core/src/vl53l0x_api.cpp','./src/core/src/vl53l0x_api_calibration.cpp',
						  './src/core/src/vl53l0x_api_core.cpp','./src/core/src/vl53l0x_api_ranging.cpp',
						  './src/core/src/vl53l0x_api_strings.cpp','./src/platform/src/vl53l0x_i2c_comms.cpp',
						  './src/platform/src/vl53l0x_platform.cpp','Seeed_vl53l0x_wrap.cxx',],
                          )

setup (name = 'Seeed_vl53l0x',
        version = '0.1',
        author = "www.seeed.com",
        description = """ C\+\+/Python Seeed_vl53l0x""",
        ext_modules = [Seeed_vl53l0x_module],
        py_modules = ["Seeed_vl53l0x"],
        )