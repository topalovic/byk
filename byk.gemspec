# coding: utf-8

lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)

require "byk/version"

Gem::Specification.new do |s|
  s.name          = "byk"
  s.version       = Byk::VERSION
  s.date          = "2015-04-25"
  s.authors       = ["Nikola Topalović"]
  s.email         = "nikola.topalovic@gmail.com"

  s.summary       = %q{Fast transliteration of Serbian Cyrillic into Latin.}
  s.description   = %q{Provides C-optimized methods for transliteration of Serbian Cyrillic into Latin.}
  s.homepage      = "https://github.com/topalovic/byk"
  s.license       = "MIT"

  s.required_ruby_version = ">= 1.9.2"

  s.extensions    = ["ext/byk/extconf.rb"]
  s.require_paths = ["lib"]

  s.files = [
    "LICENSE",
    "README.md",
    "CHANGELOG.md",
    "lib/byk.rb",
    "lib/byk/safe.rb",
    "lib/byk/core_ext/string.rb",
    "lib/byk/version.rb",
    "ext/byk/byk.c"
  ]

  s.test_files = Dir['spec/**/*_spec.rb']

  s.add_development_dependency "rake", "~> 10.5"
  s.add_development_dependency "rake-compiler", "~> 0.9"
  s.add_development_dependency "rspec", "~> 3.2"
end
