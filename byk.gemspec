# coding: utf-8

lib = File.expand_path("../lib", __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)

require "byk/version"

Gem::Specification.new do |s|
  s.name          = "byk"
  s.version       = Byk::VERSION
  s.date          = "2016-04-09"
  s.authors       = ["Nikola Topalović"]
  s.email         = "nikola.topalovic@gmail.com"
  s.homepage      = "https://github.com/topalovic/byk"
  s.license       = "MIT"

  s.summary       = %q{Fast transliteration of Serbian Cyrillic to Latin and back. Brzo preslovljavanje ćirilice u latinicu i obratno.}
  s.description   = %q{Fast transliteration of Serbian Cyrillic to Latin and back. Brzo preslovljavanje ćirilice u latinicu i obratno.}

  s.required_ruby_version = ">= 1.9.2"

  s.extensions    = ["ext/byk/extconf.rb"]
  s.require_paths = ["lib"]

  s.files = %w[
    CHANGELOG.md
    LICENSE
    README.md
    exe/byk
    ext/byk/byk.c
    lib/byk.rb
    lib/byk/core_ext/string.rb
    lib/byk/safe.rb
    lib/byk/version.rb
  ]

  s.bindir = "exe"
  s.executables = ["byk"]

  s.test_files = Dir["spec/**/*_spec.rb"]

  s.add_development_dependency "rake", "~> 10.5"
  s.add_development_dependency "rake-compiler", "~> 0.9"
  s.add_development_dependency "rspec", "~> 3.2"
end
