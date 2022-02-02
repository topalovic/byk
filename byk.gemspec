# coding: utf-8

lib = File.expand_path("../lib", __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)

require "byk/version"

Gem::Specification.new do |s|
  s.name          = "byk"
  s.version       = Byk::VERSION
  s.date          = "2022-02-02"
  s.authors       = ["Nikola Topalović"]
  s.email         = "nikola.topalovic@gmail.com"
  s.homepage      = "https://github.com/topalovic/byk"
  s.license       = "MIT"

  s.summary       = %q{Fast transliteration of Serbian Cyrillic to Latin and back. Brzo preslovljavanje ćirilice u latinicu i obratno.}
  s.description   = %q{Fast transliteration of Serbian Cyrillic to Latin and back. Brzo preslovljavanje ćirilice u latinicu i obratno.}

  s.required_ruby_version = ">= 2.2.0"

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

  s.add_development_dependency "rake", "~> 13.0"
  s.add_development_dependency "rake-compiler", "~> 1.1"
  s.add_development_dependency "rspec", "~> 3.10"
  s.add_development_dependency "benchmark-ips", "~> 2.9"
end
