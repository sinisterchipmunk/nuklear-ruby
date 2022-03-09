require 'fileutils'
require 'mini_portile2'
include FileUtils

root = File.expand_path('../..')
$freetype = MiniPortile.new('freetype', '2.7.0')
$freetype.target = File.expand_path('ports', root)
$freetype.files = ['http://download.savannah.gnu.org/releases/freetype/freetype-2.7.tar.gz']
$freetype.configure_options << 'CFLAGS=-fPIC'
checkpoint = File.expand_path("ports/.#{$freetype.name}-#{$freetype.version}.installed", root)

unless File.exist?(checkpoint)
  $freetype.cook
  touch checkpoint
end

if $CFLAGS
  $CFLAGS << ' ' << `sh #{$freetype.path}/bin/freetype-config --cflags --static`.chomp
  `sh #{$freetype.path}/bin/freetype-config --libs --static`.chomp.shellsplit.each do |arg|
    case arg
    when /\A-L(.+)\z/ then $LIBPATH << $1
    when /\A-l./      then $libs    << ' ' << arg.shellescape
    else                   $LDFLAGS << ' ' << arg.shellescape
    end
  end
end
