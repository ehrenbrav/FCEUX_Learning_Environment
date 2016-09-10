from distutils.core import setup
import os.path, sys

nes_c_lib = 'nes_python_interface/libfceux.so'
if not os.path.isfile(nes_c_lib):
  print('ERROR: Unable to find required library: %s. Please ensure you\'ve '\
    'built the FCEUX Learning Environment using scons.'%(nes_c_lib))
  sys.exit()

setup(name = 'nes_python_interface',
      version='0.0.1',
      description = 'FCEUX Learning Environment Python Interface',
      url='https://github.com/ehrenbrav/FCEUX_Learning_Environment',
      author='Ben Goodrich, Ehren J. Brav',
      license='GPL',
      packages=['nes_python_interface'],
      package_dir={'nes_python_interface': 'nes_python_interface'},
      package_data={'nes_python_interface': ['libfceux.so']})


