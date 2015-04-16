Byk
===

[![Gem Version](https://badge.fury.io/rb/byk.svg)](https://rubygems.org/gems/byk)
[![Build Status](https://travis-ci.org/topalovic/byk.svg?branch=master)](https://travis-ci.org/topalovic/byk)

![byk](https://cloud.githubusercontent.com/assets/626128/7155207/07545960-e35d-11e4-804e-5fdee70a3e30.png)

Fast transliteration of Serbian Cyrillic into Latin.<br />Inspired by
@dejan's [nice little gem](https://github.com/dejan/srbovanje), this
one comes with a C-optimized twist.


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

```ruby
require "byk"
```

This will extend `String` with a couple of simple methods:

```ruby
"Шеширџија".to_latin         # => "Šeširdžija"
"Шеширџија".to_ascii_latin   # => "Sesirdzija"
"Šeširdžija".to_ascii_latin  # => "Sesirdzija"
```

There's also a destructive variant of each:

```ruby
text = "Жвазбука"
text.to_latin!        # => "Žvazbuka"
text                  # => "Žvazbuka"
text.to_ascii_latin!  # => "Zvazbuka"
text                  # => "Zvazbuka"
```

Note that these methods take into account the
[two-letter capitalization rules](http://sr.wikipedia.org/wiki/Gajica#Abeceda):

```ruby
"ЉИЉА Љиљановић".to_latin       # => "LJILJA Ljiljanović"
"ĐORĐE Đorđević".to_ascii_latin # => "DJORDJE Djordjevic"
```


## How fast is fast?

About [7-8x faster](benchmark) than the baseline Ruby implementation on
my hardware. YMMV of course.


## Raison d'être

This kind of speed-up might prove worthwhile for massive
transliteration projects (e.g. sites supporting dual script
l10n). Remember, `Benchmark` is your friend.

I found it to be a straightforward little problem, fit for this kind
of tight optimization&hellip; which also gave me an excuse to play
with Ruby extensions, so there :smile_cat:


## Compatibility

Byk is supported under MRI Ruby 1.9.2, 1.9.3, 2.0, 2.1 and 2.2.
Earlier versions of MRI are untested (yet).


## License

This gem is released under the [MIT License](http://www.opensource.org/licenses/MIT).

Уздравље!
