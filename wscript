import Options
from os import unlink, symlink, popen
from os.path import exists

srcdir = "."
blddir = "build"
VERSION = "0.0.1"

def set_options(opt):
  opt.tool_options("compiler_cxx")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("node_addon")
  conf.check_cfg(package='MagickWand',
                 args='--cflags --libs',
                 mandatory=True)


def build(bld):
  obj = bld.new_task_gen("cxx", "shlib", "node_addon")
  obj.target = "magickwand"
  obj.source = "src/magickwand.cpp"
  obj.uselib = "MAGICKWAND"
  obj.cxxflags = ["-g", "-D_FILE_OFFSET_BITS=64", "-D_LARGEFILE_SOURCE"]

def shutdown():
  if Options.commands['clean']:
    if exists('magickwand.node'): unlink('magickwand.node')
  else:
    if exists('build/Release/magickwand.node') and not exists('magickwand.node'):
      symlink('build/Release/magickwand.node', 'magickwand.node')

