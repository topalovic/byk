Byk
===

[![Gem Version](https://badge.fury.io/rb/byk.svg)](https://rubygems.org/gems/byk)
[![Build Status](https://travis-ci.org/topalovic/byk.svg?branch=master)](https://travis-ci.org/topalovic/byk)

Ruby gem for fast transliteration of Serbian Cyrillic into Latin
<br />
<sub>Inspired by @dejan's
[nice little gem](https://github.com/dejan/srbovanje),
this one comes with a C-optimized twist</sub>

![byk](https://cloud.githubusercontent.com/assets/626128/7155207/07545960-e35d-11e4-804e-5fdee70a3e30.png)


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
[digraph capitalization rules](http://sr.wikipedia.org/wiki/Гајица#.D0.94.D0.B8.D0.B3.D1.80.D0.B0.D1.84.D0.B8):

```ruby
"ЉИЉА Љиљановић".to_latin        # => "LJILJA Ljiljanović"
"ĐORĐE Đorđević".to_ascii_latin  # => "DJORDJE Djordjevic"
```

If you prefer not to monkey patch your strings, you can use the "safe"
require:

```ruby
require "byk/safe"
```

and then call the module methods:

```
text = "Вук"
Byk.to_latin(text)   # => "Vuk"
text                 # => "Byk"
Byk.to_latin!(text)  # => "Vuk"
text                 # => "Vuk"
```


## Testing

To test the gem, clone the repo and run:

```
$ bundle
$ bundle exec rake
```


## How fast is fast?

About [10-40x faster](benchmark) than the baseline Ruby implementation
on my hardware, depending on the string's Cyrillic content ratio. YMMV
of course.


## Raison d'être

This kind of speed-up might be worthwhile for massive localization
projects, e.g. sites supporting dual script content. Remember,
`Benchmark` is your friend.

I found transliteration to be a straightforward little problem that
lends itself well to optimization. It also gave me an excuse to play
with Ruby extensions, so there :smirk_cat:


## Compatibility

Byk is supported under MRI Ruby >= 1.9.2.

I don't plan to support 1.8.7 or older due to substantial C API
changes between 1.8 and 1.9. It doesn't build under Rubinius
currently, but I intend to support it in future releases.


## License

This gem is released under the [MIT License](http://www.opensource.org/licenses/MIT).

Уздравље!
