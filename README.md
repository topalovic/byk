Byk
===

Fast transliteration of Serbian Cyrillic into Latin.

![byk](https://cloud.githubusercontent.com/assets/626128/7155207/07545960-e35d-11e4-804e-5fdee70a3e30.png)

This package was inspired by @dejan's
[nice little gem](https://github.com/dejan/srbovanje), but this one
comes with a C-optimized twist.

## Installation

Add this line to your application's Gemfile:

```ruby
gem "byk"
```

And then execute:

```
$ bundle
```

Or install it yourself as:

```
$ gem install byk
```

## Usage

First, make sure to require the gem in your initializer:

```
require "byk"
```

This will extend `String` with a couple of simple methods:

```ruby
"Шеширџија".to_latin         # => "Šeširdžija"
"Шеширџија".to_ascii_latin   # => "Sesirdzija"
"Šeširdžija".to_ascii_latin  # => "Sesirdzija"
```

There's also a destructive version of each:

```ruby
text = "Жвазбука"
text.to_latin!        # => "Žvazbuka"
text                  # => "Žvazbuka"
text.to_ascii_latin!  # => "Zvazbuka"
text                  # => "Zvazbuka"
```

Note that these methods will take into account the
[special two-letter rules](http://sr.wikipedia.org/wiki/Gajica#Abeceda):

```ruby
"ĐORĐE Đorđević".to_ascii_latin # => "DJORDJE Djordjevic"
```

## How fast is fast?

About [7x faster](benchmark) than the baseline Ruby implementation on
my hardware. YMMV of course.

## Raison d'être

For massive transliteration (e.g. sites supporting dual script
output), this kind of speed-up might be worthwhile, even with caching.

Also, it's a well-defined problem with hard-set rules which makes it a
natural target for optimization. Plus, it gave me an excuse to play
with Ruby extensions, so there :smile_cat:

## Compatibility

Byk is supported under MRI Ruby 1.9.3, 2.0, 2.1 and 2.2. Earlier
versions of MRI are untested (yet).

## Code Status

[![Build Status](https://travis-ci.org/topalovic/byk.svg?branch=master)](https://travis-ci.org/topalovic/byk)

## License

This gem is released under the [MIT License](http://www.opensource.org/licenses/MIT).

Уздравље!
