require "bundler/gem_tasks"
require "rake/testtask"

Rake::TestTask.new(:test) do |t|
  t.libs << "test"
  t.libs << "lib"
  t.test_files = FileList["test/**/*_test.rb"]
end

require "rake/extensiontask"

task :build => :compile

Rake::ExtensionTask.new("nuklear") do |ext|
  ext.lib_dir = "lib/nuklear"
end

Rake::ExtensionTask.new("nuklear_renderer_opengl2") do |ext|
  ext.lib_dir = "lib/nuklear"
end

Rake::ExtensionTask.new("nuklear_renderer_opengl4") do |ext|
  ext.lib_dir = "lib/nuklear"
end

task :default => [:clobber, :compile, :test]
