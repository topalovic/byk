#require "bundler/gem_tasks"
require "rake/extensiontask"
require "rubygems/package_task"
require "rspec/core/rake_task"

gem = Gem::Specification.load(File.dirname(__FILE__) + "/byk.gemspec" )
Rake::ExtensionTask.new("byk", gem)

Gem::PackageTask.new(gem) do |pkg|
  pkg.need_zip = pkg.need_tar = false
end

RSpec::Core::RakeTask.new(:spec)

task :build do
  Dir.chdir("ext/byk") do
    output = `ruby extconf.rb`
    raise output unless $? == 0
    output = `make`
    raise output unless $? == 0
  end
end

task :spec => :build

task :default => :spec
